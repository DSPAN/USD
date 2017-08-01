#ifndef RENDER_COMPONENT_H
#define RENDER_COMPONENT_H
//��Ⱦ���������һ�����ʺ�һ�������������ύ����Ⱦ�㡣

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
	Object* m_obj;//�������������Ķ���
	MateriaPtr m_mat;
	MeshPtr m_mesh;
};


#endif