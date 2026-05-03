// Laura Reyes Carrillo
// Sol con triangulos: dodecagono (12 triángulos) + 12 rayos
// 20 de febrero de 2026
// Practica 2 - Computacion Grafica
//320015764

#include <iostream>
#include <vector>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "Shader.h"

void resize(GLFWwindow* window, int width, int height);

const GLint WIDTH = 800, HEIGHT = 600;

struct Vec3 { float x, y, z; };
struct Color3 { float r, g, b; };

int main()
{
    // 1) Inicializamos GLFW
    if (!glfwInit())
    {
        std::cout << "Failed to init GLFW\n";
        return EXIT_FAILURE;
    }

    // 2) Creamos la ventana
    GLFWwindow* window = glfwCreateWindow(
        WIDTH, HEIGHT,
        "Reyes Carrillo Laura - Sol",
        NULL, NULL
    );

    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window\n";
        glfwTerminate();
        return EXIT_FAILURE;
    }

    // 3) Callback resize
    glfwSetFramebufferSizeCallback(window, resize);

    // 4) Contexto actual
    glfwMakeContextCurrent(window);

    // 5) Inicializamos GLEW
    glewExperimental = GL_TRUE;
    if (GLEW_OK != glewInit())
    {
        std::cout << "Failed to initialise GLEW\n";
        glfwTerminate();
        return EXIT_FAILURE;
    }

    // Info
    std::cout << "> Version: " << glGetString(GL_VERSION) << std::endl;
    std::cout << "> Vendor: " << glGetString(GL_VENDOR) << std::endl;
    std::cout << "> Renderer: " << glGetString(GL_RENDERER) << std::endl;
    std::cout << "> SL Version: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;

    // 6) Shaders desde archivos
    Shader ourShader("Shader/core.vs", "Shader/core.frag");

    // =========================================================
    // POSICIONES (solo x,y,z) - 37 puntos
    // 0 = centro
    // 1..12 = dodecagono
    // 13..24 = bases rayos
    // 25..36 = puntas rayos
    // =========================================================
    Vec3 pos[37] = {
        { 0.0f,  0.0f, 0.0f },   // 0 centro

        { 0.4f,   0.1f, 0.0f },  // 1
        { 0.4f,  -0.1f, 0.0f },  // 2
        { 0.29f, -0.29f,0.0f },  // 3
        { 0.1f,  -0.4f, 0.0f },  // 4
        {-0.1f,  -0.4f, 0.0f },  // 5
        {-0.29f, -0.29f,0.0f },  // 6
        {-0.4f,  -0.1f, 0.0f },  // 7
        {-0.4f,   0.1f, 0.0f },  // 8
        {-0.29f,  0.29f,0.0f },  // 9
        {-0.1f,   0.4f, 0.0f },  // 10
        { 0.1f,   0.4f, 0.0f },  // 11
        { 0.29f,  0.29f,0.0f },  // 12

        // bases rayos 13..24
        { 0.44f,  0.10f, 0.0f }, // 13
        { 0.44f, -0.10f, 0.0f }, // 14
        { 0.33f, -0.31f, 0.0f }, // 15
        { 0.12f, -0.43f, 0.0f }, // 16
        {-0.12f, -0.43f, 0.0f }, // 17
        {-0.33f, -0.31f, 0.0f }, // 18
        {-0.44f, -0.10f, 0.0f }, // 19
        {-0.44f,  0.10f, 0.0f }, // 20
        {-0.33f,  0.31f, 0.0f }, // 21
        {-0.12f,  0.43f, 0.0f }, // 22
        { 0.12f,  0.43f, 0.0f }, // 23
        { 0.33f,  0.31f, 0.0f }, // 24

        // puntas rayos 25..36
        { 0.85f,  0.00f, 0.0f }, // 25
        { 0.75f, -0.42f, 0.0f }, // 26
        { 0.41f, -0.79f, 0.0f }, // 27
        { 0.00f, -0.85f, 0.0f }, // 28
        {-0.41f, -0.79f, 0.0f }, // 29
        {-0.75f, -0.42f, 0.0f }, // 30
        {-0.85f,  0.00f, 0.0f }, // 31
        {-0.75f,  0.42f, 0.0f }, // 32
        {-0.41f,  0.79f, 0.0f }, // 33
        { 0.00f,  0.85f, 0.0f }, // 34
        { 0.41f,  0.79f, 0.0f }, // 35
        { 0.75f,  0.42f, 0.0f }  // 36
    };

    // =========================================================
    // INDICES (72 índices = 24 triángulos)
    // =========================================================
    unsigned int indices[] = {
        // Centro (12 triángulos)
        0,  1,  2,
        0,  2,  3,
        0,  3,  4,
        0,  4,  5,
        0,  5,  6,
        0,  6,  7,
        0,  7,  8,
        0,  8,  9,
        0,  9, 10,
        0, 10, 11,
        0, 11, 12,
        0, 12,  1,

        // Rayos (12 triángulos)
        13, 14, 25,
        14, 15, 26,
        15, 16, 27,
        16, 17, 28,
        17, 18, 29,
        18, 19, 30,
        19, 20, 31,
        20, 21, 32,
        21, 22, 33,
        22, 23, 34,
        23, 24, 35,
        24, 13, 36
    };

    // =========================================================
    // COLORES que se repiten: amarillo, naranja claro, naranja oscuro
    // =========================================================
    Color3 palette[3] = {
        {1.0f, 1.0f, 0.0f},  // amarillo
        {1.0f, 0.8f, 0.0f},  // naranja claro
        {1.0f, 0.5f, 0.0f}   // naranja oscuro
    };

    // =========================================================
    // EXPANDIR VÉRTICES:
    // Vamos a crear un arreglo de 72 VÉRTICES (24 triángulos * 3)
    // Cada vértice tendrá: x,y,z,r,g,b
    // =========================================================
    std::vector<float> expandedVertices;
    expandedVertices.reserve(72 * 6);

    // 24 triángulos (porque 72 indices / 3 = 24)
    for (int tri = 0; tri < 24; tri++)
    {
        // color sólido para TODO el triángulo (mismo color en sus 3 vértices)
        Color3 c = palette[tri % 3];

        // cada triángulo tiene 3 índices
        for (int k = 0; k < 3; k++)
        {
            unsigned int idx = indices[tri * 3 + k];
            Vec3 p = pos[idx];

            // posición
            expandedVertices.push_back(p.x);
            expandedVertices.push_back(p.y);
            expandedVertices.push_back(p.z);

            // color (igual en los 3 vértices del triángulo)
            expandedVertices.push_back(c.r);
            expandedVertices.push_back(c.g);
            expandedVertices.push_back(c.b);
        }
    }

    // =========================================================
    // VAO/VBO  (YA NO usamos EBO porque ya duplicamos vértices)
    // =========================================================
    GLuint VAO, VBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER,
        expandedVertices.size() * sizeof(float),
        expandedVertices.data(),
        GL_STATIC_DRAW);

    // Atributo 0: posición (x,y,z)
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (GLvoid*)0);
    glEnableVertexAttribArray(0);

    // Atributo 1: color (r,g,b)
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (GLvoid*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);

    // =========================================================
    // LOOP
    // =========================================================
    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();

        glClearColor(0.f, 0.f, 0.f, 1.f);
        glClear(GL_COLOR_BUFFER_BIT);

        ourShader.Use();
        glBindVertexArray(VAO);

        // 72 vértices (porque son 24 triángulos * 3 vértices)
        glDrawArrays(GL_TRIANGLES, 0, 72);

        glBindVertexArray(0);
        glfwSwapBuffers(window);
    }

    glfwTerminate();
    return EXIT_SUCCESS;
}

void resize(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}