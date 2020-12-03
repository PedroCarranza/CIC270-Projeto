#include <stdio.h>
#include <stdlib.h>
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <map>
#include "./lib/shader.h"
#define STB_IMAGE_IMPLEMENTATION
#include "./lib/stb_image.h"

int win_width = 800;
int win_height = 800;

unsigned int VAO;
unsigned int VBO;

Shader *shad;

int lastTime = 0;

void display()
{
    glClearColor(1.0, 1.0, 1.0, 1.0); //fundo branco
    glClear(GL_COLOR_BUFFER_BIT);
    if (shad)
        shad->Bind();
    glBindVertexArray(VAO);

    glDrawArrays(GL_TRIANGLES, 0, 3);

    glutSwapBuffers();
}

/**
 * Reshape function.
 *
 * Called when window is resized.
 *
 * @param width New window width.
 * @param height New window height.
 */
void reshape(int width, int height)
{
    win_width = width;
    win_height = height;
    glViewport(0, 0, width, height);
    glutPostRedisplay();
}

void idle()
{
    int now = glutGet(GLUT_ELAPSED_TIME);
    float elapsedTime = (now - lastTime) / 1000.0f;
    lastTime = now;

    glutPostRedisplay();
}

void initData()
{
    float vertices[] = {
        -0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f,
        0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
        0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f};

    // Vertex array.
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    // Vertex buffer
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // Set attributes.
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)(3 * sizeof(float)));
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);

    // Unbind Vertex Array Object.
    glBindVertexArray(0);
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

    /* Initialize the points of image */
    initData();

    glutSetKeyRepeat(GLUT_KEY_REPEAT_OFF);

    glutReshapeFunc(reshape);
    glutDisplayFunc(display);
    glutIdleFunc(idle);

    glutMainLoop();
}
