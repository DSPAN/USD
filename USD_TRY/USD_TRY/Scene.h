#ifndef SCENE_H
#define SCENE_H
#include "Object.h"
#include "RenderLayer.h"
#include "Camera.h"

#include <vector>
//����:������Ⱦ��,�Լ����������
class Scene
{
public:
	
	Scene()
	{
		R_Layer=new RenderLayer();
		main_camera=new Camera();
		R_Layer->SetCamera(main_camera);

	}
	virtual ~Scene()
	{
		//����ʱ���ͷ������ָ��
		if(R_Layer!=NULL)
			delete R_Layer;
		if(main_camera!=NULL)
			delete main_camera;

		for(int i=0;i<objs.size();i++)
		{
			if(objs[i]!=NULL)
				delete objs[i];
		}
	}
	void AddObj(Object* o)
	{
		//��ӳ�������,��ӳ�������ʱӦ�ðѸó���������Ӷ���Ҳ��ӽ���
		objs.push_back(o);
		if(o->GetRenderComponent()!=NULL)
			R_Layer->Add(o->GetRenderComponent());
		for(int i=0;i<o->GetChild().size();i++)
		{
			AddObj(o->GetChild()[i]);
		}
		
	}

	void Updata()
	{
		R_Layer->Updata();
		
	}
	Camera* GetCamera()
	{
		return main_camera;
	}

private:
	std::vector<Object*> objs;
	RenderLayer* R_Layer;
	Camera* main_camera;


};

#endif