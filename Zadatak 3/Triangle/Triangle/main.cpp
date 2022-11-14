/**
* Modified file info:
* Author: Nedeljko Tesanovic
* Zadatak 3 + 5
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
    const std::string WindowTitle = "Japan";

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

    float circle[360 * 2];
    float r = 0.5;

    circle[0] = 0; //Centar X0
    circle[1] = 0; //Centar Y0

    for (int i = 2; i < 360 * 2; i++)
    {
        if(i%2 == 0)
            circle[i] = r * cos((3.141592 / 180) * i); //Xi
        else
            circle[i] = r * sin((3.141592 / 180) * i); //Yi
    }


    unsigned int stride = 2 * sizeof(float);

    unsigned VAO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    unsigned VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(circle), circle, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, stride, (void*)0);
    glEnableVertexAttribArray(0);
    
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    while (!glfwWindowShouldClose(Window)) {

        glfwPollEvents();
        
        glClearColor(1.0, 1.0, 1.0, 1.0);
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(BasicProgram.mId);
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLE_FAN, 0, sizeof(circle)/stride);

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



