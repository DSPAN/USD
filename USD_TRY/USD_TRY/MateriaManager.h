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
		//���û�д���Դ������һ��������У�ʹ��������Դ
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

	// ������Դ
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

	// ɾ����Դ
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
			//������ڸ���ɫ����Դ���Ƴ���
			removeImpl(res);
		}
	}
	virtual void removeAll(void)
	{
		m_mapResources.clear();
		m_mapResourcesByHandle.clear();
	}

	// �ͷ���Դ
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

	// ȡ����Դ
	virtual MateriaPtr getByName(const std::string& name)
	{
		std::string uniqueName(name);
		MateriaPtr res;
		// ��������Դ��ɾ��
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
		// �Ӿ����Դ��ɾ��

		MateriaHandleMap::iterator handleIt = m_mapResourcesByHandle.find(handle);
		if (handleIt != m_mapResourcesByHandle.end())
		{
			res = handleIt->second;
		}
		return res;
	}

	// ��֤��Դ
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

	// ���룬������ʵ��
	virtual Materia* createImpl(const std::string& name, TextureHandle handle)
	{
		Materia* ret=new Materia(name,handle);
		return ret;

	}

	virtual void addImpl( MateriaPtr& res)
	{
		// ����������Դ��
		std::pair<MateriaNameMap::iterator, bool> result;
		result = m_mapResources.insert( MateriaNameMap::value_type( res->getName(), res ) );

		// ��������Դ��
		std::pair<MateriaHandleMap::iterator, bool> resultHandle = 
			m_mapResourcesByHandle.insert( MateriaHandleMap::value_type( res->getHandle(), res ) );
		if (!resultHandle.second)
		{
			//OutputDebugString("++++gkResourceManager.cpp: a gkResouceHandle:\n%d has already exist!\n");
		}
	}
	virtual void removeImpl( MateriaPtr& res )
	{
		// ��������Դ��ɾ��
		MateriaNameMap::iterator nameIt = m_mapResources.find(res->getName());
		if (nameIt != m_mapResources.end())
		{
			m_mapResources.erase(nameIt);
		}

		// �Ӿ����Դ��ɾ��
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