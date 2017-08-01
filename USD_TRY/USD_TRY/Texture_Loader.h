#ifndef TEXTURE_LOADER_H
#define TEXTURE_LOADER_H


#include <FreeImage.h>
#pragma comment(lib, "FreeImage.lib")
class Texture_Loader
{
public:
    Texture_Loader()
	{
		dib=NULL;
	}
	~Texture_Loader(){}
    bool Load(const char* name)
	{
		FREE_IMAGE_FORMAT fif = FIF_UNKNOWN;
		fif = FreeImage_GetFileType(name, 0);
		if(fif == FIF_UNKNOWN)
		{
			fif = FreeImage_GetFIFFromFilename(name);
		}

		if(fif == FIF_UNKNOWN)
			return false;

	    if(FreeImage_FIFSupportsReading(fif))
		{
			dib = FreeImage_Load(fif, name);
		}

		if(!dib)
			return false;
		return true;
	}
    int GetHeight(){return FreeImage_GetHeight(dib);}
    int GetWidth(){return FreeImage_GetWidth(dib);}
    BYTE* GetDate(){return FreeImage_GetBits(dib);}
    void UnLoad()
	{
		FreeImage_Unload(dib);
	}
private:
    FIBITMAP *dib;
};


#endif