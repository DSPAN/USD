#ifndef OBJECT_H
#define OBJECT_H
//场景对象：包含一个网格和一个材质
#include "Math\Math.h"
#include "Mesh.h"
#include "Materia.h"
#include "RenderComponent.h"
#include <vector>

class Object
{
public:
	Object()
	{
		ID=0;
		m_parent=NULL;
		m_RenderComponent=NULL;
		m_childrens.clear();
		m_pos = glm::vec3(0,0,0);
        m_rot = glm::quat(1,0,0,0);
        m_scale = glm::vec3(1,1,1);
	}
	virtual ~Object()
	{
		
	}

	void attachChild( Object* ent )
	{
		//添加子物体
		m_childrens.push_back(ent);
		ent->SetParent(this);
	}

	void detachChild( Object* ent )
	{
		//删除子物体
		std::vector<Object*>::iterator it = m_childrens.begin();
		for (; it != m_childrens.end(); )
		{
			if ( (*it) == ent )
			{	
				ent->SetParent(NULL);
				it = m_childrens.erase(it);
			}
			else
			{
				++it;
			}
		}
	}

	void detachAllChild()
	{
		//删除所有子物体
		std::vector<Object*>::iterator it = m_childrens.begin();
		for (; it != m_childrens.end(); ++it)
		{
			(*it)->SetParent(NULL);
		}
		m_childrens.clear();
	}
	void detachFromFather()
	{
		//删除父物体
		if (m_parent)
		{
			m_parent->detachChild(this);
		}
	}

	void SetParent(Object* P)
	{
		m_parent=P;
	}
	std::vector<Object*> GetChild()
	{
		return m_childrens;
	}
	void SetRenderComponent(RenderComponent* rc)
	{
		m_RenderComponent=rc;
	}
	RenderComponent* GetRenderComponent()
	{
		return m_RenderComponent;
	}
	glm::mat4 GetTransformation()
    {
        glm::mat4 translationMatrix = InitTranslation(m_pos);
        glm::mat4 rotationMatrix = glm::toMat4(m_rot);
        glm::mat4 scaleMatrix = InitScale(m_scale);
		if(m_parent!=NULL)
		{
			glm::mat4 ParentMatrix=m_parent->GetTransformation();
			return ParentMatrix*translationMatrix*rotationMatrix*scaleMatrix;
		}
		return translationMatrix*rotationMatrix*scaleMatrix;
    }

    glm::vec3 GetPos()
    {
        return m_pos;
    }

    void SetPos(glm::vec3 pos)
    {
        this->m_pos = pos;
    }

    glm::quat GetRot()
    {
        return m_rot;
    }

    void SetRot(glm::quat rotation)
    {
        this->m_rot = rotation;
    }

    glm::vec3 GetScale()
    {
        return m_scale;
    }

    void SetScale(glm::vec3 scale)
    {
        this->m_scale = scale;
    }


private:
	uint ID;
	Object* m_parent;
	std::vector<Object*> m_childrens;
    glm::vec3   m_pos;
    glm::quat   m_rot;
    glm::vec3   m_scale;
	RenderComponent* m_RenderComponent;

};


#endif