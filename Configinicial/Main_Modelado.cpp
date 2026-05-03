// Practica 4 Modelado geométrico
// 3/Marzo/2026
// Reyes Carrillo Laura
// 320015764

#include <iostream>

//#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// Shaders
#include "Shader.h"

void Inputs(GLFWwindow* window);

const GLint WIDTH = 800, HEIGHT = 600;

float movX = 0.0f;
float movY = 0.0f;
float movZ = -5.0f;
float rot = 0.0f;

int main() {
    glfwInit();

    //manipular la vista sin tener que cerrar y volver a ejecutar el programa, se puede usar las teclas w,s,a,d para mover la camara y las teclas i,j,k,l para rotar la camara
    // (Si tienes problemas de compatibilidad, descomenta los window hints de versión)
    /*
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    */

    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Reyes Carrillo Laura Modelado geometrico", nullptr, nullptr);

    int screenWidth, screenHeight;
    glfwGetFramebufferSize(window, &screenWidth, &screenHeight);

    if (nullptr == window) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return EXIT_FAILURE;
    }

    glfwMakeContextCurrent(window);
    glfwSwapInterval(1); // VSync ON (~60 FPS)
    glewExperimental = GL_TRUE;

    if (GLEW_OK != glewInit()) {
        std::cout << "Failed to initialise GLEW" << std::endl;
        return EXIT_FAILURE;
    }
    // Define las dimensiones del viewport
    glViewport(0, 0, screenWidth, screenHeight);

    // OpenGL options
    glEnable(GL_DEPTH_TEST);

    // Alpha support (por si luego dibujas cosas con transparencia)
    //enable alpha support
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    // Build and compile our shader program
    Shader ourShader("Shader/core.vs", "Shader/core.frag");
    // Set up vertex data (and buffer(s)) and attribute pointers
    // / use with Perspective Projection //definicion de los vertices que definen el cubo , cada cara del cubo tiene un color diferente para poder distinguirlas, cada vertice tiene 6 atributos: 3 para la posicion y 3 para el color
    // Cubo: 36 vertices (12 triángulos). Cada vértice: 3 pos + 3 color (aunque luego vamos a usar color constante)
    float vertices[] = {

        // Front
        -0.5f, -0.5f,  0.5f,
         0.5f, -0.5f,  0.5f,
         0.5f,  0.5f,  0.5f,
         0.5f,  0.5f,  0.5f,
        -0.5f,  0.5f,  0.5f,
        -0.5f, -0.5f,  0.5f,

        // Back
        -0.5f, -0.5f, -0.5f,
         0.5f, -0.5f, -0.5f,
         0.5f,  0.5f, -0.5f,
         0.5f,  0.5f, -0.5f,
        -0.5f,  0.5f, -0.5f,
        -0.5f, -0.5f, -0.5f,

        // Right
         0.5f, -0.5f,  0.5f,
         0.5f, -0.5f, -0.5f,
         0.5f,  0.5f, -0.5f,
         0.5f,  0.5f, -0.5f,
         0.5f,  0.5f,  0.5f,
         0.5f, -0.5f,  0.5f,

         // Left
         -0.5f,  0.5f,  0.5f,
         -0.5f,  0.5f, -0.5f,
         -0.5f, -0.5f, -0.5f,
         -0.5f, -0.5f, -0.5f,
         -0.5f, -0.5f,  0.5f,
         -0.5f,  0.5f,  0.5f,

         // Bottom
         -0.5f, -0.5f, -0.5f,
          0.5f, -0.5f, -0.5f,
          0.5f, -0.5f,  0.5f,
          0.5f, -0.5f,  0.5f,
         -0.5f, -0.5f,  0.5f,
         -0.5f, -0.5f, -0.5f,

         // Top
         -0.5f,  0.5f, -0.5f,
          0.5f,  0.5f, -0.5f,
          0.5f,  0.5f,  0.5f,
          0.5f,  0.5f,  0.5f,
         -0.5f,  0.5f,  0.5f,
         -0.5f,  0.5f, -0.5f
    };

    GLuint VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    //glGenBuffers(1, &EBO);
    // Enlazar Vertex Array Object
    glBindVertexArray(VAO);
    //2.- Copiamos nuestros arreglo de vertices en un buffer de vertices para que OpenGL lo use
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    // 3.Copiamos nuestro arreglo de indices en un elemento del buffer para que OpenGL lo use
    /*glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO); glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);*/
    // 4. Despues colocamos las caracteristicas de los vertices
    // Posición (location = 0) el color lo mandamos despues 
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);// Unbind VAO (it's always a good thing to unbind any buffer/array to prevent strange bugs)

    // Matriz de proyección
    glm::mat4 projection = glm::perspective(
        glm::radians(45.0f),
        (GLfloat)screenWidth / (GLfloat)screenHeight,
        0.1f, 100.0f
    );


    while (!glfwWindowShouldClose(window)) {
        Inputs(window);
        // Check if any events have been activiated (key pressed, mouse moved etc.) and call corresponding response functions
        glfwPollEvents();
        // Render 
        // Clear the colorbuffer
        glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        // Draw our first triangle
        ourShader.Use();

        // View (cámara)
        // Create transformations 
        // Herramoentas de transformacion de la camara, se pueden usar para mover la camara y rotarla, se pueden usar las teclas w,s,a,d para mover la camara 
        glm::mat4 view = glm::mat4(1.0f);
        // se manda el valor de movX, movY, movZ y rot a la matriz de vista para poder mover y rotar la camara, se pueden usar las teclas w,s,a,d para mover la camara 
        view = glm::translate(view, glm::vec3(movX, movY, movZ));
        // se manda rot que es la rotacion en y
        view = glm::rotate(view, glm::radians(rot), glm::vec3(0.0f, 1.0f, 0.0f));

        // Uniform locations
        // se manda la informacion a los shaders por medio de las variables uniform
        GLint modelLoc = glGetUniformLocation(ourShader.Program, "model");
        GLint viewLoc = glGetUniformLocation(ourShader.Program, "view");
        GLint projecLoc = glGetUniformLocation(ourShader.Program, "projection");
        //mandamos la informacion de las matrices a los shaders por medio de las variables uniform
        glUniformMatrix4fv(projecLoc, 1, GL_FALSE, glm::value_ptr(projection));
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));

        glBindVertexArray(VAO);

        // ===============================
        // Usaremos color constante por cubo con el atributo 1:
        // - Desactivamos el arreglo de colores del VBO
        // - Forzamos un color con glVertexAttrib3f(1, r,g,b)
        glDisableVertexAttribArray(1);
        //PETALOS
        //PETALOS
        // ============================================
        // ROSA VOXEL: centro oscuro + pétalos envolventes
        // ============================================

        // ===== CENTRO OSCURO (profundidad interna) =====
        glm::mat4 model = glm::mat4(1.0f);
        glVertexAttrib3f(1, 0.18f, 0.00f, 0.00f);
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::scale(model, glm::vec3(0.2f));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        glDrawArrays(GL_TRIANGLES, 0, 36);

        glVertexAttrib3f(1, 0.22f, 0.00f, 0.00f);
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(-0.2f, 1.0f, 0.0f));
        model = glm::scale(model, glm::vec3(0.2f));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        glDrawArrays(GL_TRIANGLES, 0, 36);

        glVertexAttrib3f(1, 0.22f, 0.00f, 0.00f);
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.0f, 0.8f, 0.0f));
        model = glm::scale(model, glm::vec3(0.2f));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        glDrawArrays(GL_TRIANGLES, 0, 36);

        // ===== BASE FRONTAL DE LA ROSA =====
        glVertexAttrib3f(1, 0.85f, 0.08f, 0.08f);
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(-0.2f, 0.6f, 0.2f));
        model = glm::scale(model, glm::vec3(0.2f));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        glDrawArrays(GL_TRIANGLES, 0, 36);

        glVertexAttrib3f(1, 0.90f, 0.12f, 0.12f);
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.0f, 0.6f, 0.2f));
        model = glm::scale(model, glm::vec3(0.2f));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        glDrawArrays(GL_TRIANGLES, 0, 36);

        glVertexAttrib3f(1, 0.85f, 0.08f, 0.08f);
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.2f, 0.6f, 0.2f));
        model = glm::scale(model, glm::vec3(0.2f));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        glDrawArrays(GL_TRIANGLES, 0, 36);

        // ===== FRENTE MEDIO =====
        glVertexAttrib3f(1, 0.95f, 0.18f, 0.18f);
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(-0.4f, 0.8f, 0.2f));
        model = glm::scale(model, glm::vec3(0.2f));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        glDrawArrays(GL_TRIANGLES, 0, 36);

        glVertexAttrib3f(1, 0.92f, 0.16f, 0.16f);
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(-0.2f, 0.8f, 0.2f));
        model = glm::scale(model, glm::vec3(0.2f));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        glDrawArrays(GL_TRIANGLES, 0, 36);

        glVertexAttrib3f(1, 0.96f, 0.22f, 0.22f);
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.0f, 0.8f, 0.2f));
        model = glm::scale(model, glm::vec3(0.2f));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        glDrawArrays(GL_TRIANGLES, 0, 36);

        glVertexAttrib3f(1, 0.92f, 0.16f, 0.16f);
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.2f, 0.8f, 0.2f));
        model = glm::scale(model, glm::vec3(0.2f));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        glDrawArrays(GL_TRIANGLES, 0, 36);

        glVertexAttrib3f(1, 0.95f, 0.18f, 0.18f);
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.4f, 0.8f, 0.2f));
        model = glm::scale(model, glm::vec3(0.2f));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        glDrawArrays(GL_TRIANGLES, 0, 36);

        // ===== FRENTE SUPERIOR =====
        glVertexAttrib3f(1, 0.95f, 0.24f, 0.24f);
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(-0.4f, 1.0f, 0.2f));
        model = glm::scale(model, glm::vec3(0.2f));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        glDrawArrays(GL_TRIANGLES, 0, 36);

        glVertexAttrib3f(1, 0.88f, 0.10f, 0.10f);
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(-0.2f, 1.0f, 0.2f));
        model = glm::scale(model, glm::vec3(0.2f));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        glDrawArrays(GL_TRIANGLES, 0, 36);

        glVertexAttrib3f(1, 0.90f, 0.12f, 0.12f);
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.2f, 1.0f, 0.2f));
        model = glm::scale(model, glm::vec3(0.2f));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        glDrawArrays(GL_TRIANGLES, 0, 36);

        glVertexAttrib3f(1, 0.95f, 0.24f, 0.24f);
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.4f, 1.0f, 0.2f));
        model = glm::scale(model, glm::vec3(0.2f));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        glDrawArrays(GL_TRIANGLES, 0, 36);

        // ===== AROS LATERALES =====
        glVertexAttrib3f(1, 0.88f, 0.10f, 0.10f);
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(-0.6f, 0.8f, 0.0f));
        model = glm::scale(model, glm::vec3(0.2f));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        glDrawArrays(GL_TRIANGLES, 0, 36);

        glVertexAttrib3f(1, 0.83f, 0.06f, 0.06f);
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(-0.6f, 1.0f, 0.0f));
        model = glm::scale(model, glm::vec3(0.2f));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        glDrawArrays(GL_TRIANGLES, 0, 36);

        glVertexAttrib3f(1, 0.90f, 0.14f, 0.14f);
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(-0.4f, 1.2f, 0.0f));
        model = glm::scale(model, glm::vec3(0.2f));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        glDrawArrays(GL_TRIANGLES, 0, 36);

        glVertexAttrib3f(1, 0.88f, 0.10f, 0.10f);
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.6f, 0.8f, 0.0f));
        model = glm::scale(model, glm::vec3(0.2f));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        glDrawArrays(GL_TRIANGLES, 0, 36);

        glVertexAttrib3f(1, 0.83f, 0.06f, 0.06f);
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.6f, 1.0f, 0.0f));
        model = glm::scale(model, glm::vec3(0.2f));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        glDrawArrays(GL_TRIANGLES, 0, 36);

        glVertexAttrib3f(1, 0.90f, 0.14f, 0.14f);
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.4f, 1.2f, 0.0f));
        model = glm::scale(model, glm::vec3(0.2f));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        glDrawArrays(GL_TRIANGLES, 0, 36);

        // ===== PARTE SUPERIOR =====
        glVertexAttrib3f(1, 0.94f, 0.20f, 0.20f);
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(-0.2f, 1.4f, 0.0f));
        model = glm::scale(model, glm::vec3(0.2f));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        glDrawArrays(GL_TRIANGLES, 0, 36);

        glVertexAttrib3f(1, 0.88f, 0.10f, 0.10f);
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.0f, 1.4f, 0.0f));
        model = glm::scale(model, glm::vec3(0.2f));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        glDrawArrays(GL_TRIANGLES, 0, 36);

        glVertexAttrib3f(1, 0.94f, 0.20f, 0.20f);
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.2f, 1.4f, 0.0f));
        model = glm::scale(model, glm::vec3(0.2f));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        glDrawArrays(GL_TRIANGLES, 0, 36);

        glVertexAttrib3f(1, 0.96f, 0.26f, 0.26f);
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.0f, 1.6f, 0.0f));
        model = glm::scale(model, glm::vec3(0.2f));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        glDrawArrays(GL_TRIANGLES, 0, 36);

        // ===== VOLUMEN POSTERIOR =====
        glVertexAttrib3f(1, 0.72f, 0.04f, 0.04f);
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(-0.4f, 0.8f, -0.2f));
        model = glm::scale(model, glm::vec3(0.2f));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        glDrawArrays(GL_TRIANGLES, 0, 36);

        glVertexAttrib3f(1, 0.66f, 0.02f, 0.02f);
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(-0.2f, 0.8f, -0.2f));
        model = glm::scale(model, glm::vec3(0.2f));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        glDrawArrays(GL_TRIANGLES, 0, 36);

        glVertexAttrib3f(1, 0.66f, 0.02f, 0.02f);
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.0f, 0.8f, -0.2f));
        model = glm::scale(model, glm::vec3(0.2f));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        glDrawArrays(GL_TRIANGLES, 0, 36);

        glVertexAttrib3f(1, 0.66f, 0.02f, 0.02f);
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.2f, 0.8f, -0.2f));
        model = glm::scale(model, glm::vec3(0.2f));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        glDrawArrays(GL_TRIANGLES, 0, 36);

        glVertexAttrib3f(1, 0.72f, 0.04f, 0.04f);
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.4f, 0.8f, -0.2f));
        model = glm::scale(model, glm::vec3(0.2f));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        glDrawArrays(GL_TRIANGLES, 0, 36);

        glVertexAttrib3f(1, 0.60f, 0.00f, 0.00f);
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(-0.2f, 1.0f, -0.2f));
        model = glm::scale(model, glm::vec3(0.2f));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        glDrawArrays(GL_TRIANGLES, 0, 36);

        glVertexAttrib3f(1, 0.55f, 0.00f, 0.00f);
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.0f, 1.0f, -0.2f));
        model = glm::scale(model, glm::vec3(0.2f));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        glDrawArrays(GL_TRIANGLES, 0, 36);

        glVertexAttrib3f(1, 0.60f, 0.00f, 0.00f);
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.2f, 1.0f, -0.2f));
        model = glm::scale(model, glm::vec3(0.2f));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        glDrawArrays(GL_TRIANGLES, 0, 36);

        // ===== CIERRE TRASERO SUPERIOR =====
        glVertexAttrib3f(1, 0.74f, 0.05f, 0.05f);
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(-0.2f, 1.2f, -0.2f));
        model = glm::scale(model, glm::vec3(0.2f));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        glDrawArrays(GL_TRIANGLES, 0, 36);

        glVertexAttrib3f(1, 0.70f, 0.04f, 0.04f);
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.0f, 1.2f, -0.2f));
        model = glm::scale(model, glm::vec3(0.2f));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        glDrawArrays(GL_TRIANGLES, 0, 36);

        glVertexAttrib3f(1, 0.74f, 0.05f, 0.05f);
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.2f, 1.2f, -0.2f));
        model = glm::scale(model, glm::vec3(0.2f));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        glDrawArrays(GL_TRIANGLES, 0, 36);

        // ===== PETALOS EXTERNOS ALTOS =====
        glVertexAttrib3f(1, 0.96f, 0.28f, 0.28f);
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(-0.2f, 1.6f, 0.2f));
        model = glm::scale(model, glm::vec3(0.2f));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        glDrawArrays(GL_TRIANGLES, 0, 36);

        glVertexAttrib3f(1, 0.98f, 0.34f, 0.34f);
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.2f, 1.6f, 0.2f));
        model = glm::scale(model, glm::vec3(0.2f));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        glDrawArrays(GL_TRIANGLES, 0, 36);

        glVertexAttrib3f(1, 0.92f, 0.18f, 0.18f);
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(-0.4f, 1.2f, 0.2f));
        model = glm::scale(model, glm::vec3(0.2f));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        glDrawArrays(GL_TRIANGLES, 0, 36);

        glVertexAttrib3f(1, 0.92f, 0.18f, 0.18f);
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.4f, 1.2f, 0.2f));
        model = glm::scale(model, glm::vec3(0.2f));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        glDrawArrays(GL_TRIANGLES, 0, 36);
        // ============================================
// PETALOS INFERIORES / BASE DE LA FLOR
// para unir mejor la rosa con el tallo
// ============================================

// base frontal izquierda
        glVertexAttrib3f(1, 0.75f, 0.08f, 0.08f);
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(-0.2f, 0.4f, 0.2f));
        model = glm::scale(model, glm::vec3(0.2f));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        glDrawArrays(GL_TRIANGLES, 0, 36);

        // base frontal centro
        glVertexAttrib3f(1, 0.82f, 0.10f, 0.10f);
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.0f, 0.4f, 0.2f));
        model = glm::scale(model, glm::vec3(0.2f));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        glDrawArrays(GL_TRIANGLES, 0, 36);

        // base frontal derecha
        glVertexAttrib3f(1, 0.75f, 0.08f, 0.08f);
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.2f, 0.4f, 0.2f));
        model = glm::scale(model, glm::vec3(0.2f));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        glDrawArrays(GL_TRIANGLES, 0, 36);

        // base izquierda lateral
        glVertexAttrib3f(1, 0.68f, 0.05f, 0.05f);
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(-0.4f, 0.4f, 0.0f));
        model = glm::scale(model, glm::vec3(0.2f));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        glDrawArrays(GL_TRIANGLES, 0, 36);

        // base derecha lateral
        glVertexAttrib3f(1, 0.68f, 0.05f, 0.05f);
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.4f, 0.4f, 0.0f));
        model = glm::scale(model, glm::vec3(0.2f));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        glDrawArrays(GL_TRIANGLES, 0, 36);

        // base trasera izquierda
        glVertexAttrib3f(1, 0.55f, 0.02f, 0.02f);
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(-0.2f, 0.4f, -0.2f));
        model = glm::scale(model, glm::vec3(0.2f));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        glDrawArrays(GL_TRIANGLES, 0, 36);

        // base trasera centro
        glVertexAttrib3f(1, 0.48f, 0.00f, 0.00f);
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.0f, 0.4f, -0.2f));
        model = glm::scale(model, glm::vec3(0.2f));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        glDrawArrays(GL_TRIANGLES, 0, 36);

        // base trasera derecha
        glVertexAttrib3f(1, 0.55f, 0.02f, 0.02f);
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.2f, 0.4f, -0.2f));
        model = glm::scale(model, glm::vec3(0.2f));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        glDrawArrays(GL_TRIANGLES, 0, 36);

        // cubo central de unión con la flor
        glVertexAttrib3f(1, 0.62f, 0.04f, 0.04f);
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.0f, 0.4f, 0.0f));
        model = glm::scale(model, glm::vec3(0.2f));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        glDrawArrays(GL_TRIANGLES, 0, 36);





        //Tallo 
        // Cubo 1 - VERDE CLARO  
        glVertexAttrib3f(1, 0.60f, 0.95f, 0.60f);
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.0f, 0.2f, 0.0f));
        model = glm::scale(model, glm::vec3(0.2f));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        glDrawArrays(GL_TRIANGLES, 0, 36);

        // Cubo 2 - VERDE MEDIO (CENTRO) cubo central 
        glVertexAttrib3f(1, 0.20f, 0.70f, 0.20f);
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
        model = glm::scale(model, glm::vec3(0.2f));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        glDrawArrays(GL_TRIANGLES, 0, 36);

        // Cubo 3 - VERDE FUERTE (CENTRO)
        glVertexAttrib3f(1, 0.05f, 0.40f, 0.05f);
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.0f,-0.2f, 0.0f));
        model = glm::scale(model, glm::vec3(0.2f));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        glDrawArrays(GL_TRIANGLES, 0, 36);

        // Cubo 4 - VERDE CLARO  
        glVertexAttrib3f(1, 0.60f, 0.95f, 0.60f);
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.0f, -0.4f, 0.0f));
        model = glm::scale(model, glm::vec3(0.2f));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        glDrawArrays(GL_TRIANGLES, 0, 36);

        // Cubo  5- VERDE FUERTE (CENTRO)
        glVertexAttrib3f(1, 0.05f, 0.40f, 0.05f);
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.0f, -0.6f, 0.0f));
        model = glm::scale(model, glm::vec3(0.2f));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        glDrawArrays(GL_TRIANGLES, 0, 36);

        // Cubo 6 - VERDE MEDIO (CENTRO) cubo central 
        glVertexAttrib3f(1, 0.20f, 0.70f, 0.20f);
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.0f, -0.8f, 0.0f));
        model = glm::scale(model, glm::vec3(0.2f));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        glDrawArrays(GL_TRIANGLES, 0, 36);

        // Cubo 7 - VERDE CLARO  
        glVertexAttrib3f(1, 0.60f, 0.95f, 0.60f);
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.0f, -1.0f, 0.0f));
        model = glm::scale(model, glm::vec3(0.2f));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        glDrawArrays(GL_TRIANGLES, 0, 36);

        // Cubo  8- VERDE FUERTE (CENTRO)
        glVertexAttrib3f(1, 0.05f, 0.40f, 0.05f);
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.0f, -1.2f, 0.0f));
        model = glm::scale(model, glm::vec3(0.2f));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        glDrawArrays(GL_TRIANGLES, 0, 36);

        // Cubo 9 - VERDE CLARO  
        glVertexAttrib3f(1, 0.60f, 0.95f, 0.60f);
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.0f, -1.4f, 0.0f));
        model = glm::scale(model, glm::vec3(0.2f));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        glDrawArrays(GL_TRIANGLES, 0, 36);

        //EN EL CUBO 6 INICIA PRIMERA HOJA 
        glVertexAttrib3f(1, 0.30f, 0.80f, 0.30f);
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.2f, -0.8f, 0.0f));
        model = glm::scale(model, glm::vec3(0.2f));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        glDrawArrays(GL_TRIANGLES, 0, 36);

        glVertexAttrib3f(1, 0.18f, 0.65f, 0.18f);
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.4f, -0.8f, 0.0f));
        model = glm::scale(model, glm::vec3(0.2f));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        glDrawArrays(GL_TRIANGLES, 0, 36);

        glVertexAttrib3f(1, 0.15f, 0.55f, 0.15f);
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.6f, -0.8f, 0.0f));
        model = glm::scale(model, glm::vec3(0.2f));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        glDrawArrays(GL_TRIANGLES, 0, 36);

        glVertexAttrib3f(1, 0.15f, 0.55f, 0.15f);
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.4f, -0.6f, 0.0f));
        model = glm::scale(model, glm::vec3(0.2f));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        glDrawArrays(GL_TRIANGLES, 0, 36);

        glVertexAttrib3f(1, 0.08f, 0.35f, 0.08f);
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.6f, -0.6f, 0.0f));
        model = glm::scale(model, glm::vec3(0.2f));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        glDrawArrays(GL_TRIANGLES, 0, 36);

        glVertexAttrib3f(1, 0.15f, 0.55f, 0.15f);
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.8f, -0.6f, 0.0f));
        model = glm::scale(model, glm::vec3(0.2f));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        glDrawArrays(GL_TRIANGLES, 0, 36);

        glVertexAttrib3f(1, 0.18f, 0.65f, 0.18f);
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.6f, -0.4f, 0.0f));
        model = glm::scale(model, glm::vec3(0.2f));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        glDrawArrays(GL_TRIANGLES, 0, 36);

        glVertexAttrib3f(1, 0.55f, 0.85f, 0.55f);
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.8f, -0.4f, 0.0f));
        model = glm::scale(model, glm::vec3(0.2f));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        glDrawArrays(GL_TRIANGLES, 0, 36);



        //SEGUNDA HOJA 
        // Cubo 4 - VERDE CLARO  
        glVertexAttrib3f(1, 0.18f, 0.65f, 0.18f);
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(-0.2f, -0.4f, 0.0f));
        model = glm::scale(model, glm::vec3(0.2f));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        glDrawArrays(GL_TRIANGLES, 0, 36);

        glVertexAttrib3f(1, 0.10f, 0.50f, 0.10f);
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(-0.4f, -0.4f, 0.0f));
        model = glm::scale(model, glm::vec3(0.2f));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        glDrawArrays(GL_TRIANGLES, 0, 36);

        glVertexAttrib3f(1, 0.03f, 0.30f, 0.03f);
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(-0.6f, -0.4f, 0.0f));
        model = glm::scale(model, glm::vec3(0.2f));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        glDrawArrays(GL_TRIANGLES, 0, 36);

        glVertexAttrib3f(1, 0.15f, 0.55f, 0.15f);
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(-0.4f, -0.2f, 0.0f));
        model = glm::scale(model, glm::vec3(0.2f));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        glDrawArrays(GL_TRIANGLES, 0, 36);

        glVertexAttrib3f(1, 0.55f, 0.85f, 0.55f);
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(-0.6f, -0.2f, 0.0f));
        model = glm::scale(model, glm::vec3(0.2f));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        glDrawArrays(GL_TRIANGLES, 0, 36);

        glVertexAttrib3f(1, 0.15f, 0.55f, 0.15f);
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(-0.8f, -0.2f, 0.0f));
        model = glm::scale(model, glm::vec3(0.2f));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        glDrawArrays(GL_TRIANGLES, 0, 36);

        glVertexAttrib3f(1, 0.10f, 0.50f, 0.10f);
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(-0.6f, 0.0f, 0.0f));
        model = glm::scale(model, glm::vec3(0.2f));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        glDrawArrays(GL_TRIANGLES, 0, 36);

        glVertexAttrib3f(1, 0.03f, 0.30f, 0.03f);
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(-0.8f, 0.0f, 0.0f));
        model = glm::scale(model, glm::vec3(0.2f));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        glDrawArrays(GL_TRIANGLES, 0, 36);


        // (Opcional) Si luego quieres volver a usar color por vértice del VBO:
        glEnableVertexAttribArray(1);

        glBindVertexArray(0);

        glfwSwapBuffers(window);
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);

    glfwTerminate();
    return EXIT_SUCCESS;
}

// Función para mover y rotar la cámara
void Inputs(GLFWwindow* window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        movX += 0.01f;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) 
        movX -= 0.01f;
    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) 
        movY += 0.01f;
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) 
        movY -= 0.01f;
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) 
        movZ -= 0.01f;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) 
        movZ += 0.01f;
    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) 
        rot += 0.3f;
    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) 
        rot -= 0.3f;
}