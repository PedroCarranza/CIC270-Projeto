#include <stdio.h>
#include <stdlib.h>
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "./lib/utils.h"
#define STB_IMAGE_IMPLEMENTATION
#include "./lib/stb_image.h"
#include <map>

/* Globals */
/** Window width. */
int win_width = 800;
/** Window height. */
int win_height = 800;

/** Program variable. */
int program;
/** Vertex array object. */
unsigned int VAO;
/** Vertex buffer object. */
unsigned int VBO;

/** Texture */
unsigned int texture;

/** Vertex size */
int size;

/** Image parameters */
int imagewidth, imageheight, imagechannels;

/** Transform */
glm::vec3 desloc = {0.0f, 0.0f, -1.5f};
glm::vec3 rot = {0, 0, 0};
glm::vec2 scale = {1, 1};

std::map<unsigned char, bool> teclas;

/** Primitive */
unsigned int primitive = GL_TRIANGLES;

bool useTexture = false;

unsigned char modo = 't';

int lastTime = 0;

/** Vertex shader. */
const char *vertex_code = "\n"
                          "#version 330 core\n"
                          "layout (location = 0) in vec2 position;\n"
                          "layout (location = 1) in vec2 textureCoord;\n"
                          "\n"
                          "out vec2 v_TexCoord;\n"
                          "\n"
                          "uniform float yOffset;\n"
                          "uniform mat4 transform;\n"
                          "uniform mat4 projection;\n"
                          "\n"
                          "void main()\n"
                          "{\n"
                          "    gl_Position = projection * transform * vec4(position.x-0.5, position.y-0.5*yOffset, 0, 1.0);\n"
                          "    v_TexCoord = textureCoord;\n"
                          "}\0";

/** Fragment shader. */
const char *fragment_code = "\n"
                            "#version 330 core\n"
                            "in vec2 v_TexCoord;\n"
                            "\n"
                            "out vec4 FragColor;\n"
                            "\n"
                            "uniform bool useTex;\n"
                            "uniform sampler2D u_Texture;\n"
                            "\n"
                            "void main()\n"
                            "{\n"
                            "    if(useTex)\n"
                            "    FragColor = texture(u_Texture, v_TexCoord);\n"
                            "    else\n"
                            "    FragColor = vec4(0.0f, 0.0f, 0.0f, 1.0f);\n"
                            "}\0";

/* Functions. */
void display(void);
void reshape(int, int);
void keyboard(unsigned char, int, int);
void initData(void);
void initShaders(void);

/** 
 * Drawing function.
 */
void display()
{
    glClearColor(1.0, 1.0, 1.0, 1.0); //fundo branco
    glClear(GL_COLOR_BUFFER_BIT);

    glUseProgram(program);
    glBindVertexArray(VAO);
    glBindTexture(GL_TEXTURE_2D, texture);

    int coordLocation = glGetUniformLocation(program, "yOffset");
    glUniform1f(coordLocation, (float)imageheight / imagewidth);

    int useTexLocation = glGetUniformLocation(program, "useTex");
    glUniform1i(useTexLocation, useTexture);

    if (useTexture)
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    else
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    //Translation
    glm::mat4 T = glm::translate(glm::mat4(1.0f), desloc);

    //Rotation
    glm::mat4 R1 = glm::rotate(glm::mat4(1.0f), glm::radians(rot.x), glm::vec3(1.0f, 0.0f, 0.0f));
    glm::mat4 R2 = glm::rotate(glm::mat4(1.0f), glm::radians(rot.y), glm::vec3(0.0f, 1.0f, 0.0f));
    glm::mat4 R3 = glm::rotate(glm::mat4(1.0f), glm::radians(rot.z), glm::vec3(0.0f, 0.0f, 1.0f));

    //Scale
    glm::mat4 S = glm::scale(glm::mat4(1.0f), glm::vec3(scale, 1.0f));

    // M = T*R*S
    glm::mat4 M = T * R1 * R2 * R3 * S;

    unsigned int loc = glGetUniformLocation(program, "transform");
    glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(M));

    // Define projection matrix.
    glm::mat4 projection = glm::perspective(glm::radians(60.0f), (win_width / (float)win_height), 0.1f, 100.0f);
    // Retrieve location of tranform variable in shader.
    loc = glGetUniformLocation(program, "projection");
    // Send matrix to shader.
    glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(projection));

    glDrawArrays(primitive, 0, size);

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

void keyUp(unsigned char key, int x, int y)
{
    teclas[key] = false;
}

void keyDown(unsigned char key, int x, int y)
{
    teclas[key] = true;
}

void idle()
{
    int now = glutGet(GLUT_ELAPSED_TIME);
    float elapsedTime = (now - lastTime) / 1000.0f;
    lastTime = now;

    static bool lastKey = false;

    if (teclas[27] || teclas['Q'] || teclas['q'])
        exit(0);
    if (teclas['T'] || teclas['t'])
        modo = 't';
    if (teclas['R'] || teclas['r'])
        modo = 'r';
    if (teclas['E'] || teclas['e'])
        modo = 'e';
    if (teclas['V'] || teclas['v'] && !lastKey)
    {
        if (primitive == GL_TRIANGLES)
        {
            primitive = GL_POINTS;
            useTexture = true;
        }
        else
        {
            primitive = GL_TRIANGLES;
            useTexture = false;
        }
    }
    lastKey = (teclas['V'] || teclas['v']);
    if (teclas['W'] || teclas['w'])
    {
        switch (modo)
        {
        case 't':
            desloc.y += 0.5 * elapsedTime;
            break;
        case 'r':
            rot.y += 50 * elapsedTime;
            break;
        case 'e':
            scale.y += 1 * elapsedTime;
            break;
        }
    }
    if (teclas['S'] || teclas['s'])
    {
        switch (modo)
        {
        case 't':
            desloc.y -= 0.5 * elapsedTime;
            break;
        case 'r':
            rot.y -= 50 * elapsedTime;
            break;
        case 'e':
            scale.y -= 1 * elapsedTime;
            break;
        }
    }
    if (teclas['D'] || teclas['d'])
    {
        switch (modo)
        {
        case 't':
            desloc.x += 0.5 * elapsedTime;
            break;
        case 'r':
            rot.x += 50 * elapsedTime;
            break;
        case 'e':
            scale.x += 1 * elapsedTime;
            break;
        }
    }
    if (teclas['A'] || teclas['a'])
    {
        switch (modo)
        {
        case 't':
            desloc.x -= 0.5 * elapsedTime;
            break;
        case 'r':
            rot.x -= 50 * elapsedTime;
            break;
        case 'e':
            scale.x -= 1 * elapsedTime;
            break;
        }
    }
    if (teclas['P'] || teclas['p'])
        switch (modo)
        {
        case 't':
            desloc.z += 0.5 * elapsedTime;
            break;
        case 'r':
            rot.z += 50 * elapsedTime;
            break;
        }
    if (teclas['N'] || teclas['n'])
    {
        switch (modo)
        {
        case 't':
            desloc.z -= 0.5 * elapsedTime;
            break;
        case 'r':
            rot.z -= 50 * elapsedTime;
            break;
        }
    }

    if (scale.x <= 0)
        scale.x = 0.01;
    if (scale.y <= 0)
        scale.y = 0.01;

    glutPostRedisplay();
}

void initData(char *path)
{
    stbi_set_flip_vertically_on_load(true);
    unsigned char *img = stbi_load(path, &imagewidth, &imageheight, &imagechannels, 4);
    printf("Largura da Imagem: %d, Altura da Imagem: %d\n", imagewidth, imageheight);

    if (!img)
    {
        printf("Imagem não encontrada!\n");
        exit(1);
    }

    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, imagewidth, imageheight, 0, GL_RGBA, GL_UNSIGNED_BYTE, img);
    glBindTexture(GL_TEXTURE_2D, 0);

    size = imagewidth * imageheight * 3 * 2 * 2 * 2;
    float *vertices = new float[size];

    int indice = 0;
    for (int i = 0; i < imagewidth; i++)
    {
        for (int j = 0; j < imageheight; j++)
        {
            vertices[indice++] = (float)(i) / imagewidth;
            vertices[indice++] = (float)(j) / imagewidth;
            vertices[indice++] = (float)(i) / imagewidth;
            vertices[indice++] = (float)(j) / imageheight;

            vertices[indice++] = (float)(i + 1) / imagewidth;
            vertices[indice++] = (float)(j) / imagewidth;
            vertices[indice++] = (float)(i + 1) / imagewidth;
            vertices[indice++] = (float)(j) / imageheight;

            vertices[indice++] = (float)(i) / imagewidth;
            vertices[indice++] = (float)(j - 1) / imagewidth;
            vertices[indice++] = (float)(i) / imagewidth;
            vertices[indice++] = (float)(j - 1) / imageheight;

            vertices[indice++] = (float)(i + 1) / imagewidth;
            vertices[indice++] = (float)(j) / imagewidth;
            vertices[indice++] = (float)(i + 1) / imagewidth;
            vertices[indice++] = (float)(j) / imageheight;

            vertices[indice++] = (float)(i + 1) / imagewidth;
            vertices[indice++] = (float)(j - 1) / imagewidth;
            vertices[indice++] = (float)(i + 1) / imagewidth;
            vertices[indice++] = (float)(j - 1) / imageheight;

            vertices[indice++] = (float)(i) / imagewidth;
            vertices[indice++] = (float)(j - 1) / imagewidth;
            vertices[indice++] = (float)(i) / imagewidth;
            vertices[indice++] = (float)(j - 1) / imageheight;
        }
    }

    // Vertex array.
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    // Vertex buffer
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * size, vertices, GL_STATIC_DRAW);

    // Set attributes.
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void *)0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void *)(2 * sizeof(float)));
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);

    // Unbind Vertex Array Object.
    glBindVertexArray(0);

    delete[](vertices);
    stbi_image_free(img);
}

/** Create program (shaders).
 * 
 * Compile shaders and create the program.
 */
void initShaders()
{
    // Request a program and shader slots from GPU
    program = createShaderProgram(vertex_code, fragment_code);
}

int main(int argc, char **argv)
{
    if (argc < 2)
    {
        printf("Esperava uma imagem\n");
        return 1;
    }

    glutInit(&argc, argv);
    glutInitContextVersion(3, 3);
    glutInitContextProfile(GLUT_CORE_PROFILE);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
    glutInitWindowSize(win_width, win_height);
    glutCreateWindow(argv[0]);
    glewExperimental = GL_TRUE;
    glewInit();

    /* Initialize the points of image */
    initData(argv[1]);

    // Create shaders.
    initShaders();

    glutSetKeyRepeat(GLUT_KEY_REPEAT_OFF);

    glutReshapeFunc(reshape);
    glutDisplayFunc(display);
    glutKeyboardFunc(keyDown);
    glutKeyboardUpFunc(keyUp);
    glutIdleFunc(idle);

    glutMainLoop();
}
