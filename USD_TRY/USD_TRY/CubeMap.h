#ifndef CUBEMAP_H
#define CUBEMAP_H

#include "glew.h"
#include "glut.h"
#include <iostream>
#include <vector>

#include "Texture_Loader.h"
#include "Fileutil.h"
#include "Math\Math.h"
#include  <memory>
#define PI 3.1415926
typedef  unsigned int uint;
//CubeMap:也应该作为资源来管理,但是由于赶工暂时直接使用,还有关于采样计算球谐基可以单独放在一个线程来进行
class CubeMap
{
public:

	CubeMap(){}
	virtual ~CubeMap()
	{
		glDeleteTextures(1, &ID);
	}


	void Load(std::vector<std::string> faces)
	{
		glGenTextures(1, &ID);
		glBindTexture(GL_TEXTURE_CUBE_MAP, ID);
		for(GLuint i = 0; i < faces.size(); i++)
		{
			std::string res_path =GetResFile_Path(faces[i]);
			Texture_Loader loader;
			if(!loader.Load(res_path.c_str()))
			{
				std::cout<<"load texture"<<res_path.c_str()<<std::endl;
			}

			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB,loader.GetWidth(), loader.GetHeight(), 0, GL_BGR, GL_UNSIGNED_BYTE, loader.GetDate());
			loader.UnLoad();
		}
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
		glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
	}

	
	GLuint GetID()
	{
		return ID;
	}

	std::vector<glm::vec3>  sphericalHarmonicsFromTexture()
	{
		std::vector<glm::vec3> output(9);//储存九个系数结果
	    float count = 0.0f;
		for(uint i=0; i < output.size(); i++)
	    {
		   output[i].x = 0;
		   output[i].y = 0;
		   output[i].z = 0;
	    }

	   // bind current texture
	   glBindTexture(GL_TEXTURE_CUBE_MAP, ID);
	   // for each face of cube texture
		for(int face=0; face < 6; face++)
		{
			GLubyte* data=NULL;//纹理数据
			GLint width, height;//当前纹理的宽高
			GLint internalFormat;//当前纹理的格式
			GLint numComponents;
		    // get width and height
		    glGetTexLevelParameteriv(GL_TEXTURE_CUBE_MAP_POSITIVE_X + face, 0, GL_TEXTURE_WIDTH, &width);
		    glGetTexLevelParameteriv(GL_TEXTURE_CUBE_MAP_POSITIVE_X + face, 0, GL_TEXTURE_HEIGHT, &height);

		    if(width != height)
		    {
				std::cout<<"fail in fun with sphericalHarmonicsFromTexture"<<std::endl;

			}

		    // get format of data in texture
		    glGetTexLevelParameteriv(GL_TEXTURE_CUBE_MAP_POSITIVE_X + face, 0,GL_TEXTURE_INTERNAL_FORMAT, &internalFormat);

		    // get data from texture
		    if(internalFormat == GL_RGBA)
		    {
				numComponents = 4;
				data = new GLubyte[numComponents * width * width];
			}
		    else if(internalFormat == GL_RGB)
		    {
				numComponents = 3;
			    data = new GLubyte[numComponents * width * width];
			}
			else
			{
				std::cout<<"sphericalHarmonicsFromTexture "<<std::endl;

			}
			glGetTexImage(GL_TEXTURE_CUBE_MAP_POSITIVE_X + face, 0, internalFormat, GL_UNSIGNED_BYTE, data);

			// step between two texels for range [0, 1]
			float invWidth = 1.0f / float(width);
			// initial negative bound for range [-1, 1]
			float negativeBound = -1.0f + invWidth;
			// step between two texels for range [-1, 1]
			float invWidthBy2 = 2.0f / float(width);

			for(int y=0; y < width; y++)
			{
				// texture coordinate V in range [-1 to 1]
				const float fV = negativeBound + float(y) * invWidthBy2;

				for(int x=0; x < width; x++)
				{
					// texture coordinate U in range [-1 to 1]
					const float fU = negativeBound + float(x) * invWidthBy2;

					// determine direction from center of cube texture to current texel
					glm::vec3 dir;
					switch(GL_TEXTURE_CUBE_MAP_POSITIVE_X + face)
					{
					   case GL_TEXTURE_CUBE_MAP_POSITIVE_X:
						  dir.x = 1.0f;
						  dir.y = 1.0f - (invWidthBy2 * float(y) + invWidth);
						  dir.z = 1.0f - (invWidthBy2 * float(x) + invWidth);
						  dir = -dir;
						  break;
					   case GL_TEXTURE_CUBE_MAP_NEGATIVE_X:
						  dir.x = -1.0f;
						  dir.y = 1.0f - (invWidthBy2 * float(y) + invWidth);
						  dir.z = -1.0f + (invWidthBy2 * float(x) + invWidth);
						  dir = -dir;
						  break;
					   case GL_TEXTURE_CUBE_MAP_POSITIVE_Y:
						  dir.x = - 1.0f + (invWidthBy2 * float(x) + invWidth);
						  dir.y = 1.0f;
						  dir.z = - 1.0f + (invWidthBy2 * float(y) + invWidth);
						  dir = -dir;
						  break;
					   case GL_TEXTURE_CUBE_MAP_NEGATIVE_Y:
						  dir.x = - 1.0f + (invWidthBy2 * float(x) + invWidth);
						  dir.y = - 1.0f;
						  dir.z = 1.0f - (invWidthBy2 * float(y) + invWidth);
						  dir = -dir;
						  break;
					   case GL_TEXTURE_CUBE_MAP_POSITIVE_Z:
						  dir.x = - 1.0f + (invWidthBy2 * float(x) + invWidth);
						  dir.y = 1.0f - (invWidthBy2 * float(y) + invWidth);
						  dir.z = 1.0f;
						  break;
					   case GL_TEXTURE_CUBE_MAP_NEGATIVE_Z:
						  dir.x = 1.0f - (invWidthBy2 * float(x) + invWidth);
						  dir.y = 1.0f - (invWidthBy2 * float(y) + invWidth);
						  dir.z = - 1.0f;
						  break;
					   default:
							std::cout<<"sphericalHarmonicsFromTexture "<<std::endl;
					}
					// normalize direction
					dir = glm::normalize(dir);

					float r = sqrt(dir.x*dir.x + dir.y*dir.y + dir.z*dir.z);
					std::vector<float> basis=sphericalHarmonicsEvaluateDirection(dir,r);
					// index of texel in texture
					uint pixOffsetIndex = (x + y * width) * numComponents;
					// get color from texture and map to range [0, 1]
					glm::vec3 clr(
						  float(data[pixOffsetIndex]) / 255,
						  float(data[pixOffsetIndex+1]) / 255,
						  float(data[pixOffsetIndex+2]) / 255);
					for(int i=0;i<output.size();i++)
					{
						output[i].r+=clr.r*basis[i];
						output[i].g+=clr.g*basis[i];
						output[i].b+=clr.b*basis[i];
					}
					count++;
				}//width
			}//height
			if(data!=NULL)
				delete data;
		}

		const float fNormProj = (4.0f * PI) / count;
	    // save result
		for(uint i=0; i < output.size(); i++)
		{
			output[i].r *= fNormProj;
			output[i].g *= fNormProj;
			output[i].b *= fNormProj;
		}
		return output;
	}
private:

	std::vector<float> sphericalHarmonicsEvaluateDirection(const glm::vec3 & dir,float r)
	{
		float r2=r*r;
		std::vector<float> basis(9);
		//计算三个级数的球谐函数
		float x=dir.x;
		float y=dir.y;
		float z=dir.z;

		basis[0] = 1.f / 2.f * sqrt(1.f / PI);
		basis[1] = sqrt(3.f / (4.f*PI))*y / r;
		basis[2] = sqrt(3.f / (4.f*PI))*z / r;
		basis[3] = sqrt(3.f / (4.f*PI))*x / r;
		basis[4] = 1.f / 2.f * sqrt(15.f / PI) * x * y / r2;
		basis[5] = 1.f / 2.f * sqrt(15.f / PI) * y * z / r2;
		basis[6] = 1.f / 4.f * sqrt(5.f / PI) * (-x*x - y*y + 2 * z*z) / r2;
		basis[7] = 1.f / 2.f * sqrt(15.f / PI) * z * x / r2;
		basis[8] = 1.f / 4.f * sqrt(15.f / PI) * (x*x - y*y) / r2;
		return basis;
	}

private:
	GLuint ID;

};

#endif