// Laura Reyes Carrillo
// Ilustracion de Materiales e Iluminacion y carga de modelos 
// 6 de Abril de 2026
// Practica 8
// 320015764

// Std. Includes
#include <string>
#include <cmath>

// GLEW
#include <GL/glew.h>

// GLFW
#include <GLFW/glfw3.h>

// GL includes
#include "Shader.h"
#include "Camera.h"
#include "Model.h"

// GLM Mathematics
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// Other Libs
#include "SOIL2/SOIL2.h"
#include "stb_image.h"

// Properties
const GLuint WIDTH = 800, HEIGHT = 600;
int SCREEN_WIDTH, SCREEN_HEIGHT;

// Function prototypes
void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mode);
void MouseCallback(GLFWwindow* window, double xPos, double yPos);
void DoMovement();

// Camera
Camera camera(glm::vec3(0.0f, 0.0f, 0.0f));
bool keys[1024];
GLfloat lastX = 400, lastY = 300;
bool firstMouse = true;

// Posicion inicial de las luces
glm::vec3 lightPos(0.0f, 0.0f, -5.0f);
glm::vec3 lightPos2(2.0f, 0.0f, -5.0f);

// Variables para mover luces manualmente
float movelightPos1 = 0.0f;
float movelightPos2 = 0.0f;

// Delta time
GLfloat deltaTime = 0.0f;
GLfloat lastFrame = 0.0f;

// Otras variables
float rot = 0.0f;
bool activanim = false;

// -------- NUEVAS VARIABLES PARA EL CICLO DIA/NOCHE --------
bool animTiempo = false;
float sunAngle = -90.0f;      // empieza en horizonte izquierdo
float orbitRadius = 10.0f;    // tamaño del arco
float orbitCenterX = 0.0f;    // centro horizontal
float orbitCenterY = -2.0f;   // altura del horizonte
float orbitZ = -15.0f;        // profundidad del recorrido
// ----------------------------------------------------------

int main()
{
    // Init GLFW
    glfwInit();

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Reyes Carrillo Laura Materiales e iluminacion", nullptr, nullptr);

    if (nullptr == window)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return EXIT_FAILURE;
    }

    glfwMakeContextCurrent(window);
    glfwGetFramebufferSize(window, &SCREEN_WIDTH, &SCREEN_HEIGHT);

    glfwSetKeyCallback(window, KeyCallback);
    glfwSetCursorPosCallback(window, MouseCallback);

    glewExperimental = GL_TRUE;
    if (GLEW_OK != glewInit())
    {
        std::cout << "Failed to initialize GLEW" << std::endl;
        return EXIT_FAILURE;
    }

    glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
    glEnable(GL_DEPTH_TEST);

    // Shaders
    Shader shader("Shader/modelLoading.vs", "Shader/modelLoading.frag");
    Shader lampshader("Shader/lamp.vs", "Shader/lamp.frag");
    Shader lightingShader("Shader/lighting.vs", "Shader/lighting.frag");

    // Load models
    Model red_dog((char*)"Models/RedDog.obj");
    Model cat((char*)"Models/miGato.obj");
    Model Japanese_Temple((char*)"Models/Japanese_Temple.obj");
    Model shrineLamp((char*)"Models/ShrineLamp.obj");
    Model lantern((char*)"Models/SM_Lantern_01.obj");
    Model torii((char*)"Models/torii.obj");
    Model tree((char*)"Models/tree.obj");

    glm::mat4 projection = glm::perspective(camera.GetZoom(), (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, 0.1f, 100.0f);

    // Cubo para dibujar las fuentes de luz
    float vertices[] = {
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
         0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,

        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
         0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,

        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
         0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
         0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
    };

    GLuint VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // Textura auxiliar
    GLuint texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    int textureWidth, textureHeight, nrChannels;
    stbi_set_flip_vertically_on_load(true);
    unsigned char* image;

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    image = stbi_load("Models/Texture_albedo.jpg", &textureWidth, &textureHeight, &nrChannels, 0);
    if (image)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, textureWidth, textureHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(image);

    // Game loop
    while (!glfwWindowShouldClose(window))
    {
        GLfloat currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        glfwPollEvents();
        DoMovement();

        // -------- CALCULO DEL MOVIMIENTO DEL SOL Y LA LUNA --------
        float sunRad = glm::radians(sunAngle);
        float moonRad = glm::radians(sunAngle + 180.0f);

        glm::vec3 currentSunPos(
            orbitCenterX + cos(sunRad) * orbitRadius,
            orbitCenterY + sin(sunRad) * orbitRadius,
            orbitZ
        );

        glm::vec3 currentMoonPos(
            orbitCenterX + cos(moonRad) * orbitRadius,
            orbitCenterY + sin(moonRad) * orbitRadius,
            orbitZ
        );

        bool sunVisible = currentSunPos.y > orbitCenterY;
        bool moonVisible = currentMoonPos.y > orbitCenterY;
        // ----------------------------------------------------------

        // Fondo segun tiempo
        if (sunVisible)
            glClearColor(0.45f, 0.75f, 0.95f, 1.0f); // dia
        else
            glClearColor(0.05f, 0.05f, 0.12f, 1.0f); // noche

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Shader de iluminacion principal
        lightingShader.Use();

        GLint viewPosLoc = glGetUniformLocation(lightingShader.Program, "viewPos");

        glUniform3f(glGetUniformLocation(lightingShader.Program, "light1.position"),
            currentSunPos.x,
            currentSunPos.y,
            currentSunPos.z);

        glUniform3f(glGetUniformLocation(lightingShader.Program, "light2.position"),
            currentMoonPos.x,
            currentMoonPos.y,
            currentMoonPos.z);

        glUniform3f(viewPosLoc, camera.GetPosition().x, camera.GetPosition().y, camera.GetPosition().z);

        // Luz 1 = SOL
        if (sunVisible)
        {
            glUniform3f(glGetUniformLocation(lightingShader.Program, "light1.ambient"), 0.6f, 0.6f, 0.5f);
            glUniform3f(glGetUniformLocation(lightingShader.Program, "light1.diffuse"), 1.0f, 0.95f, 0.8f);
            glUniform3f(glGetUniformLocation(lightingShader.Program, "light1.specular"), 1.0f, 1.0f, 0.9f);
        }
        else
        {
            glUniform3f(glGetUniformLocation(lightingShader.Program, "light1.ambient"), 0.03f, 0.03f, 0.05f);
            glUniform3f(glGetUniformLocation(lightingShader.Program, "light1.diffuse"), 0.0f, 0.0f, 0.0f);
            glUniform3f(glGetUniformLocation(lightingShader.Program, "light1.specular"), 0.0f, 0.0f, 0.0f);
        }

        // Luz 2 = LUNA
        if (moonVisible)
        {
            glUniform3f(glGetUniformLocation(lightingShader.Program, "light2.ambient"), 0.12f, 0.12f, 0.20f);
            glUniform3f(glGetUniformLocation(lightingShader.Program, "light2.diffuse"), 0.25f, 0.30f, 0.45f);
            glUniform3f(glGetUniformLocation(lightingShader.Program, "light2.specular"), 0.20f, 0.20f, 0.30f);
        }
        else
        {
            glUniform3f(glGetUniformLocation(lightingShader.Program, "light2.ambient"), 0.0f, 0.0f, 0.0f);
            glUniform3f(glGetUniformLocation(lightingShader.Program, "light2.diffuse"), 0.0f, 0.0f, 0.0f);
            glUniform3f(glGetUniformLocation(lightingShader.Program, "light2.specular"), 0.0f, 0.0f, 0.0f);
        }

        glm::mat4 view = camera.GetViewMatrix();
        glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
        glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "view"), 1, GL_FALSE, glm::value_ptr(view));

        glUniform3f(glGetUniformLocation(lightingShader.Program, "material.ambient"), 1.0f, 1.0f, 1.0f);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "material.diffuse"), 1.0f, 1.0f, 1.0f);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "material.specular"), 0.3f, 0.3f, 0.3f);
        glUniform1f(glGetUniformLocation(lightingShader.Program, "material.shininess"), 32.0f);

        // Perro
        glm::mat4 model(1.0f);
        model = glm::translate(model, glm::vec3(0.0f, -5.0f, 5.0f));
        model = glm::scale(model, glm::vec3(3.0f, 3.0f, 3.0f));
        glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        red_dog.Draw(lightingShader);

        // Gato
        glm::mat4 model2(1.0f);
        model2 = glm::translate(model2, glm::vec3(3.0f, -5.0f, 5.0f));
        model2 = glm::scale(model2, glm::vec3(0.5f, 0.5f, 0.5f));
        glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model2));
        cat.Draw(lightingShader);

        // Templo
        glm::mat4 model3(1.0f);
        model3 = glm::translate(model3, glm::vec3(0.0f, -5.0f, -15.0f));
        model3 = glm::scale(model3, glm::vec3(0.8f, 0.8f, 0.8f));
        glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model3));
        Japanese_Temple.Draw(lightingShader);

        // Torii
        glm::mat4 model4(1.0f);
        model4 = glm::translate(model4, glm::vec3(0.0f, -5.0f, 0.0f));
        glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model4));
        torii.Draw(lightingShader);

        // Lampara izquierda
        glm::mat4 model5(1.0f);
        model5 = glm::translate(model5, glm::vec3(-5.0f, -5.0f, 0.0f));
        model5 = glm::scale(model5, glm::vec3(1.0f, 1.0f, 1.0f));
        glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model5));
        shrineLamp.Draw(lightingShader);

        // Lampara derecha
        glm::mat4 model6(1.0f);
        model6 = glm::translate(model6, glm::vec3(5.0f, -5.0f, 0.0f));
        model6 = glm::scale(model6, glm::vec3(1.0f, 1.0f, 1.0f));
        glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model6));
        shrineLamp.Draw(lightingShader);

        // Arbol izquierdo
        glm::mat4 model7(1.0f);
        model7 = glm::translate(model7, glm::vec3(-5.0f, -5.0f, 5.0f));
        model7 = glm::scale(model7, glm::vec3(0.01f, 0.01f, 0.01f));
        glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model7));
        tree.Draw(lightingShader);

        // Arbol derecho
        glm::mat4 model8(1.0f);
        model8 = glm::translate(model8, glm::vec3(5.0f, -5.0f, 5.0f));
        model8 = glm::scale(model8, glm::vec3(0.01f, 0.01f, 0.01f));
        glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model8));
        tree.Draw(lightingShader);

        // Linterna izquierda
        glm::mat4 model9(1.0f);
        model9 = glm::translate(model9, glm::vec3(-5.0f, -5.0f, 8.0f));
        model9 = glm::scale(model9, glm::vec3(0.05f, 0.05f, 0.05f));
        model9 = glm::rotate(model9, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
        glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model9));
        lantern.Draw(lightingShader);

        // Linterna derecha
        glm::mat4 model10(1.0f);
        model10 = glm::translate(model10, glm::vec3(5.0f, -5.0f, 8.0f));
        model10 = glm::scale(model10, glm::vec3(0.05f, 0.05f, 0.05f));
        model10 = glm::rotate(model10, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
        glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model10));
        lantern.Draw(lightingShader);

        glBindVertexArray(0);

        // -------- DIBUJAR SOL Y LUNA --------
        lampshader.Use();
        glUniformMatrix4fv(glGetUniformLocation(lampshader.Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
        glUniformMatrix4fv(glGetUniformLocation(lampshader.Program, "view"), 1, GL_FALSE, glm::value_ptr(view));

        glBindVertexArray(VAO);

        // Sol
        if (sunVisible)
        {
            glUniform3f(glGetUniformLocation(lampshader.Program, "lampColor"), 1.0f, 0.9f, 0.2f);

            glm::mat4 sunModel(1.0f);
            sunModel = glm::translate(sunModel, currentSunPos);
            sunModel = glm::scale(sunModel, glm::vec3(0.7f, 0.7f, 0.7f));
            glUniformMatrix4fv(glGetUniformLocation(lampshader.Program, "model"), 1, GL_FALSE, glm::value_ptr(sunModel));

            glDrawArrays(GL_TRIANGLES, 0, 36);
        }

        // Luna
        if (moonVisible)
        {
            glUniform3f(glGetUniformLocation(lampshader.Program, "lampColor"), 0.2f, 0.3f, 0.7f);

            glm::mat4 moonModel(1.0f);
            moonModel = glm::translate(moonModel, currentMoonPos);
            moonModel = glm::scale(moonModel, glm::vec3(0.45f, 0.45f, 0.45f));
            glUniformMatrix4fv(glGetUniformLocation(lampshader.Program, "model"), 1, GL_FALSE, glm::value_ptr(moonModel));

            glDrawArrays(GL_TRIANGLES, 0, 36);
        }

        glBindVertexArray(0);
        // -----------------------------------

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

    if (activanim)
    {
        if (rot > -90.0f)
            rot -= 0.1f;
    }

    // -------- ANIMACION DEL TIEMPO --------
    if (animTiempo)
    {
        sunAngle += 20.0f * deltaTime;

        if (sunAngle > 270.0f)
            sunAngle = -90.0f;
    }
    // --------------------------------------
}

// Is called whenever a key is pressed/released via GLFW
void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
    if (GLFW_KEY_ESCAPE == key && GLFW_PRESS == action)
        glfwSetWindowShouldClose(window, GL_TRUE);

    if (key >= 0 && key < 1024)
    {
        if (action == GLFW_PRESS)
            keys[key] = true;
        else if (action == GLFW_RELEASE)
            keys[key] = false;
    }

    // activar / pausar paso del tiempo
    if (key == GLFW_KEY_T && action == GLFW_PRESS)
    {
        animTiempo = !animTiempo;
    }

    // mover luz 1 manualmente si quieres seguir usando esto
    if (key == GLFW_KEY_O && action == GLFW_PRESS)
        movelightPos1 += 0.1f;

    if (key == GLFW_KEY_L && action == GLFW_PRESS)
        movelightPos1 -= 0.1f;

    if (key == GLFW_KEY_K && action == GLFW_PRESS)
        movelightPos2 += 0.1f;

    if (key == GLFW_KEY_J && action == GLFW_PRESS)
        movelightPos2 -= 0.1f;
}

void MouseCallback(GLFWwindow* window, double xPos, double yPos)
{
    if (firstMouse)
    {
        lastX = xPos;
        lastY = yPos;
        firstMouse = false;
    }

    GLfloat xOffset = xPos - lastX;
    GLfloat yOffset = lastY - yPos;

    lastX = xPos;
    lastY = yPos;

    camera.ProcessMouseMovement(xOffset, yOffset);
}