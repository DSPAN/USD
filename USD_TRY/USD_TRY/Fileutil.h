#ifndef FILEUTIL_H
#define FILEUTIL_H

#include <iostream>
#include <windows.h>
#include <string>
#include <fstream>
#include <sstream>


std::string GetCurPath();

std::string GetFileExt(std::string filename);

std::string GetResFile_Path(std::string res_name);

std::string loadDataFromFile(const char *File);

//�ж��ļ��Ƿ����
#endif