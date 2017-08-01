#ifndef FRAMEBUFFER_H
#define FRAMEBUFFER_H
#include "glew.h"
#include "glut.h"
#include <iostream>
//帧缓存对象:后续将添加扩展选项,和修改生成阴影贴图的帧缓存继承该类
class FrameBuffer
{
public:
	FrameBuffer(int w,int h)
	{
		width=w;
		height=h;
		glGenFramebuffers(1, &FB_ID);
        glBindFramebuffer(GL_FRAMEBUFFER, FB_ID);
        // - SSAO color buffer
        glGenTextures(1, &FB_TEX);
        glBindTexture(GL_TEXTURE_2D, FB_TEX);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_FLOAT, NULL);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, FB_TEX, 0);
        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
            std::cout << "Framebuffer not complete!" << std::endl;
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
		std::cout<<FB_ID<<"->"<<FB_TEX<<std::endl;

	}
	~FrameBuffer()
	{
		glDeleteFramebuffers(1, &FB_ID);
		glDeleteTextures(1, &FB_TEX);
	}
	void Bind()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, FB_ID);
	}
	void UnBind()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}
	GLuint GetTextureID()
	{
		return FB_TEX;
	}

private:
	int width;
	int height;
	GLuint FB_ID;
    GLuint FB_TEX;//帧缓存纹理


	

};

#endif