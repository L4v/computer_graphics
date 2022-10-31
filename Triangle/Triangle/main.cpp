/**
 * @file main.cpp
 * @author Jovan Ivosevic
 * @brief Triangle drawing example
 *
 * @copyright Copyright (c) 2022
 *
 */
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "shader.hpp"

const int WindowWidth = 800;
const int WindowHeight = 600;
const std::string WindowTitle = "Triangle";

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

    Window = glfwCreateWindow(WindowWidth, WindowHeight, WindowTitle.c_str(), 0, 0);
    if (!Window) {
        std::cerr << "Failed to create window" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwSetErrorCallback(ErrorCallback);
    glfwMakeContextCurrent(Window);

    GLenum GlewError = glewInit();
    if (GlewError != GLEW_OK) {
        std::cerr << "Failed to init GLEW: " << glewGetErrorString(GlewError);
        glfwTerminate();
        return -1;
    }

    Shader BasicProgram("basic.vert", "basic.frag");

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

        glUseProgram(BasicProgram.mId);
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        glBindVertexArray(0);
        glUseProgram(0);

        glfwSwapBuffers(Window);
    }

    glfwTerminate();

    return 0;
}



