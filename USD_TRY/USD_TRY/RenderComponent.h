#ifndef RENDER_COMPONENT_H
#define RENDER_COMPONENT_H
//渲染组件：包含一个材质和一个网格对象，最后提交到渲染层。

#include "Mesh.h"
#include "Materia.h"
class Object;
class RenderComponent
{
public:
	RenderComponent(){}
	virtual ~RenderComponent()
	{
		
	}

	Object* GetObj()
	{
		return m_obj;
	}
	MeshPtr GetMesh()
	{
		return m_mesh;
	}
	MateriaPtr GetMateria()
	{
		return m_mat;
	}
	void SetMesh(MeshPtr mesh)
	{
		m_mesh=mesh;
	}
	void SetMateria(MateriaPtr mat)
	{
		m_mat=mat;
	}
	void SetObject(Object* o)
	{
		m_obj=o;
	}

private:
	Object* m_obj;//储存包含该组件的对象
	MateriaPtr m_mat;
	MeshPtr m_mesh;
};


#endif