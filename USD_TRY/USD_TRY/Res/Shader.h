#ifndef SHADER_H
#define SHADER_H


#include "Math\Math.h"
#include  <memory>
#include "Fileutil.h"
#include "glew.h"
#include "glut.h"
#include "rapidjson/document.h"
#include "rapidjson/stringbuffer.h"
#include "rapidjson/writer.h"



typedef unsigned long long int ShaderHandle;
class Shader
{
public:
    // State
    GLuint ID;
    // Constructor
	Shader(std::string name,ShaderHandle handle) 
	{
		m_Name=name;
		m_Handle=handle;
		m_bIsLoaded=false;
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
		//加载着色器资源,编译着色器
		std::string res_path =GetResFile_Path(m_Name);
		//此处应添加一个判断资源文件是否存在的判断,赶工暂时不实现

		std::string _jsonBuffer =loadDataFromFile(res_path.c_str());
		rapidjson::Document _jsonReader;
		if (_jsonReader.ParseInsitu<0>((char*)_jsonBuffer.c_str()).HasParseError())
		{
			std::cout<<"Parse json failed in Shader::loadImpl"<<std::endl;
			return false;
		}
		const rapidjson::Value& Data_1 = _jsonReader["vertex"];
		std::string vertex_path=GetResFile_Path(Data_1.GetString());

		const rapidjson::Value& Data_2 = _jsonReader["fragment"];
		std::string fragment_path=GetResFile_Path(Data_2.GetString());
		
		std::string vertexSource=loadDataFromFile(vertex_path.c_str());
		
		std::string fragmentSource=loadDataFromFile(fragment_path.c_str());
		if(!Compile(vertexSource.c_str(),fragmentSource.c_str()))
		{
			return false;
		}
		
		return true;
	}

	virtual void unloadImpl()
	{
		glDeleteProgram(ID);
	}
	std::string getName()
	{
		return m_Name;
	}
	ShaderHandle getHandle()
	{
		return m_Handle;
	}
    // Sets the current shader as active
    Shader  &Use();
    // Compiles the shader from given source code
    bool    Compile(const GLchar *vertexSource, const GLchar *fragmentSource, const GLchar *geometrySource = nullptr); // Note: geometry source code is optional
    // Utility functions
    void    SetFloat    (const GLchar *name, GLfloat value, GLboolean useShader = false);
    void    SetInteger  (const GLchar *name, GLint value, GLboolean useShader = false);
    void    SetVector2f (const GLchar *name, GLfloat x, GLfloat y, GLboolean useShader = false);
    void    SetVector2f (const GLchar *name, const glm::vec2 &value, GLboolean useShader = false);
    void    SetVector3f (const GLchar *name, GLfloat x, GLfloat y, GLfloat z, GLboolean useShader = false);
    void    SetVector3f (const GLchar *name, const glm::vec3 &value, GLboolean useShader = false);
    void    SetVector4f (const GLchar *name, GLfloat x, GLfloat y, GLfloat z, GLfloat w, GLboolean useShader = false);
    void    SetVector4f (const GLchar *name, const glm::vec4 &value, GLboolean useShader = false);
    void    SetMatrix4  (const GLchar *name, const glm::mat4 &matrix, GLboolean useShader = false);
protected:
	std::string m_Name;		
	ShaderHandle m_Handle;
	bool m_bIsLoaded;

private:
    // Checks if compilation or linking failed and if so, print the error logs
    bool    checkCompileErrors(GLuint object, std::string type);
};

typedef std::shared_ptr<Shader> ShaderPtr;



#endif