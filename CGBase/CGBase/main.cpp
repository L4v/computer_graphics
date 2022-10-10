/**
 * @file main.cpp
 * @author Jovan Ivosevic
 * @brief Base project for Computer Graphics course
 * @version 0.1
 * @date 2022-10-09
 *
 * @copyright Copyright (c) 2022
 *
 */
#include "include/glad/glad.h"
#include <GLFW/glfw3.h>
#include <iostream>

const int WindowWidth = 800;
const int WindowHeight = 600;
const std::string WindowTitle = "Base";

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
    gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);

    while (!glfwWindowShouldClose(Window)) {
        glfwPollEvents();
        glClear(GL_COLOR_BUFFER_BIT);



        glfwSwapBuffers(Window);
    }

    glfwTerminate();

    return 0;
}



