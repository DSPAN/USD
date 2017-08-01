#ifndef RENDER_LAYER_H
#define RENDER_LAYER_H
#include "Object.h"
#include "Camera.h"
#include "CubeMap.h"
#include "GBuffer.h"
#include "FrameBuffer.h"
#include <vector>
//渲染层:只负责组件的渲染并不负责组件的删除
class RenderLayer
{
public:
	RenderLayer()
	{
			GLfloat skyboxVertices[] ={
		  -10.0f,  10.0f, -10.0f,
		  -10.0f, -10.0f, -10.0f,
		   10.0f, -10.0f, -10.0f,
		   10.0f, -10.0f, -10.0f,
		   10.0f,  10.0f, -10.0f,
		  -10.0f,  10.0f, -10.0f,
  
		  -10.0f, -10.0f,  10.0f,
		  -10.0f, -10.0f, -10.0f,
		  -10.0f,  10.0f, -10.0f,
		  -10.0f,  10.0f, -10.0f,
		  -10.0f,  10.0f,  10.0f,
		  -10.0f, -10.0f,  10.0f,
  
		   10.0f, -10.0f, -10.0f,
		   10.0f, -10.0f,  10.0f,
		   10.0f,  10.0f,  10.0f,
		   10.0f,  10.0f,  10.0f,
		   10.0f,  10.0f, -10.0f,
		   10.0f, -10.0f, -10.0f,
   
		  -10.0f, -10.0f,  10.0f,
		  -10.0f,  10.0f,  10.0f,
		   10.0f,  10.0f,  10.0f,
		   10.0f,  10.0f,  10.0f,
		   10.0f, -10.0f,  10.0f,
		  -10.0f, -10.0f,  10.0f,
  
		  -10.0f,  10.0f, -10.0f,
		   10.0f,  10.0f, -10.0f,
		   10.0f,  10.0f,  10.0f,
		   10.0f,  10.0f,  10.0f,
		  -10.0f,  10.0f,  10.0f,
		  -10.0f,  10.0f, -10.0f,
  
		  -10.0f, -10.0f, -10.0f,
		  -10.0f, -10.0f,  10.0f,
		   10.0f, -10.0f, -10.0f,
		   10.0f, -10.0f, -10.0f,
		  -10.0f, -10.0f,  10.0f,
		   10.0f, -10.0f,  10.0f
		};
		glGenVertexArrays(1, &skyboxVAO);
		glGenBuffers(1, &skyboxVBO);
		glBindVertexArray(skyboxVAO);
		glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
		glBindVertexArray(0);
		//开始加载Shader
		skybox_shader=usd_ShaderManager->load("skybox.shader");
		LightingPass=usd_ShaderManager->load("LightingPass.shader");
		FXAA_HDR=usd_ShaderManager->load("FXAA_HDR.shader");
		SSR=usd_ShaderManager->load("SSR_Pass.shader");
		//创建帧缓存对象
		L_Pass=new FrameBuffer(800,600);
		SSR_Pass=new FrameBuffer(800,600);
		//创建G-Buffer
		m_Gbuffer=new GBuffer(800,600);
		GLfloat quadVertices[] =
        {
            // Positions        // Texture Coords
            -1.0f, 1.0f, 0.0f, 0.0f, 1.0f,
            -1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
            1.0f, 1.0f, 0.0f, 1.0f, 1.0f,
            1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
        };
        // Setup plane VAO
        GLuint quadVBO;
        glGenVertexArrays(1, &quadVAO);
        glGenBuffers(1, &quadVBO);
        glBindVertexArray(quadVAO);
        glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));

		//加载CubeMap信息
		skybox=new CubeMap();
		std::vector<std::string> ts;
		ts.push_back("right.jpg");
		ts.push_back("left.jpg");
		ts.push_back("top.jpg");
		ts.push_back("bottom.jpg");
		ts.push_back("back.jpg");
		ts.push_back("front.jpg");
		skybox->Load(ts);
		//存放系数
		c.push_back(glm::vec3(1.5708,1.81037,2.02323));
		c.push_back(glm::vec3(-0.39426,-0.566373,-0.781223));
		c.push_back(glm::vec3(-0.171695,-0.156558,-0.128591));
		c.push_back(glm::vec3(-0.0515858,-0.0748879,-0.0892103));
		c.push_back(glm::vec3(0.111305,0.101288,0.0827074));
		c.push_back(glm::vec3(-0.125341,-0.133175,-0.131456));
		c.push_back(glm::vec3(0.300504,0.255417,0.17619));
		c.push_back(glm::vec3(-0.239875,-0.204021,-0.154055));
		c.push_back(glm::vec3(0.260582,0.257831,0.19751));
	}
	virtual ~RenderLayer()
	{
		Clear();
		glDeleteVertexArrays(1, &skyboxVAO);//quadVAO
		glDeleteVertexArrays(1, &quadVAO);
		glDeleteBuffers(1, &skyboxVBO);

	}
	void Updata()
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		

		//m_Gbuffer->Bind();
		glBindFramebuffer(GL_FRAMEBUFFER,m_Gbuffer->GetID() );
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		//天空盒渲染
		skybox_shader->Use();
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_CUBE_MAP, skybox->GetID());
		skybox_shader->SetInteger("cube_texture", 0,true);
		skybox_shader->SetMatrix4("view",glm::mat4(glm::mat3(camera_->GetView())),true);
		skybox_shader->SetMatrix4("projection",camera_->GetProj(),true);
		glBindVertexArray(skyboxVAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glBindVertexArray(0);
		//填充步骤可优化
		for(int i=0;i<RenderComponents_.size();i++)
		{
			RenderComponents_[i]->GetMateria()->GetShader()->Use();
			RenderComponents_[i]->GetMateria()->BindTexture();
			RenderComponents_[i]->GetMateria()->GetShader()->SetMatrix4("model",RenderComponents_[i]->GetObj()->GetTransformation(),true);
			RenderComponents_[i]->GetMateria()->GetShader()->SetMatrix4("view",camera_->GetView(),true);
			RenderComponents_[i]->GetMateria()->GetShader()->SetMatrix4("projection",camera_->GetProj(),true);
			//设置材质参数:以后修改成从材质文件中自动赋值
			RenderComponents_[i]->GetMateria()->GetShader()->SetFloat("metallic",0.5,true);
			RenderComponents_[i]->GetMateria()->GetShader()->SetFloat("roughness",1,true);
			RenderComponents_[i]->GetMateria()->GetShader()->SetFloat("ao",1,true);
			RenderComponents_[i]->GetMesh()->Draw();
		}
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		//光照阶段
		L_Pass->Bind();

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		LightingPass->Use();
        glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D,m_Gbuffer->GetgPosition());
        LightingPass->SetInteger("gPosition",0,true);
        glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D,m_Gbuffer->GetgNormal());
        LightingPass->SetInteger("gNormal",1,true);
        glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D,m_Gbuffer->GetgAlbedoSpec());
        LightingPass->SetInteger("gAlbedoSpec",2,true);
        glActiveTexture(GL_TEXTURE3);
		glBindTexture(GL_TEXTURE_2D,m_Gbuffer->GetgMateria());
        LightingPass->SetInteger("gMateria",3,true);
		LightingPass->SetVector3f("camPos",camera_->GetPos(),true);
		RenderQuad();
		L_Pass->UnBind();

		/*
		//SSR未完成
		SSR_Pass->Bind();

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		SSR->Use();
        glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D,m_Gbuffer->GetgPosition());
        SSR->SetInteger("gPosition",0,true);
        glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D,m_Gbuffer->GetgNormal());
        SSR->SetInteger("gNormal",1,true);
        glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D,m_Gbuffer->GetgAlbedoSpec());
        SSR->SetInteger("gAlbedoSpec",2,true);
        glActiveTexture(GL_TEXTURE3);
		glBindTexture(GL_TEXTURE_2D,L_Pass->GetTextureID());
        SSR->SetInteger("effect_map",3,true);
		SSR->SetMatrix4("projection",camera_->GetProj()*camera_->GetView(),true);
		SSR->SetVector3f("cameraPos",camera_->GetPos(),true);
		RenderQuad();
		SSR_Pass->UnBind();
		*/


		//后期处理阶段:HDR,FXAA...
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		FXAA_HDR->Use();
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D,L_Pass->GetTextureID());
        FXAA_HDR->SetInteger("buf0",0,true);
		FXAA_HDR->SetVector2f("frameBufSize",glm::vec2(800,600),true);
		RenderQuad();
		


	}
	void RenderQuad()
    {
        glBindVertexArray(quadVAO);
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
        glBindVertexArray(0);
    }

	void Add(RenderComponent* rc)
	{
		if(rc==NULL)
			return ;
		//添加渲染组件
		RenderComponents_.push_back(rc);
	}
	void Clear()
	{
		//清空组件
		RenderComponents_.clear();
		if(skybox!=NULL)
			delete skybox;
		if(m_Gbuffer!=NULL)
			delete m_Gbuffer;
		if(L_Pass!=NULL)
			delete L_Pass;
		if(SSR_Pass!=NULL)
			delete SSR_Pass;
	}
	void SetCamera(Camera* c)
	{
		camera_=c;
	}
private:
	std::vector<RenderComponent*> RenderComponents_;//场景对象的渲染组件
	Camera* camera_;
	std::vector<glm::vec3> c;
	GLuint skyboxVAO;
	GLuint skyboxVBO;
	GLuint quadVAO;
	ShaderPtr skybox_shader;
	ShaderPtr LightingPass;
	ShaderPtr FXAA_HDR;
	ShaderPtr SSR;
	FrameBuffer* L_Pass;//存储光照阶段后的帧缓存
	FrameBuffer* SSR_Pass;
	CubeMap* skybox;
	GBuffer* m_Gbuffer;
	

};

#endif