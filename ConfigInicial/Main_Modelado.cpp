//Lab de compu grafica
// Grupo 7
// practica 4: Modelado geometrico
// Sanchez Zamora Jesus
//422084798


#include<iostream>

//#define GLEW_STATIC

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// Shaders
#include "Shader.h"

void Inputs(GLFWwindow* window);

const GLint WIDTH = 800, HEIGHT = 600;
float movX = 0.0f;  // Centrado frontal
float movY = -0.2f; // Ligeramente abajo
float movZ = -2.5f; // Distancia ajustada
float rot = 0.0f;   // Sin rotación inicial

int main() {
    glfwInit();
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Modelado geometrico Practica 4, Sanchez Zamora Jesus", nullptr, nullptr);

    int screenWidth, screenHeight;
    glfwGetFramebufferSize(window, &screenWidth, &screenHeight);

    if (nullptr == window) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return EXIT_FAILURE;
    }

    glfwMakeContextCurrent(window);
    glewExperimental = GL_TRUE;

    if (GLEW_OK != glewInit()) {
        std::cout << "Failed to initialise GLEW" << std::endl;
        return EXIT_FAILURE;
    }

    glViewport(0, 0, screenWidth, screenHeight);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    Shader ourShader("Shader/core.vs", "Shader/core.frag");

    // Vértices (colores en VBO ignorados, usamos constantes)
    float vertices[] = {
        // Front
        -0.5f, -0.5f,  0.5f, 1.0f, 0.0f, 0.0f,
         0.5f, -0.5f,  0.5f, 1.0f, 0.0f, 0.0f,
         0.5f,  0.5f,  0.5f, 1.0f, 0.0f, 0.0f,
         0.5f,  0.5f,  0.5f, 1.0f, 0.0f, 0.0f,
        -0.5f,  0.5f,  0.5f, 1.0f, 0.0f, 0.0f,
        -0.5f, -0.5f,  0.5f, 1.0f, 0.0f, 0.0f,

        // Back
        -0.5f, -0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
         0.5f, -0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
         0.5f,  0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
         0.5f,  0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
        -0.5f,  0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
        -0.5f, -0.5f, -0.5f, 0.0f, 1.0f, 0.0f,

        // Right
         0.5f, -0.5f,  0.5f, 0.0f, 0.0f, 1.0f,
         0.5f, -0.5f, -0.5f, 0.0f, 0.0f, 1.0f,
         0.5f,  0.5f, -0.5f, 0.0f, 0.0f, 1.0f,
         0.5f,  0.5f, -0.5f, 0.0f, 0.0f, 1.0f,
         0.5f,  0.5f,  0.5f, 0.0f, 0.0f, 1.0f,
         0.5f, -0.5f,  0.5f, 0.0f, 0.0f, 1.0f,

         // Left
         -0.5f,  0.5f,  0.5f, 1.0f, 1.0f, 0.0f,
         -0.5f,  0.5f, -0.5f, 1.0f, 1.0f, 0.0f,
         -0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 0.0f,
         -0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 0.0f,
         -0.5f, -0.5f,  0.5f, 1.0f, 1.0f, 0.0f,
         -0.5f,  0.5f,  0.5f, 1.0f, 1.0f, 0.0f,

         // Bottom
         -0.5f, -0.5f, -0.5f, 0.0f, 1.0f, 1.0f,
          0.5f, -0.5f, -0.5f, 0.0f, 1.0f, 1.0f,
          0.5f, -0.5f,  0.5f, 0.0f, 1.0f, 1.0f,
          0.5f, -0.5f,  0.5f, 0.0f, 1.0f, 1.0f,
         -0.5f, -0.5f,  0.5f, 0.0f, 1.0f, 1.0f,
         -0.5f, -0.5f, -0.5f, 0.0f, 1.0f, 1.0f,

         // Top
         -0.5f,  0.5f, -0.5f, 1.0f, 0.2f, 0.5f,
          0.5f,  0.5f, -0.5f, 1.0f, 0.2f, 0.5f,
          0.5f,  0.5f,  0.5f, 1.0f, 0.2f, 0.5f,
          0.5f,  0.5f,  0.5f, 1.0f, 0.2f, 0.5f,
         -0.5f,  0.5f,  0.5f, 1.0f, 0.2f, 0.5f,
         -0.5f,  0.5f, -0.5f, 1.0f, 0.2f, 0.5f,
    };

    GLuint VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // Posición
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);
    // Color por vértice (lo desactivaremos para usar constantes)
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    glm::mat4 projection = glm::perspective(glm::radians(45.0f), (GLfloat)screenWidth / (GLfloat)screenHeight, 0.1f, 100.0f);

    while (!glfwWindowShouldClose(window)) {
        Inputs(window);
        glfwPollEvents();

        glClearColor(0.627f, 0.784f, 0.847f, 1.0f);  // Fondo azul claro de la imagen
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        ourShader.Use();
        glm::mat4 model = glm::mat4(1);
        glm::mat4 view = glm::mat4(1);

        view = glm::translate(view, glm::vec3(movX, movY, movZ));
        view = glm::rotate(view, glm::radians(rot), glm::vec3(0.0f, 1.0f, 0.0f));

        GLint modelLoc = glGetUniformLocation(ourShader.Program, "model");
        GLint viewLoc = glGetUniformLocation(ourShader.Program, "view");
        GLint projLoc = glGetUniformLocation(ourShader.Program, "projection");

        glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));

        glBindVertexArray(VAO);

        // Desactivar colores del VBO, usar constantes
        glDisableVertexAttribArray(1);

        // Colores ajustados
        const GLfloat NAR_R = 0.8f, NAR_G = 0.4f, NAR_B = 0.2f;  // Naranja
        const GLfloat AMA_R = 0.945f, AMA_G = 0.847f, AMA_B = 0.369f;  // Beige/amarillo claro

        // Patas (beige/amarillo claro)
        glVertexAttrib3f(1, AMA_R, AMA_G, AMA_B);

        model = glm::mat4(1.0f);
        model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.3f));
        model = glm::translate(model, glm::vec3(0.0f, -0.5f, 0.0f));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        glDrawArrays(GL_TRIANGLES, 0, 36);

        model = glm::mat4(1.0f);
        model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.3f));
        model = glm::translate(model, glm::vec3(3.0f, -0.5f, 0.0f));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        glDrawArrays(GL_TRIANGLES, 0, 36);

        model = glm::mat4(1.0f);
        model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.3f));
        model = glm::translate(model, glm::vec3(0.0f, -0.5f, -3.0f));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        glDrawArrays(GL_TRIANGLES, 0, 36);

        model = glm::mat4(1.0f);
        model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.3f));
        model = glm::translate(model, glm::vec3(3.0f, -0.5f, -3.0f));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        glDrawArrays(GL_TRIANGLES, 0, 36);

        // Piernas (naranja)
        glVertexAttrib3f(1, NAR_R, NAR_G, NAR_B);

        model = glm::mat4(1.0f);
        model = glm::scale(model, glm::vec3(0.1f, 0.4f, 0.1f));
        model = glm::translate(model, glm::vec3(3.0f, 0.5f, -1.0f));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        glDrawArrays(GL_TRIANGLES, 0, 36);

        model = glm::mat4(1.0f);
        model = glm::scale(model, glm::vec3(0.1f, 0.4f, 0.1f));
        model = glm::translate(model, glm::vec3(0.0f, 0.5f, -1.0f));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        glDrawArrays(GL_TRIANGLES, 0, 36);

        model = glm::mat4(1.0f);
        model = glm::scale(model, glm::vec3(0.1f, 0.4f, 0.1f));
        model = glm::translate(model, glm::vec3(3.0f, 0.5f, -10.0f));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        glDrawArrays(GL_TRIANGLES, 0, 36);

        model = glm::mat4(1.0f);
        model = glm::scale(model, glm::vec3(0.1f, 0.4f, 0.1f));
        model = glm::translate(model, glm::vec3(0.0f, 0.5f, -10.0f));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        glDrawArrays(GL_TRIANGLES, 0, 36);

        // Torso (naranja)
        glVertexAttrib3f(1, NAR_R, NAR_G, NAR_B);
        model = glm::mat4(1.0f);
        model = glm::scale(model, glm::vec3(0.5f, 0.5f, 1.2f));
        model = glm::translate(model, glm::vec3(0.29f, 1.0f, -0.4f));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        glDrawArrays(GL_TRIANGLES, 0, 36);

        // Cabeza - Sección inferior naranja
        glVertexAttrib3f(1, NAR_R, NAR_G, NAR_B);
        model = glm::mat4(1.0f);
        model = glm::scale(model, glm::vec3(0.61f, 0.1f, 0.31f)); // 20% de altura
        model = glm::translate(model, glm::vec3(0.25f, 5.0f, 0.3f)); // Base
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        glDrawArrays(GL_TRIANGLES, 0, 36);

        // Sección media beige (más grande)
        glVertexAttrib3f(1, AMA_R, AMA_G, AMA_B);
        model = glm::mat4(1.0f);
        model = glm::scale(model, glm::vec3(0.6f, 0.5f, 0.3f)); // 60% de altura
        model = glm::translate(model, glm::vec3(0.25f, 1.5f + 0.1f, 0.3f)); // Desde 0.1f
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        glDrawArrays(GL_TRIANGLES, 0, 36);

        // Cabeza - Sección trasera cabeza
        glVertexAttrib3f(1, NAR_R, NAR_G, NAR_B);
        model = glm::mat4(1.0f);
        model = glm::scale(model, glm::vec3(0.61f, 0.5f, 0.20f)); // 20% de altura
        model = glm::translate(model, glm::vec3(0.25f, 1.5f + 0.1f, 0.19f)); // Base
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        glDrawArrays(GL_TRIANGLES, 0, 36);

        // Sección superior naranja
        glVertexAttrib3f(1, NAR_R, NAR_G, NAR_B);
        model = glm::mat4(1.0f);
        model = glm::scale(model, glm::vec3(0.61f, 0.1f, 0.31f)); // 20% de altura
        model = glm::translate(model, glm::vec3(0.25f, 10.0f, 0.3f)); // Desde 0.4f
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        glDrawArrays(GL_TRIANGLES, 0, 36);

        // Hocico y orejas (beige/amarillo claro)
        glVertexAttrib3f(1, AMA_R, AMA_G, AMA_B);

        // Hocico
        model = glm::mat4(1.0f);
        model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));
        model = glm::translate(model, glm::vec3(0.7f, 3.0f, 1.5f));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        glDrawArrays(GL_TRIANGLES, 0, 36);

        // Oreja izq
        model = glm::mat4(1.0f);
        model = glm::scale(model, glm::vec3(0.13f, 0.13f, 0.1f));
        model = glm::translate(model, glm::vec3(-1.6f, 6.0f, 1.8f));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        glDrawArrays(GL_TRIANGLES, 0, 36);

        // Oreja der
        model = glm::mat4(1.0f);
        model = glm::scale(model, glm::vec3(0.13f, 0.13f, 0.1f));
        model = glm::translate(model, glm::vec3(3.9f, 6.0f, 1.8f));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        glDrawArrays(GL_TRIANGLES, 0, 36);

        // Ojos (negro - manteniendo posiciones originales)
        glVertexAttrib3f(1, 0.0f, 0.0f, 0.0f);

        // Ojo izquierdo
        model = glm::mat4(1.0f);
        model = glm::scale(model, glm::vec3(0.05f, 0.05f, 0.05f));
        model = glm::translate(model, glm::vec3(1.0f, 16.5f, 5.5f));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        glDrawArrays(GL_TRIANGLES, 0, 36);

        // Ojo derecho
        model = glm::mat4(1.0f);
        model = glm::scale(model, glm::vec3(0.05f, 0.05f, 0.05f));
        model = glm::translate(model, glm::vec3(5.0f, 16.5f, 5.5f));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        glDrawArrays(GL_TRIANGLES, 0, 36);

        // Cola (naranja)
        glVertexAttrib3f(1, NAR_R, NAR_G, NAR_B);

        model = glm::mat4(1.0f);
        model = glm::scale(model, glm::vec3(0.1f, 0.4f, 0.1f));
        model = glm::translate(model, glm::vec3(1.5f, 2.3f, -10.0f));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        glDrawArrays(GL_TRIANGLES, 0, 36);

        model = glm::mat4(1.0f);
        model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.6f));
        model = glm::translate(model, glm::vec3(1.5f, 11.7f, -2.09f));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        glDrawArrays(GL_TRIANGLES, 0, 36);

        model = glm::mat4(1.0f);
        model = glm::scale(model, glm::vec3(0.1f, 0.4f, 0.1f));
        model = glm::translate(model, glm::vec3(1.5f, 2.3f, -15.0f));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        glDrawArrays(GL_TRIANGLES, 0, 36);

        glBindVertexArray(0);
        glfwSwapBuffers(window);
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);

    glfwTerminate();
    return EXIT_SUCCESS;
}

void Inputs(GLFWwindow* window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        movX += 0.08f;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        movX -= 0.08f;
    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
        movY += 0.08f;
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
        movY -= 0.08f;
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        movZ -= 0.08f;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        movZ += 0.08f;
    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
        rot += 0.4f;
    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
        rot -= 0.4f;
}