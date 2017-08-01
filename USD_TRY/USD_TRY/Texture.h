#ifndef TEXTURE_H
#define TEXTURE_H

#include "Math\Math.h"
#include  <memory>
#include "Fileutil.h"
#include "glew.h"
#include "glut.h"
#include "Texture_Loader.h"

typedef unsigned long long int TextureHandle;
class Texture
{
public:
	Texture(std::string name,TextureHandle handle)
		: Width(0), Height(0), Internal_Format(GL_RGB), Image_Format(GL_BGR), Wrap_S(GL_REPEAT), Wrap_T(GL_REPEAT), Filter_Min(GL_LINEAR), Filter_Max(GL_LINEAR)
	{
		m_Name=name;
		m_Handle=handle;
		m_bIsLoaded=false;
	}
	virtual ~Texture(){}

	GLuint ID;
    // Texture image dimensions
    GLuint Width, Height; // Width and height of loaded image in pixels
    // Texture Format
    GLuint Internal_Format; // Format of texture object
    GLuint Image_Format; // Format of loaded image
    // Texture configuration
    GLuint Wrap_S; // Wrapping mode on S axis
    GLuint Wrap_T; // Wrapping mode on T axis
    GLuint Filter_Min; // Filtering mode if texture pixels < screen pixels
    GLuint Filter_Max; // Filtering mode if texture pixels > screen pixels

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

		glGenTextures(1, &this->ID);
		//加载网格资源路径
		std::string res_path =GetResFile_Path(m_Name);
		//此处应添加一个判断资源文件是否存在的判断,赶工暂时不实现

		Texture_Loader loader;
		if(!loader.Load(res_path.c_str()))
		{
			std::cout<<"load texture"<<res_path.c_str()<<std::endl;
			return false;
		}
		Width=loader.GetWidth();
		Height=loader.GetHeight();
		// Create Texture
		glBindTexture(GL_TEXTURE_2D, this->ID);
		glTexImage2D(GL_TEXTURE_2D, 0, this->Internal_Format, loader.GetWidth(), loader.GetHeight(), 0, this->Image_Format, GL_UNSIGNED_BYTE, loader.GetDate());
		// Set Texture wrap and filter modes
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, this->Wrap_S);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, this->Wrap_T);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, this->Filter_Min);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, this->Filter_Max);
		// Unbind texture
		glBindTexture(GL_TEXTURE_2D, 0);

		loader.UnLoad();
		return true;
	}

	void Texture::Bind() const
	{
		glBindTexture(GL_TEXTURE_2D, this->ID);
	}


	virtual void unloadImpl()
	{
		glDeleteTextures(1, &ID);
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
	TextureHandle m_Handle;
	bool m_bIsLoaded;

private:

};
typedef std::shared_ptr<Texture> TexturePtr;

#endif