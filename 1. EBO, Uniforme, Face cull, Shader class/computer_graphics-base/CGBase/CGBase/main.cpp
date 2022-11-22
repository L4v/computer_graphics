//Autor: Nedeljko Tesanovic
//Namjena: Demonstracija upotrebe EBO, Uniformi i Face Culling-a. Funkcije za sejdere pomjerene u posebnu klasu

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include "shader.hpp" //Sejder klasa. U njoj je sadrzan kompajler za sejdere i funkcije za uniforme

const int WindowWidth = 800;
const int WindowHeight = 800;
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
    glfwMakeContextCurrent(Window);

    GLenum GlewError = glewInit();
    if (GlewError != GLEW_OK) {
        std::cerr << "Failed to init glew: " << glewGetErrorString(GlewError) << std::endl;
        glfwTerminate();
        return -1;
    }

    Shader Basic("shaders/basic.vert", "shaders/basic.frag");

    glfwSetErrorCallback(ErrorCallback);

    float vertices[] = //Tjemena trougla koja sadrze sve informacije o njemu
    {
        //Pozicija    |    Boja
        //X    Y       R    G    B    A
        -0.25, 0.0,    1.0, 0.0, 0.0, 0.0,  //tjeme 0
        0.25, 0.0,     0.0, 1.0, 0.0, 0.0,  //tjeme 1
        -0.25, 0.25,    0.0, 0.0, 1.0, 0.0,  //tjeme 2
        0.25, 0.25,    1.0, 1.0, 0.0, 0.0,   //tjeme 3
    };
    unsigned int stride = (2 + 4) * sizeof(float);

    unsigned int indices[] =
    {
        0, 1, 2, //Prvi trougao sacinjen od tjemena 0, tjemena 1 i tjemena 2 (Prednja strana mu je naprijed)
        1, 2, 3, //Drugi trougao (Zadnja strana mu je naprijed)
    };

    //Vertex Array Object - Kontejner za VBO i pokazivace na njihove atribute i tjemena
    unsigned int VAO;
    glGenVertexArrays(1, &VAO); //Generisi 1 kontejner na adresi od promjenljive "VAO"
    glBindVertexArray(VAO); //Povezi VAO za aktivni kontekst - Sve sto radimo ispod ovoga ce se odnositi na kontejner "VAO"

    //Vertex Buffer Object - Nase figure koje crtamo
    unsigned int VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, stride, (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, stride, (void*)(2 * sizeof(float)));
    glEnableVertexAttribArray(1);

    //Element Buffer Object
    //Radimo identicne stvari kao za VBO, samo sto saljemo podatke o indeksima tjemena - na Element array buffer
    unsigned int EBO;
    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glBindVertexArray(0);

    while (!glfwWindowShouldClose(Window)) {
        glfwPollEvents();
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(Basic.GetId()); // Aktiviramo Sejder
        glBindVertexArray(VAO);

        //Eliminisanje lica
        glEnable(GL_CULL_FACE); //Ukljuci eliminisanje lica
        glCullFace(GL_BACK); //Koja lica eliminisemo Za prednja: GL_FRONT, Za zadnja: GL_BACK, za prednja i zadnja: GL_FRONT_AND_BACK

        //Slanje vrijednosti na uniformu
        float R = abs(sin(glfwGetTime())); //Absolutna vrijednost sinusa trenutnog vremena
        float G = 0.25;
        int colorOffsetLocation = glGetUniformLocation(Basic.GetId(), "colorOffset"); //Nadji adresu uniforme
        //glGetUniformLocation(objedinjeni sejder, ime uniforme kako je definisana unutar sejdera) - vraca lokaciju uniforme unutar sejder programa
        glUniform3f(colorOffsetLocation, R, G, 0); //Postavi uniformu na odredjenu vrijednost. 3f jer je colorOffset definisan kao vec3
        //glUniform(adresa uniforme, nova vrijednost uniforme) - Postavlja uniformu na prosledjenu vrijednost.
        //Ime zavisi od tipa uniforme (pogledati nomenklaturu sa prve prezentacije). 

        //Crtanje bez indeksa
        //glDrawArrays(GL_TRIANGLE_STRIP, 0, 4); //To i nacrtamo
        //glDrawArrays(tip primitive, indeks pocetnog tjemena, koliko narednih tjemena crtamo);
        
        //Crtanje sa indeksima
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void*)(0 * sizeof(unsigned int)));
        //glDrawElements(Tip primitive, koliko indeksa se koristi, koji su tip indeksi, pomjeraj sa pocetka niza indeksa da bi dosli do prvog indeksa koji koristimo


        glDisable(GL_CULL_FACE); //Iskljuci eliminisanje lica - ovako mozete da ukljucujete/iskljucujete pri iscrtavanju samo odredjenih objekata koji su trenutno bind-ovani

        glfwSwapBuffers(Window);
    }

    glfwTerminate();
    return 0;
}



