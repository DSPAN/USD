#ifndef SCENE_H
#define SCENE_H
#include "Object.h"
#include "RenderLayer.h"
#include "Camera.h"

#include <vector>
//场景:包含渲染层,以及其他组件层
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
		//析构时先释放组件层指针
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
		//添加场景对象,添加场景对象时应该把该场景对象的子对象也添加进来
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