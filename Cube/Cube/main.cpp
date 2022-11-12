/**
 * @file main.cpp
 * @author Jovan Ivosevic
 * @brief Cube drawing.
 * Controls:
 *     D - Switch depth testing
 *     1 - Draw quad
 *     2 - Draw cube
 * @version 0.1
 * @date 2022-10-09
 *
 * @copyright Copyright (c) 2022
 *
 */
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "shader.hpp"

const int WindowWidth = 800;
const int WindowHeight = 800;
const std::string WindowTitle = "Cube";

/**
* @brief Struct for holding user input (will be moved out to
* a "state" struct later on"
*/
struct Input {
    unsigned CurrentDrawing;
    bool DepthTesting;
};

/**
 * @brief Keyboard callback function for GLFW. See GLFW docs for details
 *
 * @param window GLFW window context object
 * @param key Triggered key GLFW code
 * @param scancode Triggered key scan code
 * @param action Triggered key action: pressed, released or repeated
 * @param mode Modifiers
 */
static void
KeyboardCallback(GLFWwindow* window, int key, int scancode, int action, int mode) {
    Input* UserInput = (Input*)glfwGetWindowUserPointer(window);
    bool IsDown = action == GLFW_PRESS;
    if (IsDown) {
        switch (key) {
            // NOTE(Jovan): ^= XORs with true fliping the value
            // Equivalent to: UserInput->DepthTesting = !UserInput->DepthTesting;
            case GLFW_KEY_D: UserInput->DepthTesting ^= true; break;
            case GLFW_KEY_1: UserInput->CurrentDrawing = 0; break;
            case GLFW_KEY_2: UserInput->CurrentDrawing = 1; break;
            case GLFW_KEY_ESCAPE: glfwSetWindowShouldClose(window, GLFW_TRUE); break;
        }
    }
}

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
    glfwMakeContextCurrent(Window);

    GLenum GlewError = glewInit();
    if (GlewError != GLEW_OK) {
        std::cerr << "Failed to init glew: " << glewGetErrorString(GlewError) << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwSetErrorCallback(ErrorCallback);
    glfwSetKeyCallback(Window, KeyboardCallback);

    Input UserInput = { 0 };
    glfwSetWindowUserPointer(Window, &UserInput);

    // TODO(Jovan): Reduce in order to use indexing
    std::vector<float> CubeVertices = {
        // X    Y      Z       R    G     B
        -0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f,
        0.5f, -0.5f, -0.5f,  1.0f, 0.0f, 0.0f,
        0.5f, 0.5f, -0.5f,   1.0f, 0.0f, 0.0f,
        0.5f, 0.5f, -0.5f,   1.0f, 0.0f, 0.0f,
        -0.5f, 0.5f, -0.5f,  1.0f, 0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f,

        -0.5f, -0.5f, 0.5f, 0.0f, 1.0f, 0.0f,
        0.5f, -0.5f, 0.5f,  0.0f, 1.0f, 0.0f,
        0.5f, 0.5f, 0.5f,   0.0f, 1.0f, 0.0f,
        0.5f, 0.5f, 0.5f,   0.0f, 1.0f, 0.0f,
        -0.5f, 0.5f, 0.5f,  0.0f, 1.0f, 0.0f,
        -0.5f, -0.5f, 0.5f, 0.0f, 1.0f, 0.0f,

        -0.5f, 0.5f, 0.5f,  0.0f, 0.0f, 1.0f,
        -0.5f, 0.5f, -0.5f, 0.0f, 0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,0.0f, 0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,0.0f, 0.0f, 1.0f,
        -0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f,
        -0.5f, 0.5f, 0.5f,  0.0f, 0.0f, 1.0f,

        0.5f, 0.5f, 0.5f,  1.0f, 1.0f, 0.0f,
        0.5f, 0.5f, -0.5f, 1.0f, 1.0f, 0.0f,
        0.5f, -0.5f, -0.5f,1.0f, 1.0f, 0.0f,
        0.5f, -0.5f, -0.5f,1.0f, 1.0f, 0.0f,
        0.5f, -0.5f, 0.5f, 1.0f, 1.0f, 0.0f,
        0.5f, 0.5f, 0.5f,  1.0f, 1.0f, 0.0f,

        -0.5f, -0.5f, -0.5f,1.0f, 0.0f, 1.0f,
        0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 1.0f,
        0.5f, -0.5f, 0.5f,  1.0f, 0.0f, 1.0f,
        0.5f, -0.5f, 0.5f,  1.0f, 0.0f, 1.0f,
        -0.5f, -0.5f, 0.5f, 1.0f, 0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,1.0f, 0.0f, 1.0f,

        -0.5f, 0.5f, -0.5f,0.0f, 1.0f, 1.0f,
        0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 1.0f,
        0.5f, 0.5f, 0.5f,  0.0f, 1.0f, 1.0f,
        0.5f, 0.5f, 0.5f,  0.0f, 1.0f, 1.0f,
        -0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 1.0f,
        -0.5f, 0.5f, -0.5f,0.0f, 1.0f, 1.0f
    };

    // TODO(Jovan): Add indices
    unsigned CubeDataStride = 6 * sizeof(float);
    unsigned CubeVAO;
    glGenVertexArrays(1, &CubeVAO);
    glBindVertexArray(CubeVAO);
    unsigned CubeVBO;
    glGenBuffers(1, &CubeVBO);
    glBindBuffer(GL_ARRAY_BUFFER, CubeVBO);
    glBufferData(GL_ARRAY_BUFFER, CubeVertices.size() * sizeof(float), CubeVertices.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(Shader::POSITION_LOCATION, 3, GL_FLOAT, GL_FALSE, CubeDataStride, (void*)0);
    glEnableVertexAttribArray(Shader::POSITION_LOCATION);
    glVertexAttribPointer(Shader::COLOR_LOCATION, 3, GL_FLOAT, GL_FALSE, CubeDataStride, (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(Shader::COLOR_LOCATION);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    // TODO(Jovan): Indexing
    std::vector<float> QuadVertices = {
        // X     Y     Z     R     G     B
        -0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, // Down Left
         0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, // Down Right
        -0.5f,  0.5f, 0.0f, 0.0f, 0.0f, 1.0f, // Up Left

         0.5f, -0.5f, 0.0f, 1.0f, 1.0f, 0.0f, // Down Right
         0.5f,  0.5f, 0.0f, 1.0f, 0.0f, 1.0f, // Up Right
        -0.5f,  0.5f, 0.0f, 0.0f, 1.0f, 1.0f  // Up Left
    };


    unsigned QuadDataStride = 6 * sizeof(float);
    unsigned QuadVAO;
    glGenVertexArrays(1, &QuadVAO);
    glBindVertexArray(QuadVAO);
    unsigned QuadVBO;
    glGenBuffers(1, &QuadVBO);
    glBindBuffer(GL_ARRAY_BUFFER, QuadVBO);
    glBufferData(GL_ARRAY_BUFFER, QuadVertices.size() * sizeof(float), QuadVertices.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(Shader::POSITION_LOCATION, 3, GL_FLOAT, GL_FALSE, QuadDataStride, (void*)0);
    glEnableVertexAttribArray(Shader::POSITION_LOCATION);
    glVertexAttribPointer(Shader::COLOR_LOCATION, 3, GL_FLOAT, GL_FALSE, QuadDataStride, (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(Shader::COLOR_LOCATION);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    Shader Basic("shaders/basic.vert", "shaders/basic.frag");
    
    glViewport(0, 0, WindowWidth, WindowHeight);
    glm::mat4 Projection = glm::perspective(45.0f, WindowWidth / (float)WindowHeight, 0.1f, 100.0f);
    glm::mat4 View = glm::lookAt(glm::vec3(0.0f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f));


    while (!glfwWindowShouldClose(Window)) {
        if (UserInput.DepthTesting) {
            glEnable(GL_DEPTH_TEST);
        }
        else {
            glDisable(GL_DEPTH_TEST);
        }

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glfwPollEvents();

        glUseProgram(Basic.GetId());

        glm::mat4 Model(1.0f);
        Model = glm::translate(Model, glm::vec3(0.0f, 0.0f, -2.0f));
        // TODO(Jovan): Indexed drawing
        switch (UserInput.CurrentDrawing) {
            case 0: {
                Basic.SetUniform4m("uMVP", Projection* View* Model);
                glBindVertexArray(QuadVAO);
                glDrawArrays(GL_TRIANGLES, 0, QuadVertices.size() / 3);
            }; break;
            case 1: {
                static float angle = 1.0f;
                Model = glm::rotate(Model, glm::radians(++angle), glm::vec3(1.0f, 1.0f, 1.0));
                Basic.SetUniform4m("uMVP", Projection* View* Model);
                glBindVertexArray(CubeVAO);
                glDrawArrays(GL_TRIANGLES, 0, CubeVertices.size() / 3);
            }; break;
        }

        glBindVertexArray(0);
        glUseProgram(0);

        glfwSwapBuffers(Window);
    }

    glfwTerminate();
    return 0;
}



