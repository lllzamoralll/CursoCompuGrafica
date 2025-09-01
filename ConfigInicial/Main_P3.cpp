//Sanchez Zamora Jesus
//422084798
//Lab. Compu Grafica
// Grupo 7
//Practica 4 Proyecciones y transformaciones


#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// Shaders
#include "Shader.h"

const GLint WIDTH = 800, HEIGHT = 600;

int main() {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Proyecciones y transformaciones Jesus Sanchez Zamora", nullptr, nullptr);

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

    GLfloat vertices[] = {
        // Cara Frontal (Rojo brillante)
        -0.5f, -0.5f,  0.5f,   1.0f, 0.0f, 0.0f,
         0.5f, -0.5f,  0.5f,   1.0f, 0.0f, 0.0f,
         0.5f,  0.5f,  0.5f,   1.0f, 0.0f, 0.0f,
         0.5f,  0.5f,  0.5f,   1.0f, 0.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,   1.0f, 0.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,   1.0f, 0.0f, 0.0f,

        // Cara Trasera (Rojo brillante)
        -0.5f, -0.5f, -0.5f,   1.0f, 0.0f, 0.0f,
         0.5f, -0.5f, -0.5f,   1.0f, 0.0f, 0.0f,
         0.5f,  0.5f, -0.5f,   1.0f, 0.0f, 0.0f,
         0.5f,  0.5f, -0.5f,   1.0f, 0.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,   1.0f, 0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,   1.0f, 0.0f, 0.0f,

        // Cara Derecha (Azul brillante)
         0.5f, -0.5f,  0.5f,   0.0f, 0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,   0.0f, 0.0f, 1.0f,
         0.5f,  0.5f, -0.5f,   0.0f, 0.0f, 1.0f,
         0.5f,  0.5f, -0.5f,   0.0f, 0.0f, 1.0f,
         0.5f,  0.5f,  0.5f,   0.0f, 0.0f, 1.0f,
         0.5f, -0.5f,  0.5f,   0.0f, 0.0f, 1.0f,

         // Cara Izquierda (Amarillo brillante)
         -0.5f,  0.5f,  0.5f,   1.0f, 1.0f, 0.0f,
         -0.5f,  0.5f, -0.5f,   1.0f, 1.0f, 0.0f,
         -0.5f, -0.5f, -0.5f,   1.0f, 1.0f, 0.0f,
         -0.5f, -0.5f, -0.5f,   1.0f, 1.0f, 0.0f,
         -0.5f, -0.5f,  0.5f,   1.0f, 1.0f, 0.0f,
         -0.5f,  0.5f,  0.5f,   1.0f, 1.0f, 0.0f,

         // Cara Superior (Cian brillante)
         -0.5f,  0.5f, -0.5f,   0.0f, 1.0f, 1.0f,
          0.5f,  0.5f, -0.5f,   0.0f, 1.0f, 1.0f,
          0.5f,  0.5f,  0.5f,   0.0f, 1.0f, 1.0f,
          0.5f,  0.5f,  0.5f,   0.0f, 1.0f, 1.0f,
         -0.5f,  0.5f,  0.5f,   0.0f, 1.0f, 1.0f,
         -0.5f,  0.5f, -0.5f,   0.0f, 1.0f, 1.0f,

         // Cara Inferior (Verde brillante)
         -0.5f, -0.5f, -0.5f,   0.0f, 1.0f, 0.0f,
          0.5f, -0.5f, -0.5f,   0.0f, 1.0f, 0.0f,
          0.5f, -0.5f,  0.5f,   0.0f, 1.0f, 0.0f,
          0.5f, -0.5f,  0.5f,   0.0f, 1.0f, 0.0f,
         -0.5f, -0.5f,  0.5f,   0.0f, 1.0f, 0.0f,
         -0.5f, -0.5f, -0.5f,   0.0f, 1.0f, 0.0f,
    };

    GLuint VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    glm::mat4 projection = glm::mat4(1);
    projection = glm::perspective(glm::radians(35.0f), (GLfloat)screenWidth / (GLfloat)screenHeight, 0.1f, 100.0f);

    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();
        glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        ourShader.Use();
        glm::mat4 view = glm::mat4(1);
        view = glm::translate(view, glm::vec3(0.0f, 0.0f, -8.0f));
        view = glm::rotate(view, glm::radians(25.0f), glm::vec3(1.0f, 0.0f, 0.0f));
        view = glm::rotate(view, glm::radians(10.0f), glm::vec3(0.0f, 1.0f, 0.0f));

        glm::mat4 model = glm::mat4(1);
        GLint modelLoc = glGetUniformLocation(ourShader.Program, "model");
        GLint viewLoc = glGetUniformLocation(ourShader.Program, "view");
        GLint projecLoc = glGetUniformLocation(ourShader.Program, "projection");

        glUniformMatrix4fv(projecLoc, 1, GL_FALSE, glm::value_ptr(projection));
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));

        // Cubo 1 (base izquierda) - Rotación para mostrar cara FRONTAL (Roja)
        model = glm::mat4(1);
        model = glm::translate(model, glm::vec3(-1.5f, -1.0f, 0.0f));
        model = glm::rotate(model, glm::radians(45.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        glBindVertexArray(0);

        // Cubo 2 (base centro) - Rotación para mostrar cara DERECHA (Azul)
        model = glm::mat4(1);
        model = glm::translate(model, glm::vec3(0.0f, -1.0f, 0.0f));
        model = glm::rotate(model, glm::radians(45.0f - 90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        glBindVertexArray(0);

        // Cubo 3 (base derecha) - Rotación para mostrar cara TRASERA (Roja)
        model = glm::mat4(1);
        model = glm::translate(model, glm::vec3(1.5f, -1.0f, 0.0f));
        model = glm::rotate(model, glm::radians(45.0f + 180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        glBindVertexArray(0);

        // Cubo 4 (arriba izquierda) - Rotación para mostrar cara SUPERIOR (Cian)
        model = glm::mat4(1);
        model = glm::scale(model, glm::vec3(0.9f, 0.9f, 0.9f));
        model = glm::translate(model, glm::vec3(-0.75f, 0.0f, 0.0f));
        model = glm::rotate(model, glm::radians(45.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        glBindVertexArray(0);

        // Cubo 5 (arriba derecha) - Rotación para mostrar cara IZQUIERDA (Amarilla)
        model = glm::mat4(1);
        model = glm::scale(model, glm::vec3(0.9f, 0.9f, 0.9f));
        model = glm::translate(model, glm::vec3(0.75f, 0.0f, 0.0f));
        model = glm::rotate(model, glm::radians(135.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        glBindVertexArray(0);

        glfwSwapBuffers(window);
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glfwTerminate();
    return EXIT_SUCCESS;
}