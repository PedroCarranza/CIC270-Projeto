#include <iostream>
#include <unordered_map>

#include <GL/glew.h>
#include <GL/freeglut.h>

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "./lib/shader.h"
#include "./lib/texture.h"
#include "mesh.h"
#include "camera.h"
#define STB_IMAGE_IMPLEMENTATION
#include "./lib/stb_image.h"

int win_width = 1280;
int win_height = 720;
int mX, mY;

std::unordered_map<char, bool> keys;

glm::mat4 proj = glm::perspective(glm::radians(70.0f), 16.0f / 9.0f, 0.1f, -100.0f);

Shader *shad;
Mesh *me;
Camera *cam;
Texture *tex;

int lastTime = 0;
float rotation = 0;
bool mouseMove = false;

void display()
{
    glClearColor(0.0f, 0.0f, 0.0f, 1.0); //fundo preto
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    glm::mat4 model = glm::mat4(1);
    glm::mat4 view = glm::lookAt(cam->getPos(), cam->getPos() + cam->getLook(), cam->getUp());
    tex->Bind();

    shad->Bind();
    shad->setUniformMat4f("model", model);
    shad->setUniformMat4f("view", view);
    shad->setUniformMat4f("projection", proj);
    shad->setUniform3f("objectColor", 1.0f, 1.0f, 0.0f);
    shad->setUniform3f("lightColor", 1.0f, 1.0f, 1.0f);
    shad->setUniform3f("lightPosition", 0.0f, 0.0f, 0.0f);
    shad->setUniform3f("cameraPosition", cam->getPos());
    shad->setUniform1i("isSun", true);
    me->Draw();

    model = glm::translate(glm::vec3(5.0f, 0.0f, 0.0f));
    model = glm::rotate(glm::radians(rotation), glm::vec3(0.0f, 1.0f, 0.0f)) * model * glm::scale(glm::vec3(0.5f, 0.5f, 0.5f)) * glm::rotate(glm::radians(rotation) * 365.24f, glm::vec3(0.0f, 1.0f, 0.0f));
    shad->setUniformMat4f("model", model);
    shad->setUniform3f("objectColor", 0.0f, 0.0f, 1.0f);
    shad->setUniform1i("isSun", false);
    me->Draw();

    glutSwapBuffers();
}

void reshape(int width, int height)
{
    glutReshapeWindow(win_width, win_height);
}

void idle()
{
    int now = glutGet(GLUT_ELAPSED_TIME);
    float elapsedTime = (now - lastTime) / 1000.0f;
    lastTime = now;

    rotation += 10 * elapsedTime;
    cam->update(elapsedTime, keys, mX, mY);
    if (mouseMove)
    {
        glutSetCursor(GLUT_CURSOR_NONE);
        glutWarpPointer(win_width / 2, win_height / 2);
    }
    else
    {
        mX = win_width / 2;
        mY = win_height / 2;
        glutSetCursor(GLUT_CURSOR_LEFT_ARROW);
    }

    glutPostRedisplay();
}

void keyDown(unsigned char key, int x, int y)
{
    if (key == 27)
        exit(0);
    keys[key] = true;
}

void keyUp(unsigned char key, int x, int y)
{
    keys[key] = false;
}

void mPos(int x, int y)
{
    mX = x;
    mY = y;
}

int main(int argc, char **argv)
{

    glutInit(&argc, argv);
    glutInitContextVersion(3, 3);
    glutInitContextProfile(GLUT_CORE_PROFILE);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
    glutInitWindowSize(win_width, win_height);
    glutCreateWindow(argv[0]);
    glewExperimental = GL_TRUE;
    glewInit();

    shad = new Shader("res/test");
    tex = new Texture("res/irth.jpg");
    me = new Mesh("sphere.obj");
    cam = new Camera();

    glEnable(GL_DEPTH_TEST);

    glutSetKeyRepeat(GLUT_KEY_REPEAT_OFF);

    glutReshapeFunc(reshape);
    glutDisplayFunc(display);
    glutKeyboardFunc(keyDown);
    glutKeyboardUpFunc(keyUp);
    glutPassiveMotionFunc(mPos);
    glutIdleFunc(idle);

    glutMainLoop();

    delete shad;
    delete me;
    delete cam;
}
