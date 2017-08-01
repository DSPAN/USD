#include "Math\Math.h"
glm::mat4 InitTranslation(glm::vec3 x)
{
    glm::mat4 m;
    m[3][0]=x.x;
    m[3][1]=x.y;
    m[3][2]=x.z;
    return m;
}
glm::mat4 InitScale(glm::vec3 x)
{
    glm::mat4 m;
    m[0][0]=x.x;
    m[1][1]=x.y;
    m[2][2]=x.z;
    return m;
}