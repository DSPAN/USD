#ifndef MESH_MANAGER_H
#define MESH_MANAGER_H

#include "Mesh.h"
#include <map>

typedef std::map< MeshHandle, MeshPtr>	MeshHandleMap;
typedef std::map< std::string, MeshPtr>	MeshNameMap;

class MeshManager
{
public:
	MeshManager():m_hNextHandle(0){}
	virtual ~MeshManager()
	{
		unloadAll();
		removeAll();
	};

	MeshHandle getNextHandle()
	{
		return m_hNextHandle++;
	}

	virtual MeshPtr create(const std::string& name)
	{
		MeshPtr ret = getByName(name);
		if ( !ret)
		{
			ret =  MeshPtr(createImpl(name, getNextHandle()));
			addImpl(ret);
		}
		return ret;
	}

	typedef std::pair<MeshPtr, bool> ResourceCreateOrRetrieveResult;

	virtual ResourceCreateOrRetrieveResult createOrRetrieve(const std::string& name)
	{
		//如果没有此资源，创建一个，如果有，使用已有资源
		std::string uniqueName(name);
		MeshPtr res = getByName(uniqueName);
		bool created = false;
		if (!res)
		{
			created = true;
			res = create(uniqueName);
		}

		return ResourceCreateOrRetrieveResult(res, created);
	}

	// 载入资源
	virtual MeshPtr load(const std::string& name)
	{
		std::string uniqueName(name);
		MeshPtr r = createOrRetrieve(uniqueName).first;

		// ensure loaded
		if (r->load())
			return r;
		
		remove(uniqueName);
		r.reset();
		return r;
	}

	// 删除资源
	virtual void remove(const std::string& name)
	{

		MeshPtr res = getByName(name);
		if (res)
		{
			removeImpl(res);
		}
	}
	virtual void remove(MeshHandle handle)
	{
		MeshPtr res = getByHandle(handle);

		if (res)
		{
			//如果存在该着色器资源就移除它
			removeImpl(res);
		}
	}
	virtual void removeAll(void)
	{
		m_mapResources.clear();
		m_mapResourcesByHandle.clear();
	}

	// 释放资源
	virtual void unload(const std::string& name)
	{
		std::string uniqueName(name);

		MeshPtr res = getByName(uniqueName);
		if (res)
		{
			res->unload();
		}
	}

	virtual void unload(MeshHandle handle)
	{
		MeshPtr res = getByHandle(handle);
		if (res)
		{
			res->unload();
		}
	}

	virtual void unloadAll(void)
	{
		MeshNameMap::iterator i, iend;
		i = m_mapResources.begin();
		iend = m_mapResources.end();
		for (; i != iend; ++i)
		{		
			i->second->unload();
		}
	}

	// 取得资源
	virtual MeshPtr getByName(const std::string& name)
	{
		std::string uniqueName(name);
		MeshPtr res;
		// 从名称资源树删除
		MeshNameMap::iterator nameIt = m_mapResources.find(uniqueName);
		if (nameIt != m_mapResources.end())
		{
			res = nameIt->second;
		}
		return res;
	}
	
	virtual MeshPtr getByHandle(MeshHandle handle)
	{
		MeshPtr res;
		// 从句柄资源树删除

		MeshHandleMap::iterator handleIt = m_mapResourcesByHandle.find(handle);
		if (handleIt != m_mapResourcesByHandle.end())
		{
			res = handleIt->second;
		}
		return res;
	}

	// 验证资源
	virtual bool resourceExists(const std::string& name)
	{
		if(getByName(name))
			return true;
		return false;
	}

	virtual bool resourceExists(MeshHandle handle)
	{
		if(getByHandle(handle))
			return true;
		return false;
	}

	// 加入，创建的实现
	virtual Mesh* createImpl(const std::string& name, MeshHandle handle)
	{
		Mesh* ret=new Mesh(name,handle);
		return ret;

	}

	virtual void addImpl( MeshPtr& res)
	{
		// 加入名称资源树
		std::pair<MeshNameMap::iterator, bool> result;
		result = m_mapResources.insert( MeshNameMap::value_type( res->getName(), res ) );

		// 加入句柄资源树
		std::pair<MeshHandleMap::iterator, bool> resultHandle = 
			m_mapResourcesByHandle.insert( MeshHandleMap::value_type( res->getHandle(), res ) );
		if (!resultHandle.second)
		{
			//OutputDebugString("++++gkResourceManager.cpp: a gkResouceHandle:\n%d has already exist!\n");
		}
	}
	virtual void removeImpl( MeshPtr& res )
	{
		// 从名称资源树删除
		MeshNameMap::iterator nameIt = m_mapResources.find(res->getName());
		if (nameIt != m_mapResources.end())
		{
			m_mapResources.erase(nameIt);
		}

		// 从句柄资源树删除
		MeshHandleMap::iterator handleIt = m_mapResourcesByHandle.find(res->getHandle());
		if (handleIt != m_mapResourcesByHandle.end())
		{
			if(handleIt->second)
			{
				handleIt->second->unload();
			}
			m_mapResourcesByHandle.erase(handleIt);
		}
	}

protected:
	MeshHandleMap	m_mapResourcesByHandle;
	MeshNameMap m_mapResources;
	MeshHandle m_hNextHandle;
private:
	 

};
extern MeshManager* usd_MeshManager;
#endif