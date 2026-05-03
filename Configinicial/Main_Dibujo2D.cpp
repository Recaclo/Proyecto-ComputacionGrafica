// Laura Reyes Carrillo
// Dibujo de primitivas en 2D
// 15 de febrero de 2026
//Previo 2
//320015764

#include<iostream>

//#define GLEW_STATIC

#include <GL/glew.h>
#include <GLFW/glfw3.h>

// Shaders
#include "Shader.h"

// Callback: función que GLFW llama automáticamente cuando cambia el tamaño de la ventana
void resize(GLFWwindow* window, int width, int height);

// Tamaño inicial de la ventana
const GLint WIDTH = 800, HEIGHT = 600;

int main() {

	// 1) Inicializamos GLFW (biblioteca para crear ventana y manejar input)
	glfwInit();

	//Verificaci�n de compatibilidad 
	/*glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);*/

	// 2) Creamos la ventana (contexto de OpenGL)
	//    El título aparece en la parte superior
	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Reyes Carrillo Laura Dibujo de Primitivas en 2D", NULL, NULL);

	// 3) Registramos la función resize para que se actualice el viewport cuando cambie el tamaño de la ventana
	glfwSetFramebufferSizeCallback(window, resize);

	//Verificaci�n de errores de creacion  ventana
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return EXIT_FAILURE;
	}

	// 4) Le decimos a GLFW que esta ventana es la que usaremos como contexto actual de OpenGL
	glfwMakeContextCurrent(window);

	// 5) GLEW ayuda a cargar funciones de OpenGL (porque no todas están disponibles directo)
	glewExperimental = GL_TRUE;

	//Verificaci�n de errores de inicializaci�n de glew
	if (GLEW_OK != glewInit()) {
		std::cout << "Failed to initialise GLEW" << std::endl;
		return EXIT_FAILURE;
	}

	// Imprimimos informacin de OpenGL del sistema (para confirmar versión y GPU)
	std::cout << "> Version: " << glGetString(GL_VERSION) << std::endl;
	std::cout << "> Vendor: " << glGetString(GL_VENDOR) << std::endl;
	std::cout << "> Renderer: " << glGetString(GL_RENDERER) << std::endl;
	std::cout << "> SL Version: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;

	// Define las dimensiones del viewport
	//glViewport(0, 0, screenWidth, screenHeight);

	// cargar la informcion de los archivos que se tiene por separado
	//se manda la ruta de la caroeta llamada shader y los archivos 
	// .vs es del shacer de vertices 
	// .frag es del shader de fragmentos 

	// 6) Creamos el objeto Shader: lee, compila y enlaza los shaders desde archivos
	//    core.vs  = vertex shader
	//    core.frag = fragment shader
	Shader ourShader("Shader/core.vs", "Shader/core.frag");

	//definicion de los vertices con los que vamos a trabajar en coordenadas ene l eje X Y Z 
	// el eje Z se deja en sero por que son en 2d
	// despues de los vertices esat el colo de cada uno en rgb	de 0 a 1
	// Set up vertex data (and buffer(s)) and attribute pointers 

	// 7) Cada vértice tiene 6 valores: (x, y, z, r, g, b)
	//    Por eso más abajo se usa 6 * sizeof(GLfloat) como "stride"
	float vertices[] = {
		0.5f,  0.5f, 0.0f,    1.0f,1.0f,0.0f,  // top right superior derecha 
		0.5f, -0.5f, 0.0f,    1.0f,1.0f,0.0f,  // bottom right inferior derecha
		-0.5f, -0.5f, 0.0f,   1.0f,0.0f,1.0f,  // bottom left inferior izquierda
		-0.5f,  0.5f, 0.0f,   1.0f,1.0f,0.0f, // top left superior izquierda
	};

	// 8) Índices: en vez de repetir vértices, indicamos el orden en que se deben usar
	//    Esto se usa con glDrawElements.
	unsigned int indices[] = {  // note that we start from 0!
		3,2,1,// second Triangle
		0,1,3,
	};

	// cargar la informacion de los vertices en los buffer para poderlos enlazar y enviar al shader 
	GLuint VBO, VAO, EBO;

	// 9) Generamos IDs para:
	//    VAO = guarda la configuración de atributos (como se leen los datos)
	//    VBO = guarda los datos de vértices
	//    EBO = guarda los índices
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	// Enlazar  Vertex Array Object
	// 10) Activamos el VAO para empezar a configurar "qué datos y cómo se interpretan"
	glBindVertexArray(VAO);

	//2.- Copiamos nuestros arreglo de vertices en un buffer de vertices para que OpenGL lo use
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	// 11) Subimos el arreglo "vertices" a la GPU (memoria de video)
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// 3.Copiamos nuestro arreglo de indices en  un elemento del buffer para que OpenGL lo use
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

	// 12) Subimos el arreglo "indices" a la GPU
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// 4. Despues colocamos las caracteristicas de los vertices

	//Posicion
	// 13) Atributo 0: posición (x,y,z)
	//     - 3 valores
	//     - empezando desde offset 0
	//     - stride = 6 floats porque cada vértice tiene 6 valores
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	//Color
	// 14) Atributo 1: color (r,g,b)
	//     - 3 valores
	//     - offset = 3 floats (porque primero viene x,y,z)
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	// 15) Se puede des-enlazar el VBO; la configuración quedó guardada en el VAO
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// 16) Des-enlazamos el VAO (ya quedó configurado)
	glBindVertexArray(0); // Unbind VAO (it's always a good thing to unbind any buffer/array to prevent strange bugs)

	// ciclo del codigo, seccion donde se dibuja en diferentes velocidades, fotogras por segundo 
	while (!glfwWindowShouldClose(window))
	{
		// Check if any events have been activiated (key pressed, mouse moved etc.) and call corresponding response functions
		glfwPollEvents();

		// Render
		// Clear the colorbuffer
		// 17) Pintamos el fondo de negro (RGB = 0,0,0) y alfa 1
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

		// 18) Limpiamos la pantalla con ese color
		glClear(GL_COLOR_BUFFER_BIT);

		// Draw our first triangle

		// 19) Activamos el shader (para que OpenGL use esos shaders al dibujar)
		ourShader.Use();

		// 20) Activamos el VAO para que OpenGL sepa qué datos usar
		glBindVertexArray(VAO);

		// lo mas basico que podemos hacer es un punto 
		//glPointSize(10);// define el tamaño del vertice
		//glDrawArrays(GL_POINTS, 0, 4); //el 4 representa el numero de vertices que se van a dibujar, en este caso los 4 que se definieron en el arreglo de vertices

		// despues de los puntos lo siguiente es dibujar lineas, para esto se cambia el modo de dibujo a GL_LINES, y se le indica que se van a dibujar 2 vertices, en este caso el vertice 0 y el vertice 1
		//glDrawArrays(GL_LINES,0,4);

		// este modo de dibujo es para dibujar un cuadrado, se le indica que se van a dibujar 4 vertices, en este caso el vertice 0,1,2 y 3, pero se dibuja una linea entre el vertice 0 y el vertice 1, luego entre el vertice 1 y el vertice 2, luego entre el vertice 2 y el vertice 3, y finalmente entre el vertice 3 y el vertice 0
		//glDrawArrays(GL_LINE_LOOP,0,4);

		// este modo de dibujo es para dibujar un triangulo, se le indica que se van a dibujar 3 vertices, en este caso el vertice 0,1 y 2, pero se dibuja una linea entre el vertice 0 y el vertice 1, luego entre el vertice 1 y el vertice 2, y finalmente entre el vertice 2 y el vertice 0
		//glDrawArrays(GL_TRIANGLES,1,3);

		// este modo de dibujo es para dibujar un triangulo, se le indica que se van a dibujar 4 vertices, en este caso el vertice 0,1,2 y 3, pero se dibuja una linea entre el vertice 0 y el vertice 1, luego entre el vertice 1 y el vertice 2, luego entre el vertice 2 y el vertice 3, y finalmente entre el vertice 3 y el vertice 0

		// 21) IMPORTANTE:
		// glDrawElements usa el arreglo de índices (EBO)
		// - GL_TRIANGLES: interpreta cada 3 índices como un triángulo
		// - 3: cantidad de índices que va a leer (AQUÍ SOLO DIBUJAS 1 TRIÁNGULO)
		// - GL_UNSIGNED_INT: tipo de dato del arreglo indices
		// - 0: offset dentro del EBO (empieza desde el primer índice)
		glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0);

		// 22) Des-enlazamos el VAO por limpieza (buena práctica)
		glBindVertexArray(0);

		// Swap the screen buffers
		// 23) Muestra en pantalla el frame ya dibujado (doble buffer)
		glfwSwapBuffers(window);
	}

	// 24) Cerramos GLFW y liberamos recursos
	glfwTerminate();
	return EXIT_SUCCESS;
}

// Función callback del resize
void resize(GLFWwindow* window, int width, int height)
{
	// Set the Viewport to the size of the created window
	// 25) Cuando la ventana cambia de tamaño, actualizamos el área de dibujo (viewport)
	glViewport(0, 0, width, height);
	//glViewport(0, 0, screenWidth, screenHeight);
}
