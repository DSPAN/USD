#ifndef TEXTURE_MANAGER_H
#define TEXTURE_MANAGER_H

#include <map>
#include "Texture.h"

typedef std::map< TextureHandle, TexturePtr>	TextureHandleMap;
typedef std::map< std::string, TexturePtr>	TextureNameMap;

class TextureManager
{
public:
	TextureManager():m_hNextHandle(0){}
	virtual ~TextureManager()
	{
		unloadAll();
		removeAll();
	};

	TextureHandle getNextHandle()
	{
		return m_hNextHandle++;
	}

	virtual TexturePtr create(const std::string& name)
	{
		TexturePtr ret = getByName(name);
		if ( !ret)
		{
			ret =  TexturePtr(createImpl(name, getNextHandle()));
			addImpl(ret);
		}
		return ret;
	}

	typedef std::pair<TexturePtr, bool> ResourceCreateOrRetrieveResult;

	virtual ResourceCreateOrRetrieveResult createOrRetrieve(const std::string& name)
	{
		//���û�д���Դ������һ��������У�ʹ��������Դ
		std::string uniqueName(name);
		TexturePtr res = getByName(uniqueName);
		bool created = false;
		if (!res)
		{
			created = true;
			res = create(uniqueName);
		}

		return ResourceCreateOrRetrieveResult(res, created);
	}

	// ������Դ
	virtual TexturePtr load(const std::string& name)
	{
		std::string uniqueName(name);
		TexturePtr r = createOrRetrieve(uniqueName).first;

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

		TexturePtr res = getByName(name);
		if (res)
		{
			removeImpl(res);
		}
	}
	virtual void remove(TextureHandle handle)
	{
		TexturePtr res = getByHandle(handle);

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

		TexturePtr res = getByName(uniqueName);
		if (res)
		{
			res->unload();
		}
	}

	virtual void unload(TextureHandle handle)
	{
		TexturePtr res = getByHandle(handle);
		if (res)
		{
			res->unload();
		}
	}

	virtual void unloadAll(void)
	{
		TextureNameMap::iterator i, iend;
		i = m_mapResources.begin();
		iend = m_mapResources.end();
		for (; i != iend; ++i)
		{		
			i->second->unload();
		}
	}

	// ȡ����Դ
	virtual TexturePtr getByName(const std::string& name)
	{
		std::string uniqueName(name);
		TexturePtr res;
		// ��������Դ��ɾ��
		TextureNameMap::iterator nameIt = m_mapResources.find(uniqueName);
		if (nameIt != m_mapResources.end())
		{
			res = nameIt->second;
		}
		return res;
	}
	
	virtual TexturePtr getByHandle(TextureHandle handle)
	{
		TexturePtr res;
		// �Ӿ����Դ��ɾ��

		TextureHandleMap::iterator handleIt = m_mapResourcesByHandle.find(handle);
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
	virtual Texture* createImpl(const std::string& name, TextureHandle handle)
	{
		Texture* ret=new Texture(name,handle);
		return ret;

	}

	virtual void addImpl( TexturePtr& res)
	{
		// ����������Դ��
		std::pair<TextureNameMap::iterator, bool> result;
		result = m_mapResources.insert( TextureNameMap::value_type( res->getName(), res ) );

		// ��������Դ��
		std::pair<TextureHandleMap::iterator, bool> resultHandle = 
			m_mapResourcesByHandle.insert( TextureHandleMap::value_type( res->getHandle(), res ) );
		if (!resultHandle.second)
		{
			//OutputDebugString("++++gkResourceManager.cpp: a gkResouceHandle:\n%d has already exist!\n");
		}
	}
	virtual void removeImpl( TexturePtr& res )
	{
		// ��������Դ��ɾ��
		TextureNameMap::iterator nameIt = m_mapResources.find(res->getName());
		if (nameIt != m_mapResources.end())
		{
			m_mapResources.erase(nameIt);
		}

		// �Ӿ����Դ��ɾ��
		TextureHandleMap::iterator handleIt = m_mapResourcesByHandle.find(res->getHandle());
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
	TextureHandleMap m_mapResourcesByHandle;
	TextureNameMap m_mapResources;
	TextureHandle m_hNextHandle;
private:
	 

};

extern TextureManager* usd_TextureManager;

#endif