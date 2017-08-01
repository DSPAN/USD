#ifndef CMAERA_H
#define CMAERA_H

#include "Math\Math.h"
#define M_PI 3.1415926

class Camera
{
public:
    Camera();
	~Camera(){}
    void T_Forward(float speed);
    void T_Right(float speed);
    void T_UP(float speed);
    void roll(float angle);
    void pitch(float angle);
    void yaw(float angle);
    void Update();

    glm::mat4 GetView(){return view;}
    glm::mat4 GetProj(){return m_proj;}
    glm::vec3 GetPos() {return m_pos;}
private:
    glm::vec3 m_pos;
    glm::vec3 m_look;
    glm::vec3 m_up;

    glm::vec3 N;
    glm::vec3 U;
    glm::vec3 V;

    glm::mat4 view;
    glm::mat4 m_proj;
    float m_aspect;
    float m_fovY;
    float m_nearZ;
    float m_farZ;
};
#endif