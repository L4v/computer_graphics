//Author: Nedeljko Tesanovic
//Module: main.cpp
//Zadatak 8

#include <GL/glew.h> // Eases use of OpenGL functions
#include <GLFW/glfw3.h> // Creates context/window

//Basic IO
#include <iostream>	

//File reading
#include <fstream>
#include <sstream>


void framebuffer_size_callback(GLFWwindow* context, int width, int height); //Adjusts the render upon window resize
void processInput(GLFWwindow* context, float &x, float &y, float &z); //Process mouse/keyboard input 

int compileShader(const char* type, const char* source); // Compiles shader of type 'type' (VERTEX/FRAGMENT) from file located in 'source'
int createShader(const char* vsSource, const char* fsSource); // Creates a unified shader program by compiling vertex and fragment shaders from source filepaths

std::string readFile(const char* source); //Returns the contents of file located in 'source'

int main()
{
	//Window info
	int wWidth = 700;
	int wHeight = 700;
	const char* wTitle = "Zadatak 8";

	//GLFW Init
	if (!glfwInit())
	{
		std::cout << "GLFW Init failed!\n";
		return 1;
	}

	//OpenGL version
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	//Context (window) creation
	GLFWwindow* context = NULL;
	context = glfwCreateWindow(wWidth, wHeight, wTitle, NULL, NULL);
	if (context == NULL)
	{
		std::cout << "GLFW Context not created!\n";
		glfwTerminate();
		return 1;
	}
	glfwMakeContextCurrent(context);
	glfwSetFramebufferSizeCallback(context, framebuffer_size_callback);

	//GLEW Init
	if (glewInit() != GLEW_OK)
	{
		std::cout << "GLEW Init failed!\n";
		glfwTerminate();
		return 1;
	}

	//Vertices
	float vertices[] = {
		// X     Y	   Z      R    G    B    A
	  	0.25, -0.25, 0.0,    0.0, 1.0, 0.3, 1.0,
		-0.25, -0.25, 0.0,   0.0, 1.0, 0.3, 1.0,
		0.25, 0.25, 0.0,    1.0, 0.0, 0.0, 1.0,
		-0.25, 0.25, 0.0,   0.0, 1.0, 0.3, 1.0
	};
	int stride = 7 * sizeof(float); // 3 coordinates + 4 colours = 7 floats
	unsigned int indices[] = {
		0, 1, 2,
		1, 2, 3
	};

	//Shader
	int shader = createShader("basic.vert", "basic.frag");

	//Buffer bindings
	unsigned int VAO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	unsigned int VBO;
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	unsigned int EBO;
	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, (void*)0); // Coordinates
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, stride, (void*)(3 * sizeof(float))); // Colour
	glEnableVertexAttribArray(1);

	//Unbind (for safety reasons)
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	float x = 0, y = 0, z = 0;

	//Render loop
	while (!glfwWindowShouldClose(context))
	{
		//Event Polling
		glfwPollEvents();

		//Input processing

		processInput(context, x, y, z);

		//Screen clearing
		glClearColor(0, 0, 0, 0);
		glClear(GL_COLOR_BUFFER_BIT);

		//Draw calls
			//Offset calculation
		int offsetLocation = glGetUniformLocation(shader, "offset");
		glUniform3f(offsetLocation, x, y, z);
			//Drawing proper
		glUseProgram(shader);
		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, sizeof(indices) / 3, GL_UNSIGNED_INT, (void*) 0);
		glBindVertexArray(0);


		//Buffer swapping
		glfwSwapBuffers(context);
	}

	//Clean-up
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteProgram(shader);
	glfwTerminate();

	return 0;
}

void framebuffer_size_callback(GLFWwindow* context, int width, int height)
{
	glViewport(0, 0, width, height);
}
void processInput(GLFWwindow* context, float &x, float &y, float &z)
{
	if (glfwGetKey(context, GLFW_KEY_ESCAPE) == GLFW_PRESS || (x > 0.7 && y > 0.7))
	{
		glfwSetWindowShouldClose(context, true);
	}
	if (glfwGetKey(context, GLFW_KEY_D) == GLFW_PRESS)
	{
		x += 0.0005;
		if (x > 0.75)
			x = 0.75;
	}
	if (glfwGetKey(context, GLFW_KEY_A) == GLFW_PRESS)
	{
		x -= 0.0005;
		if (x < -0.75)
			x = -0.75;
	}
	if (glfwGetKey(context, GLFW_KEY_W) == GLFW_PRESS)
	{
		y += 0.0005;
		if (y > 0.75)
			y = 0.75;
	}
	if (glfwGetKey(context, GLFW_KEY_S) == GLFW_PRESS)
	{
		y -= 0.0005;
		if (y < -0.75)
			y = -0.75;
	}
}

std::string readFile(const char* source)
{
	std::string content = "";
	std::ifstream file(source);
	std::stringstream ss;
	if (file.is_open())
	{
		ss << file.rdbuf();
		return ss.str();
	}
	std::cout << "Couldn't open file!\n";
	return "";
}
int compileShader(const char* shaderType, const char* source)
{
	int shader;
	if(shaderType == "VERTEX")
		shader = glCreateShader(GL_VERTEX_SHADER);
	else if (shaderType == "FRAGMENT")
		shader = glCreateShader(GL_FRAGMENT_SHADER);
	else
	{
		std::cout << "Shader type unknown!\n";
		return -1;
	}

	std::string temp = readFile(source);
	const char* code = temp.c_str();

	glShaderSource(shader, 1, &code, NULL);
	glCompileShader(shader);
	int success;
	const int infoLength = 512;
	char infoLog[infoLength];
	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(shader, infoLength, NULL, infoLog);
		std::cout << shaderType << " SHADER Compilation failed!\nInfo: " << infoLog << std::endl;
	}
	return shader;
}
int createShader(const char* vsSource, const char* fsSource) 
{
	int unifiedShader;
	unifiedShader = glCreateProgram();
	
	int vertexShader = compileShader("VERTEX", vsSource);
	if (vertexShader == -1)
	{
		std::cout << "Vertex shader creation failed!";
		return -1;
	}
	glAttachShader(unifiedShader, vertexShader);

	int fragmentShader = compileShader("FRAGMENT", fsSource);
	if (fragmentShader == -1)
	{
		std::cout << "Fragment shader creation failed!";
		return -1;
	}
	glAttachShader(unifiedShader, fragmentShader);

	glLinkProgram(unifiedShader);
	int success;
	const int infoLength = 512;
	char infoLog[infoLength];
	glGetProgramiv(unifiedShader, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(unifiedShader, infoLength, NULL, infoLog);
		std::cout << "Unified shader program linking failed!\nInfo: " << infoLog;
		return -1;
	}

	glDetachShader(unifiedShader, vertexShader);
	glDeleteShader(vertexShader);
	glDetachShader(unifiedShader, fragmentShader);
	glDeleteShader(fragmentShader);

	return unifiedShader;	
}