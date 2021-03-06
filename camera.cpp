#include "camera.h"

#include <glm/gtx/transform.hpp>

extern int win_width;
extern int win_height;
extern bool mouseMove;
extern bool rotate;

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

    glm::vec3 camRight = glm::normalize(glm::cross(look, up));

    if (keys['w'])
        camPos += speed * fElapsedTime * look;
    if (keys['s'])
        camPos -= speed * fElapsedTime * look;
    if (keys['d'])
        camPos += speed * fElapsedTime * camRight;
    if (keys['a'])
        camPos -= speed * fElapsedTime * camRight;
    if (keys['q'])
        camPos.y += speed * fElapsedTime;
    if (keys['e'])
        camPos.y -= speed * fElapsedTime;

    static bool pressionadoT;
    if (keys['t'] && !pressionadoT)
        rotate = !rotate;
    pressionadoT = keys['t'];

    static bool pressionadoSpc;
    if (keys[0x20] && !pressionadoSpc)
        mouseMove = !mouseMove;
    pressionadoSpc = keys[0x20];

    if (mouseMove)
    {
        look = glm::rotate(glm::radians((float)(win_width / 2 - x) * 0.2f), glm::vec3(0.0f, 1.0f, 0.0f)) * glm::vec4(look, 1.0f);
        look = glm::rotate(glm::radians((float)(win_height / 2 - y) * 0.2f), camRight) * glm::vec4(look, 1.0f);
    }
}