#include <iostream>
#include <vector>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "Shader.h"

// Tamaño de la ventana
const GLuint WIDTH = 800, HEIGHT = 800;

// Dimensiones de la matriz
const int ROWS = 27;
const int COLS = 27;

// Matriz de colores (sin modificar, como la original)
int crashMatrix[ROWS][COLS] = {
    {3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,1,3,3,1,3,3,3,3,3,3,3,3},
    {3,3,3,3,3,3,3,3,3,3,3,1,3,3,1,1,3,1,1,3,3,3,3,3,3,3,3},
    {3,3,3,3,3,3,3,3,1,3,1,1,3,1,2,1,1,2,1,3,3,3,3,3,3,3,3},
    {3,3,3,3,3,3,3,1,2,1,2,1,1,2,2,1,2,2,1,3,3,3,3,3,3,3,3},
    {1,1,1,3,3,3,3,1,2,1,2,1,2,2,1,2,2,1,3,3,3,3,3,3,3,3,3},
    {1,6,6,1,1,3,1,2,2,2,2,2,2,2,2,2,2,1,3,3,3,3,3,3,3,3,3},
    {1,1,1,6,6,1,1,2,2,2,2,2,2,2,2,2,2,1,3,3,3,3,3,3,3,3,3},
    {1,1,2,1,1,6,1,2,2,1,1,4,4,2,2,2,1,4,3,3,3,3,3,3,3,3,3},
    {3,1,2,2,2,1,6,1,1,4,4,4,4,1,1,1,4,4,4,3,3,3,3,3,3,3,3},
    {3,1,1,2,2,2,1,6,4,4,3,3,3,4,6,4,3,3,4,3,3,3,3,4,4,4,3},
    {3,3,1,1,2,1,6,6,4,3,3,3,3,3,6,3,3,3,1,3,3,3,3,4,4,4,3},
    {3,3,1,6,1,1,6,6,6,3,1,1,3,3,6,1,1,3,3,3,3,3,1,4,4,4,3},
    {3,3,3,1,6,6,6,6,6,3,1,1,3,3,6,1,1,3,3,1,1,1,4,4,4,3,3},
    {3,3,3,3,1,6,6,6,6,6,3,3,3,6,6,6,3,3,6,6,6,6,5,5,1,3,3},
    {3,3,3,3,1,6,6,5,5,5,6,6,6,6,6,6,6,6,6,6,6,5,5,1,3,3,3},
    {3,3,3,1,6,6,5,5,5,5,5,5,5,6,6,6,6,6,5,5,5,5,1,3,3,3,3},
    {3,3,3,1,6,5,5,1,1,1,1,5,5,5,5,5,5,5,5,5,5,1,3,3,3,3,3},
    {3,3,3,1,6,5,5,1,3,3,3,1,1,5,5,5,5,5,1,1,1,3,3,3,3,3,3},
    {3,3,3,3,1,5,5,1,3,3,3,3,3,1,1,1,1,1,3,3,1,3,3,3,3,3,3},
    {3,3,3,3,1,6,5,1,3,3,3,3,3,3,3,3,3,3,3,1,3,3,3,3,3,3,3},
    {3,3,3,3,3,1,5,5,1,3,3,4,3,3,3,3,3,3,1,3,3,3,3,3,3,3,3},
    {3,3,3,3,3,1,6,5,1,3,3,3,4,4,4,3,3,3,1,3,3,3,3,3,3,3,3},
    {3,3,3,3,3,3,1,5,5,1,3,3,3,3,3,3,3,1,3,3,3,3,3,3,3,3,3},
    {3,3,3,3,3,3,1,6,5,5,1,3,3,3,3,3,3,1,3,3,3,3,3,3,3,3,3},
    {3,3,3,3,3,3,3,1,6,5,5,1,1,1,1,1,1,1,3,3,3,3,3,3,3,3,3},
    {3,3,3,3,3,3,3,3,1,6,5,5,5,5,5,5,5,1,3,3,3,3,3,3,3,3,3},
    {3,3,3,3,3,3,3,3,3,1,1,1,1,1,1,1,1,3,3,3,3,3,3,3,3,3,3}
};

// Función para obtener un color RGB según el número en la matriz
std::vector<float> getColorFromCode(int code) {
    switch (code) {
    case 1: return { 0.0f, 0.0f, 0.0f }; // negro
    case 2: return { 1.0f, 0.0f, 0.0f }; // rojo
    case 3: return { 1.0f, 1.0f, 1.0f }; // blanco
    case 4: return { 0.4f, 0.2f, 0.0f }; // café
    case 5: return { 1.0f, 0.87f, 0.68f }; // beige
    case 6: return { 1.0f, 0.5f, 0.0f }; // naranja
    default: return { 1.0f, 1.0f, 1.0f }; // blanco por defecto
    }
}

void resize(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

int main() {
    // Inicializar GLFW
    if (!glfwInit()) {
        std::cerr << "Error al inicializar GLFW" << std::endl;
        return -1;
    }

    // Configurar OpenGL 3.3 Core
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Crear ventana
    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Crash Bandicoot Pixel Art", nullptr, nullptr);
    if (!window) {
        std::cerr << "Error al crear la ventana" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    // Inicializar GLEW
    if (glewInit() != GLEW_OK) {
        std::cerr << "Error al inicializar GLEW" << std::endl;
        return -1;
    }

    glfwSetFramebufferSizeCallback(window, resize);

    // Compilar shaders
    Shader shader("Shader/core.vs", "Shader/core.frag");

    // Datos para los píxeles (cada cuadro un cuadrado)
    float pixelSize = 2.0f / ROWS; // cuadrícula de -1 a 1
    std::vector<float> vertices;

    for (int y = 0; y < ROWS; y++) {
        for (int x = 0; x < COLS; x++) {
            auto color = getColorFromCode(crashMatrix[y][x]);

            float xpos = -1.0f + x * pixelSize;
            float ypos = 1.0f - y * pixelSize - pixelSize;

            // Cada pixel = 2 triángulos (6 vértices)
            float quad[] = {
                xpos,         ypos + pixelSize, 0.0f,  color[0], color[1], color[2],
                xpos,         ypos,             0.0f,  color[0], color[1], color[2],
                xpos + pixelSize, ypos,           0.0f,  color[0], color[1], color[2],

                xpos,         ypos + pixelSize, 0.0f,  color[0], color[1], color[2],
                xpos + pixelSize, ypos,           0.0f,  color[0], color[1], color[2],
                xpos + pixelSize, ypos + pixelSize, 0.0f,  color[0], color[1], color[2]
            };

            vertices.insert(vertices.end(), std::begin(quad), std::end(quad));
        }
    }

    // Crear VAO y VBO
    GLuint VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

    // Posiciones
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (GLvoid*)0);
    glEnableVertexAttribArray(0);

    // Colores
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (GLvoid*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    // Loop principal
    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();

        glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // Fondo negro para mejor visibilidad
        glClear(GL_COLOR_BUFFER_BIT);

        shader.Use();
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, vertices.size() / 6);
        glBindVertexArray(0);

        glfwSwapBuffers(window);
    }

    // Liberar recursos
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);

    glfwTerminate();
    return 0;
}