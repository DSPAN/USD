#include "Fileutil.h"
std::string GetCurPath()
{
	char buf[100];
	GetCurrentDirectory(100,buf);  //得到当前工作路径
	std::string path=buf;
	return path;
}

std::string GetFileExt(std::string filename)
{
	std::string suffixStr = filename.substr(filename.find_last_of('.') + 1);//获取文件后缀  
	return suffixStr;
}

std::string GetResFile_Path(std::string res_name)
{
	std::string cur_path=GetCurPath();
	std::string assert_path="/Asset/";
	std::string ext=GetFileExt(res_name);
	if(ext=="mesh")
	{
		assert_path+="Mesh/";
	}
	else if(ext=="p")
	{
		assert_path+="Prefab/";
	}
	else if(ext=="mat")
	{
		assert_path+="Mat/";
	}
	else if(ext=="shader"||ext=="vs"||ext=="frag")
	{
		assert_path+="Shader/";
	}
	else if(ext=="tga"||ext=="png"||ext=="jpg")
	{
		assert_path+="Texture/";
	}

	
	std::string path=cur_path+assert_path+res_name;
	std::string::iterator it = path.begin();
	for (; it != path.end(); ++it)
	{
		if ( *it == '\\')
		{
			*it = '/';
		}
	}
	return path;
}

std::string loadDataFromFile(const char *File)
{
    std::string Buffer;
    try
    {
        // Open files
        std::ifstream T_File(File);
        std::stringstream Stream;
        // Read file's buffer contents into streams
        Stream << T_File.rdbuf();
        // close file handlers
        T_File.close();
        // Convert stream into string
        Buffer = Stream.str();
    }
    catch (std::exception e)
    {
        std::cout << "ERROR: Failed to read  files" << std::endl;
    }

    return Buffer;
}