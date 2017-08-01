#ifndef MATERIA_MANAGER_H
#define MATERIA_MANAGER_H

#include "Materia.h"
#include <map>

typedef std::map< MateriaHandle, MateriaPtr>	MateriaHandleMap;
typedef std::map< std::string, MateriaPtr>	MateriaNameMap;

class MateriaManager
{
public:
	MateriaManager():m_hNextHandle(0){}
	virtual ~MateriaManager()
	{
		unloadAll();
		removeAll();
	};

	MateriaHandle getNextHandle()
	{
		return m_hNextHandle++;
	}

	virtual MateriaPtr create(const std::string& name)
	{
		MateriaPtr ret = getByName(name);
		if ( !ret)
		{
			ret =  MateriaPtr(createImpl(name, getNextHandle()));
			addImpl(ret);
		}
		return ret;
	}

	typedef std::pair<MateriaPtr, bool> ResourceCreateOrRetrieveResult;

	virtual ResourceCreateOrRetrieveResult createOrRetrieve(const std::string& name)
	{
		//如果没有此资源，创建一个，如果有，使用已有资源
		std::string uniqueName(name);
		MateriaPtr res = getByName(uniqueName);
		bool created = false;
		if (!res)
		{
			created = true;
			res = create(uniqueName);
		}

		return ResourceCreateOrRetrieveResult(res, created);
	}

	// 载入资源
	virtual MateriaPtr load(const std::string& name)
	{
		std::string uniqueName(name);
		MateriaPtr r = createOrRetrieve(uniqueName).first;

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

		MateriaPtr res = getByName(name);
		if (res)
		{
			removeImpl(res);
		}
	}
	virtual void remove(TextureHandle handle)
	{
		MateriaPtr res = getByHandle(handle);

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

		MateriaPtr res = getByName(uniqueName);
		if (res)
		{
			res->unload();
		}
	}

	virtual void unload(TextureHandle handle)
	{
		MateriaPtr res = getByHandle(handle);
		if (res)
		{
			res->unload();
		}
	}

	virtual void unloadAll(void)
	{
		MateriaNameMap::iterator i, iend;
		i = m_mapResources.begin();
		iend = m_mapResources.end();
		for (; i != iend; ++i)
		{		
			i->second->unload();
		}
	}

	// 取得资源
	virtual MateriaPtr getByName(const std::string& name)
	{
		std::string uniqueName(name);
		MateriaPtr res;
		// 从名称资源树删除
		MateriaNameMap::iterator nameIt = m_mapResources.find(uniqueName);
		if (nameIt != m_mapResources.end())
		{
			res = nameIt->second;
		}
		return res;
	}
	
	virtual MateriaPtr getByHandle(TextureHandle handle)
	{
		MateriaPtr res;
		// 从句柄资源树删除

		MateriaHandleMap::iterator handleIt = m_mapResourcesByHandle.find(handle);
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

	virtual bool resourceExists(TextureHandle handle)
	{
		if(getByHandle(handle))
			return true;
		return false;
	}

	// 加入，创建的实现
	virtual Materia* createImpl(const std::string& name, TextureHandle handle)
	{
		Materia* ret=new Materia(name,handle);
		return ret;

	}

	virtual void addImpl( MateriaPtr& res)
	{
		// 加入名称资源树
		std::pair<MateriaNameMap::iterator, bool> result;
		result = m_mapResources.insert( MateriaNameMap::value_type( res->getName(), res ) );

		// 加入句柄资源树
		std::pair<MateriaHandleMap::iterator, bool> resultHandle = 
			m_mapResourcesByHandle.insert( MateriaHandleMap::value_type( res->getHandle(), res ) );
		if (!resultHandle.second)
		{
			//OutputDebugString("++++gkResourceManager.cpp: a gkResouceHandle:\n%d has already exist!\n");
		}
	}
	virtual void removeImpl( MateriaPtr& res )
	{
		// 从名称资源树删除
		MateriaNameMap::iterator nameIt = m_mapResources.find(res->getName());
		if (nameIt != m_mapResources.end())
		{
			m_mapResources.erase(nameIt);
		}

		// 从句柄资源树删除
		MateriaHandleMap::iterator handleIt = m_mapResourcesByHandle.find(res->getHandle());
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
	MateriaHandleMap m_mapResourcesByHandle;
	MateriaNameMap m_mapResources;
	MateriaHandle m_hNextHandle;
private:
	 

};

extern MateriaManager* usd_MateriaManager;

#endif