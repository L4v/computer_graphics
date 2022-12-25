/**
 * @file main.cpp
 * @author Jovan Ivosevic
 * @brief Gouraud
 * @version 0.1
 * @date 2022-10-09
 * 
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
#include <chrono>
#include <thread>
#include "shader.hpp"
#include "camera.hpp"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include "model.hpp"
#include "icosahedron.hpp"

 /**
  * @brief Returns x value inside range
  *
  * @param x - Value to constrict to range
  * @param min - minimum allowed value for x
  * @param max - maximum allowed value for x
  * 
  * @returns x value between min and max
  */
float
Clamp(float x, float min, float max) {
    return x < min ? min : x > max ? max : x;
}

const int WindowWidth = 800;
const int WindowHeight = 800;
const float TargetFPS = 60.0f;
const std::string WindowTitle = "Gouraud";

struct Input {
    bool MoveLeft;
    bool MoveRight;
    bool MoveUp;
    bool MoveDown;
    bool LookLeft;
    bool LookRight;
    bool LookUp;
    bool LookDown;
};

struct EngineState {
    Input* mInput;
    Camera* mCamera;
    float mDT;
};

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
KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mode) {
    EngineState* State = (EngineState*)glfwGetWindowUserPointer(window);
    Input* UserInput = State->mInput;
    bool IsDown = action == GLFW_PRESS || action == GLFW_REPEAT;
    switch (key) {
    case GLFW_KEY_A: UserInput->MoveLeft = IsDown; break;
    case GLFW_KEY_D: UserInput->MoveRight = IsDown; break;
    case GLFW_KEY_W: UserInput->MoveUp = IsDown; break;
    case GLFW_KEY_S: UserInput->MoveDown = IsDown; break;

    case GLFW_KEY_RIGHT: UserInput->LookLeft = IsDown; break;
    case GLFW_KEY_LEFT: UserInput->LookRight = IsDown; break;
    case GLFW_KEY_UP: UserInput->LookUp = IsDown; break;
    case GLFW_KEY_DOWN: UserInput->LookDown = IsDown; break;

    case GLFW_KEY_ESCAPE: glfwSetWindowShouldClose(window, GLFW_TRUE); break;
    }
}

/**
 * @brief Updates engine state based on input
 * 
 * @param state EngineState
 */
static void
HandleInput(EngineState* state) {
    Input* UserInput = state->mInput;
    Camera* FPSCamera = state->mCamera;
    if (UserInput->MoveLeft) FPSCamera->Move(-1.0f, 0.0f, state->mDT);
    if (UserInput->MoveRight) FPSCamera->Move(1.0f, 0.0f, state->mDT);
    if (UserInput->MoveDown) FPSCamera->Move(0.0f, -1.0f, state->mDT);
    if (UserInput->MoveUp) FPSCamera->Move(0.0f, 1.0f, state->mDT);

    if (UserInput->LookLeft) FPSCamera->Rotate(1.0f, 0.0f, state->mDT);
    if (UserInput->LookRight) FPSCamera->Rotate(-1.0f, 0.0f, state->mDT);
    if (UserInput->LookDown) FPSCamera->Rotate(0.0f, -1.0f, state->mDT);
    if (UserInput->LookUp) FPSCamera->Rotate(0.0f, 1.0f, state->mDT);

}

/**
 * @brief Draws flattened cubes
 *
 * @param vao - Cube VAO
 * @param shader - Shader
 */
static void
DrawFloor(unsigned vao, const Shader& shader) {
    glUseProgram(shader.GetId());
    glBindVertexArray(vao);

    float Size = 4.0f;
    for (int i = -2; i < 4; ++i) {
        for (int j = -2; j < 4; ++j) {
            glm::mat4 Model(1.0f);
            Model = glm::translate(Model, glm::vec3(i * Size, -2.0f, j * Size));
            Model = glm::scale(Model, glm::vec3(Size, 0.1f, Size));
            shader.SetModel(Model);
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }
    }

    glBindVertexArray(0);
    glUseProgram(0);
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

    EngineState State = { 0 };
    Camera FPSCamera;
    Input UserInput = { 0 };
    State.mCamera = &FPSCamera;
    State.mInput = &UserInput;
    glfwSetWindowUserPointer(Window, &State);

    glfwSetErrorCallback(ErrorCallback);
    glfwSetKeyCallback(Window, KeyCallback);

    glViewport(0.0f, 0.0f, WindowWidth, WindowHeight);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);

    std::vector<float> CubeVertices = {
        // X     Y     Z     NX    NY    NZ   FRONT SIDE
        -0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, // L D
         0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, // R D
        -0.5f,  0.5f, 0.5f, 0.0f, 0.0f, 1.0f, // L U
         0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, // R D
         0.5f,  0.5f, 0.5f, 0.0f, 0.0f, 1.0f, // R U
        -0.5f,  0.5f, 0.5f, 0.0f, 0.0f, 1.0f, // L U
                                        // LEFT SIDE
        -0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f, // L D
        -0.5f, -0.5f,  0.5f, -1.0f, 0.0f, 0.0f, // R D
        -0.5f,  0.5f, -0.5f, -1.0f, 0.0f, 0.0f, // L U
        -0.5f, -0.5f,  0.5f, -1.0f, 0.0f, 0.0f, // R D
        -0.5f,  0.5f,  0.5f, -1.0f, 0.0f, 0.0f, // R U
        -0.5f,  0.5f, -0.5f, -1.0f, 0.0f, 0.0f, // L U
                                        // RIGHT SIDE
         0.5f, -0.5f,  0.5f, 1.0f, 0.0f, 0.0f, // L D
         0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, // R D
         0.5f,  0.5f,  0.5f, 1.0f, 0.0f, 0.0f, // L U
         0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, // R D
         0.5f,  0.5f, -0.5f, 1.0f, 0.0f, 0.0f, // R U
         0.5f,  0.5f,  0.5f, 1.0f, 0.0f, 0.0f, // L U
                                        // BOTTOM SIDE
        -0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, // L D
         0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, // R D
        -0.5f, -0.5f,  0.5f, 0.0f, -1.0f, 0.0f, // L U
         0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, // R D
         0.5f, -0.5f,  0.5f, 0.0f, -1.0f, 0.0f, // R U
        -0.5f, -0.5f,  0.5f, 0.0f, -1.0f, 0.0f, // L U
                                        // TOP SIDE
        -0.5f,  0.5f,  0.5f, 0.0f, 1.0f, 0.0f, // L D
         0.5f,  0.5f,  0.5f, 0.0f, 1.0f, 0.0f, // R D
        -0.5f,  0.5f, -0.5f, 0.0f, 1.0f, 0.0f, // L U
         0.5f,  0.5f,  0.5f, 0.0f, 1.0f, 0.0f, // R D
         0.5f,  0.5f, -0.5f, 0.0f, 1.0f, 0.0f, // R U
        -0.5f,  0.5f, -0.5f, 0.0f, 1.0f, 0.0f, // L U
                                        // BACK SIDE
         0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, // L D
        -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, // R D
         0.5f,  0.5f, -0.5f, 0.0f, 0.0f, -1.0f, // L U
        -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, // R D
        -0.5f,  0.5f, -0.5f, 0.0f, 0.0f, -1.0f, // R U
         0.5f,  0.5f, -0.5f, 0.0f, 0.0f, -1.0f  // L U
    };

    unsigned CubeVAO;
    glGenVertexArrays(1, &CubeVAO);
    glBindVertexArray(CubeVAO);
    unsigned CubeVBO;
    glGenBuffers(1, &CubeVBO);
    glBindBuffer(GL_ARRAY_BUFFER, CubeVBO);
    glBufferData(GL_ARRAY_BUFFER, CubeVertices.size() * sizeof(float), CubeVertices.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    Icosahedron Ico;
    std::vector<float> IcosahedronVertices = Ico.GetVertices();
    /* TODO(Jovan):
    * Use different normals for different levels of lighting precision
    * GetSurfaceNormals() - flat shading
    * GetVertexNormals() - Gouraud shading
    */
    std::vector<float> IcosahedronNormals = Ico.GetSurfaceNormals();

    unsigned IcosahedronVAO;
    glGenVertexArrays(1, &IcosahedronVAO);
    glBindVertexArray(IcosahedronVAO);
    unsigned IcosahedronVBO;
    glGenBuffers(1, &IcosahedronVBO);
    glBindBuffer(GL_ARRAY_BUFFER, IcosahedronVBO);
    glBufferData(GL_ARRAY_BUFFER, IcosahedronVertices.size() * sizeof(float), IcosahedronVertices.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    unsigned IcosahedronNormalsVBO;
    glGenBuffers(1, &IcosahedronNormalsVBO);
    glBindBuffer(GL_ARRAY_BUFFER, IcosahedronNormalsVBO);
    glBufferData(GL_ARRAY_BUFFER, IcosahedronNormals.size() * sizeof(float), IcosahedronNormals.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)(0 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    /* TODO(Jovan):
    * Illustrate the different normals:
    * Surface normals - GetDebugSurfaceNormals()
    * Vertex normals - GetDebugVertexNormals()
    */
    std::vector<float> IcosahedronDebugNormals = Ico.GetDebugSurfaceNormals();
    unsigned DebugNormalsVAO;
    glGenVertexArrays(1, &DebugNormalsVAO);
    glBindVertexArray(DebugNormalsVAO);
    unsigned IcosahedronDebugNormalsVBO;
    glGenBuffers(1, &IcosahedronDebugNormalsVBO);
    glBindBuffer(GL_ARRAY_BUFFER, IcosahedronDebugNormalsVBO);
    glBufferData(GL_ARRAY_BUFFER, IcosahedronDebugNormals.size() * sizeof(float), IcosahedronDebugNormals.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    Model Alduin("res/alduin/alduin-dragon.obj");
    if (!Alduin.Load()) {
        std::cerr << "Failed to load alduin\n";
        glfwTerminate();
        return -1;
    }

    Model Amongus("res/amongus/amongus.obj");
    if (!Amongus.Load()) {
        std::cerr << "Failed to load amongus\n";
        glfwTerminate();
        return -1;
    }

    Model Fox("res/low-poly-fox/low-poly-fox.obj");
    if (!Amongus.Load()) {
        std::cerr << "Failed to load fox\n";
        glfwTerminate();
        return -1;
    }

    Model Spider("res/spider/spider.obj");
    if (!Amongus.Load()) {
        std::cerr << "Failed to load spider\n";
        glfwTerminate();
        return -1;
    }

    // NOTE(Jovan): Used to only define color
    Shader ColorShader("shaders/color.vert", "shaders/color.frag");

    // NOTE(Jovan): Used for Gouraud shading
    Shader GouraudShader("shaders/gouraud.vert", "shaders/basic.frag");
    glUseProgram(GouraudShader.GetId());
    GouraudShader.SetUniform3f("uDirLight.Direction", glm::vec3(1.0f, -1.0f, 0.0f));
    GouraudShader.SetUniform3f("uDirLight.Ka", glm::vec3(0.0f, 0.0f, 0.1f));
    GouraudShader.SetUniform3f("uDirLight.Kd", glm::vec3(0.0f, 0.0f, 0.1f));
    GouraudShader.SetUniform3f("uDirLight.Ks", glm::vec3(1.0f));

    GouraudShader.SetUniform3f("uPointLight.Ka", glm::vec3(0.0f, 0.2f, 0.0f));
    GouraudShader.SetUniform3f("uPointLight.Kd", glm::vec3(0.0f, 0.5f, 0.0f));
    GouraudShader.SetUniform3f("uPointLight.Ks", glm::vec3(1.0f));
    // NOTE(Jovan): Parameters from https://wiki.ogre3d.org/-Point+Light+Attenuation
    GouraudShader.SetUniform1f("uPointLight.Kc", 1.0f);
    GouraudShader.SetUniform1f("uPointLight.Kl", 0.092f);
    GouraudShader.SetUniform1f("uPointLight.Kq", 0.032f);

    GouraudShader.SetUniform3f("uSpotlight.Position", glm::vec3(0.0f, 3.5f, -2.0f));
    GouraudShader.SetUniform3f("uSpotlight.Direction", glm::vec3(0.0f, -1.0f, 1.0f));
    GouraudShader.SetUniform3f("uSpotlight.Ka", glm::vec3(0.2f, 0.0f, 0.0f));
    GouraudShader.SetUniform3f("uSpotlight.Kd", glm::vec3(0.5f, 0.0f, 0.0f));
    GouraudShader.SetUniform3f("uSpotlight.Ks", glm::vec3(1.0f));
    GouraudShader.SetUniform1f("uSpotlight.Kc", 1.0f);
    GouraudShader.SetUniform1f("uSpotlight.Kl", 0.092f);
    GouraudShader.SetUniform1f("uSpotlight.Kq", 0.032f);
    GouraudShader.SetUniform1f("uSpotlight.InnerCutOff", glm::cos(glm::radians(12.5f)));
    GouraudShader.SetUniform1f("uSpotlight.OuterCutOff", glm::cos(glm::radians(17.5f)));
    glUseProgram(0);

    glm::mat4 Projection = glm::perspective(45.0f, WindowWidth / (float)WindowHeight, 0.1f, 100.0f);
    glm::mat4 View = glm::lookAt(FPSCamera.GetPosition(), FPSCamera.GetTarget(), FPSCamera.GetUp());
    glm::mat4 ModelMatrix(1.0f);
    
    // NOTE(Jovan): Current angle around Y axis, with regards to XZ plane at which the point light is situated at
    float Angle = 0.0f;
    // NOTE(Jovan): Distance of point light from center of rotation
    float Distance = 5.0f;
    float TargetFrameTime = 1.0f / TargetFPS;
    float StartTime = glfwGetTime();
    float EndTime = glfwGetTime();
    glClearColor(0.1f, 0.1f, 0.2f, 0.0f);
    while (!glfwWindowShouldClose(Window)) {
        glfwPollEvents();
        HandleInput(&State);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        View = glm::lookAt(FPSCamera.GetPosition(), FPSCamera.GetTarget(), FPSCamera.GetUp());
        StartTime = glfwGetTime();
        glUseProgram(GouraudShader.GetId());
        GouraudShader.SetProjection(Projection);
        GouraudShader.SetView(View);
        GouraudShader.SetUniform3f("uViewPos", FPSCamera.GetPosition());

        // NOTE(Jovan): Rotate point light around 0, 0, -2
        glm::vec3 PointLightPosition(Distance * cos(Angle), 2.0f, -2.0f + Distance * sin(Angle));
        GouraudShader.SetUniform3f("uPointLight.Position", PointLightPosition);
        Angle += State.mDT;

        ModelMatrix = glm::mat4(1.0f);
        ModelMatrix = glm::translate(ModelMatrix, glm::vec3(0.0f, 1.0f, -2.0f));
        GouraudShader.SetModel(ModelMatrix);
        /* TODO(Jovan): Draw one of the following:
        * Cube - CubeVAO
        * Icosahedron - IcosahedronVAO
        * One of the models:
        *   - Alduin.Render()
        *   - Amongus.Render()
        *   - Spider.render()
        *   - Fox.Render()
        *
        */
        //glBindVertexArray(IcosahedronVAO);
        //glDrawArrays(GL_TRIANGLES, 0, IcosahedronVertices.size() / 3);
        Alduin.Render();

        DrawFloor(CubeVAO, GouraudShader);

        glUseProgram(ColorShader.GetId());
        ColorShader.SetProjection(Projection);
        ColorShader.SetView(View);
        ModelMatrix = glm::mat4(1.0f);
        ModelMatrix = glm::translate(ModelMatrix, glm::vec3(0.0f, 1.0f, -2.0f));
        ColorShader.SetModel(ModelMatrix);

        // NOTE(Jovan): Draw DebugNormals
        ColorShader.SetUniform3f("uColor", glm::vec3(1.0f));
        glBindVertexArray(DebugNormalsVAO);
        glDrawArrays(GL_LINES, 0, IcosahedronDebugNormals.size() / 3);

        // NOTE(Jovan): Draw point light
        ModelMatrix = glm::translate(ModelMatrix, PointLightPosition);
        ModelMatrix = glm::scale(ModelMatrix, glm::vec3(0.5f));
        ColorShader.SetModel(ModelMatrix);
        glBindVertexArray(CubeVAO);
        ColorShader.SetUniform3f("uColor", glm::vec3(0.0f, 1.0f, 0.0f));
        glDrawArrays(GL_TRIANGLES, 0, 36);

        // NOTE(Jovan): Draw spotlight
        ModelMatrix = glm::mat4(1.0f);
        ModelMatrix = glm::translate(ModelMatrix, glm::vec3(0.0f, 3.5f, -2.0f));
        ModelMatrix = glm::scale(ModelMatrix, glm::vec3(0.5f));
        ColorShader.SetModel(ModelMatrix);
        ColorShader.SetUniform3f("uColor", glm::vec3(1.0f, 0.0f, 0.0f));
        glBindVertexArray(CubeVAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        glBindVertexArray(0);
        glUseProgram(0);
        glfwSwapBuffers(Window);

        // NOTE(Jovan): Time management
        EndTime = glfwGetTime();
        float WorkTime = EndTime - StartTime;
        if (WorkTime < TargetFrameTime) {
            int DeltaMS = (int)((TargetFrameTime - WorkTime) * 1000.0f);
            std::this_thread::sleep_for(std::chrono::milliseconds(DeltaMS));
            EndTime = glfwGetTime();
        }
        State.mDT = EndTime - StartTime;
    }

    glfwTerminate();
    return 0;
}
