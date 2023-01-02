/**
 * @file main.cpp
 * @author Jovan Ivosevic
 * @brief Phong
 * 
 * Controls:
 * - WASD moves camera
 * - Left, Right, Down, Up to look around
 * - 1 Gouraud shading
 * - 2 Phong shading without materials
 * - 3 Phong shading with materials
 * - 4 Phong shading with texture materials
 * - L Toggle debug lines
 * 
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
#include "model.hpp"
#include "texture.hpp"

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

int WindowWidth = 800;
int WindowHeight = 800;
const float TargetFPS = 60.0f;
const std::string WindowTitle = "Phong";

enum ShadingMode {
    GOURAUD = 0,
    PHONG,
    PHONG_MATERIAL,
    PHONG_MATERIAL_TEXTURE
};

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
    unsigned mShadingMode;
    bool mDrawDebugLines;
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

    case GLFW_KEY_1: State->mShadingMode = 0; break;
    case GLFW_KEY_2: State->mShadingMode = 1; break;
    case GLFW_KEY_3: State->mShadingMode = 2; break;
    case GLFW_KEY_4: State->mShadingMode = 3; break;

    case GLFW_KEY_L: {
        if (IsDown) {
            State->mDrawDebugLines ^= true; break;
        }
    } break;

    case GLFW_KEY_ESCAPE: glfwSetWindowShouldClose(window, GLFW_TRUE); break;
    }
}

static void
FramebufferSizeCallback(GLFWwindow* window, int width, int height) {
    WindowWidth = width;
    WindowHeight = height;
    glViewport(0, 0, width, height);
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
DrawFloor(unsigned vao, const Shader& shader, unsigned diffuse, unsigned specular) {
    glUseProgram(shader.GetId());
    glBindVertexArray(vao);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, diffuse);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, specular);
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
    glfwSetFramebufferSizeCallback(Window, FramebufferSizeCallback);
    glfwSetKeyCallback(Window, KeyCallback);

    glViewport(0.0f, 0.0f, WindowWidth, WindowHeight);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);

    unsigned CubeDiffuseTexture = Texture::LoadImageToTexture("res/container_diffuse.png");
    unsigned CubeSpecularTexture = Texture::LoadImageToTexture("res/container_specular.png");
    unsigned FloorDiffuseTexture = Texture::LoadImageToTexture("res/floor_diffuse.jpg");
    unsigned FloorSpecularTexture = Texture::LoadImageToTexture("res/floor_specular.jpg");

    std::vector<float> CubeVertices = {
        // X     Y     Z     NX    NY    NZ    U     V    FRONT SIDE
        -0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, // L D
         0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f, // R D
        -0.5f,  0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, // L U
         0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f, // R D
         0.5f,  0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, // R U
        -0.5f,  0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, // L U
                                                        // LEFT SIDE
        -0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, // L D
        -0.5f, -0.5f,  0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f, // R D
        -0.5f,  0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f, // L U
        -0.5f, -0.5f,  0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f, // R D
        -0.5f,  0.5f,  0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f, // R U
        -0.5f,  0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f, // L U
                                                        // RIGHT SIDE
         0.5f, -0.5f,  0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, // L D
         0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, // R D
         0.5f,  0.5f,  0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, // L U
         0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, // R D
         0.5f,  0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, // R U
         0.5f,  0.5f,  0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, // L U
                                                        // BOTTOM SIDE
        -0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f, // L D
         0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f, // R D
        -0.5f, -0.5f,  0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f, // L U
         0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f, // R D
         0.5f, -0.5f,  0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 1.0f, // R U
        -0.5f, -0.5f,  0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f, // L U
                                                        // TOP SIDE
        -0.5f,  0.5f,  0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, // L D
         0.5f,  0.5f,  0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, // R D
        -0.5f,  0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, // L U
         0.5f,  0.5f,  0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, // R D
         0.5f,  0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, // R U
        -0.5f,  0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, // L U
                                                        // BACK SIDE
         0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, // L D
        -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f, // R D
         0.5f,  0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f, // L U
        -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f, // R D
        -0.5f,  0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f, // R U
         0.5f,  0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f, // L U
    };

    unsigned CubeVAO;
    glGenVertexArrays(1, &CubeVAO);
    glBindVertexArray(CubeVAO);
    unsigned CubeVBO;
    glGenBuffers(1, &CubeVBO);
    glBindBuffer(GL_ARRAY_BUFFER, CubeVBO);
    glBufferData(GL_ARRAY_BUFFER, CubeVertices.size() * sizeof(float), CubeVertices.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    Model Alduin("res/alduin/alduin-dragon.obj");
    if (!Alduin.Load()) {
        std::cerr << "Failed to load alduin\n";
        glfwTerminate();
        return -1;
    }

    Model Fox("res/low-poly-fox/low-poly-fox.obj");
    if (!Fox.Load()) {
        std::cerr << "Failed to load fox\n";
        glfwTerminate();
        return -1;
    }

    // NOTE(Jovan): Used to only define color
    Shader ColorShader("shaders/color.vert", "shaders/color.frag");

    // NOTE(Jovan): Gouraud, Phong, Phong with materials and Phong with materials and textures
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

    Shader PhongShader("shaders/basic.vert", "shaders/phong.frag");
    glUseProgram(PhongShader.GetId());
    PhongShader.SetUniform3f("uDirLight.Direction", glm::vec3(1.0f, -1.0f, 0.0f));
    PhongShader.SetUniform3f("uDirLight.Ka", glm::vec3(0.0f, 0.0f, 0.1f));
    PhongShader.SetUniform3f("uDirLight.Kd", glm::vec3(0.0f, 0.0f, 0.1f));
    PhongShader.SetUniform3f("uDirLight.Ks", glm::vec3(1.0f));

    PhongShader.SetUniform3f("uPointLight.Ka", glm::vec3(0.0f, 0.2f, 0.0f));
    PhongShader.SetUniform3f("uPointLight.Kd", glm::vec3(0.0f, 0.5f, 0.0f));
    PhongShader.SetUniform3f("uPointLight.Ks", glm::vec3(1.0f));
    PhongShader.SetUniform1f("uPointLight.Kc", 1.0f);
    PhongShader.SetUniform1f("uPointLight.Kl", 0.092f);
    PhongShader.SetUniform1f("uPointLight.Kq", 0.032f);

    PhongShader.SetUniform3f("uSpotlight.Position", glm::vec3(0.0f, 3.5f, -2.0f));
    PhongShader.SetUniform3f("uSpotlight.Direction", glm::vec3(0.0f, -1.0f, 1.0f));
    PhongShader.SetUniform3f("uSpotlight.Ka", glm::vec3(0.2f, 0.0f, 0.0f));
    PhongShader.SetUniform3f("uSpotlight.Kd", glm::vec3(0.5f, 0.0f, 0.0f));
    PhongShader.SetUniform3f("uSpotlight.Ks", glm::vec3(1.0f));
    PhongShader.SetUniform1f("uSpotlight.Kc", 1.0f);
    PhongShader.SetUniform1f("uSpotlight.Kl", 0.092f);
    PhongShader.SetUniform1f("uSpotlight.Kq", 0.032f);
    PhongShader.SetUniform1f("uSpotlight.InnerCutOff", glm::cos(glm::radians(12.5f)));
    PhongShader.SetUniform1f("uSpotlight.OuterCutOff", glm::cos(glm::radians(17.5f)));
    glUseProgram(0);

    // NOTE(Jovan): Phong shader with material support
    Shader PhongShaderMaterial("shaders/basic.vert", "shaders/phong_material.frag");
    glUseProgram(PhongShaderMaterial.GetId());
    PhongShaderMaterial.SetUniform3f("uDirLight.Direction", glm::vec3(1.0f, -1.0f, 0.0f));
    PhongShaderMaterial.SetUniform3f("uDirLight.Ka", glm::vec3(0.0f, 0.0f, 0.1f));
    PhongShaderMaterial.SetUniform3f("uDirLight.Kd", glm::vec3(0.0f, 0.0f, 0.1f));
    PhongShaderMaterial.SetUniform3f("uDirLight.Ks", glm::vec3(1.0f));

    PhongShaderMaterial.SetUniform3f("uPointLight.Ka", glm::vec3(0.0f, 0.2f, 0.0f));
    PhongShaderMaterial.SetUniform3f("uPointLight.Kd", glm::vec3(0.0f, 0.5f, 0.0f));
    PhongShaderMaterial.SetUniform3f("uPointLight.Ks", glm::vec3(1.0f));
    PhongShaderMaterial.SetUniform1f("uPointLight.Kc", 1.0f);
    PhongShaderMaterial.SetUniform1f("uPointLight.Kl", 0.092f);
    PhongShaderMaterial.SetUniform1f("uPointLight.Kq", 0.032f);

    PhongShaderMaterial.SetUniform3f("uSpotlight.Position", glm::vec3(0.0f, 3.5f, -2.0f));
    PhongShaderMaterial.SetUniform3f("uSpotlight.Direction", glm::vec3(0.0f, -1.0f, 1.0f));
    PhongShaderMaterial.SetUniform3f("uSpotlight.Ka", glm::vec3(0.2f, 0.0f, 0.0f));
    PhongShaderMaterial.SetUniform3f("uSpotlight.Kd", glm::vec3(0.5f, 0.0f, 0.0f));
    PhongShaderMaterial.SetUniform3f("uSpotlight.Ks", glm::vec3(1.0f));
    PhongShaderMaterial.SetUniform1f("uSpotlight.Kc", 1.0f);
    PhongShaderMaterial.SetUniform1f("uSpotlight.Kl", 0.092f);
    PhongShaderMaterial.SetUniform1f("uSpotlight.Kq", 0.032f);
    PhongShaderMaterial.SetUniform1f("uSpotlight.InnerCutOff", glm::cos(glm::radians(12.5f)));
    PhongShaderMaterial.SetUniform1f("uSpotlight.OuterCutOff", glm::cos(glm::radians(17.5f)));
    // NOTE(Jovan): Reflects the light's ambient component in full strength
    PhongShaderMaterial.SetUniform3f("uMaterial.Ka", glm::vec3(1.0f));
    // NOTE(Jovan): Diminishes the light's diffuse component by half, tinting it slightly red
    PhongShaderMaterial.SetUniform3f("uMaterial.Kd", glm::vec3(0.8f, 0.5f, 0.5f));
    // NOTE(Jovan): Makes the object really shiny
    PhongShaderMaterial.SetUniform3f("uMaterial.Ks", glm::vec3(1.0f));
    PhongShaderMaterial.SetUniform1f("uMaterial.Shininess", 128.0f);
    glUseProgram(0);

    // NOTE(Jovan): Phong shader with material and texture support
    Shader PhongShaderMaterialTexture("shaders/basic.vert", "shaders/phong_material_texture.frag");
    glUseProgram(PhongShaderMaterialTexture.GetId());
    PhongShaderMaterialTexture.SetUniform3f("uDirLight.Direction", glm::vec3(1.0f, -1.0f, 0.0f));
    PhongShaderMaterialTexture.SetUniform3f("uDirLight.Ka", glm::vec3(0.0f, 0.0f, 0.1f));
    PhongShaderMaterialTexture.SetUniform3f("uDirLight.Kd", glm::vec3(0.0f, 0.0f, 0.1f));
    PhongShaderMaterialTexture.SetUniform3f("uDirLight.Ks", glm::vec3(1.0f));

    PhongShaderMaterialTexture.SetUniform3f("uPointLight.Ka", glm::vec3(0.0f, 0.2f, 0.0f));
    PhongShaderMaterialTexture.SetUniform3f("uPointLight.Kd", glm::vec3(0.0f, 0.5f, 0.0f));
    PhongShaderMaterialTexture.SetUniform3f("uPointLight.Ks", glm::vec3(1.0f));
    PhongShaderMaterialTexture.SetUniform1f("uPointLight.Kc", 1.0f);
    PhongShaderMaterialTexture.SetUniform1f("uPointLight.Kl", 0.092f);
    PhongShaderMaterialTexture.SetUniform1f("uPointLight.Kq", 0.032f);

    PhongShaderMaterialTexture.SetUniform3f("uSpotlight.Position", glm::vec3(0.0f, 3.5f, -2.0f));
    PhongShaderMaterialTexture.SetUniform3f("uSpotlight.Direction", glm::vec3(0.0f, -1.0f, 1.0f));
    PhongShaderMaterialTexture.SetUniform3f("uSpotlight.Ka", glm::vec3(0.2f, 0.0f, 0.0f));
    PhongShaderMaterialTexture.SetUniform3f("uSpotlight.Kd", glm::vec3(0.5f, 0.0f, 0.0f));
    PhongShaderMaterialTexture.SetUniform3f("uSpotlight.Ks", glm::vec3(1.0f));
    PhongShaderMaterialTexture.SetUniform1f("uSpotlight.Kc", 1.0f);
    PhongShaderMaterialTexture.SetUniform1f("uSpotlight.Kl", 0.092f);
    PhongShaderMaterialTexture.SetUniform1f("uSpotlight.Kq", 0.032f);
    PhongShaderMaterialTexture.SetUniform1f("uSpotlight.InnerCutOff", glm::cos(glm::radians(12.5f)));
    PhongShaderMaterialTexture.SetUniform1f("uSpotlight.OuterCutOff", glm::cos(glm::radians(17.5f)));
    // NOTE(Jovan): Diminishes the light's diffuse component by half, tinting it slightly red
    PhongShaderMaterialTexture.SetUniform1i("uMaterial.Kd", 0);
    // NOTE(Jovan): Makes the object really shiny
    PhongShaderMaterialTexture.SetUniform1i("uMaterial.Ks", 1);
    PhongShaderMaterialTexture.SetUniform1f("uMaterial.Shininess", 128.0f);
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

    // NOTE(Jovan): Currently used shader
    Shader* CurrentShader = &GouraudShader;
    while (!glfwWindowShouldClose(Window)) {
        glfwPollEvents();
        HandleInput(&State);

        switch (State.mShadingMode) {
            case 0: CurrentShader = &GouraudShader; break;
            case 1: CurrentShader = &PhongShader; break;
            case 2: CurrentShader = &PhongShaderMaterial; break;
            case 3: CurrentShader = &PhongShaderMaterialTexture; break;
        }

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        // NOTE(Jovan): In case of window resize, update projection. Bit bad for performance to do it every iteration.
        // If laggy, remove this line
        Projection = glm::perspective(45.0f, WindowWidth / (float)WindowHeight, 0.1f, 100.0f);
        View = glm::lookAt(FPSCamera.GetPosition(), FPSCamera.GetTarget(), FPSCamera.GetUp());
        StartTime = glfwGetTime();
        glUseProgram(CurrentShader->GetId());
        CurrentShader->SetProjection(Projection);
        CurrentShader->SetView(View);
        CurrentShader->SetUniform3f("uViewPos", FPSCamera.GetPosition());

        // NOTE(Jovan): Rotate point light around 0, 0, -2
        glm::vec3 PointLightPosition(Distance * cos(Angle), 2.0f, -2.0f + Distance * sin(Angle));
        CurrentShader->SetUniform3f("uPointLight.Position", PointLightPosition);
        Angle += State.mDT;

        // NOTE(Jovan): Set cube specular and diffuse textures
        ModelMatrix = glm::mat4(1.0f);
        ModelMatrix = glm::translate(ModelMatrix, glm::vec3(2.0f, 1.0f, 3.0f));
        CurrentShader->SetModel(ModelMatrix);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, CubeDiffuseTexture);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, CubeSpecularTexture);
        glBindVertexArray(CubeVAO);
        glDrawArrays(GL_TRIANGLES, 0, CubeVertices.size() / 8);

        // NOTE(Jovan): Models have their textures automatically loaded and set (if existent)
        ModelMatrix = glm::mat4(1.0f);
        ModelMatrix = glm::translate(ModelMatrix, glm::vec3(0.0f, 1.0f, -2.0f));
        CurrentShader->SetModel(ModelMatrix);
        Alduin.Render();

        ModelMatrix = glm::mat4(1.0f);
        ModelMatrix = glm::translate(ModelMatrix, glm::vec3(0.0f, 1.0f, 4.0f));
        CurrentShader->SetModel(ModelMatrix);
        Fox.Render();

        DrawFloor(CubeVAO, *CurrentShader, FloorDiffuseTexture, FloorSpecularTexture);

        glUseProgram(ColorShader.GetId());
        ColorShader.SetProjection(Projection);
        ColorShader.SetView(View);
        ModelMatrix = glm::mat4(1.0f);
        ModelMatrix = glm::translate(ModelMatrix, glm::vec3(0.0f, 1.0f, -2.0f));
        ColorShader.SetModel(ModelMatrix);

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
