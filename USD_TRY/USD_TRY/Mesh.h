#ifndef MESH_H
#define MESH_H

#include "Math\Math.h"
#include  <memory>
#include "Fileutil.h"
#include "glew.h"
#include "glut.h"

typedef unsigned int uint;
typedef unsigned char byte;

typedef unsigned long long int MeshHandle;
struct Vertex
{
	glm::vec3 position;
	glm::vec3 normal;
	glm::vec2 uv;
};
class Mesh
{
public:
	Mesh(std::string name,MeshHandle handle)
	{
		m_Name=name;
		m_Handle=handle;
		m_bIsLoaded=false;
	}
	virtual ~Mesh()
	{
		unloadImpl();
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
		//加载网格资源路径
		std::string res_path =GetResFile_Path(m_Name);
		//此处应添加一个判断资源文件是否存在的判断,赶工暂时不实现

		FILE* f = fopen(res_path.c_str(), "rb");
		//读取网格顶点数量
		byte buffer[4];
		ReadBytes(f, buffer, 4);
		vertexBufferSize = *(uint*)buffer;
		//读取网格顶点数据
		byte* vertex_Data = new byte[vertexBufferSize];
		ReadBytes(f, vertex_Data, vertexBufferSize);
		vertexData=(void*)vertex_Data;

		//读取网格索引数量
		byte buffer1[4];
		ReadBytes(f, buffer1, 4);
		indexBufferSize = *(uint*)buffer1;
		//读取网格索引数据
		byte* index_Data = new byte[indexBufferSize];
		ReadBytes(f, index_Data, indexBufferSize);
		indexData=(void*)index_Data;
		fclose(f);
		//绑定网格
		setupMesh();

		return true;
	}
	void Draw()
    {
        glBindVertexArray(this->VAO);
        glDrawElements(GL_TRIANGLES, this->indexBufferSize, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
    }

	virtual void unloadImpl()
	{
		if(vertexData!=NULL)
			delete vertexData;
		if(indexData!=NULL)
			delete indexData;
		glDeleteVertexArrays(1, &VAO);
		glDeleteBuffers(1, &VBO);
		glDeleteBuffers(1, &EBO);

	}
	std::string getName()
	{
		return m_Name;
	}
	MeshHandle getHandle()
	{
		return m_Handle;
	}

protected:
	std::string m_Name;		
	MeshHandle m_Handle;
	bool m_bIsLoaded;

private:
	byte* ReadBytes(FILE* file, byte* buffer, uint size)
	{
		fread(buffer, 1, size, file);
		return buffer;
	}
	void setupMesh()
    {
        // Create buffers/arrays
        glGenVertexArrays(1, &this->VAO);
        glGenBuffers(1, &this->VBO);
        glGenBuffers(1, &this->EBO);

        glBindVertexArray(this->VAO);
        // Load data into vertex buffers
		
        glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
        glBufferData(GL_ARRAY_BUFFER, this->vertexBufferSize, this->vertexData, GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, this->indexBufferSize , indexData, GL_STATIC_DRAW);

        // Set the vertex attribute pointers
        // Vertex Positions
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)0);
        // Vertex Normals
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, normal));
        // Vertex Texture Coords
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, uv));

        glBindVertexArray(0);
    }

	GLuint VAO,VBO,EBO;
	uint vertexBufferSize;
	void* vertexData;
	uint indexBufferSize;
	void* indexData;
};

typedef std::shared_ptr<Mesh> MeshPtr;


#endif