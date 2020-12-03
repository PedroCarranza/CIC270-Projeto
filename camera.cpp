#include "camera.h"

#include <glm/gtx/transform.hpp>

extern int win_width;
extern int win_height;

Camera::Camera() : camPos(glm::vec3(0.0f, 0.0f, 5.0f)), look(glm::vec3(0.0f, 0.0f, -1.0f))
{
}

Camera::~Camera()
{
}

void Camera::update(float fElapsedTime, std::unordered_map<char, bool> keys, int x, int y)
{
    float speed = 1.0f;
    glm::vec3 up(0.0f, 1.0f, 0.0f);

    glm::vec3 camRight = glm::normalize(glm::cross(up, glm::vec3(look.x, 0.0f, look.z)));

    if (keys['w'])
        camPos += speed * fElapsedTime * glm::vec3(look.x, 0.0f, look.z);
    if (keys['s'])
        camPos -= speed * fElapsedTime * glm::vec3(look.x, 0.0f, look.z);
    if (keys['d'])
        camPos -= speed * fElapsedTime * camRight;
    if (keys['a'])
        camPos += speed * fElapsedTime * camRight;

    if (keys[0x20])
        camPos.y += speed * fElapsedTime;

    look = glm::rotate(glm::radians((float)(win_width / 2 - x) * 0.2f), glm::vec3(0.0f, 1.0f, 0.0f)) * glm::vec4(look, 1.0f);
    look = glm::rotate(glm::radians((float)(y - win_height / 2) * 0.2f), camRight) * glm::vec4(look, 1.0f);
}