// Laura Reyes Carrillo
// Carga de modelos - Cubo con textura de dado por cara
// 22 de Marzo de 2026
// Previo 7
// 320015764

#include <iostream>
#include <cmath>

// GLEW
#include <GL/glew.h>

// GLFW
#include <GLFW/glfw3.h>

// Other Libs
#include "stb_image.h"

// GLM Mathematics
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// Other includes
#include "Shader.h"
#include "Camera.h"

// Function prototypes
void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mode);
void MouseCallback(GLFWwindow* window, double xPos, double yPos);
void DoMovement();

// Window dimensions
const GLuint WIDTH = 800, HEIGHT = 600;
int SCREEN_WIDTH, SCREEN_HEIGHT;

// Camera
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
GLfloat lastX = WIDTH / 2.0f;
GLfloat lastY = HEIGHT / 2.0f;
bool keys[1024];
bool firstMouse = true;

// Deltatime
GLfloat deltaTime = 0.0f;
GLfloat lastFrame = 0.0f;

int main()
{
    // Init GLFW
    glfwInit();

    // Set all the required options for GLFW
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

    // Create window
    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Reyes Carrillo Laura - Cubo con dado", nullptr, nullptr);

    if (window == nullptr)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return EXIT_FAILURE;
    }

    glfwMakeContextCurrent(window);
    glfwGetFramebufferSize(window, &SCREEN_WIDTH, &SCREEN_HEIGHT);

    // Callbacks
    glfwSetKeyCallback(window, KeyCallback);
    glfwSetCursorPosCallback(window, MouseCallback);

    // Captura el mouse
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // GLEW
    glewExperimental = GL_TRUE;
    if (GLEW_OK != glewInit())
    {
        std::cout << "Failed to initialize GLEW" << std::endl;
        return EXIT_FAILURE;
    }

    // Viewport
    glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

    // OpenGL options
    glEnable(GL_DEPTH_TEST);

    // Shader
    Shader shader("Shader/lamp.vs", "Shader/lamp.frag");

    // =========================================================
// UVs (736x952 aprox)
// acomodada en forma de cruz:
//
//         1
//      2  3  5
//         6
//         4
// =========================================================

// Límites horizontales de las columnas
    float xL = 75.0f / 736.0f;   // inicio cara izquierda
    float xC1 = 272.0f / 736.0f;  // inicio columna central
    float xC2 = 468.0f / 736.0f;  // fin columna central
    float xR = 666.0f / 736.0f;   // fin cara derecha

    // Límites verticales de las filas
    float y1 = 87.0f / 952.0f;   // cara superior
    float y2 = 283.0f / 952.0f;

    float y3 = 285.0f / 952.0f;   // fila del centro
    float y4 = 481.0f / 952.0f;

    float y5 = 483.0f / 952.0f;   // cara debajo del centro
    float y6 = 679.0f / 952.0f;

    float y7 = 681.0f / 952.0f;   // última cara inferior
    float y8 = 877.0f / 952.0f;

    // Cara 1 (arriba)
    float c1_u_min = xC1, c1_u_max = xC2;
    float c1_v_min = 1.0f - y2, c1_v_max = 1.0f - y1;

    // Cara 2 (izquierda)
    float c2_u_min = xL, c2_u_max = xC1;
    float c2_v_min = 1.0f - y4, c2_v_max = 1.0f - y3;

    // Cara 3 (centro)
    float c3_u_min = xC1, c3_u_max = xC2;
    float c3_v_min = 1.0f - y4, c3_v_max = 1.0f - y3;

    // Cara 5 (derecha)
    float c5_u_min = xC2, c5_u_max = xR;
    float c5_v_min = 1.0f - y4, c5_v_max = 1.0f - y3;

    // Cara 6 (debajo del centro)
    float c6_u_min = xC1, c6_u_max = xC2;
    float c6_v_min = 1.0f - y6, c6_v_max = 1.0f - y5;

    // Cara 4 (última inferior)
    float c4_u_min = xC1, c4_u_max = xC2;
    float c4_v_min = 1.0f - y8, c4_v_max = 1.0f - y7;

    // =========================================================
    // Cubo: 6 caras, 2 triángulos por cara, 3 vértices por triángulo
    // Formato por vértice:
    // posicion(x,y,z), color(r,g,b), textura(u,v)
    // =========================================================
    GLfloat vertices[] =
    {
        // =====================================================
        // FRENTE = cara 3
        // =====================================================
        -0.5f, -0.5f,  0.5f,   1.0f, 1.0f, 1.0f,   c3_u_min, c3_v_min,
         0.5f, -0.5f,  0.5f,   1.0f, 1.0f, 1.0f,   c3_u_max, c3_v_min,
         0.5f,  0.5f,  0.5f,   1.0f, 1.0f, 1.0f,   c3_u_max, c3_v_max,

         0.5f,  0.5f,  0.5f,   1.0f, 1.0f, 1.0f,   c3_u_max, c3_v_max,
        -0.5f,  0.5f,  0.5f,   1.0f, 1.0f, 1.0f,   c3_u_min, c3_v_max,
        -0.5f, -0.5f,  0.5f,   1.0f, 1.0f, 1.0f,   c3_u_min, c3_v_min,

        // =====================================================
        // DERECHA = cara 5
        // =====================================================
         0.5f, -0.5f,  0.5f,   1.0f, 1.0f, 1.0f,   c5_u_min, c5_v_min,
         0.5f, -0.5f, -0.5f,   1.0f, 1.0f, 1.0f,   c5_u_max, c5_v_min,
         0.5f,  0.5f, -0.5f,   1.0f, 1.0f, 1.0f,   c5_u_max, c5_v_max,

         0.5f,  0.5f, -0.5f,   1.0f, 1.0f, 1.0f,   c5_u_max, c5_v_max,
         0.5f,  0.5f,  0.5f,   1.0f, 1.0f, 1.0f,   c5_u_min, c5_v_max,
         0.5f, -0.5f,  0.5f,   1.0f, 1.0f, 1.0f,   c5_u_min, c5_v_min,

         // =====================================================
         // ATRÁS = cara 4
         // =====================================================
          0.5f, -0.5f, -0.5f,   1.0f, 1.0f, 1.0f,   c4_u_min, c4_v_min,
         -0.5f, -0.5f, -0.5f,   1.0f, 1.0f, 1.0f,   c4_u_max, c4_v_min,
         -0.5f,  0.5f, -0.5f,   1.0f, 1.0f, 1.0f,   c4_u_max, c4_v_max,

         -0.5f,  0.5f, -0.5f,   1.0f, 1.0f, 1.0f,   c4_u_max, c4_v_max,
          0.5f,  0.5f, -0.5f,   1.0f, 1.0f, 1.0f,   c4_u_min, c4_v_max,
          0.5f, -0.5f, -0.5f,   1.0f, 1.0f, 1.0f,   c4_u_min, c4_v_min,

          // =====================================================
          // IZQUIERDA = cara 2
          // =====================================================
          -0.5f, -0.5f, -0.5f,   1.0f, 1.0f, 1.0f,   c2_u_min, c2_v_min,
          -0.5f, -0.5f,  0.5f,   1.0f, 1.0f, 1.0f,   c2_u_max, c2_v_min,
          -0.5f,  0.5f,  0.5f,   1.0f, 1.0f, 1.0f,   c2_u_max, c2_v_max,

          -0.5f,  0.5f,  0.5f,   1.0f, 1.0f, 1.0f,   c2_u_max, c2_v_max,
          -0.5f,  0.5f, -0.5f,   1.0f, 1.0f, 1.0f,   c2_u_min, c2_v_max,
          -0.5f, -0.5f, -0.5f,   1.0f, 1.0f, 1.0f,   c2_u_min, c2_v_min,

          // =====================================================
          // ARRIBA = cara 1
          // =====================================================
          -0.5f,  0.5f,  0.5f,   1.0f, 1.0f, 1.0f,   c1_u_min, c1_v_min,
           0.5f,  0.5f,  0.5f,   1.0f, 1.0f, 1.0f,   c1_u_max, c1_v_min,
           0.5f,  0.5f, -0.5f,   1.0f, 1.0f, 1.0f,   c1_u_max, c1_v_max,

           0.5f,  0.5f, -0.5f,   1.0f, 1.0f, 1.0f,   c1_u_max, c1_v_max,
          -0.5f,  0.5f, -0.5f,   1.0f, 1.0f, 1.0f,   c1_u_min, c1_v_max,
          -0.5f,  0.5f,  0.5f,   1.0f, 1.0f, 1.0f,   c1_u_min, c1_v_min,

          // =====================================================
          // ABAJO = cara 6
          // =====================================================
          -0.5f, -0.5f, -0.5f,   1.0f, 1.0f, 1.0f,   c6_u_min, c6_v_min,
           0.5f, -0.5f, -0.5f,   1.0f, 1.0f, 1.0f,   c6_u_max, c6_v_min,
           0.5f, -0.5f,  0.5f,   1.0f, 1.0f, 1.0f,   c6_u_max, c6_v_max,

           0.5f, -0.5f,  0.5f,   1.0f, 1.0f, 1.0f,   c6_u_max, c6_v_max,
          -0.5f, -0.5f,  0.5f,   1.0f, 1.0f, 1.0f,   c6_u_min, c6_v_max,
          -0.5f, -0.5f, -0.5f,   1.0f, 1.0f, 1.0f,   c6_u_min, c6_v_min
    };

    GLuint VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // Position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);

    // Color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);

    // Texture Coordinate attribute
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));
    glEnableVertexAttribArray(2);

    glBindVertexArray(0);

    // =========================================================
    // TEXTURA
    // =========================================================
    GLuint texture1;
    glGenTextures(1, &texture1);
    glBindTexture(GL_TEXTURE_2D, texture1);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    int textureWidth, textureHeight, nrChannels;
    stbi_set_flip_vertically_on_load(true);

    unsigned char* image = stbi_load("images/Dice.jfif", &textureWidth, &textureHeight, &nrChannels, 0);

    if (image)
    {
        GLenum format = GL_RGB;
        if (nrChannels == 1) format = GL_RED;
        else if (nrChannels == 3) format = GL_RGB;
        else if (nrChannels == 4) format = GL_RGBA;

        glTexImage2D(GL_TEXTURE_2D, 0, format, textureWidth, textureHeight, 0, format, GL_UNSIGNED_BYTE, image);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture: images/Dice.jfif" << std::endl;
    }

    stbi_image_free(image);

    // =========================================================
    // GAME LOOP
    // =========================================================
    while (!glfwWindowShouldClose(window))
    {
        GLfloat currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        glfwPollEvents();
        DoMovement();

        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        shader.Use();

        glm::mat4 view = camera.GetViewMatrix();
        glm::mat4 projection = glm::perspective(camera.GetZoom(), (GLfloat)SCREEN_WIDTH / (GLfloat)SCREEN_HEIGHT, 0.1f, 100.0f);

        glm::mat4 model(1.0f);
        model = glm::rotate(model, (GLfloat)glfwGetTime() * glm::radians(25.0f), glm::vec3(1.0f, 1.0f, 0.0f));

        GLint modelLoc = glGetUniformLocation(shader.Program, "model");
        GLint viewLoc = glGetUniformLocation(shader.Program, "view");
        GLint projLoc = glGetUniformLocation(shader.Program, "projection");
        GLint texLoc = glGetUniformLocation(shader.Program, "ourTexture");

        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

        glUniform1i(texLoc, 0);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture1);

        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        glBindVertexArray(0);

        glfwSwapBuffers(window);
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);

    glfwTerminate();
    return 0;
}

// Moves/alters the camera positions based on user input
void DoMovement()
{
    if (keys[GLFW_KEY_W] || keys[GLFW_KEY_UP])
        camera.ProcessKeyboard(FORWARD, deltaTime);

    if (keys[GLFW_KEY_S] || keys[GLFW_KEY_DOWN])
        camera.ProcessKeyboard(BACKWARD, deltaTime);

    if (keys[GLFW_KEY_A] || keys[GLFW_KEY_LEFT])
        camera.ProcessKeyboard(LEFT, deltaTime);

    if (keys[GLFW_KEY_D] || keys[GLFW_KEY_RIGHT])
        camera.ProcessKeyboard(RIGHT, deltaTime);
}

// Is called whenever a key is pressed/released via GLFW
void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);

    if (key >= 0 && key < 1024)
    {
        if (action == GLFW_PRESS)
            keys[key] = true;
        else if (action == GLFW_RELEASE)
            keys[key] = false;
    }
}

void MouseCallback(GLFWwindow* window, double xPos, double yPos)
{
    if (firstMouse)
    {
        lastX = (GLfloat)xPos;
        lastY = (GLfloat)yPos;
        firstMouse = false;
    }

    GLfloat xOffset = (GLfloat)xPos - lastX;
    GLfloat yOffset = lastY - (GLfloat)yPos;

    lastX = (GLfloat)xPos;
    lastY = (GLfloat)yPos;

    camera.ProcessMouseMovement(xOffset, yOffset);
}