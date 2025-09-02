// Previo 4 Modelado geométrico
// Laboratorio de computacion grafica
// Grupo 07
// Sanchez Zamora Jesus
// 422084798

#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Shader.h"

const GLint WIDTH = 800, HEIGHT = 600;
float movX = 0.0f, movY = 0.0f, movZ = -5.0f;
float rot = 0.0f;
float mesaX = 0.0f, mesaY = 0.5f, mesaZ = 0.0f;
float mesaRot = 0.0f;
glm::vec3 mesaColor = glm::vec3(1.0f, 0.0f, 0.0f); // Rojo por defecto

void Inputs(GLFWwindow* window);

int main() {
    glfwInit();
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Modelado geometrico - Sanchez Zamora Jesus", nullptr, nullptr);
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

    float vertices[] = {
        -0.5f, -0.5f, 0.5f, 1.0f, 0.0f,0.0f,//Front
        0.5f, -0.5f, 0.5f,  1.0f, 0.0f,0.0f,
        0.5f,  0.5f, 0.5f,  1.0f, 0.0f,0.0f,
        0.5f,  0.5f, 0.5f,  1.0f, 0.0f,0.0f,
        -0.5f,  0.5f, 0.5f, 1.0f, 0.0f,0.0f,
        -0.5f, -0.5f, 0.5f, 1.0f, 0.0f,0.0f,

        -0.5f, -0.5f,-0.5f, 0.0f, 1.0f,0.0f,//Back
         0.5f, -0.5f,-0.5f, 0.0f, 1.0f,0.0f,
         0.5f,  0.5f,-0.5f, 0.0f, 1.0f,0.0f,
         0.5f,  0.5f,-0.5f, 0.0f, 1.0f,0.0f,
        -0.5f,  0.5f,-0.5f, 0.0f, 1.0f,0.0f,
        -0.5f, -0.5f,-0.5f, 0.0f, 1.0f,0.0f,

         0.5f, -0.5f,  0.5f,  0.0f, 0.0f,1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, 0.0f,1.0f,
         0.5f,  0.5f, -0.5f,  0.0f, 0.0f,1.0f,
         0.5f,  0.5f, -0.5f,  0.0f, 0.0f,1.0f,
         0.5f,  0.5f,  0.5f,  0.0f, 0.0f,1.0f,
         0.5f,  -0.5f, 0.5f, 0.0f, 0.0f,1.0f,

        -0.5f,  0.5f,  0.5f,  1.0f, 1.0f,0.0f,
        -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,0.0f,
        -0.5f, -0.5f, -0.5f,  1.0f, 1.0f,0.0f,
        -0.5f, -0.5f, -0.5f,  1.0f, 1.0f,0.0f,
        -0.5f, -0.5f,  0.5f,  1.0f, 1.0f,0.0f,
        -0.5f,  0.5f,  0.5f,  1.0f, 1.0f,0.0f,

        -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,1.0f,
        0.5f, -0.5f, -0.5f,  0.0f, 1.0f,1.0f,
        0.5f, -0.5f,  0.5f,  0.0f, 1.0f,1.0f,
        0.5f, -0.5f,  0.5f,  0.0f, 1.0f,1.0f,
        -0.5f, -0.5f,  0.5f, 0.0f, 1.0f,1.0f,
        -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,1.0f,

        -0.5f,  0.5f, -0.5f, 1.0f, 0.2f,0.5f,
        0.5f,  0.5f, -0.5f,  1.0f, 0.2f,0.5f,
        0.5f,  0.5f,  0.5f,  1.0f, 0.2f,0.5f,
        0.5f,  0.5f,  0.5f,  1.0f, 0.2f,0.5f,
        -0.5f,  0.5f,  0.5f, 1.0f, 0.2f,0.5f,
        -0.5f,  0.5f, -0.5f, 1.0f, 0.2f,0.5f,
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

    glm::mat4 projection = glm::perspective(glm::radians(45.0f), (GLfloat)screenWidth / (GLfloat)screenHeight, 0.1f, 100.0f);

    while (!glfwWindowShouldClose(window)) {
        Inputs(window);
        glfwPollEvents();

        glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        ourShader.Use();
        glm::mat4 view = glm::mat4(1.0f);
        view = glm::translate(view, glm::vec3(movX, movY, movZ));
        view = glm::rotate(view, glm::radians(rot), glm::vec3(0.0f, 1.0f, 0.0f));

        GLint modelLoc = glGetUniformLocation(ourShader.Program, "model");
        GLint viewLoc = glGetUniformLocation(ourShader.Program, "view");
        GLint projLoc = glGetUniformLocation(ourShader.Program, "projection");
        GLint colorLoc = glGetUniformLocation(ourShader.Program, "objectColor");

        glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));

        glBindVertexArray(VAO);

        // Mesa principal (superficie + cuatro patas)
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(mesaX, mesaY, mesaZ));
        model = glm::rotate(model, glm::radians(mesaRot), glm::vec3(0.0f, 1.0f, 0.0f));

        // Superficie
        glm::mat4 modelSuperficie = model;
        modelSuperficie = glm::scale(modelSuperficie, glm::vec3(2.0f, 0.2f, 1.0f));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelSuperficie));
        glUniform3fv(colorLoc, 1, glm::value_ptr(mesaColor));
        glDrawArrays(GL_TRIANGLES, 0, 36);

        // Primera pata
        glm::mat4 modelPata1 = model;
        modelPata1 = glm::translate(modelPata1, glm::vec3(0.8f, -0.3f, 0.4f));
        modelPata1 = glm::scale(modelPata1, glm::vec3(0.1f, 0.5f, 0.1f));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelPata1));
        glUniform3fv(colorLoc, 1, glm::value_ptr(glm::vec3(0.0f, 1.0f, 0.0f)));
        glDrawArrays(GL_TRIANGLES, 0, 36);

        // Segunda pata
        glm::mat4 modelPata2 = model;
        modelPata2 = glm::translate(modelPata2, glm::vec3(0.8f, -0.3f, -0.4f));
        modelPata2 = glm::scale(modelPata2, glm::vec3(0.1f, 0.5f, 0.1f));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelPata2));
        glUniform3fv(colorLoc, 1, glm::value_ptr(glm::vec3(0.0f, 1.0f, 0.0f)));
        glDrawArrays(GL_TRIANGLES, 0, 36);

        // Tercera pata
        glm::mat4 modelPata3 = model;
        modelPata3 = glm::translate(modelPata3, glm::vec3(-0.8f, -0.3f, 0.4f));
        modelPata3 = glm::scale(modelPata3, glm::vec3(0.1f, 0.5f, 0.1f));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelPata3));
        glUniform3fv(colorLoc, 1, glm::value_ptr(glm::vec3(0.0f, 1.0f, 0.0f)));
        glDrawArrays(GL_TRIANGLES, 0, 36);

        // Cuarta pata
        glm::mat4 modelPata4 = model;
        modelPata4 = glm::translate(modelPata4, glm::vec3(-0.8f, -0.3f, -0.4f));
        modelPata4 = glm::scale(modelPata4, glm::vec3(0.1f, 0.5f, 0.1f));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelPata4));
        glUniform3fv(colorLoc, 1, glm::value_ptr(glm::vec3(0.0f, 1.0f, 0.0f)));
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
    if (glfwGetKey(window, GLFW_KEY_PAGE_UP) == GLFW_PRESS)
        movY += 0.08f;
    if (glfwGetKey(window, GLFW_KEY_PAGE_DOWN) == GLFW_PRESS)
        movY -= 0.08f;
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        movZ -= 0.08f;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        movZ += 0.08f;
    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
        rot += 0.4f;
    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
        rot -= 0.4f;
    if (glfwGetKey(window, GLFW_KEY_I) == GLFW_PRESS)
        mesaZ -= 0.08f;
    if (glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS)
        mesaZ += 0.08f;
    if (glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS)
        mesaRot += 0.4f;
    if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS)
        mesaRot -= 0.4f;
    static bool lastCState = false;
    bool currentCState = glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS;
    if (currentCState && !lastCState) {
        if (mesaColor == glm::vec3(1.0f, 0.0f, 0.0f))
            mesaColor = glm::vec3(0.0f, 1.0f, 0.0f);
        else if (mesaColor == glm::vec3(0.0f, 1.0f, 0.0f))
            mesaColor = glm::vec3(0.0f, 0.0f, 1.0f);
        else
            mesaColor = glm::vec3(1.0f, 0.0f, 0.0f);
    }
    lastCState = currentCState;
}