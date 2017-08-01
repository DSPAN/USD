#ifndef PREFAB_H
#define PREFAB_H
#include "global.h"
#include "Object.h"
#include "rapidjson/document.h"
#include "rapidjson/stringbuffer.h"
#include "rapidjson/writer.h"

Object* Prefab_Loader(std::string file,glm::vec3 scale);
#endif