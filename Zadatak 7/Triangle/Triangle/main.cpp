/**
* Modified file info:
* Author: Nedeljko Tesanovic
* Zadatak 7
*
Original (base) file info:
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


static void
ErrorCallback(int error, const char* description) {
    std::cerr << "GLFW Error: " << description << std::endl;
}

int main() {
    if (!glfwInit()) {
        std::cerr << "Failed to init glfw" << std::endl;
        return -1;
    }

    const int WindowWidth = 600;
    const int WindowHeight = 600;
    const std::string WindowTitle = "Zadatak 1";

    GLFWwindow* Window = 0;

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

    float TriangleVertices[] = {
         0.7, -0.7,
         0.8, -0.8,
         0.6, -0.8
    };
    unsigned int stride = 2 * sizeof(float);

    unsigned VAO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    unsigned VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(TriangleVertices), TriangleVertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, stride, (void*)0);
    glEnableVertexAttribArray(0);
    
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    while (!glfwWindowShouldClose(Window)) {

        glfwPollEvents();
        
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(BasicProgram.mId);
        float c = abs(sin(glfwGetTime()));
        unsigned int offsetLocation = glGetUniformLocation(BasicProgram.mId, "offset");
        glUniform1f(offsetLocation, c);

        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        glBindVertexArray(0);
        glUseProgram(0);

        glfwSwapBuffers(Window);
    }

    glDeleteBuffers(1, &VBO);
    glDeleteVertexArrays(1, &VAO);
    glDeleteProgram(BasicProgram.mId);

    glfwTerminate();

    return 0;
}



