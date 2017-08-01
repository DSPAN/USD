#ifndef GBUFFER_H
#define GBUFFER_H
#include "glew.h"
#include "glut.h"
#include <iostream>
class GBuffer
{
public:
	GBuffer(int w,int h)
	{
		width=w;
		height=h;
		glGenFramebuffers(1, &gBuffer);
		glBindFramebuffer(GL_FRAMEBUFFER, gBuffer);
        // - Position color buffer
        glGenTextures(1, &gPosition);
        glBindTexture(GL_TEXTURE_2D, gPosition);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, width, height, 0, GL_RGBA, GL_FLOAT, NULL);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, gPosition, 0);
        // - Normal color buffer
        glGenTextures(1, &gNormal);
        glBindTexture(GL_TEXTURE_2D, gNormal);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, width, height, 0, GL_RGB, GL_FLOAT, NULL);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, gNormal, 0);
        // - Color + Specular color buffer
        glGenTextures(1, &gAlbedoSpec);
        glBindTexture(GL_TEXTURE_2D, gAlbedoSpec);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, gAlbedoSpec, 0);
		
        // - Pick color buffer拾取贴图暂时不加入
        glGenTextures(1, &gMateria);
        glBindTexture(GL_TEXTURE_2D, gMateria);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, width, height, 0, GL_RGB, GL_FLOAT, NULL);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT3, GL_TEXTURE_2D, gMateria, 0);
		

        GLuint attachments[4] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2,GL_COLOR_ATTACHMENT3};

        glDrawBuffers(4, attachments);
        // - Create and attach depth buffer (renderbuffer)

        GLuint rbo1;
        glGenRenderbuffers(1, &rbo1);
        glBindRenderbuffer(GL_RENDERBUFFER, rbo1);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
        glBindRenderbuffer(GL_RENDERBUFFER, 0);
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo1);

        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        {
            std::cout<<"GBUFFER error, status: 0x%x\n"<<std::endl;
        }

	}
	~GBuffer()
	{
		glDeleteFramebuffers(1, &gBuffer);
		glDeleteTextures(1, &gPosition);
		glDeleteTextures(1, &gNormal);
		glDeleteTextures(1, &gAlbedoSpec);
		glDeleteTextures(1, &gMateria);
	}
	void Bind()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, gBuffer);
		//glViewport(0, 0, width, height);
	}
	void unBind()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}
	GLuint GetID()
	{
		return gBuffer;
	}
	GLuint GetgPosition()
	{
		return gPosition;
	}
	GLuint GetgNormal()
	{
		return gNormal;
	}
	GLuint GetgAlbedoSpec()
	{
		return gAlbedoSpec;
	}
	GLuint GetgMateria()
	{
		return gMateria;
	}

private:
	int width;
	int height;
	GLuint gBuffer;
    GLuint gPosition;//G缓存位置
    GLuint gNormal; //G缓存法线
    GLuint gAlbedoSpec;//G缓存颜色
	GLuint gMateria;//G缓存材质

};


#endif