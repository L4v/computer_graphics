/**
 * @file main.cpp
 * @author Jovan Ivosevic
 * @brief Projections showcase
 * @version 0.1
 * @date 2022-10-09
 *
 * @copyright Copyright (c) 2022
 *
 */
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <chrono>
#include <thread>
#include "shader.hpp"
#include "cube.hpp"
#include "buffer.hpp"
#include "camera.hpp"
#include "model.hpp"

const int WindowWidth = 800;
const int WindowHeight = 800;
const std::string WindowTitle = "Projection";
const float TargetFPS = 60.0f;

struct Input {
    bool Left;
    bool Right;
    bool Up;
    bool Down;
};

struct EngineState {
    OrbitalCamera* mOrbitalCamera;
    Input* mInput;
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
        case GLFW_KEY_A: UserInput->Left = IsDown; break;
        case GLFW_KEY_D: UserInput->Right = IsDown; break;
        case GLFW_KEY_W: UserInput->Up = IsDown; break;
        case GLFW_KEY_S: UserInput->Down = IsDown; break;
        case GLFW_KEY_ESCAPE: glfwSetWindowShouldClose(window, GLFW_TRUE); break;
    }
}

static void
HandleInput(EngineState* state) {
    Input* UserInput = state->mInput;
    OrbitalCamera* Camera = state->mOrbitalCamera;
    
    if(UserInput->Left) Camera->Rotate(-1.0f, 0.0f, state->mDT);
    if(UserInput->Right) Camera->Rotate(1.0f, 0.0f, state->mDT);
    if(UserInput->Down) Camera->Rotate(0.0f, 1.0f, state->mDT);
    if(UserInput->Up) Camera->Rotate(0.0f, -1.0f, state->mDT);
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

    glfwSetErrorCallback(ErrorCallback);
    Window = glfwCreateWindow(WindowWidth, WindowHeight, WindowTitle.c_str(), 0, 0);
    if (!Window) {
        std::cerr << "Failed to create window" << std::endl;
        glfwTerminate();
        return -1;
    }
    
    glfwMakeContextCurrent(Window);
    glfwSetKeyCallback(Window, KeyCallback);

    GLenum GlewError = glewInit();
    if (GlewError != GLEW_OK) {
        std::cerr << "Failed to init glew: " << glewGetErrorString(GlewError) << std::endl;
        glfwTerminate();
        return -1;
    }

    OrbitalCamera Camera(45.0f, 5.0f);
    Input UserInput = {0};
    EngineState State = {0};
    State.mOrbitalCamera = &Camera;
    State.mInput = &UserInput;
    glfwSetWindowUserPointer(Window, &State);

    Shader BasicShader("shaders/basic.vert", "shaders/basic.frag");
    float RenderDistance = 100.0f;
    glm::mat4 ModelMatrix(1.0f);
    glm::mat4 FreeView = glm::lookAt(Camera.mPosition, Camera.mPosition + Camera.mFront, Camera.mUp);
    glm::mat4 FrontView = glm::lookAt(glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    glm::mat4 LeftView = glm::lookAt(glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    glm::mat4 TopView = glm::lookAt(glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f));

    float HalfWindowWidth = WindowWidth / 2.0f;
    float HalfWindowHeight = WindowHeight / 2.0f;
    glm::mat4 Perspective = glm::perspective(45.0f, WindowWidth / (float)WindowHeight, 0.1f, RenderDistance);
    glm::mat4 Ortho = glm::ortho(-0.5f, 0.5f, -0.5f, 0.5f, 0.1f, RenderDistance);

    Cube BasicCube;
    Buffer CubeBuffer(BasicCube);
    Model House("res/cottage_obj/cottage_obj.obj");
    House.Load();

    float StartTime = glfwGetTime();
    float EndTime = glfwGetTime();
    float TargetFrameTime = 1.0f / TargetFPS;

    glEnable(GL_DEPTH_TEST);
    while (!glfwWindowShouldClose(Window)) {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glfwPollEvents();
        HandleInput(&State);

        StartTime = glfwGetTime();

        glUseProgram(BasicShader.GetId());
        FreeView = glm::lookAt(Camera.mPosition, Camera.mPosition + Camera.mFront, Camera.mUp);
        BasicShader.SetView(FreeView);

        // NOTE(Jovan): Bottom right viewport
        glViewport(HalfWindowWidth, 0, HalfWindowWidth, HalfWindowHeight);
        BasicShader.SetProjection(Perspective);
        ModelMatrix = glm::mat4(1.0f);
        ModelMatrix = glm::scale(ModelMatrix, glm::vec3(8e-2f));
        BasicShader.SetModel(ModelMatrix);
        House.Render(BasicShader);

        // NOTE(Jovan): Upper left viewport
        glViewport(0, HalfWindowHeight, HalfWindowWidth, HalfWindowHeight);
        BasicShader.SetProjection(Ortho);
        BasicShader.SetView(FrontView);
        ModelMatrix = glm::mat4(1.0f);
        ModelMatrix = glm::scale(ModelMatrix, glm::vec3(8e-2f));
        BasicShader.SetModel(ModelMatrix);
        House.Render(BasicShader);

        // NOTE(Jovan): Upper right viewport
        glViewport(HalfWindowWidth, HalfWindowHeight, HalfWindowWidth, HalfWindowHeight);
        BasicShader.SetProjection(Ortho);
        BasicShader.SetView(LeftView);
        ModelMatrix = glm::mat4(1.0f);
        ModelMatrix = glm::scale(ModelMatrix, glm::vec3(8e-2f));
        BasicShader.SetModel(ModelMatrix);
        House.Render(BasicShader);

        // NOTE(Jovan): Bottom left viewport
        glViewport(0, 0, HalfWindowWidth, HalfWindowHeight);
        BasicShader.SetProjection(Ortho);
        BasicShader.SetView(TopView);
        ModelMatrix = glm::mat4(1.0f);
        ModelMatrix = glm::scale(ModelMatrix, glm::vec3(8e-2f));
        BasicShader.SetModel(ModelMatrix);
        House.Render(BasicShader);


        glUseProgram(0);
        glfwSwapBuffers(Window);
        // NOTE(Jovan): Time management (might not work :'()
        EndTime = glfwGetTime();
        float WorkTime = EndTime - StartTime;
        if(WorkTime < TargetFrameTime) {
            int DeltaMS = (int)((TargetFrameTime - WorkTime) * 1000.0f);
            std::this_thread::sleep_for(std::chrono::milliseconds(DeltaMS));
            EndTime = glfwGetTime();
        }
        State.mDT = EndTime - StartTime;
    }

    glfwTerminate();
    return 0;
}



