#include "Camera.h"

Camera::Camera():
    m_up(0.f,1.f,0.f),
    m_look(0.f,0.f,1.f),
    m_pos(0.0f,0.0f,0.0f),
    m_aspect(800.f/600),
    m_fovY(M_PI*0.25),
    m_nearZ(1.0f),
    m_farZ(100.f)
{
    m_proj=glm::perspective(m_fovY,m_aspect,m_nearZ,m_farZ);
    N=glm::normalize(m_look - m_pos);
    U=glm::normalize(glm::cross(m_up, N));
    V=glm::cross(N,U);
    Update();
}

void Camera::T_Forward(float speed)
{
    m_pos+=speed*N;
    Update();
}

void Camera::T_Right(float speed)
{
    m_pos+=speed*U;
    Update();
}

void Camera::T_UP(float speed)
{
    m_pos+=speed*V;
    Update();
}

void Camera::roll(float angle)
{
    float cs=cos(angle*M_PI/180);
    float sn=sin(angle*M_PI/180);
    glm::vec3 t=U;
    glm::vec3 s=V;
    U.x=cs*t.x-sn*s.x;
    U.y=cs*t.y-sn*s.y;
    U.z=cs*t.z-sn*s.z;

    V.x=sn*t.x+cs*s.x;
    V.y=sn*t.y+cs*s.y;
    V.z=sn*t.z+cs*s.z;
    Update();
}
void Camera::pitch(float angle)
{
    float cs=cos(angle*M_PI/180);
    float sn=sin(angle*M_PI/180);
    glm::vec3 t=V;
    glm::vec3 s=N;
    V.x=cs*t.x-sn*s.x;
    V.y=cs*t.y-sn*s.y;
    V.z=cs*t.z-sn*s.z;

    N.x=sn*t.x+cs*s.x;
    N.y=sn*t.y+cs*s.y;
    N.z=sn*t.z+cs*s.z;
    Update();
}
void Camera::yaw(float angle)
{
    float cs=cos(angle*M_PI/180);
    float sn=sin(angle*M_PI/180);
    glm::vec3 t=N;
    glm::vec3 s=U;
    N.x=cs*t.x-sn*s.x;
    N.y=cs*t.y-sn*s.y;
    N.z=cs*t.z-sn*s.z;

    U.x=sn*t.x+cs*s.x;
    U.y=sn*t.y+cs*s.y;
    U.z=sn*t.z+cs*s.z;
    Update();
}

void Camera::Update()
{
    glm::mat4 m;
    m[0][0] = U.x;   m[1][0] = U.y;   m[2][0] = U.z;   m[3][0] = -glm::dot(U,m_pos);
    m[0][1] = V.x;   m[1][1] = V.y;   m[2][1] = V.z;   m[3][1] = -glm::dot(V,m_pos);
    m[0][2] = N.x;   m[1][2] = N.y;   m[2][2] = N.z;   m[3][2] = -glm::dot(N,m_pos);
    m[0][3] = 0.0f;  m[1][3] = 0.0f;  m[2][3] = 0.0f;  m[3][3] = 1.0f;
    view=m;
}

