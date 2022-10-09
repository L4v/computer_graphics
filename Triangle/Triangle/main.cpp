/**
 * @file main.cpp
 * @author Jovan Ivosevic
 * @brief Triangle drawing example
 *
 * @copyright Copyright (c) 2022
 *
 */
#include "include/glad/glad.h"
#include <GLFW/glfw3.h>
#include <iostream>

const int WindowWidth = 800;
const int WindowHeight = 600;
const std::string WindowTitle = "Gallery";

const std::string VertexShaderSource = "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "void main()\n"
    "{\n"
    "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
    "}\0";

const std::string FragmentShaderSource = "#version 330 core\n"
    "out vec4 FragColor;\n"
    "void main()\n"
    "{\n"
    "   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
    "}\n\0";

/**
 * @brief Error callback function for GLFW. See GLFW docs for details
 *
 * @param error Error code
 * @param description Error message
 */
static void
ErrorCallback(int error, const char* description) {
    std::cerr << "GLFW Error: " << description << std::endl;
}

int main() {
    GLFWwindow* Window = 0;
    if (!glfwInit()) {
        std::cerr << "Failed to init glfw" << std::endl;
        return -1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    Window = glfwCreateWindow(800, 600, "Gallery", 0, 0);
    if (!Window) {
        std::cerr << "Failed to create window" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwSetErrorCallback(ErrorCallback);
    glfwMakeContextCurrent(Window);
    gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);

    // TODO|TASK: Move out shader program compilation into separate function / class
    unsigned VertexShader = glCreateShader(GL_VERTEX_SHADER);
    const char* VertexShaderSourceC_Str = VertexShaderSource.c_str();
    glShaderSource(VertexShader, 1, &VertexShaderSourceC_Str, NULL);
    glCompileShader(VertexShader);
    int Success;
    char InfoLog[512];
    glGetShaderiv(VertexShader, GL_COMPILE_STATUS, &Success);
    if (!Success) {
        glGetShaderInfoLog(VertexShader, 512, NULL, InfoLog);
        std::cerr << "Error compiling Vertex shader " << InfoLog << std::endl;
    }

    unsigned FragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    const char* FragmentShaderSourceC_Str = FragmentShaderSource.c_str();
    glShaderSource(FragmentShader, 1, &FragmentShaderSourceC_Str, NULL);
    glGetShaderiv(FragmentShader, GL_COMPILE_STATUS, &Success);
    if (!Success) {
        glGetShaderInfoLog(FragmentShader, 512, NULL, InfoLog);
        std::cerr << "Error compiling Fragment shader " << InfoLog << std::endl;
    }

    unsigned int BasicProgram = glCreateProgram();
    glAttachShader(BasicProgram, VertexShader);
    glAttachShader(BasicProgram, FragmentShader);
    glLinkProgram(BasicProgram);
    glGetProgramiv(BasicProgram, GL_LINK_STATUS, &Success);
    if (!Success) {
        glGetProgramInfoLog(BasicProgram, 512, NULL, InfoLog);
        std::cerr << "Error linking Shader Program " << InfoLog << std::endl;
    }
    glDeleteShader(VertexShader);
    glDeleteShader(FragmentShader);

    // TODO|TASK: Change to a square
    float TriangleVertices[] = {
        -0.5f, -0.5f, 0.0f,
         0.5f, -0.5f, 0.0f,
         0.0f,  0.5f, 0.0f
    };

    unsigned VAO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    unsigned VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(TriangleVertices), TriangleVertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    while (!glfwWindowShouldClose(Window)) {
        glfwPollEvents();
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(BasicProgram);
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        glBindVertexArray(0);
        glUseProgram(0);

        glfwSwapBuffers(Window);
    }

    glfwTerminate();

    return 0;
}



