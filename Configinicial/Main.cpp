#include<iostream>  // Librería estándar para usar cout

//#define GLEW_STATIC

#include <GL/glew.h>   // Librería para manejar extensiones de OpenGL
#include <GLFW/glfw3.h> // Librería para crear ventanas y manejar entrada de teclado/mouse

// Dimensiones de la ventana
const GLint WIDTH = 800, HEIGHT = 600;

// ======================= SHADERS =======================

// Código fuente del Vertex Shader (se ejecuta en la GPU)
const GLchar* vertexShaderSource =
{
	"#version 330 core\n"
	"layout (location = 0) in vec3 position;\n" // Recibe la posición del vértice
	"void main()\n"
	"{\n"
	"gl_Position = vec4(position.x, position.y, position.z, 1.0);\n" // Define la posición final
	"}\0"
};

// Código fuente del Fragment Shader (define el color del objeto)
const GLchar* fragmentShaderSource =
{
	"#version 330 core\n"
	"out vec4 color;\n"
	"void main()\n"
	"{\n"
	"color = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n" // Color naranja
	"}\n\0"
};

// Funciones de configuracion del shader
void CrearShader(void);

int main() {

	// Inicializa GLFW (necesario antes de usar la librería)
	glfwInit();

	// Verificación de compatibilidad (indica que usaremos OpenGL 3.3)
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

	// Indicamos que usaremos el perfil moderno (Core Profile)
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Compatibilidad para sistemas Apple
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	// Evita que la ventana sea redimensionable
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	// Creamos la ventana con mi nombre como título
	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Laura Reyes Carrillo ", nullptr, nullptr);

	int screenWidth, screenHeight;

	// Obtenemos el tamaño real del framebuffer
	glfwGetFramebufferSize(window, &screenWidth, &screenHeight);

	// Verificación de errores de creacion ventana
	if (nullptr == window)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return EXIT_FAILURE;
	}

	// Hacemos que la ventana sea el contexto actual
	glfwMakeContextCurrent(window);

	// Activamos modo experimental de GLEW
	glewExperimental = GL_TRUE;

	// Verificación de errores de inicialización de glew
	if (GLEW_OK != glewInit()) {
		std::cout << "Failed to initialise GLEW" << std::endl;
		return EXIT_FAILURE;
	}

	// Imprimimos información de OpenGL del sistema (GPU y versión)
	std::cout << "> Version: " << glGetString(GL_VERSION) << std::endl;
	std::cout << "> Vendor: " << glGetString(GL_VENDOR) << std::endl;
	std::cout << "> Renderer: " << glGetString(GL_RENDERER) << std::endl;
	std::cout << "> SL Version: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;

	// Define las dimensiones del viewport (área visible)
	glViewport(0, 0, screenWidth, screenHeight);

	// CrearShader();  // (Actualmente está comentado)

	// ======================= DATOS DEL TRIÁNGULO =======================

	// Coordenadas de los vértices del triángulo
	GLfloat vertices[] =
	{
		-0.5f, -0.5f, 0.0f, // Left
		 0.5f, -0.5f, 0.0f, // Right
		 0.0f,  0.5f, 0.0f  // Top
	};

	GLuint VBO, VAO;

	// Generamos identificadores para el VAO y VBO
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	// Bind the Vertex Array Object first...
	glBindVertexArray(VAO);

	// Enviamos los datos del triángulo a la GPU
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// Indicamos cómo debe interpretar OpenGL los datos enviados
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	// Desvinculamos el buffer
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// Desvinculamos el VAO
	glBindVertexArray(0);

	// ======================= BUCLE PRINCIPAL =======================

	while (!glfwWindowShouldClose(window))
	{
		// Check if any events have been activiated
		glfwPollEvents();

		// Limpiamos la pantalla con un color de fondo
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		// Dibujamos el triángulo
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 3);
		glBindVertexArray(0);

		// Intercambiamos buffers (mostramos lo que se dibujó)
		glfwSwapBuffers(window);
	}

	// Cerramos GLFW
	glfwTerminate();
	return EXIT_SUCCESS;
}