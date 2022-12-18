/**
 * @file main.cpp
 * @author Jovan Ivosevic
 * @brief Textures
 * @version 0.1
 * @date 2022-10-09
 * 
 * Controls:
 * WASD - Movement
 * LEFT, RIGHT, UP and DOWN - Look around
 * I, O - Higher or lower texture interpolation mixing
 * M - Cycle texture minifying filter
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

float
Clamp(float x, float min, float max) {
    return x < min ? min : x > max ? max : x;
}

const int WindowWidth = 800;
const int WindowHeight = 800;
const float TargetFPS = 60.0f;
const std::string WindowTitle = "Textures";

const std::vector<int> MinFilterValues = {
    GL_NEAREST,
    GL_LINEAR,
    GL_NEAREST_MIPMAP_NEAREST,
    GL_NEAREST_MIPMAP_LINEAR,
    GL_LINEAR_MIPMAP_NEAREST,
    GL_LINEAR_MIPMAP_LINEAR
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
    bool UpInterpolation;
    bool LowerInteroplation;
    bool SwitchMinFilter;
};

struct EngineState {
    Input* mInput;
    Camera* mCamera;
    float mDT;
    unsigned mCurrentFilter;
    float mInterpolation;
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

    case GLFW_KEY_I: UserInput->LowerInteroplation = IsDown; break;
    case GLFW_KEY_O: UserInput->UpInterpolation= IsDown; break;

    case GLFW_KEY_M: UserInput->SwitchMinFilter = IsDown && action != GLFW_REPEAT; break;

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

    if (UserInput->LowerInteroplation) state->mInterpolation = Clamp(state->mInterpolation - state->mDT, 0.0f, 1.0f);
    if (UserInput->UpInterpolation) state->mInterpolation = Clamp(state->mInterpolation + state->mDT, 0.0f, 1.0f);

    if (UserInput->SwitchMinFilter) {
        state->mCurrentFilter = (++state->mCurrentFilter) % MinFilterValues.size();
        UserInput->SwitchMinFilter = false;
    }
}

/**
 * @brief Loads image file and creates an OpenGL texture.
 * NOTE: Try avoiding .jpg and other lossy compression formats as
 * they are uncompressed during loading and the memory benefit is
 * negated with the addition of loss of quality
 *
 * @param filePath Image file path
 * @returns TextureID
 */
static unsigned
LoadImageToTexture(const std::string& filePath) {
    int TextureWidth;
    int TextureHeight;
    int TextureChannels;
    unsigned char* ImageData = stbi_load(filePath.c_str(), &TextureWidth, &TextureHeight, &TextureChannels, 0);
    // NOTE(Jovan): Images should usually flipped vertically as they are loaded "upside-down"
    stbi__vertical_flip(ImageData, TextureWidth, TextureHeight, TextureChannels);

    // NOTE(Jovan): Checks or "guesses" the loaded image's format
    GLint InternalFormat = -1;
    switch (TextureChannels) {
        case 1: InternalFormat = GL_RED; break;
        case 3: InternalFormat = GL_RGB; break;
        case 4: InternalFormat = GL_RGBA; break;
        default: InternalFormat = GL_RGB; break;
    }

    unsigned Texture;
    glGenTextures(1, &Texture);
    glBindTexture(GL_TEXTURE_2D, Texture);
    glTexImage2D(GL_TEXTURE_2D, 0, InternalFormat, TextureWidth, TextureHeight, 0, InternalFormat, GL_UNSIGNED_BYTE, ImageData);
    glBindTexture(GL_TEXTURE_2D, 0);
    // NOTE(Jovan): ImageData is no longer necessary in RAM and can be deallocated
    stbi_image_free(ImageData);
    return Texture;
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

    unsigned CheckerTexture = LoadImageToTexture("res/texel_checker.png");
    glBindTexture(GL_TEXTURE_2D, CheckerTexture);
    // NOTE(Jovan): Generate MipMaps
    glGenerateMipmap(GL_TEXTURE_2D);
    // NOTE(Jovan): Possible values: GL_REPEAT, GL_CLAMP_TO_EDGE, GL_CLAMP_TO_BORDER
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    /* NOTE(Jovan):
    * Possible mipmapping values for GL_TEXTURE_MIN_FILTER:
    *
    *  GL_NEAREST_MIPMAP_NEAREST
    *  GL_NEAREST_MIPMAP_LINEAR
    *  GL_LINEAR_MIPMAP_NEAREST
    *  GL_LINEAR_MIPMAP_LINEAR
    *
    * Two parts define the mipmapping algorithm
    * First - mipmap image picking
    *   NEAREST - Choose the mipmap that most closely matches the size of the pixel being textured
    *   LINEAR - Chooses two mipmaps that most closely match the size of the pixel being textured
    * Second - Color interpolation from picked image(s)
    *   NEAREST - Nearest neighbour algorithm
    *   LINEAR - Linear interpolation
    */
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

    // NOTE(Jovan): Possible values: GL_NEAREST, GL_LINEAR. NO MIPMAPPING!
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glBindTexture(GL_TEXTURE_2D, 0);

    unsigned BalrogTexture = LoadImageToTexture("res/balrog.png");
    glBindTexture(GL_TEXTURE_2D, BalrogTexture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glBindTexture(GL_TEXTURE_2D, 0);

    unsigned FlamesTexture = LoadImageToTexture("res/flames.png");
    glBindTexture(GL_TEXTURE_2D, FlamesTexture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glBindTexture(GL_TEXTURE_2D, 0);

    std::vector<float> TriangleVertices = {
        // X              Y         Z     U     V
        -0.5f, -sqrt(3.0f) / 4.0f, 0.0f, 0.0f, 0.0f,
         0.5f, -sqrt(3.0f) / 4.0f, 0.0f, 1.0f, 0.0f,
         0.0f,  sqrt(3.0f) / 4.0f, 0.0f, 0.5f, 1.0f
    };

    unsigned TriangleVAO;
    glGenVertexArrays(1, &TriangleVAO);
    glBindVertexArray(TriangleVAO);
    unsigned TriangleVBO;
    glGenBuffers(1, &TriangleVBO);
    glBindBuffer(GL_ARRAY_BUFFER, TriangleVBO);
    glBufferData(GL_ARRAY_BUFFER, TriangleVertices.size() * sizeof(float), TriangleVertices.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    std::vector<float> CubeVertices = {
        // X     Y     Z     U     V       FRONT SIDE
        -0.5f, -0.5f, 0.5f, 0.0f, 0.0f, // L D
         0.5f, -0.5f, 0.5f, 1.0f, 0.0f, // R D
        -0.5f,  0.5f, 0.5f, 0.0f, 1.0f, // L U
         0.5f, -0.5f, 0.5f, 1.0f, 0.0f, // R D
         0.5f,  0.5f, 0.5f, 1.0f, 1.0f, // R U
        -0.5f,  0.5f, 0.5f, 0.0f, 1.0f, // L U
                                        // LEFT SIDE
        -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, // L D
        -0.5f, -0.5f,  0.5f, 1.0f, 0.0f, // R D
        -0.5f,  0.5f, -0.5f, 0.0f, 1.0f, // L U
        -0.5f, -0.5f,  0.5f, 1.0f, 0.0f, // R D
        -0.5f,  0.5f,  0.5f, 1.0f, 1.0f, // R U
        -0.5f,  0.5f, -0.5f, 0.0f, 1.0f, // L U
                                        // RIGHT SIDE
         0.5f, -0.5f,  0.5f, 0.0f, 0.0f, // L D
         0.5f, -0.5f, -0.5f, 1.0f, 0.0f, // R D
         0.5f,  0.5f,  0.5f, 0.0f, 1.0f, // L U
         0.5f, -0.5f, -0.5f, 1.0f, 0.0f, // R D
         0.5f,  0.5f, -0.5f, 1.0f, 1.0f, // R U
         0.5f,  0.5f,  0.5f, 0.0f, 1.0f, // L U
                                        // BOTTOM SIDE
        -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, // L D
         0.5f, -0.5f, -0.5f, 1.0f, 0.0f, // R D
        -0.5f, -0.5f,  0.5f, 0.0f, 1.0f, // L U
         0.5f, -0.5f, -0.5f, 1.0f, 0.0f, // R D
         0.5f, -0.5f,  0.5f, 1.0f, 1.0f, // R U
        -0.5f, -0.5f,  0.5f, 0.0f, 1.0f, // L U
                                        // TOP SIDE
        -0.5f,  0.5f,  0.5f, 0.0f, 0.0f, // L D
         0.5f,  0.5f,  0.5f, 1.0f, 0.0f, // R D
        -0.5f,  0.5f, -0.5f, 0.0f, 1.0f, // L U
         0.5f,  0.5f,  0.5f, 1.0f, 0.0f, // R D
         0.5f,  0.5f, -0.5f, 1.0f, 1.0f, // R U
        -0.5f,  0.5f, -0.5f, 0.0f, 1.0f, // L U
                                        // BACK SIDE
         0.5f, -0.5f, -0.5f, 0.0f, 0.0f, // L D
        -0.5f, -0.5f, -0.5f, 1.0f, 0.0f, // R D
         0.5f,  0.5f, -0.5f, 0.0f, 1.0f, // L U
        -0.5f, -0.5f, -0.5f, 1.0f, 0.0f, // R D
        -0.5f,  0.5f, -0.5f, 1.0f, 1.0f, // R U
         0.5f,  0.5f, -0.5f, 0.0f, 1.0f, // L U
    };
    unsigned CubeVAO;
    glGenVertexArrays(1, &CubeVAO);
    glBindVertexArray(CubeVAO);
    unsigned CubeVBO;
    glGenBuffers(1, &CubeVBO);
    glBindBuffer(GL_ARRAY_BUFFER, CubeVBO);
    glBufferData(GL_ARRAY_BUFFER, CubeVertices.size() * sizeof(float), CubeVertices.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    Shader TexturedProgram("shaders/basic.vert", "shaders/textured.frag");
    glUseProgram(TexturedProgram.GetId());
    // NOTE(Jovan): Set texture unit for each sampler
    TexturedProgram.SetUniform1i("uTexture1", 0);
    TexturedProgram.SetUniform1i("uTexture2", 1);
    /* NOTE(Jovan) : Set interpolation factor:
    * 0.0f - Uses first texture
    * 1.0f - Uses second
    * (0.0f, 1.0f) - Interpolates between the two
    */ 
    TexturedProgram.SetUniform1f("uInterpolationFactor", 0.0f);
    glUseProgram(0);

    glm::mat4 Projection = glm::perspective(45.0f, WindowWidth / (float)WindowHeight, 0.1f, 100.0f);
    glm::mat4 View = glm::lookAt(FPSCamera.GetPosition(), FPSCamera.GetTarget(), FPSCamera.GetUp());
    glm::mat4 Model(1.0f);

    float Angle = 0.0f;
    float TargetFrameTime = 1.0f / TargetFPS;
    float StartTime = glfwGetTime();
    float EndTime = glfwGetTime();
    glClearColor(0.1f, 0.1f, 0.2f, 0.0f);
    while (!glfwWindowShouldClose(Window)) {
        glfwPollEvents();
        HandleInput(&State);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        StartTime = glfwGetTime();
        glUseProgram(TexturedProgram.GetId());
        TexturedProgram.SetProjection(Projection);
        View = glm::lookAt(FPSCamera.GetPosition(), FPSCamera.GetTarget(), FPSCamera.GetUp());
        TexturedProgram.SetView(View);

        // NOTE(Jovan): Draw spinning cube
        glBindVertexArray(CubeVAO);
        Model = glm::mat4(1.0f);
        Model = glm::translate(Model, glm::vec3(-2.0f, 0.0f, -2.0f));
        Model = glm::rotate(Model, glm::radians(Angle), glm::vec3(1.0f));
        Angle += 1.0f;
        TexturedProgram.SetModel(Model);
        // NOTE(Jovan): Using only one texture, so setting interpolation to 0.0f
        TexturedProgram.SetUniform1f("uInterpolationFactor", 0.0f);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, CheckerTexture);
        glDrawArrays(GL_TRIANGLES, 0, CubeVertices.size() / 5);
        glBindTexture(GL_TEXTURE_2D, 0);

        // NOTE(Jovan): Draw Balrog with flames
        Model = glm::mat4(1.0f);
        Model = glm::translate(Model, glm::vec3(2.0f, 0.0f, -2.0f));
        Model = glm::scale(Model, glm::vec3(1.0f, 1.0f, 0.1f));
        TexturedProgram.SetModel(Model);
        TexturedProgram.SetUniform1f("uInterpolationFactor", State.mInterpolation);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, FlamesTexture);
        // NOTE(Jovan): Can also use GL_TEXTURE1, but below example is useful for loops
        glActiveTexture(GL_TEXTURE0 + 1);
        glBindTexture(GL_TEXTURE_2D, BalrogTexture);
        glDrawArrays(GL_TRIANGLES, 0, CubeVertices.size() / 5);

        // NOTE(Jovan): Floor
        TexturedProgram.SetUniform1f("uInterpolationFactor", 0.0f);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, CheckerTexture);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, MinFilterValues[State.mCurrentFilter]);
        for (int i = 0; i < 10; ++i) {
            Model = glm::mat4(1.0f);
            Model = glm::translate(Model, glm::vec3(0.0f, -1.0f, -i * 10.0f));
            Model = glm::scale(Model, glm::vec3(10.0f, 0.1f, 10.0f));
            TexturedProgram.SetModel(Model);
            glDrawArrays(GL_TRIANGLES, 0, CubeVertices.size() / 5);
        }
 
        glBindVertexArray(0);

        /* NOTE(Jovan): Unbind GL_TEXTURE_2D objects for all units
        * Going from max to 0 so that the last active unit is GL_TEXTURE0
        * which is considered default
        */
        for (int i = GL_MAX_TEXTURE_UNITS - 1; i >= 0; --i) {
            glActiveTexture(GL_TEXTURE0 + i);
            glBindTexture(GL_TEXTURE_2D, 0);
        }

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



