/**
 * @file main.cpp
 * @author Jovan Ivosevic
 * @brief First week starter project. Exercise: draw first letter of your name
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

const int WindowWidth = 800;
const int WindowHeight = 800;
const std::string WindowTitle = "Week1";

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
 * @brief Creates a shader program. For example purposes only
 * Real shader loading will be handled later during the course.
 * WARNING: Bad code ahead!
 *
 * returns Shader program ID
 */
static unsigned
GetShaderProgram() {
    unsigned ProgramId = 0;

    // NOTE(Jovan): Hard-coded, for example purposes only!
    const char* VertexShaderSource = "#version 330 core\n"
        "layout (location = 0) in vec3 aPos;\n"
        "void main()\n"
        "{\n"
        "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
        "}\0";
    const char* FragmentShaderSource = "#version 330 core\n"
        "out vec4 FragColor;\n"
        "void main()\n"
        "{\n"
        "   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
        "}\n\0";

    unsigned VertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(VertexShader, 1, &VertexShaderSource, 0);
    glCompileShader(VertexShader);
    int Success = 0;
    char InfoLog[512];
    glGetShaderiv(VertexShader, GL_COMPILE_STATUS, &Success);
    if (!Success) {
        glGetShaderInfoLog(VertexShader, 512, 0, InfoLog);
        std::cerr << "Error compiling vertex shader: " << InfoLog << std::endl;
        return 0;
    }

    glCompileShader(VertexShader);
    unsigned FragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(FragmentShader, 1, &FragmentShaderSource, 0);
    glCompileShader(FragmentShader);
    glGetShaderiv(FragmentShader, GL_COMPILE_STATUS, &Success);
    if (!Success) {
        glGetShaderInfoLog(FragmentShader, 512, 0, InfoLog);
        std::cerr << "Error compiling fragment shader: " << InfoLog << std::endl;
        return 0;
    }

    ProgramId = glCreateProgram();
    glAttachShader(ProgramId, VertexShader);
    glAttachShader(ProgramId, FragmentShader);
    glLinkProgram(ProgramId);

    glGetProgramiv(ProgramId, GL_LINK_STATUS, &Success);
    if (!Success) {
        glGetProgramInfoLog(ProgramId, 512, NULL, InfoLog);
        std::cerr << "Failed to link shader program:" << InfoLog << std::endl;
        return 0;
    }

    return ProgramId;
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

    // TODO(Jovan): Exercise 1: Add / Update points to draw
    // the first letter of your name. (Use pen and paper to practice)
    std::vector<float> PointCoordinates = {
        // X   Y     Z
        0.0f, 0.0f, 0.0f,
        1.0f, 1.0f, 0.0f
    };

    // NOTE(Jovan): Load data into buffers and send to GPU
    // Explained further into course
    unsigned VAO = 0;
    unsigned VBO = 0;
    glCreateVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
    glCreateBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, PointCoordinates.size() * sizeof(float), PointCoordinates.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    unsigned BasicProgram = GetShaderProgram();

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    while (!glfwWindowShouldClose(Window)) {
        glClear(GL_COLOR_BUFFER_BIT);
        glfwPollEvents();
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(BasicProgram);
        glBindVertexArray(VAO);

        // NOTE(Jovan): Main draw call
        glDrawArrays(GL_LINE_STRIP, 0, PointCoordinates.size() / 3);

        glUseProgram(0);
        glfwSwapBuffers(Window);
    }

    glfwTerminate();
    return 0;
}



