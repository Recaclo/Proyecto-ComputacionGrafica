//27/ Febrero / 2026
//Reyes Carrillo Laura
//320015764
//Practica 3: Proyecciones y transformaciones
// Hacer la misma cantidad de cubos que la cantidad de letras en mi nombre


#include<iostream>

//#define GLEW_STATIC

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// Shaders
#include "Shader.h"

const GLint WIDTH = 800, HEIGHT = 600;

int main() {
    glfwInit();
    //Verificaci�n de compatibilidad
    // Set all the required options for GLFW
    /*glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);*/

    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT,
        "Reyes Carrillo Laura Proyecciones y transformaciones basicas",
        nullptr, nullptr);

    int screenWidth, screenHeight;
    glfwGetFramebufferSize(window, &screenWidth, &screenHeight);

    //Verificaci�n de errores de creacion  ventana
    if (nullptr == window)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return EXIT_FAILURE;
    }

    glfwMakeContextCurrent(window);
    glewExperimental = GL_TRUE;

    //Verificaci�n de errores de inicializaci�n de glew
    if (GLEW_OK != glewInit()) {
        std::cout << "Failed to initialise GLEW" << std::endl;
        return EXIT_FAILURE;
    }

    // Define las dimensiones del viewport
    glViewport(0, 0, screenWidth, screenHeight);

    // Setup OpenGL options
    glEnable(GL_DEPTH_TEST);

    // enable alpha support
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // Build and compile our shader program
    Shader ourShader("Shader/core.vs", "Shader/core.frag");

    // use with Perspective Projection
    float vertices[] = {
        -0.5f, -0.5f,  0.5f, 1.0f, 0.0f, 0.0f,//Front
         0.5f, -0.5f,  0.5f, 1.0f, 0.0f, 0.0f,
         0.5f,  0.5f,  0.5f, 1.0f, 0.0f, 0.0f,
         0.5f,  0.5f,  0.5f, 1.0f, 0.0f, 0.0f,
        -0.5f,  0.5f,  0.5f, 1.0f, 0.0f, 0.0f,
        -0.5f, -0.5f,  0.5f, 1.0f, 0.0f, 0.0f,

        -0.5f, -0.5f, -0.5f, 0.0f, 1.0f, 0.0f,//Back
         0.5f, -0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
         0.5f,  0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
         0.5f,  0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
        -0.5f,  0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
        -0.5f, -0.5f, -0.5f, 0.0f, 1.0f, 0.0f,

         0.5f, -0.5f,  0.5f, 0.0f, 0.0f, 1.0f,
         0.5f, -0.5f, -0.5f, 0.0f, 0.0f, 1.0f,
         0.5f,  0.5f, -0.5f, 0.0f, 0.0f, 1.0f,
         0.5f,  0.5f, -0.5f, 0.0f, 0.0f, 1.0f,
         0.5f,  0.5f,  0.5f, 0.0f, 0.0f, 1.0f,
         0.5f, -0.5f,  0.5f, 0.0f, 0.0f, 1.0f,

        -0.5f,  0.5f,  0.5f, 1.0f, 1.0f, 0.0f,
        -0.5f,  0.5f, -0.5f, 1.0f, 1.0f, 0.0f,
        -0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 0.0f,
        -0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 0.0f,
        -0.5f, -0.5f,  0.5f, 1.0f, 1.0f, 0.0f,
        -0.5f,  0.5f,  0.5f, 1.0f, 1.0f, 0.0f,

        -0.5f, -0.5f, -0.5f, 0.0f, 1.0f, 1.0f,
         0.5f, -0.5f, -0.5f, 0.0f, 1.0f, 1.0f,
         0.5f, -0.5f,  0.5f, 0.0f, 1.0f, 1.0f,
         0.5f, -0.5f,  0.5f, 0.0f, 1.0f, 1.0f,
        -0.5f, -0.5f,  0.5f, 0.0f, 1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, 0.0f, 1.0f, 1.0f,

        -0.5f,  0.5f, -0.5f, 1.0f, 0.2f, 0.5f,
         0.5f,  0.5f, -0.5f, 1.0f, 0.2f, 0.5f,
         0.5f,  0.5f,  0.5f, 1.0f, 0.2f, 0.5f,
         0.5f,  0.5f,  0.5f, 1.0f, 0.2f, 0.5f,
        -0.5f,  0.5f,  0.5f, 1.0f, 0.2f, 0.5f,
        -0.5f,  0.5f, -0.5f, 1.0f, 0.2f, 0.5f,
    };

    GLuint VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    // Enlazar  Vertex Array Object
    glBindVertexArray(VAO);

    //2.- Copiamos nuestros arreglo de vertices en un buffer de vertices para que OpenGL lo use
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    //Posicion
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);

    //Color
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    glm::mat4 projection = glm::mat4(1.0f);
    projection = glm::perspective(glm::radians(45.0f),
        (float)screenWidth / (float)screenHeight,
        0.1f, 100.0f);

    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();

        glClearColor(0.95f, 0.95f, 0.95f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        ourShader.Use();

        // Se crean matrices view (camara)
        glm::mat4 view = glm::mat4(1.0f);
        view = glm::translate(view, glm::vec3(1.0f, -1.5f, -15.0f));

        // comunicacion por medio de las variables uniform
        GLint modelLoc = glGetUniformLocation(ourShader.Program, "model");
        GLint viewLoc = glGetUniformLocation(ourShader.Program, "view");
        GLint projecLoc = glGetUniformLocation(ourShader.Program, "projection");

        glUniformMatrix4fv(projecLoc, 1, GL_FALSE, glm::value_ptr(projection));
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));

        glBindVertexArray(VAO);

        // --------- Transformaciones por cubo (SIN ESPACIOS, SOLO CONTACTO) ---------

        // Escalas (todos diferentes, el ultimo es el mas grande)
        float scales[5] = { 0.55f, 0.75f, 0.95f, 1.20f, 2.0f };

        // Rotaciones: el grande (indice 4) queda "parado en una esquina"
        float rotY[5] = { 15.0f, 25.0f, 30.0f, 35.0f, 45.0f };
        float rotX[5] = { 8.0f, 10.0f, 12.0f, 14.0f, 35.264f };
        float rotZ[5] = { 0.0f,  0.0f,  0.0f,  0.0f, 0.0f };

        // Cubo 0: posición base (puedes mover esto si quieres)
        glm::vec3 basePos = glm::vec3(0.0f, 0.0f, 0.0f);

        // Guardamos el model del cubo anterior para "encadenar" sin espacios
        glm::mat4 prevModel = glm::mat4(1.0f);

        for (int i = 0; i < 5; i++)
        {
            float s = scales[i];

            // Rotación y escala del cubo actual
            glm::mat4 R = glm::mat4(1.0f);
            R = glm::rotate(R, glm::radians(rotY[i]), glm::vec3(0.0f, 1.0f, 0.0f));
            R = glm::rotate(R, glm::radians(rotX[i]), glm::vec3(1.0f, 0.0f, 0.0f));
            R = glm::rotate(R, glm::radians(rotZ[i]), glm::vec3(0.0f, 0.0f, 1.0f));

            glm::mat4 S = glm::scale(glm::mat4(1.0f), glm::vec3(s, s, s));

            glm::mat4 model = glm::mat4(1.0f);

            // ======= CUBO 0 (BASE) =======
            if (i == 0)
            {
                glm::vec3 T = basePos;
                model = glm::translate(glm::mat4(1.0f), T) * R * S;

                glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
                glDrawArrays(GL_TRIANGLES, 0, 36);

                prevModel = model;
            }
            // ======= CUBOS 1..3 (SIN ESPACIO) =======
            else if (i >= 1 && i <= 3)
            {
                // Objetivo: una esquina de la cara superior del cubo anterior (escalera diagonal)
                glm::vec3 targetPoint = glm::vec3(prevModel * glm::vec4(0.5f, 0.5f, 0.5f, 1.0f));

                // Punto del cubo actual a pegar: centro de la cara inferior
                glm::vec3 attachLocal = glm::vec3(0.0f, -0.5f, 0.0f);

                // Offset del punto después de rotación+escala (sin traslación)
                glm::vec3 attachWorldOffset = glm::vec3((R * S) * glm::vec4(attachLocal, 1.0f));

                // Traslación exacta para que se toquen sin espacio
                glm::vec3 T = targetPoint - attachWorldOffset;

                model = glm::translate(glm::mat4(1.0f), T) * R * S;

                glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
                glDrawArrays(GL_TRIANGLES, 0, 36);

                prevModel = model;
            }
            // ======= CUBO 4 (GRANDE) PARADO EN UNA ESQUINA =======
            else
            {
                // Objetivo: centro de la cara superior del cubo anterior
                glm::vec3 targetPoint = glm::vec3(prevModel * glm::vec4(0.0f, 0.5f, 0.0f, 1.0f));

                // Elegimos automáticamente la esquina más baja del cubo grande
                glm::vec3 corners[8] = {
                    glm::vec3(-0.5f, -0.5f, -0.5f),
                    glm::vec3(-0.5f, -0.5f,  0.5f),
                    glm::vec3(-0.5f,  0.5f, -0.5f),
                    glm::vec3(-0.5f,  0.5f,  0.5f),
                    glm::vec3(0.5f, -0.5f, -0.5f),
                    glm::vec3(0.5f, -0.5f,  0.5f),
                    glm::vec3(0.5f,  0.5f, -0.5f),
                    glm::vec3(0.5f,  0.5f,  0.5f)
                };

                glm::vec3 cornerLocal = corners[0];
                float minY = 1e9f;

                for (int c = 0; c < 8; c++)
                {
                    glm::vec3 p = glm::vec3((R * S) * glm::vec4(corners[c], 1.0f));
                    if (p.y < minY)
                    {
                        minY = p.y;
                        cornerLocal = corners[c];
                    }
                }

                glm::vec3 cornerWorldOffset = glm::vec3((R * S) * glm::vec4(cornerLocal, 1.0f));

                // Traslación exacta: esquina inferior del cubo grande -> centro cara superior del cubo anterior
                glm::vec3 T = targetPoint - cornerWorldOffset;

                model = glm::translate(glm::mat4(1.0f), T) * R * S;

                glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
                glDrawArrays(GL_TRIANGLES, 0, 36);

                prevModel = model;
            }
        }

        glBindVertexArray(0);
        glfwSwapBuffers(window);
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);

    glfwTerminate();
    return EXIT_SUCCESS;
}