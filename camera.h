#ifndef CAMERA_H
#define CAMERA_H

#include <glm/glm.hpp>
#include <unordered_map>

class Camera
{
public:
    Camera();
    ~Camera();
    void update(float fElapsedTime, std::unordered_map<char, bool> keys, int x, int y);
    glm::vec3 getPos()
    {
        return camPos;
    }
    glm::vec3 getLook()
    {
        return look;
    }
    glm::vec3 getUp()
    {
        return glm::vec3(0.0f, 1.0f, 0.0f);
    }

private:
    glm::vec3 camPos;
    glm::vec3 look;
};

#endif