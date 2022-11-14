/**
* Modified file info:
* Author: Nedeljko Tesanovic
* Zadatak 4
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
    const std::string WindowTitle = "Zadatak 4";

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

    float starVertices[] = {
        0.0, 0.5,       1.0, 0.0, 0.0, //up
        -0.3, -0.4,     1.0, 0.0, 0.0, //down left
        0.4, 0.2,        1.0, 0.0, 0.0, //up right
        -0.4, 0.2,      1.0, 0.0, 0.0, //up left
        0.3, -0.4,      1.0, 0.0, 0.0 //down left
    };
    unsigned int starStride = 5 * sizeof(float);


    float stripVertices[] = {
        1.0, 1.0,     0.0, 1.0, 1.0,
        0.7, 0.7,     0.5, 0.0, 0.5,
        1.0, -1.0,    1.0, 1.0, 0.0,
        0.7, -0.7,   0.5, 0.2, 1.0,   
        -1.0, -1.0,   1.0, 0.0, 0.0,
        -0.7, -0.7,    0.0, 0.5, 0.5,
        -1.0, 1.0,    0.5, 0.1, 0.5,
         -0.7, 0.7,     0.5, 0.0, 0.5,
        1.0, 1.0,     0.0, 1.0, 1.0,
        0.7, 0.7,     0.5, 0.0, 0.5,
    };

    unsigned int stripStride = 5 * sizeof(float);

    unsigned VAO[2]; //0 - star, 1 - strip
    glGenVertexArrays(2, VAO);
    unsigned VBO[2];
    glGenBuffers(2, VBO);

    glBindVertexArray(VAO[0]);
    glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(starVertices), starVertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, starStride, (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, starStride, (void*)(2 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindVertexArray(VAO[1]);
    glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(stripVertices), stripVertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, stripStride, (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, stripStride, (void*)(2 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    while (!glfwWindowShouldClose(Window)) {

        glfwPollEvents();

        glClearColor(0.7, 0.7, 0.7, 1.0);
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(BasicProgram.mId);
        glBindVertexArray(VAO[0]);
        glDrawArrays(GL_LINE_LOOP, 0, sizeof(starVertices) / starStride);

        glBindVertexArray(VAO[1]);
        glDrawArrays(GL_TRIANGLE_STRIP, 0, sizeof(stripVertices) / stripStride);

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
        glUseProgram(0);

        glfwSwapBuffers(Window);
    }

    glDeleteBuffers(2, VBO);
    glDeleteVertexArrays(2, VAO);
    glDeleteProgram(BasicProgram.mId);

    glfwTerminate();

    return 0;
}