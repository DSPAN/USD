#include "Prefab.h"
Object* Prefab_Loader(std::string file,glm::vec3 scale)
{
	Object* p=NULL;
	//加载预设体资源
	std::string res_path =GetResFile_Path(file);
	//此处应添加一个判断资源文件是否存在的判断,赶工暂时不实现

	
	std::string _jsonBuffer =loadDataFromFile(res_path.c_str());
	rapidjson::Document _jsonReader;
	if (_jsonReader.ParseInsitu<0>((char*)_jsonBuffer.c_str()).HasParseError())
	{
		std::cout<<"Parse json failed in Prefab.h"<<std::endl;
		return p;
	}
	p=new Object;
	const rapidjson::Value& obj_array = _jsonReader["objs"];
    for (rapidjson::SizeType index = 0, obj_array_size = obj_array.Size(); index < obj_array_size; ++index)
    {
		Object* c=new Object;
		
		const rapidjson::Value& obj_data = obj_array[index];
		const rapidjson::Value& mesh_path = obj_data["mesh"];
		const rapidjson::Value& mat_path = obj_data["mat"];
		
		RenderComponent* rc=new RenderComponent();
		rc->SetMesh(usd_MeshManager->load(mesh_path.GetString()));
		rc->SetMateria(usd_MateriaManager->load(mat_path.GetString()));
		rc->SetObject(c);
		c->SetRenderComponent(rc);
		p->attachChild(c);

	}
	p->SetPos(glm::vec3(0,0,-5));
	p->SetScale(scale);//glm::vec3(0.01,0.01,0.01)

	return p;

}