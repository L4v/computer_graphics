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

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/matrix_operation.hpp>
#include <glm/gtx/string_cast.hpp>
#include <string>
#include <algorithm>
#include <chrono>
#include <thread>
#include "shader.hpp"
#include "phongshader.hpp"
#include "cube.hpp"
#include "quad.hpp"
#include "buffer.hpp"
#include "texture.hpp"
#include "model.hpp"
#include "gallery.hpp"

#define ArrayCount(a) (sizeof(a) / sizeof((a)[0]))

int WindowWidth = 1920;
int WindowHeight = 1080;
float TargetFPS = 60.0f;
const std::string WindowTitle = "Gallery";

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

static void
KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    EngineState* State = (EngineState*)glfwGetWindowUserPointer(window);

    if (action == GLFW_PRESS || action == GLFW_RELEASE) {
        bool IsDown = action == GLFW_PRESS;
        if (key == GLFW_KEY_W) {
            State->mMoveForward = IsDown;
        }

        if (key == GLFW_KEY_S) {
            State->mMoveBackward = IsDown;
        }

        if (key == GLFW_KEY_A) {
            State->mMoveLeft = IsDown;
        }

        if (key == GLFW_KEY_D) {
            State->mMoveRight = IsDown;
        }

        if (key == GLFW_KEY_ESCAPE) {
            glfwSetWindowShouldClose(window, GLFW_TRUE);
        }
    }
}

void
CursorPosCallback(GLFWwindow* window, double newX, double newY) {
    EngineState* State = (EngineState*)glfwGetWindowUserPointer(window);
    if (State->mFirstMouse) {
        State->mCursorPos = glm::vec2(newX, newY);
        State->mFirstMouse = false;
    }

    State->mCamera->Rotate(newX - State->mCursorPos.x, State->mCursorPos.y - newY, State->mDT);
    State->mCursorPos = glm::vec2(newX, newY);
}

void
FramebufferSizeCallback(GLFWwindow* window, int width, int height) {
    EngineState* State = (EngineState*)glfwGetWindowUserPointer(window);
    WindowWidth = width;
    WindowHeight = height;
    State->mCamera->UpdateProjection(width, height);
    glViewport(0, 0, width, height);
}

void
HandleInput(EngineState* state) {
    if (state->mMoveBackward) {
        state->mCamera->Move(Camera::MOVE_BWD, state->mDT);
    }

    if (state->mMoveForward) {
        state->mCamera->Move(Camera::MOVE_FWD, state->mDT);
    }

    if (state->mMoveLeft) {
        state->mCamera->Move(Camera::MOVE_LEFT, state->mDT);
    }

    if (state->mMoveRight) {
        state->mCamera->Move(Camera::MOVE_RIGHT, state->mDT);
    }
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

    // Borderless Fullscreen window for primary monitor (no dualscreen support)
    GLFWmonitor* Monitor = glfwGetPrimaryMonitor();
    const GLFWvidmode* WindowMode = glfwGetVideoMode(Monitor);
    glfwWindowHint(GLFW_RED_BITS, WindowMode->redBits);
    glfwWindowHint(GLFW_GREEN_BITS, WindowMode->greenBits);
    glfwWindowHint(GLFW_BLUE_BITS, WindowMode->blueBits);
    glfwWindowHint(GLFW_REFRESH_RATE, WindowMode->refreshRate);

    Window = glfwCreateWindow(WindowWidth, WindowHeight, "Gallery", Monitor, 0);
    if (!Window) {
        std::cerr << "Failed to create window" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(Window);
    GLenum GlewErr = glewInit();
    if (GlewErr != GLEW_OK) {
        std::cerr << "Failed to init GLEW " << glewGetErrorString(GlewErr) << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwSetErrorCallback(ErrorCallback);
    glfwSetKeyCallback(Window, KeyCallback);
    glfwSetCursorPosCallback(Window, CursorPosCallback);
    glfwSetFramebufferSizeCallback(Window, FramebufferSizeCallback);
    glfwSetInputMode(Window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glViewport(0, 0, WindowWidth, WindowHeight);

    Camera FPSCamera;
    EngineState State(&FPSCamera);
    glfwSetWindowUserPointer(Window, &State);

    PhongShader Phong("shaders/basic.vert", "shaders/basic.frag");
    Shader Network("shaders/basic.vert", "shaders/network.frag");
    Shader Warping("shaders/basic.vert", "shaders/warping.frag");
    Shader Mazeflow("shaders/basic.vert", "shaders/mazeflow.frag");
    Shader Voronoise("shaders/basic.vert", "shaders/voronoise.frag");
    Shader Ashanoha("shaders/basic.vert", "shaders/ashanoha.frag");
    Shader Raymarch("shaders/basic.vert", "shaders/raymarch.frag");

    float StartTime = glfwGetTime();
    float EndTime = glfwGetTime();

    Cube CubeObject;
    Quad QuadObject;
    Buffer CubeBuffer(CubeObject);
    Buffer QuadBuffer(QuadObject);

    // NOTE(Jovan): Should've been a museum piece...
    Model Amongus("res/models/amongus.obj");
    Amongus.Load();

    Texture FloorTexture("res/images/Marble_Tiles_001_basecolor.jpg", Texture::DIFFUSE, 16.0f);
    Texture WallTexture("res/images/Marble_Blue_004_basecolor.jpg", Texture::DIFFUSE, 8.0f);

    glUseProgram(Phong.mId);
    Phong.AddPointLight(PointLight(glm::vec3(0.0f, 1.0f, -16.0f), glm::vec3(0.1f), glm::vec3(0.5f), glm::vec3(0.8f)));
    Phong.AddPointLight(PointLight(glm::vec3(-15.8f, 2.0f, 0.0f), glm::vec3(0.2f, 0.1f, 0.0f), glm::vec3(0.5f, 0.4f, 0.0f), glm::vec3(0.8f, 0.7f, 0.0f)));
    Phong.AddPointLight(PointLight(glm::vec3(15.8f, 2.0f, 0.0f), glm::vec3(0.2f, 0.1f, 0.0f), glm::vec3(0.5f, 0.4f, 0.0f), glm::vec3(0.8f, 0.7f, 0.0f)));
    Phong.AddPointLight(PointLight(glm::vec3(0.0f, 2.0f, 15.8f), glm::vec3(0.2f), glm::vec3(0.5f), glm::vec3(0.8f)));
    Phong.AddPointLight(PointLight(glm::vec3(0.0f, 2.0f, 0.0f), glm::vec3(0.2f), glm::vec3(0.5f), glm::vec3(0.8f)));
    
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glEnable(GL_DEPTH_TEST);
    State.mCamera->UpdateProjection(WindowWidth, WindowHeight);
    State.mCamera->UpdateView();
    float TargetFrameTime = 1.0f / TargetFPS;
    while (!glfwWindowShouldClose(Window)) {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glfwPollEvents();
        HandleInput(&State);
        State.mCamera->Gravity(State.mDT);
        State.mCamera->UpdateView();

        StartTime = glfwGetTime();
        static float angle = 0.0f;
        ++angle;
        glUseProgram(Phong.mId);
        glm::mat4 Model(1.0f);
        Model = glm::translate(Model, glm::vec3(0.0f, 2.0f, -3.0f));
        Model = glm::rotate(Model, glm::radians(angle), glm::vec3(1.0f));
        Phong.SetMVP(Model, State.mCamera->mView, State.mCamera->mProjection);
        Phong.SetViewPosition(State.mCamera->mPosition);
        Phong.BindTexture(WallTexture);
        CubeBuffer.Render();

        // NOTE(Jovan): South wall
        for (unsigned WallIdx = 0; WallIdx < 4; ++WallIdx) {
           Model = glm::mat4(1.0f);
           Model = glm::translate(Model, glm::vec3(-12.0f + WallIdx * 8, 4.0f, -16.0f));
           Model = glm::scale(Model, glm::vec3(8.0f, 8.0f, 0.1f));
           Phong.SetModel(Model);
           CubeBuffer.Render();

        // NOTE(Jovan): North wall
           Model = glm::mat4(1.0f);
           Model = glm::translate(Model, glm::vec3(-12.0f + WallIdx * 8, 4.0f, 16.0f));
           Model = glm::scale(Model, glm::vec3(8.0f, 8.0f, 0.1f));
           Phong.SetModel(Model);
           CubeBuffer.Render();

        // NOTE(Jovan): West wall
           Model = glm::mat4(1.0f);
           Model = glm::translate(Model, glm::vec3(-16.0f, 4.0f, -12.0f + WallIdx * 8));
           Model = glm::scale(Model, glm::vec3(0.1f, 8.0f, 8.0f));
           Phong.SetModel(Model);
           CubeBuffer.Render();

        // NOTE(Jovan): East wall
           Model = glm::mat4(1.0f);
           Model = glm::translate(Model, glm::vec3(16.0f, 4.0f, -12.0f + WallIdx * 8));
           Model = glm::scale(Model, glm::vec3(0.1f, 8.0f, 8.0f));
           Phong.SetModel(Model);
           CubeBuffer.Render();
        }
        Phong.UnbindTexture(WallTexture);

        // NOTE(Jovan): Floor
        Phong.BindTexture(FloorTexture);
        Model = glm::mat4(1.0f);
        Model = glm::translate(Model, glm::vec3(0.0f, -0.5f, 0.0f));
        Model = glm::scale(Model, glm::vec3(32.0f, 1.0f, 32.0f));
        Model = glm::rotate(Model, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
        Phong.SetModel(Model);
        CubeBuffer.Render();
        Phong.UnbindTexture(FloorTexture);

        Model = glm::mat4(1.0f);
        Model = glm::translate(Model, glm::vec3(0.0f, 0.0f, 0.0f));
        Model = glm::scale(Model, glm::vec3(0.008f));
        Phong.SetModel(Model);
        Amongus.Render(Phong);

        // NOTE(Jovan): Paintings
        // NOTE(Jovan): Ashanoha "painting"
        glUseProgram(Ashanoha.mId);
        Model = glm::mat4(1.0f);
        Model = glm::translate(Model, glm::vec3(0.0f, 4.0f, -15.9f));
        Model = glm::scale(Model, glm::vec3(12.0f, 6.0f, 0.1f));
        Ashanoha.SetUniform1f("uTime", glfwGetTime());
        Warping.SetUniform2f("uResolution", glm::vec2(12, 6));
        Ashanoha.SetMVP(Model, State.mCamera->mView, State.mCamera->mProjection);
        CubeBuffer.Render();

        // NOTE(Jovan): Warping "painting"
        glUseProgram(Warping.mId);
        Model = glm::mat4(1.0f);
        Model = glm::translate(Model, glm::vec3(-15.9f, 4.0f, 0.0f));
        Model = glm::scale(Model, glm::vec3(0.1f, 6.0f, 12.0f));
        Warping.SetUniform1f("uTime", glfwGetTime());
        Warping.SetUniform2f("uResolution", glm::vec2(12, 6));
        Warping.SetMVP(Model, State.mCamera->mView, State.mCamera->mProjection);
        CubeBuffer.Render();

        // NOTE(Jovan): Mazeflow "painting"
        glUseProgram(Mazeflow.mId);
        Model = glm::mat4(1.0f);
        Model = glm::translate(Model, glm::vec3(15.9f, 4.0f, 3.0f));
        Model = glm::scale(Model, glm::vec3(0.1f, 6.0f, 6.0f));
        Mazeflow.SetUniform1f("uTime", glfwGetTime());
        Mazeflow.SetUniform2f("uResolution", glm::vec2(12, 12));
        Mazeflow.SetMVP(Model, State.mCamera->mView, State.mCamera->mProjection);
        CubeBuffer.Render();

        // NOTE(Jovan): Raymarch "painting"
        glUseProgram(Raymarch.mId);
        Model = glm::mat4(1.0f);
        Model = glm::translate(Model, glm::vec3(15.9f, 4.0f, -3.0f));
        Model = glm::scale(Model, glm::vec3(0.1f, 6.0f, 6.0f));
        Raymarch.SetUniform1f("uTime", glfwGetTime());
        Raymarch.SetUniform2f("uResolution", glm::vec2(12, 12));
        Raymarch.SetMVP(Model, State.mCamera->mView, State.mCamera->mProjection);
        CubeBuffer.Render();

        // NOTE(Jovan): Voronoise "painting"
        glUseProgram(Voronoise.mId);
        Model = glm::mat4(1.0f);
        Model = glm::translate(Model, glm::vec3(0.0f, 4.0f, 15.9f));
        Model = glm::scale(Model, glm::vec3(12.0f, 6.0f, 0.1f));
        Voronoise.SetUniform1f("uTime", glfwGetTime());
        Voronoise.SetUniform2f("uResolution", glm::vec2(12, 6));
        Voronoise.SetMVP(Model, State.mCamera->mView, State.mCamera->mProjection);
        CubeBuffer.Render();

        // NOTE(Jovan): Network "painting"
        glUseProgram(Network.mId);
        Model = glm::mat4(1.0f);
        Model = glm::translate(Model, glm::vec3(0.0f, 8.1f, 0.0f));
        Model = glm::scale(Model, glm::vec3(32.0f, 0.1f, 32.0f));
        Network.SetUniform1f("uTime", glfwGetTime());
        Network.SetUniform2f("uResolution", glm::vec2(16, 16));
        Network.SetMVP(Model, State.mCamera->mView, State.mCamera->mProjection);
        CubeBuffer.Render();

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



