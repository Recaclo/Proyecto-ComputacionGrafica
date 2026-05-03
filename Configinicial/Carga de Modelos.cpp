// Laura Reyes Carrillo
// Carga de modelos
// 15 de Marzo de 2026
//Previo 6
//320015764


// Std. Includes
#include <string>

// GLEW
#include <GL/glew.h>

// GLFW
#include <GLFW/glfw3.h>

// GL includes
#include "Shader.h"
#include "Camera.h"
#include "Model.h"

// GLM Mathemtics
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
void KeyCallback( GLFWwindow *window, int key, int scancode, int action, int mode );
void MouseCallback( GLFWwindow *window, double xPos, double yPos );
void DoMovement( );


// Camera
//posicion de la camara que vamos a tener en un inicio, se coloca a 3 unidades en el eje z para que se pueda ver el modelo que vamos a cargar
//si se coloca en el 0,0,0 no se va a ver nada porque el modelo se va a cargar en esa posicion y la camara va a estar dentro del modelo se ve algo pero muy raro, como por dentro del modelo
Camera camera( glm::vec3( 0.0f, 0.0f, 3.0f ) );
bool keys[1024];
GLfloat lastX = 400, lastY = 300;
bool firstMouse = true;

GLfloat deltaTime = 0.0f;
GLfloat lastFrame = 0.0f;



int main( )
{
    // Init GLFW
    glfwInit( );
    // Set all the required options for GLFW
    glfwWindowHint( GLFW_CONTEXT_VERSION_MAJOR, 3 );
    glfwWindowHint( GLFW_CONTEXT_VERSION_MINOR, 3 );
    glfwWindowHint( GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE );
    glfwWindowHint( GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE );
    glfwWindowHint( GLFW_RESIZABLE, GL_FALSE );
    
    // Create a GLFWwindow object that we can use for GLFW's functions
    GLFWwindow *window = glfwCreateWindow( WIDTH, HEIGHT, "Reyes Carrillo Laura Carga de modelos y camara sintetica", nullptr, nullptr );
    
    if ( nullptr == window )
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate( );
        
        return EXIT_FAILURE;
    }
    
    glfwMakeContextCurrent( window );
    
    glfwGetFramebufferSize( window, &SCREEN_WIDTH, &SCREEN_HEIGHT );
    
    // Set the required callback functions
    glfwSetKeyCallback( window, KeyCallback );
    glfwSetCursorPosCallback( window, MouseCallback );
    
    // GLFW Options
    //glfwSetInputMode( window, GLFW_CURSOR, GLFW_CURSOR_DISABLED );
    
    // Set this to true so GLEW knows to use a modern approach to retrieving function pointers and extensions
    glewExperimental = GL_TRUE;
    // Initialize GLEW to setup the OpenGL Function pointers
    if ( GLEW_OK != glewInit( ) )
    {
        std::cout << "Failed to initialize GLEW" << std::endl;
        return EXIT_FAILURE;
    }
    
    // Define the viewport dimensions
    glViewport( 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT );
    
    // OpenGL options
    glEnable( GL_DEPTH_TEST );
    
    // Setup and compile our shaders
    Shader shader( "Shader/modelLoading.vs", "Shader/modelLoading.frag" );
    
    // Load models
    //load models
	//cargar la ruta y el nombre del objeto a cargar en el constructor de la clase Model
    //el material no es necesario agregarlo dado que dentro del archivo obj esta el nombre del material 
	// a su ves dentro del material esta la ruta y el nombre de la textura a cargar
    // y como todo esta en la misma carpeta no importa 
	Model Dog((char*)"Models/RedDog.obj");
    Model Temple((char*)"Models/Japanese_Temple.obj");
	Model Tori((char*)"Models/torii.obj");
	Model Tree((char*)"Models/tree.obj");
	Model Shire((char*)"Models/ShrineLamp.obj");
    Model Statue((char*)"Models/SM_Lantern_01.obj");
    glm::mat4 projection = glm::perspective( camera.GetZoom( ), ( float )SCREEN_WIDTH/( float )SCREEN_HEIGHT, 0.1f, 100.0f );
    
  

    // Game loop
    while (!glfwWindowShouldClose(window))
    {
        // Set frame time
        GLfloat currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // Check and call events
        glfwPollEvents();
        DoMovement();

        // Clear the colorbuffer
        glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        shader.Use();

        glm::mat4 view = camera.GetViewMatrix();
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "view"), 1, GL_FALSE, glm::value_ptr(view));

        // Draw the loaded model
        // Dibujo del templo
		//inicailizamos la matriz de modelo con la matriz identidad para que el modelo se dibuje sin ninguna transformación
        glm::mat4 model(1);// reinicia a identidad
        model = glm::translate(model, glm::vec3(0.0f, -5.0f, -15.0f)); // Z negativo para alejarlo
        model = glm::scale(model, glm::vec3(0.8f, 0.8f, 0.8f)); // Ajusta el tamaño si es necesario
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        Temple.Draw(shader);


        //Dibujo de la puerta tori
        model = glm::mat4(1.0f);
		//dibujamos el modelo con el shader que hemos creado y cargado
		model = glm::translate(model, glm::vec3(0.0f, -5.0f, 0.0f)); // En el origen o cerca
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
		Tori.Draw(shader);

        //Dibujo del perrito 
        model = glm::mat4(1.0f);
        //dibujamos el modelo con el shader que hemos creado y cargado
        model = glm::translate(model, glm::vec3(0.0f, -5.0f, 5.0f)); // En el origen o cerca
        model = glm::scale(model, glm::vec3(3.0f, 3.0f, 3.0f)); // Escalarlo si el modelo es muy grande
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        Dog.Draw(shader);
        

		//Dibujo del shire lamp1
        model = glm::mat4(1.0f);
        //dibujamos el modelo con el shader que hemos creado y cargado
        model = glm::translate(model, glm::vec3(-5.0f, -5.0f, 0.0f)); // En el origen o cerca
        model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f)); // Escalarlo si el modelo es muy grande
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
		Shire.Draw(shader);

        //Dibujo del shire lamp2
        model = glm::mat4(1.0f);
        //dibujamos el modelo con el shader que hemos creado y cargado
        model = glm::translate(model, glm::vec3(5.0f, -5.0f, 0.0f)); // En el origen o cerca
        model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f)); // Escalarlo si el modelo es muy grande
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        Shire.Draw(shader);

        //Dibujo del arbol
        model = glm::mat4(1.0f);
        //dibujamos el modelo con el shader que hemos creado y cargado
        model = glm::translate(model, glm::vec3(-5.0f, -5.0f, 5.0f)); // En el origen o cerca
        model = glm::scale(model, glm::vec3(0.01f, 0.01f, 0.01f)); // Escalarlo si el modelo es muy grande
		glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
		Tree.Draw(shader);

        //Dibujo del arbol 2 
        model = glm::mat4(1.0f);
        //dibujamos el modelo con el shader que hemos creado y cargado
        model = glm::translate(model, glm::vec3(5.0f, -5.0f, 5.0f)); // En el origen o cerca
        model = glm::scale(model, glm::vec3(0.01f, 0.01f, 0.01f)); // Escalarlo si el modelo es muy grande
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        Tree.Draw(shader);

        // Dibujo estatua 1
		model = glm::mat4(1.0f);
        //dibujamos el modelo con el shader que hemos creado y cargado
        model = glm::translate(model, glm::vec3(-5.0f, -5.0f, 8.0f)); // En el origen o cerca
        model = glm::scale(model, glm::vec3(0.05f, 0.05f, 0.05f)); // Escalarlo si el modelo es muy grande
        model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
		Statue.Draw(shader);


        // Dibujo estatua 2
        model = glm::mat4(1.0f);
        //dibujamos el modelo con el shader que hemos creado y cargado
        model = glm::translate(model, glm::vec3(5.0f, -5.0f, 8.0f)); // En el origen o cerca
        model = glm::scale(model, glm::vec3(0.05f, 0.05f, 0.05f)); // Escalarlo si el modelo es muy grande
        model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        Statue.Draw(shader);

		//// aplicamos herramientas de transformacion para mover el modelo a la posicion que queremos
		//model = glm::translate(model, glm::vec3(3.0f, 0.0f, 0.0f)); // Translate it down a bit so it's at the center of the scene
		//model = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.3f));	//If the model is too big, scale it down
		//glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1,GL_FALSE, glm::value_ptr(model));
		//Dog.Draw(shader);


        // Swap the buffers
        glfwSwapBuffers( window );
    }
    
    glfwTerminate( );
    return 0;
}


// Moves/alters the camera positions based on user input
void DoMovement( )
{
    // Camera controls
    if ( keys[GLFW_KEY_W] || keys[GLFW_KEY_UP] )
    {
        camera.ProcessKeyboard( FORWARD, deltaTime );
    }
    
    if ( keys[GLFW_KEY_S] || keys[GLFW_KEY_DOWN] )
    {
        camera.ProcessKeyboard( BACKWARD, deltaTime );
    }
    
    if ( keys[GLFW_KEY_A] || keys[GLFW_KEY_LEFT] )
    {
        camera.ProcessKeyboard( LEFT, deltaTime );
    }
    
    if ( keys[GLFW_KEY_D] || keys[GLFW_KEY_RIGHT] )
    {
        camera.ProcessKeyboard( RIGHT, deltaTime );
    }

   
}

// Is called whenever a key is pressed/released via GLFW
void KeyCallback( GLFWwindow *window, int key, int scancode, int action, int mode )
{
    if ( GLFW_KEY_ESCAPE == key && GLFW_PRESS == action )
    {
        glfwSetWindowShouldClose(window, GL_TRUE);
    }
    
    if ( key >= 0 && key < 1024 )
    {
        if ( action == GLFW_PRESS )
        {
            keys[key] = true;
        }
        else if ( action == GLFW_RELEASE )
        {
            keys[key] = false;
        }
    }

 

 
}

void MouseCallback( GLFWwindow *window, double xPos, double yPos )
{
    if ( firstMouse )
    {
        lastX = xPos;
        lastY = yPos;
        firstMouse = false;
    }
    
    GLfloat xOffset = xPos - lastX;
    GLfloat yOffset = lastY - yPos;  // Reversed since y-coordinates go from bottom to left
    
    lastX = xPos;
    lastY = yPos;
    
    camera.ProcessMouseMovement( xOffset, yOffset );
}

