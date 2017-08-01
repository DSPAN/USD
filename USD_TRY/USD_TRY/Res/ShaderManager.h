#ifndef SHADER_MANAGER_H
#define SHADER_MANAGER_H
#include "Res\Shader.h"
#include <map>

typedef std::map< ShaderHandle, ShaderPtr>	ShaderHandleMap;
typedef std::map< std::string, ShaderPtr>	ShaderNameMap;

class ShaderManager
{
public:
	ShaderManager():m_hNextHandle(0){}
	virtual ~ShaderManager()
	{
		unloadAll();
		removeAll();
	};

	ShaderHandle getNextHandle()
	{
		return m_hNextHandle++;
	}

	virtual ShaderPtr create(const std::string& name)
	{
		ShaderPtr ret = getByName(name);
		if ( !ret)
		{
			
			ret =  ShaderPtr(createImpl(name, getNextHandle()));
			addImpl(ret);
		}
		return ret;
	}

	typedef std::pair<ShaderPtr, bool> ResourceCreateOrRetrieveResult;

	virtual ResourceCreateOrRetrieveResult createOrRetrieve(const std::string& name)
	{
		//如果没有此资源，创建一个，如果有，使用已有资源
		std::string uniqueName(name);
		ShaderPtr res = getByName(uniqueName);
		bool created = false;
		if (!res)
		{
			created = true;
			res = create(uniqueName);
		}

		return ResourceCreateOrRetrieveResult(res, created);
	}

	// 载入资源
	virtual ShaderPtr load(const std::string& name)
	{
		std::string uniqueName(name);
		ShaderPtr r = createOrRetrieve(uniqueName).first;

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

		ShaderPtr res = getByName(name);
		if (res)
		{
			removeImpl(res);
		}
	}
	virtual void remove(ShaderHandle handle)
	{
		ShaderPtr res = getByHandle(handle);

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

		ShaderPtr res = getByName(uniqueName);
		if (res)
		{
			res->unload();
		}
	}

	virtual void unload(ShaderHandle handle)
	{
		ShaderPtr res = getByHandle(handle);
		if (res)
		{
			res->unload();
		}
	}

	virtual void unloadAll(void)
	{
		ShaderNameMap::iterator i, iend;
		i = m_mapResources.begin();
		iend = m_mapResources.end();
		for (; i != iend; ++i)
		{		
			i->second->unload();
		}
	}

	// 取得资源
	virtual ShaderPtr getByName(const std::string& name)
	{
		std::string uniqueName(name);
		ShaderPtr res;
		// 从名称资源树删除
		ShaderNameMap::iterator nameIt = m_mapResources.find(uniqueName);
		if (nameIt != m_mapResources.end())
		{
			res = nameIt->second;
		}
		return res;
	}
	
	virtual ShaderPtr getByHandle(ShaderHandle handle)
	{
		ShaderPtr res;
		// 从句柄资源树删除

		ShaderHandleMap::iterator handleIt = m_mapResourcesByHandle.find(handle);
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

	virtual bool resourceExists(ShaderHandle handle)
	{
		if(getByHandle(handle))
			return true;
		return false;
	}

	// 加入，创建的实现
	virtual Shader* createImpl(const std::string& name, ShaderHandle handle)
	{
		Shader* ret=new Shader(name,handle);
		return ret;

	}

	virtual void addImpl( ShaderPtr& res)
	{
		// 加入名称资源树
		std::pair<ShaderNameMap::iterator, bool> result;
		result = m_mapResources.insert( ShaderNameMap::value_type( res->getName(), res ) );

		// 加入句柄资源树
		std::pair<ShaderHandleMap::iterator, bool> resultHandle = 
			m_mapResourcesByHandle.insert( ShaderHandleMap::value_type( res->getHandle(), res ) );
		if (!resultHandle.second)
		{
			//OutputDebugString("++++gkResourceManager.cpp: a gkResouceHandle:\n%d has already exist!\n");
		}
	}
	virtual void removeImpl( ShaderPtr& res )
	{
		// 从名称资源树删除
		ShaderNameMap::iterator nameIt = m_mapResources.find(res->getName());
		if (nameIt != m_mapResources.end())
		{
			m_mapResources.erase(nameIt);
		}

		// 从句柄资源树删除
		ShaderHandleMap::iterator handleIt = m_mapResourcesByHandle.find(res->getHandle());
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
	ShaderHandleMap	m_mapResourcesByHandle;
	ShaderNameMap m_mapResources;
	ShaderHandle m_hNextHandle;
private:
	 

};

extern ShaderManager* usd_ShaderManager;

#endif