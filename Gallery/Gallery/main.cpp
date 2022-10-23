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
#include "shader.hpp"
#include "cube.hpp"
#include "quad.hpp"
#include "buffer.hpp"
#include "texture.hpp"

#define ArrayCount(a) (sizeof(a) / sizeof((a)[0]))
#define G 9.81f

int WindowWidth = 800;
int WindowHeight = 800;
const std::string WindowTitle = "Gallery";

class PointLight {
public:
    glm::vec3 mPosition;

    glm::vec3 mKa;
    glm::vec3 mKd;
    glm::vec3 mKs;

    float       mKc;
    float       mKl;
    float       mKq;

    PointLight(const glm::vec3& position, glm::vec3 ka, glm::vec3 kd, glm::vec3 ks) {
        mPosition = position;
        mKa = ka;
        mKd = kd;
        mKs = ks;

        mKc = 1.0f;
        mKl = 0.022f;
        mKq = 0.0019f;
    }

};

class Camera {
public:
    enum ECameraMoveDirection {
        MOVE_LEFT = 0,
        MOVE_RIGHT,
        MOVE_BWD,
        MOVE_FWD
    };

    glm::mat4 mProjection;
    glm::mat4 mView;

    glm::vec3 mWorldUp;
    glm::vec3 mPosition;
    glm::vec3 mFront;
    glm::vec3 mUp;
    glm::vec3 mRight;

    glm::vec3 mVelocity;

    float mMoveSpeed;
    float mRotateSpeed;
    float mPitch;
    float mYaw;
    float mPlayerHeight; // Should be moved out

    Camera() {
        mPosition = glm::vec3(0.0f);
        mWorldUp = glm::vec3(0.0f, 1.0f, 0.0f);
        mRight = glm::vec3(1.0f, 0.0f, 0.0f);
        mVelocity = glm::vec3(0.0f);
        mPitch = 0.0f;
        mYaw = -90.0f;
        mMoveSpeed = 32.0f;
        mRotateSpeed = 200.0f;
        mProjection = glm::mat4(1.0f);
        mView = glm::mat4(1.0f);
        mPlayerHeight = 2.0f;
        _UpdateVectors();
    }

    void Gravity(float dt) {
        mVelocity.y -= G * dt;
        mPosition += mVelocity * dt;
        _UpdateVectors();
    }

    void Move(ECameraMoveDirection dir, float dt) {
        float Velocity = mMoveSpeed * dt;
        if (dir == MOVE_LEFT) {
            mPosition -= Velocity * mRight;
        }

        if (dir == MOVE_RIGHT) {
            mPosition += Velocity * mRight;
        }

        if (dir == MOVE_BWD) {
            mPosition -= Velocity * mFront;
        }

        if (dir == MOVE_FWD) {
            mPosition += Velocity * mFront;
        }

        _UpdateVectors();
    }

    void Rotate(float dx, float dy, float dt) {
        float RotateVelocity = mRotateSpeed * dt;
        mYaw += dx * RotateVelocity;
        mPitch += dy * RotateVelocity;

        if (mPitch > 89.0f) {
            mPitch = 89.0f;
        }
        if (mPitch < -89.0f) {
            mPitch = -89.0f;
        }

        _UpdateVectors();
    }

    void UpdateProjection(int framebufferWidth, int framebufferHeight) {
        mProjection = glm::perspective(45.0f, framebufferWidth / (float)framebufferHeight, 0.1f, 100.0f);
    }

    void UpdateView() {
        mView = glm::lookAt(mPosition, mPosition + mFront, mUp);
    }

private:
    void _UpdateVectors() {
        mFront.x = cos(glm::radians(mYaw)) * cos(glm::radians(mPitch));
        mFront.y = sin(glm::radians(mPitch));
        mFront.z = sin(glm::radians(mYaw)) * cos(glm::radians(mPitch));
        mFront = glm::normalize(mFront);
        mRight = glm::normalize(glm::cross(mFront, mWorldUp));
        mUp = glm::normalize(glm::cross(mRight, mFront));

        if (mPosition.y < mPlayerHeight) {
            mPosition.y = mPlayerHeight;
        }
    }
};

struct EngineState {
    bool mMoveForward;
    bool mMoveBackward;
    bool mMoveLeft;
    bool mMoveRight;
    bool mFirstMouse;
    glm::vec2 mCursorPos;
    float mDT;
    Camera *mCamera;

    EngineState(Camera *camera) : mDT(0.0f) {
        mCamera = camera;
        mFirstMouse = true;

        mMoveForward = false;
        mMoveBackward = false;
        mMoveLeft = false;
        mMoveRight = false;
    }
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

    Window = glfwCreateWindow(800, 600, "Gallery", 0, 0);
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

    Shader Phong("shaders/basic.vert", "shaders/basic.frag");

    float StartTime = glfwGetTime();
    float EndTime = glfwGetTime();

    Cube CubeObject;
    Quad QuadObject;
    Buffer CubeBuffer(CubeObject);
    Buffer QuadBuffer(QuadObject);


    Texture FloorTexture("res/images/Marble_Tiles_001_basecolor.jpg");
    Texture WallTexture("res/images/Marble_Blue_004_basecolor.jpg");

    glUseProgram(Phong.mId);
    std::vector<PointLight*> PointLights;
    PointLights.push_back(new PointLight(glm::vec3(0.0f, 2.0f, -15.8f), glm::vec3(0.0f, 0.0f, 0.2f), glm::vec3(0.0f, 0.0f, 0.5f), glm::vec3(0.0f, 0.0f, 0.8f)));
    PointLights.push_back(new PointLight(glm::vec3(-15.8f, 2.0f, 0.0f), glm::vec3(0.2f, 0.1f, 0.0f), glm::vec3(0.5f, 0.4f, 0.0f), glm::vec3(0.8f, 0.7f, 0.0f)));
    PointLights.push_back(new PointLight(glm::vec3(15.8f, 2.0f, 0.0f), glm::vec3(0.2f, 0.1f, 0.0f), glm::vec3(0.5f, 0.4f, 0.0f), glm::vec3(0.8f, 0.7f, 0.0f)));
    PointLights.push_back(new PointLight(glm::vec3(0.0f, 2.0f, 15.8f), glm::vec3(0.2f), glm::vec3(0.5f), glm::vec3(0.8f)));
    PointLights.push_back(new PointLight(glm::vec3(0.0f, 2.0f, 0.0f), glm::vec3(0.2f), glm::vec3(0.5f), glm::vec3(0.8f)));
    for (unsigned LightIdx = 0; LightIdx < PointLights.size(); ++LightIdx) {
        Phong.SetUniform3f("uPointLights[" + std::to_string(LightIdx) + "].Position", PointLights[LightIdx]->mPosition);
        Phong.SetUniform3f("uPointLights[" + std::to_string(LightIdx) + "].Ka", PointLights[LightIdx]->mKa);
        Phong.SetUniform3f("uPointLights[" + std::to_string(LightIdx) + "].Kd", PointLights[LightIdx]->mKd);
        Phong.SetUniform3f("uPointLights[" + std::to_string(LightIdx) + "].Ks", PointLights[LightIdx]->mKs);
        Phong.SetUniform1f("uPointLights[" + std::to_string(LightIdx) + "].Kc", PointLights[LightIdx]->mKc);
        Phong.SetUniform1f("uPointLights[" + std::to_string(LightIdx) + "].Kl", PointLights[LightIdx]->mKl);
        Phong.SetUniform1f("uPointLights[" + std::to_string(LightIdx) + "].Kq", PointLights[LightIdx]->mKq);
    }
    Phong.SetUniform1i("uPointLightCount", PointLights.size());
    
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glEnable(GL_DEPTH_TEST);
    State.mCamera->UpdateProjection(WindowWidth, WindowHeight);
    State.mCamera->UpdateView();
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
        Phong.BindDiffuse(WallTexture);
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
        
        glUseProgram(0);

        glfwSwapBuffers(Window);
        EndTime = glfwGetTime();
        State.mDT = EndTime - StartTime;
    }

    glfwTerminate();

    return 0;
}



