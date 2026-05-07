// Laura Reyes Carrillo
// Proyecto 
// 29 de Abril de 2026   
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

//Load Models
#include "SOIL2/SOIL2.h"


// Other includes
#include "Shader.h"
#include "Camera.h"
#include "Model.h"

// Function prototypes
void KeyCallback(GLFWwindow *window, int key, int scancode, int action, int mode);
void MouseCallback(GLFWwindow *window, double xPos, double yPos);
void DoMovement();
void Animation();
void InitProjectAnimations();
void UpdateProjectAnimations();
void DrawCubeObject(Shader shader, GLuint VAO, GLint modelLoc, glm::vec3 pos, glm::vec3 scale, glm::vec4 color, float rotY = 0.0f);
void DrawProjectAnimations(Shader colorShader, GLuint VAO, glm::mat4 view, glm::mat4 projection);
void DrawStandModel(Model& standModel, Shader shader, GLint modelLoc, glm::vec3 pos, glm::vec3 scale, float rotY);

// Window dimensions
const GLuint WIDTH = 800, HEIGHT = 600;
int SCREEN_WIDTH, SCREEN_HEIGHT;

// Camera
Camera  camera(glm::vec3(0.0f, 0.0f, 3.0f));
GLfloat lastX = WIDTH / 2.0;
GLfloat lastY = HEIGHT / 2.0;
bool keys[1024];
bool firstMouse = true;
// Light attributes
glm::vec3 lightPos(0.0f, 0.0f, 0.0f);
bool active;

// Positions of the point lights
glm::vec3 pointLightPositions[] = {
	glm::vec3(0.0f,2.0f, 0.0f),
	glm::vec3(0.0f,0.0f, 0.0f),
	glm::vec3(0.0f,0.0f,  0.0f),
	glm::vec3(0.0f,0.0f, 0.0f)
};

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



glm::vec3 Light1 = glm::vec3(0);
//Anim
//variables de cada parte del perro, para controlar su animacion
float rotBall = 0.0f;
bool AnimBall = false;
bool AnimDog = false;
float rotDog = 0.0f;
int dogAnim = 0;
float FLegs = 0.0f;
float RLegs = 0.0f;
float head = 0.0f;
float tail = 0.0f;
glm::vec3 dogPos (0.0f,0.0f,0.0f);
float dogRot = 0.0f;
bool step = false;
// ===============================
// ANIMACIONES DEL PROYECTO PUENTE
// ===============================

// Activadores
bool animStands = false;
bool animPeople = false;
bool animSigns = false;
bool animScreens = false;
bool animReactiveLights = true;

// Tiempo interno para animaciones
float projectTime = 0.0f;

// Animación 2: stands con KeyFrames
float standAnimTime = 0.0f;
float standScale = 0.0f;

// Posiciones de stands, ajusta X/Z si no coinciden con tu puente
const int NUM_STANDS = 8;

glm::vec3 standPositions[NUM_STANDS] = {
	// Lado izquierdo del puente
	glm::vec3(-1.4f, -0.65f, -6.6f),
	glm::vec3(-1.4f, -0.65f, -5.8f),
	glm::vec3(-1.4f, -0.65f, -5.0f),
	glm::vec3(-1.4f, -0.65f, -4.2f),

	// Lado derecho del puente
	glm::vec3(1.4f, -0.65f, -6.6f),
	glm::vec3(1.4f, -0.65f, -5.8f),
	glm::vec3(1.4f, -0.65f, -5.0f),
	glm::vec3(1.4f, -0.65f, -4.2f)
};

float standRotations[NUM_STANDS] = {
	90.0f, 90.0f, 90.0f, 90.0f,
   -90.0f, -90.0f, -90.0f, -90.0f
};

// Animación 1: personas
const int NUM_PEOPLE = 12;
glm::vec3 peoplePos[NUM_PEOPLE];
float peopleSpeed[NUM_PEOPLE];
int peopleDir[NUM_PEOPLE];

// Animación 4: señalización
float arrowPulse = 0.0f;
int arrowDirection = 1;

// Animación 6: pantallas
float screenTimer = 0.0f;
int screenSlide = 0;


// Deltatime
GLfloat deltaTime = 0.0f;	// Time between current frame and last frame
GLfloat lastFrame = 0.0f;  	// Time of last frame

int main()
{
	// Init GLFW
	glfwInit();
	// Set all the required options for GLFW
	/*glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);*/

	// Create a GLFWwindow object that we can use for GLFW's functions
	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Reyes Carrillo Laura ", nullptr, nullptr);

	if (nullptr == window)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();

		return EXIT_FAILURE;
	}

	glfwMakeContextCurrent(window);

	glfwGetFramebufferSize(window, &SCREEN_WIDTH, &SCREEN_HEIGHT);

	// Set the required callback functions
	glfwSetKeyCallback(window, KeyCallback);
	glfwSetCursorPosCallback(window, MouseCallback);

	// GLFW Options
	//glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// Set this to true so GLEW knows to use a modern approach to retrieving function pointers and extensions
	glewExperimental = GL_TRUE;
	// Initialize GLEW to setup the OpenGL Function pointers
	if (GLEW_OK != glewInit())
	{
		std::cout << "Failed to initialize GLEW" << std::endl;
		return EXIT_FAILURE;
	}

	// Define the viewport dimensions
	glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);



	Shader lightingShader("Shader/lighting.vs", "Shader/lighting.frag");
	Shader lampShader("Shader/lamp.vs", "Shader/lamp.frag");
	Shader colorShader("Shader/color.vs", "Shader/color.frag");

	//models
	//cada una de las piezas del perro pero por separado 
	/*Model DogBody((char*)"Models/DogBody.obj");
	Model HeadDog((char*)"Models/HeadDog.obj");
	Model DogTail((char*)"Models/TailDog.obj");
	Model F_RightLeg((char*)"Models/F_RightLegDog.obj");
	Model F_LeftLeg((char*)"Models/F_LeftLegDog.obj");
	Model B_RightLeg((char*)"Models/B_RightLegDog.obj");
	Model B_LeftLeg((char*)"Models/B_LeftLegDog.obj");
	Model Piso((char*)"Models/piso.obj");
	Model Ball((char*)"Models/ball.obj");*/
	// Modelo principal del proyecto: puente de la Facultad de Ingeniería
	Model Puente((char*)"Models/final_puente.obj");
	InitProjectAnimations();
	Model Stand((char*)"Models/3d-model.obj");



	// First, set the container's VAO (and VBO)
	GLuint VBO, VAO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	// Position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	// normal attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	// Set texture units
	lightingShader.Use();
	glUniform1i(glGetUniformLocation(lightingShader.Program, "Material.difuse"), 0);
	glUniform1i(glGetUniformLocation(lightingShader.Program, "Material.specular"), 1);

	glm::mat4 projection = glm::perspective(camera.GetZoom(), (GLfloat)SCREEN_WIDTH / (GLfloat)SCREEN_HEIGHT, 0.1f, 100.0f);

	// Game loop
	while (!glfwWindowShouldClose(window))
	{

		// Calculate deltatime of current frame
		GLfloat currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		// Check if any events have been activiated (key pressed, mouse moved etc.) and call corresponding response functions
		glfwPollEvents();
		DoMovement();
		Animation();
		UpdateProjectAnimations();

		// Clear the colorbuffer
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	   
		// OpenGL options
		glEnable(GL_DEPTH_TEST);

		
		glm::mat4 modelTemp = glm::mat4(1.0f); //Temp
		
	

		// Use cooresponding shader when setting uniforms/drawing objects
		lightingShader.Use();

        glUniform1i(glGetUniformLocation(lightingShader.Program, "diffuse"), 0);
		//glUniform1i(glGetUniformLocation(lightingShader.Program, "specular"),1);

		GLint viewPosLoc = glGetUniformLocation(lightingShader.Program, "viewPos");
		glUniform3f(viewPosLoc, camera.GetPosition().x, camera.GetPosition().y, camera.GetPosition().z);


		// Directional light
		glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.direction"), -0.2f, -1.0f, -0.3f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.ambient"),0.6f,0.6f,0.6f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.diffuse"), 0.6f, 0.6f, 0.6f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.specular"),0.3f, 0.3f, 0.3f);


		// Point light 1
	    glm::vec3 lightColor;
		lightColor.x= abs(sin(glfwGetTime() *Light1.x));
		lightColor.y= abs(sin(glfwGetTime() *Light1.y));
		lightColor.z= sin(glfwGetTime() *Light1.z);

		
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].position"), pointLightPositions[0].x, pointLightPositions[0].y, pointLightPositions[0].z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].ambient"), lightColor.x,lightColor.y, lightColor.z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].diffuse"), lightColor.x,lightColor.y,lightColor.z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].specular"), 1.0f, 0.2f, 0.2f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[0].constant"), 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[0].linear"), 0.045f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[0].quadratic"),0.075f);


		// SpotLight
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.position"), camera.GetPosition().x, camera.GetPosition().y, camera.GetPosition().z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.direction"), camera.GetFront().x, camera.GetFront().y, camera.GetFront().z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.ambient"), 0.2f, 0.2f, 0.8f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.diffuse"), 0.2f, 0.2f, 0.8f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.specular"), 0.0f, 0.0f, 0.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.constant"), 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.linear"), 0.3f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.quadratic"), 0.7f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.cutOff"), glm::cos(glm::radians(12.0f)));
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.outerCutOff"), glm::cos(glm::radians(18.0f)));
		

		// Set material properties
		glUniform1f(glGetUniformLocation(lightingShader.Program, "material.shininess"), 5.0f);

		// Create camera transformations
		glm::mat4 view;
		view = camera.GetViewMatrix();

		// Get the uniform locations
		GLint modelLoc = glGetUniformLocation(lightingShader.Program, "model");
		GLint viewLoc = glGetUniformLocation(lightingShader.Program, "view");
		GLint projLoc = glGetUniformLocation(lightingShader.Program, "projection");

		// Pass the matrices to the shader
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));


		glm::mat4 model(1);

	
		
		// Carga del modelo del puente
		view = camera.GetViewMatrix();

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, -1.0f, -5.0f));
		model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));

		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Puente.Draw(lightingShader);
		// ===============================
// STANDS CON MODELOS 3D REALES
// ===============================
		for (int i = 0; i < NUM_STANDS; i++)
		{
			float distToCamera = glm::distance(camera.GetPosition(), standPositions[i]);
			bool activeStand = distToCamera < 1.8f;

			float focusScale = activeStand ? 1.15f : 1.0f;

			glm::vec3 finalScale = glm::vec3(0.12f, 0.12f, 0.12f) * standScale * focusScale;

			if (i % 3 == 0)
			{
				DrawStandModel(Stand, lightingShader, modelLoc, standPositions[i], finalScale, standRotations[i]);
			}
			else if (i % 3 == 1)
			{
				DrawStandModel(Stand, lightingShader, modelLoc, standPositions[i], finalScale, standRotations[i]);
			}
			else
			{
				DrawStandModel(Stand, lightingShader, modelLoc, standPositions[i], finalScale, standRotations[i]);
			}
		}

		DrawProjectAnimations(colorShader, VAO, view, projection);

		//model = glm::mat4(1);
		//glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		//glUniform1i(glGetUniformLocation(lightingShader.Program, "transparency"), 0);
		////Body
		//modelTemp= model = glm::translate(model, dogPos);
		//modelTemp= model = glm::rotate(model, glm::radians(dogRot), glm::vec3(0.0f, 1.0f, 0.0f));
		//glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		//DogBody.Draw(lightingShader);
		////Head
		//model = modelTemp;
		//model = glm::translate(model, glm::vec3(0.0f, 0.093f, 0.208f));
		//model = glm::rotate(model, glm::radians(head), glm::vec3(0.0f, 0.0f, 1.0f));
		//glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		//HeadDog.Draw(lightingShader);
		////Tail 
		//model = modelTemp;
		//model = glm::translate(model, glm::vec3(0.0f, 0.026f, -0.288f));
		//model = glm::rotate(model, glm::radians(tail), glm::vec3(0.0f, 0.0f, -1.0f)); 
		//glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model)); 
		//DogTail.Draw(lightingShader);
		////Front Left Leg
		//model = modelTemp;
		//model = glm::translate(model, glm::vec3(0.112f, -0.044f, 0.074f));
		//model = glm::rotate(model, glm::radians(FLegs), glm::vec3(-1.0f, 0.0f, 0.0f)); 
		//glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		//F_LeftLeg.Draw(lightingShader);
		////Front Right Leg
		//model = modelTemp; 
		//model = glm::translate(model, glm::vec3(-0.111f, -0.055f, 0.074f));
		//model = glm::rotate(model, glm::radians(FLegs), glm::vec3(1.0f, 0.0f, 0.0f));
		//glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		//F_RightLeg.Draw(lightingShader);
		////Back Left Leg
		//model = modelTemp; 
		//model = glm::translate(model, glm::vec3(0.082f, -0.046, -0.218)); 
		//model = glm::rotate(model, glm::radians(RLegs), glm::vec3(1.0f, 0.0f, 0.0f)); 
		//glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model)); 
		//B_LeftLeg.Draw(lightingShader);
		////Back Right Leg
		//model = modelTemp; 
		//model = glm::translate(model, glm::vec3(-0.083f, -0.057f, -0.231f));
		//model = glm::rotate(model, glm::radians(RLegs), glm::vec3(-1.0f, 0.0f, 0.0f));
		//glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		//B_RightLeg.Draw(lightingShader); 


		//model = glm::mat4(1);
		//glEnable(GL_BLEND);//Avtiva la funcionalidad para trabajar el canal alfa
		//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		//glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		//glUniform1i(glGetUniformLocation(lightingShader.Program, "transparency"), 1);
		//model = glm::rotate(model, glm::radians(rotBall), glm::vec3(0.0f, 1.0f, 0.0f));
		//glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
	 //   Ball.Draw(lightingShader); 
		//glDisable(GL_BLEND);  //Desactiva el canal alfa 
		//glBindVertexArray(0);
	

		// Also draw the lamp object, again binding the appropriate shader
		lampShader.Use();
		// Get location objects for the matrices on the lamp shader (these could be different on a different shader)
		modelLoc = glGetUniformLocation(lampShader.Program, "model");
		viewLoc = glGetUniformLocation(lampShader.Program, "view");
		projLoc = glGetUniformLocation(lampShader.Program, "projection");

		// Set matrices
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));
		model = glm::mat4(1);
		model = glm::translate(model, lightPos);
		model = glm::scale(model, glm::vec3(0.2f)); // Make it a smaller cube
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		// Draw the light object (using light's vertex attributes)
		
			model = glm::mat4(1);
			model = glm::translate(model, pointLightPositions[0]);
			model = glm::scale(model, glm::vec3(0.2f)); // Make it a smaller cube
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			glBindVertexArray(VAO);
			glDrawArrays(GL_TRIANGLES, 0, 36);
		
		glBindVertexArray(0);



		// Swap the screen buffers
		glfwSwapBuffers(window);
	}


	// Terminate GLFW, clearing any resources allocated by GLFW.
	glfwTerminate();



	return 0;
}

// Moves/alters the camera positions based on user input
void DoMovement()
{

	// Camera controls
	if (keys[GLFW_KEY_W] || keys[GLFW_KEY_UP])
	{
		camera.ProcessKeyboard(FORWARD, deltaTime);

	}

	if (keys[GLFW_KEY_S] || keys[GLFW_KEY_DOWN])
	{
		camera.ProcessKeyboard(BACKWARD, deltaTime);


	}

	if (keys[GLFW_KEY_A] || keys[GLFW_KEY_LEFT])
	{
		camera.ProcessKeyboard(LEFT, deltaTime);


	}

	if (keys[GLFW_KEY_D] || keys[GLFW_KEY_RIGHT])
	{
		camera.ProcessKeyboard(RIGHT, deltaTime);


	}

	if (keys[GLFW_KEY_T])
	{
		pointLightPositions[0].x += 0.01f;
	}
	if (keys[GLFW_KEY_G])
	{
		pointLightPositions[0].x -= 0.01f;
	}

	if (keys[GLFW_KEY_Y])
	{
		pointLightPositions[0].y += 0.01f;
	}

	if (keys[GLFW_KEY_H])
	{
		pointLightPositions[0].y -= 0.01f;
	}
	if (keys[GLFW_KEY_U])
	{
		pointLightPositions[0].z -= 0.1f;
	}
	if (keys[GLFW_KEY_J])
	{
		pointLightPositions[0].z += 0.01f;
	}
	
}

// Is called whenever a key is pressed/released via GLFW
void KeyCallback(GLFWwindow *window, int key, int scancode, int action, int mode)
{
	if (GLFW_KEY_ESCAPE == key && GLFW_PRESS == action)
	{
		glfwSetWindowShouldClose(window, GL_TRUE);
	}

	if (key >= 0 && key < 1024)
	{
		if (action == GLFW_PRESS)
		{
			keys[key] = true;
		}
		else if (action == GLFW_RELEASE)
		{
			keys[key] = false;
		}
	}

	if (keys[GLFW_KEY_SPACE])
	{
		active = !active;
		if (active)
		{
			Light1 = glm::vec3(0.2f, 0.8f, 1.0f);
			
		}
		else
		{
			Light1 = glm::vec3(0);//Cuado es solo un valor en los 3 vectores pueden dejar solo una componente
		}
	}
	if (keys[GLFW_KEY_N])
	{
		AnimBall = !AnimBall;
		
	}
	if (keys[GLFW_KEY_B])
	{
		dogAnim = 1;
		
	}
	if (key == GLFW_KEY_1 && action == GLFW_PRESS)
	{
		animStands = !animStands;
	}

	if (key == GLFW_KEY_2 && action == GLFW_PRESS)
	{
		animPeople = !animPeople;
	}

	if (key == GLFW_KEY_3 && action == GLFW_PRESS)
	{
		animSigns = !animSigns;
	}

	if (key == GLFW_KEY_4 && action == GLFW_PRESS)
	{
		animScreens = !animScreens;
	}

	if (key == GLFW_KEY_5 && action == GLFW_PRESS)
	{
		animReactiveLights = !animReactiveLights;
	}

	
}
void Animation() {

	if (AnimBall)
	{
		rotBall += 0.4f;
		//printf("%f", rotBall);
	}

	if (AnimDog)
	{
		rotDog -= 0.6f;
		//printf("%f", rotBall);
	}

	if (dogAnim != 0) { //walk animation
		if (!step) {
			FLegs += 30.0f * deltaTime;
			RLegs += 30.0f * deltaTime;
			head += 10.0f * deltaTime;
			tail -= 30.0f * deltaTime;
			if (RLegs > 15.0f)
				step = true;
		}
		else {
			FLegs -= 30.0f * deltaTime;
			RLegs -= 30.0f * deltaTime;
			head -= 10.0f * deltaTime;
			tail += 30.0f * deltaTime;
			if (RLegs < -15.0f)
				step = false;
		}
	}
	float limiteZ = 2.2f;
	float limiteX = 2.2f;
	float velocidad = 0.3f;
	float velocidadGiro = 90.0f;
	switch (dogAnim)
	{
	case 1: // Camina en +Z
		if (dogPos.z < limiteZ)
			dogPos.z += velocidad * deltaTime;
		else
			dogAnim = 2;
		break;

	case 2: // Gira 90 grados
		if (dogRot > -90.0f) {
			dogRot -= velocidadGiro * deltaTime;
			dogPos.x += sin(glm::radians(dogRot)) * velocidad * deltaTime;
			dogPos.z += cos(glm::radians(dogRot)) * velocidad * deltaTime;
		}
		else {
			dogRot = -90.0f;
			dogAnim = 3;
		}
		break;

	case 3: // Camina en -X
		if (dogPos.x > -limiteX)
			dogPos.x -= velocidad * deltaTime;
		else
			dogAnim = 4;
		break;

	case 4: // Gira 90 grados
		if (dogRot > -180.0f){
			dogRot -= velocidadGiro * deltaTime;
			dogPos.x += sin(glm::radians(dogRot)) * velocidad * deltaTime;
			dogPos.z += cos(glm::radians(dogRot)) * velocidad * deltaTime;
			}
		else {
			dogRot = -180.0f;
			dogAnim = 5;
		}
		break;

	case 5: // Camina en -Z
		if (dogPos.z > -2.3f)
			dogPos.z -= velocidad * deltaTime;
		else
			dogAnim = 6;
		break;

	case 6: // Gira 90 grados
		if (dogRot > -270.0f) {
			dogRot -= velocidadGiro * deltaTime;
			dogPos.x += sin(glm::radians(dogRot)) * velocidad * deltaTime;
			dogPos.z += cos(glm::radians(dogRot)) * velocidad * deltaTime;
		}
		else {
			dogRot = -270.0f;
			dogAnim = 7;
		}
		break;

	case 7: // Camina en +X
		if (dogPos.x < 2.3f)
			dogPos.x += velocidad * deltaTime;
		else
			dogAnim = 8;
		break;

	case 8: // Gira y camina en diagonal al centro
		if (dogRot > -405.0f) {
			dogRot -= velocidadGiro * deltaTime;
			dogPos.x += sin(glm::radians(dogRot)) * velocidad * deltaTime;
			dogPos.z += cos(glm::radians(dogRot)) * velocidad * deltaTime;
		}
		else {
			dogRot = -405.0f;

			if (dogPos.x > 0.0f)
				dogPos.x -= velocidad * deltaTime;

			if (dogPos.z < 0.0f)
				dogPos.z += velocidad * deltaTime;

			if (dogPos.x <= 0.0f && dogPos.z >= 0.0f) {
				dogPos = glm::vec3(0.0f, 0.0f, 0.0f);
				dogRot = 0.0f;

				FLegs = 0.0f;
				RLegs = 0.0f;
				head = 0.0f;
				tail = 0.0f;

				dogAnim = 1; // ciclo
			}
		}
		break;
	}

}



void InitProjectAnimations()
{
	// Inicialización de personas caminando en ambos sentidos
	for (int i = 0; i < NUM_PEOPLE; i++)
	{
		peopleDir[i] = (i % 2 == 0) ? 1 : -1;

		float laneX = (i % 3 - 1) * 0.45f; // carriles centrales: -0.45, 0, 0.45
		float startZ = (peopleDir[i] == 1) ? -4.0f - i * 0.35f : 2.0f + i * 0.35f;

		peoplePos[i] = glm::vec3(laneX, -0.15f, startZ);
		peopleSpeed[i] = 0.6f + (i % 4) * 0.12f;
	}
}

void UpdateProjectAnimations()
{
	projectTime += deltaTime;

	// ================================
	// Animación 2: Stands por KeyFrames
	// ================================
	if (animStands)
	{
		standAnimTime += deltaTime;
		if (standAnimTime > 5.0f)
			standAnimTime = 5.0f;
	}
	else
	{
		standAnimTime -= deltaTime;
		if (standAnimTime < 0.0f)
			standAnimTime = 0.0f;
	}

	// KeyFrames:
	// KF0 = 0.0 -> scale 0.0
	// KF1 = 1.0 -> scale 0.3
	// KF2 = 2.0 -> scale 0.6
	// KF3 = 3.5 -> scale 0.9
	// KF4 = 5.0 -> scale 1.0

	if (standAnimTime < 1.0f)
		standScale = glm::mix(0.0f, 0.3f, standAnimTime / 1.0f);
	else if (standAnimTime < 2.0f)
		standScale = glm::mix(0.3f, 0.6f, (standAnimTime - 1.0f) / 1.0f);
	else if (standAnimTime < 3.5f)
		standScale = glm::mix(0.6f, 0.9f, (standAnimTime - 2.0f) / 1.5f);
	else
		standScale = glm::mix(0.9f, 1.0f, (standAnimTime - 3.5f) / 1.5f);

	// ================================
	// Animación 1: flujo de personas
	// ================================
	if (animPeople)
	{
		for (int i = 0; i < NUM_PEOPLE; i++)
		{
			peoplePos[i].z += peopleDir[i] * peopleSpeed[i] * deltaTime;

			// Simulación sencilla de evitar choques:
			// si dos personas están muy cerca, una se mueve un poco a la izquierda/derecha
			for (int j = 0; j < NUM_PEOPLE; j++)
			{
				if (i == j) continue;

				float d = glm::distance(peoplePos[i], peoplePos[j]);
				if (d < 0.35f)
				{
					if (peoplePos[i].x < peoplePos[j].x)
						peoplePos[i].x -= 0.3f * deltaTime;
					else
						peoplePos[i].x += 0.3f * deltaTime;
				}
			}

			// Evitar invadir área de stands, mantenerlos en zona central
			if (peoplePos[i].x > 1.0f) peoplePos[i].x = 1.0f;
			if (peoplePos[i].x < -1.0f) peoplePos[i].x = -1.0f;

			// Reinicio cuando salen del puente
			if (peopleDir[i] == 1 && peoplePos[i].z > 2.5f)
				peoplePos[i].z = -4.5f;

			if (peopleDir[i] == -1 && peoplePos[i].z < -4.5f)
				peoplePos[i].z = 2.5f;
		}
	}

	// ================================
	// Animación 4: señalización dinámica
	// ================================
	if (animSigns)
	{
		arrowPulse = (sin(projectTime * 4.0f) + 1.0f) / 2.0f;

		// Cambia dirección cada cierto tiempo
		if (fmod(projectTime, 8.0f) < 4.0f)
			arrowDirection = 1;
		else
			arrowDirection = -1;
	}

	// ================================
	// Animación 6: pantallas dinámicas
	// ================================
	if (animScreens)
	{
		screenTimer += deltaTime;
		if (screenTimer > 2.0f)
		{
			screenTimer = 0.0f;
			screenSlide++;

			if (screenSlide > 2)
				screenSlide = 0;
		}
	}

	// ================================
	// Animación 5: iluminación reactiva
	// ================================
	if (animReactiveLights)
	{
		// La luz principal sigue o reacciona al usuario/cámara.
		pointLightPositions[0].x = camera.GetPosition().x;
		pointLightPositions[0].y = 1.6f;
		pointLightPositions[0].z = camera.GetPosition().z;
	}
}

void DrawCubeObject(Shader shader, GLuint VAO, GLint modelLoc, glm::vec3 pos, glm::vec3 scale, glm::vec4 color, float rotY)
{
	glm::mat4 model = glm::mat4(1.0f);

	model = glm::translate(model, pos);
	model = glm::rotate(model, glm::radians(rotY), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::scale(model, scale);

	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
	glUniform4f(glGetUniformLocation(shader.Program, "objectColor"), color.r, color.g, color.b, color.a);

	glBindVertexArray(VAO);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glBindVertexArray(0);
}
void DrawStandModel(Model& standModel, Shader shader, GLint modelLoc, glm::vec3 pos, glm::vec3 scale, float rotY)
{
	glm::mat4 model = glm::mat4(1.0f);

	model = glm::translate(model, pos);
	model = glm::rotate(model, glm::radians(rotY), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::scale(model, scale);

	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
	standModel.Draw(shader);
}

void DrawProjectAnimations(Shader colorShader, GLuint VAO, glm::mat4 view, glm::mat4 projection)
{
	colorShader.Use();

	GLint modelLoc = glGetUniformLocation(colorShader.Program, "model");
	GLint viewLoc = glGetUniformLocation(colorShader.Program, "view");
	GLint projLoc = glGetUniformLocation(colorShader.Program, "projection");

	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
	glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

	// ================================
	// Animación 2 y 3: stands + enfoque
	// ================================
	for (int i = 0; i < NUM_STANDS; i++)
	{
		float distToCamera = glm::distance(camera.GetPosition(), standPositions[i]);

		bool activeStand = distToCamera < 1.8f;

		float focusScale = activeStand ? 1.15f : 1.0f;

		glm::vec4 standColor = activeStand
			? glm::vec4(0.0f, 0.8f, 1.0f, 1.0f)   // activo: azul/cian
			: glm::vec4(0.1f, 0.55f, 0.2f, 1.0f);  // normal: verde

		glm::vec3 finalScale = glm::vec3(1.0f, 0.9f, 0.55f) * standScale * focusScale;

		/*DrawCubeObject(
			colorShader,
			VAO,
			modelLoc,
			standPositions[i],
			finalScale,
			standColor
		);*/

		// Panel informativo cuando el usuario se acerca
		if (activeStand && standScale > 0.8f)
		{
			glm::vec3 panelPos = standPositions[i] + glm::vec3(0.0f, 1.0f, 0.0f);

			DrawCubeObject(
				colorShader,
				VAO,
				modelLoc,
				panelPos,
				glm::vec3(0.9f, 0.35f, 0.05f),
				glm::vec4(1.0f, 1.0f, 0.2f, 1.0f)
			);
		}

		// Pantalla dinámica en cada stand
		if (animScreens && standScale > 0.8f)
		{
			glm::vec4 screenColor;

			if (screenSlide == 0)
				screenColor = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);       // apagada / negro
			else if (screenSlide == 1)
				screenColor = glm::vec4(0.0f, 0.4f, 1.0f, 1.0f);       // logo / azul
			else
				screenColor = glm::vec4(1.0f, 0.7f, 0.1f, 1.0f);       // anuncio / amarillo

			glm::vec3 screenPos = standPositions[i] + glm::vec3(0.0f, 0.45f, -0.31f);

			DrawCubeObject(
				colorShader,
				VAO,
				modelLoc,
				screenPos,
				glm::vec3(0.65f, 0.35f, 0.03f),
				screenColor
			);
		}
	}

	// ================================
	// Animación 1: personas caminando
	// ================================
	if (animPeople)
	{
		for (int i = 0; i < NUM_PEOPLE; i++)
		{
			glm::vec4 personColor = (peopleDir[i] == 1)
				? glm::vec4(0.9f, 0.1f, 0.1f, 1.0f)
				: glm::vec4(0.1f, 0.2f, 0.9f, 1.0f);

			DrawCubeObject(
				colorShader,
				VAO,
				modelLoc,
				peoplePos[i],
				glm::vec3(0.18f, 0.5f, 0.18f),
				personColor
			);
		}
	}

	// ================================
	// Animación 4: flechas dinámicas
	// ================================
	if (animSigns)
	{
		float intensity = 0.3f + arrowPulse * 0.7f;
		glm::vec4 arrowColor = glm::vec4(intensity, intensity, 0.0f, 1.0f);

		float rot = (arrowDirection == 1) ? 0.0f : 180.0f;

		for (int i = 0; i < 5; i++)
		{
			float z = -3.5f + i * 1.2f;

			// cuerpo de la flecha
			DrawCubeObject(
				colorShader,
				VAO,
				modelLoc,
				glm::vec3(0.0f, -0.92f, z),
				glm::vec3(0.18f, 0.03f, 0.55f),
				arrowColor,
				rot
			);

			// punta de flecha simulada con cubito transversal
			DrawCubeObject(
				colorShader,
				VAO,
				modelLoc,
				glm::vec3(0.0f, -0.90f, z + 0.35f * arrowDirection),
				glm::vec3(0.45f, 0.03f, 0.18f),
				arrowColor,
				rot
			);
		}
	}
}



	void MouseCallback(GLFWwindow * window, double xPos, double yPos)
	{
		if (firstMouse)
		{
			lastX = xPos;
			lastY = yPos;
			firstMouse = false;
		}

		GLfloat xOffset = xPos - lastX;
		GLfloat yOffset = lastY - yPos;  // Reversed since y-coordinates go from bottom to left

		lastX = xPos;
		lastY = yPos;

		camera.ProcessMouseMovement(xOffset, yOffset);
	}