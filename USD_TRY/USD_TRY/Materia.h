#ifndef MATERIA_H
#define MATERIA_H
#include "global.h"
#include "Res\Shader.h"
#include "Texture.h"
#include  <memory>

typedef unsigned long long int MateriaHandle;

class Materia
{
public:
	Materia(std::string name,MateriaHandle handle)
	{
		m_Name=name;
		m_Handle=handle;
		m_bIsLoaded=false;
	}
	virtual ~Materia(){}


	ShaderPtr GetShader()
	{
		if(m_shader==NULL)
		{
			std::cout<<"materia_shader is null"<<std::endl;
		}
		return m_shader;

	}
	void BindTexture()
	{
		glActiveTexture(GL_TEXTURE1);
		m_texture->Bind();
		m_shader->SetInteger("DiffuseMap", 1,true);
	}

	virtual bool load()
	{
		if(m_bIsLoaded)
		{
			return true;
		}

		if (!loadImpl())
		{
			return false;
		}

		m_bIsLoaded = true;
		return true;
	}

	virtual void unload()
	{
		if(!m_bIsLoaded)
		{
			return;
		}
		unloadImpl();
		m_bIsLoaded = false;
	}
	virtual bool loadImpl()
	{

		//加载材质资源路径
		std::string res_path =GetResFile_Path(m_Name);
		std::string _jsonBuffer =loadDataFromFile(res_path.c_str());
		rapidjson::Document _jsonReader;
		if (_jsonReader.ParseInsitu<0>((char*)_jsonBuffer.c_str()).HasParseError())
		{
			std::cout<<"Parse json failed in Shader::loadImpl"<<std::endl;
			return false;
		}
		const rapidjson::Value& Data_1 = _jsonReader["Shader"];
		Shader_=Data_1.GetString();

		const rapidjson::Value& Data_2 = _jsonReader["DiffuseMap"];
		Texture_=Data_2.GetString();
		m_shader=usd_ShaderManager->load(Shader_);

		m_texture=usd_TextureManager->load(Texture_);
		if(m_texture==NULL)
		{
			std::cout<<"materia_shader is load faill"<<std::endl;
		}
		return true;
	}

	virtual void unloadImpl()
	{
		usd_ShaderManager->remove(Shader_);
		usd_TextureManager->remove(Texture_);

	}
	std::string getName()
	{
		return m_Name;
	}
	TextureHandle getHandle()
	{
		return m_Handle;
	}

protected:
	std::string m_Name;		
	MateriaHandle m_Handle;
	bool m_bIsLoaded;
	ShaderPtr m_shader;
	TexturePtr m_texture;

private:
	std::string Shader_;
	std::string Texture_;//保存材质的贴图名称好从贴图管理器删除
	
};

typedef std::shared_ptr<Materia> MateriaPtr;



#endif