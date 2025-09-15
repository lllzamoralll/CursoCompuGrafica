//Lab de compu grafica
//grupo 7
//practica 5
//Sanchez Zamora Jesus
//422084798


#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// Shaders
#include "Shader.h"

void Inputs(GLFWwindow* window);

const GLint WIDTH = 1200, HEIGHT = 800;

// For Keyboard
float movX = 0.0f,
movY = 0.0f,
movZ = -5.0f,
rot = 0.0f;

// For model
float hombro = 0.0f,
codo = 0.0f,
palma = 0.0f,
dedo1 = 0.0f,
dedo2 = 0.0f,
dedo3 = 0.0f,
dedo4 = 0.0f,
dedo5 = 0.0f;

int main() {
    glfwInit();
    // Verificación de compatibilidad 
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Practica 5, Sanchez Zamora Jesus", nullptr, nullptr);

    int screenWidth, screenHeight;
    glfwGetFramebufferSize(window, &screenWidth, &screenHeight);

    // Verificación de errores de creacion ventana
    if (nullptr == window) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return EXIT_FAILURE;
    }

    glfwMakeContextCurrent(window);
    glewExperimental = GL_TRUE;

    // Verificación de errores de inicialización de glew
    if (GLEW_OK != glewInit()) {
        std::cout << "Failed to initialise GLEW" << std::endl;
        return EXIT_FAILURE;
    }

    // Define las dimensiones del viewport
    glViewport(0, 0, screenWidth, screenHeight);

    // Setup OpenGL options
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // Build and compile our shader program
    Shader ourShader("Shader/core.vs", "Shader/core.frag");

    // Set up vertex data (and buffer(s)) and attribute pointers
    float vertices[] = {
        -0.5f, -0.5f, 0.5f,
         0.5f, -0.5f, 0.5f,
         0.5f,  0.5f, 0.5f,
         0.5f,  0.5f, 0.5f,
        -0.5f,  0.5f, 0.5f,
        -0.5f, -0.5f, 0.5f,

        -0.5f, -0.5f,-0.5f,
         0.5f, -0.5f,-0.5f,
         0.5f,  0.5f,-0.5f,
         0.5f,  0.5f,-0.5f,
        -0.5f,  0.5f,-0.5f,
        -0.5f, -0.5f,-0.5f,

         0.5f, -0.5f,  0.5f,
         0.5f, -0.5f, -0.5f,
         0.5f,  0.5f, -0.5f,
         0.5f,  0.5f, -0.5f,
         0.5f,  0.5f,  0.5f,
         0.5f, -0.5f, 0.5f,

        -0.5f,  0.5f,  0.5f,
        -0.5f,  0.5f, -0.5f,
        -0.5f, -0.5f, -0.5f,
        -0.5f, -0.5f, -0.5f,
        -0.5f, -0.5f,  0.5f,
        -0.5f,  0.5f,  0.5f,

        -0.5f, -0.5f, -0.5f,
         0.5f, -0.5f, -0.5f,
         0.5f, -0.5f,  0.5f,
         0.5f, -0.5f,  0.5f,
        -0.5f, -0.5f,  0.5f,
        -0.5f, -0.5f, -0.5f,

        -0.5f,  0.5f, -0.5f,
         0.5f,  0.5f, -0.5f,
         0.5f,  0.5f,  0.5f,
         0.5f,  0.5f,  0.5f,
        -0.5f,  0.5f,  0.5f,
        -0.5f,  0.5f, -0.5f,
    };

    GLuint VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    // Enlazar Vertex Array Object
    glBindVertexArray(VAO);

    // Copiamos nuestro arreglo de vértices en un buffer para que OpenGL lo use
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // Posición
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    glm::mat4 projection = glm::mat4(1);
    projection = glm::perspective(glm::radians(45.0f), (GLfloat)screenWidth / (GLfloat)screenHeight, 0.1f, 100.0f);
    glm::vec3 color = glm::vec3(0.0f, 0.0f, 1.0f);

    while (!glfwWindowShouldClose(window)) {
        Inputs(window);
        glfwPollEvents();

        // Render
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        ourShader.Use();
        glm::mat4 model = glm::mat4(1);
        glm::mat4 view = glm::mat4(1);
        glm::mat4 modelTemp = glm::mat4(1.0f);
        glm::mat4 modelTemp2 = glm::mat4(1.0f);

        // View setup
        view = glm::translate(view, glm::vec3(movX, movY, movZ));
        view = glm::rotate(view, glm::radians(rot), glm::vec3(0.0f, 1.0f, 0.0f));

        GLint modelLoc = glGetUniformLocation(ourShader.Program, "model");
        GLint viewLoc = glGetUniformLocation(ourShader.Program, "view");
        GLint projecLoc = glGetUniformLocation(ourShader.Program, "projection");
        GLint uniformColor = ourShader.uniformColor;

        glUniformMatrix4fv(projecLoc, 1, GL_FALSE, glm::value_ptr(projection));
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        glUniform3fv(uniformColor, 1, glm::value_ptr(color));

        glBindVertexArray(VAO);

        // Model bícep
        model = glm::rotate(model, glm::radians(hombro), glm::vec3(0.0f, 0.0f, 1.0f));
        modelTemp = model = glm::translate(model, glm::vec3(1.5f, 0.0f, 0.0f));
        model = glm::scale(model, glm::vec3(3.0f, 1.0f, 1.0f));
        color = glm::vec3(0.0f, 1.0f, 0.0f);
        glUniform3fv(uniformColor, 1, glm::value_ptr(color));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        glDrawArrays(GL_TRIANGLES, 0, 36);

        // Model antebrazo
        model = glm::translate(modelTemp, glm::vec3(1.5f, 0.0f, 0.0f));
        model = glm::rotate(model, glm::radians(codo), glm::vec3(0.0f, 1.0f, 0.0f));
        modelTemp = model = glm::translate(model, glm::vec3(1.0f, 0.0f, 0.0f));
        model = glm::scale(model, glm::vec3(2.0f, 1.0f, 1.0f));
        color = glm::vec3(1.0f, 1.0f, 0.0f);
        glUniform3fv(uniformColor, 1, glm::value_ptr(color));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        glDrawArrays(GL_TRIANGLES, 0, 36);

        // Model palma
        model = glm::translate(modelTemp, glm::vec3(1.0f, 0.0f, 0.0f));
        model = glm::rotate(model, glm::radians(palma), glm::vec3(1.0f, 0.0f, 0.0f));
        modelTemp2 = modelTemp = model = glm::translate(model, glm::vec3(0.25f, 0.0f, 0.0f));
        model = glm::scale(model, glm::vec3(0.5f, 1.0f, 1.0f));
        color = glm::vec3(1.0f, 1.0f, 1.0f);
        glUniform3fv(uniformColor, 1, glm::value_ptr(color));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        glDrawArrays(GL_TRIANGLES, 0, 36);

        // Dedo 1 - Primera falange (índice, arriba, dobla hacia abajo)
        model = glm::translate(modelTemp2, glm::vec3(0.25f, 0.35f, 0.357f));
        model = glm::rotate(model, glm::radians(dedo1), glm::vec3(0.0f, 0.0f, -1.0f));
        model = glm::translate(model, glm::vec3(0.5f, 0.0f, 0.0f));
        modelTemp = model;
        model = glm::scale(model, glm::vec3(1.0f, 0.15f, 0.125f));
        color = glm::vec3(1.0f, 0.2f, 0.2f);
        glUniform3fv(uniformColor, 1, glm::value_ptr(color));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        glDrawArrays(GL_TRIANGLES, 0, 36);
        // Dedo 1 - Segunda falange
        model = glm::translate(modelTemp, glm::vec3(0.5f, 0.0f, 0.0f));
        model = glm::rotate(model, glm::radians(dedo1), glm::vec3(0.0f, 0.0f, -1.0f));
        model = glm::translate(model, glm::vec3(0.5f, 0.0f, 0.0f));
        modelTemp = model;
        model = glm::scale(model, glm::vec3(1.0f, 0.15f, 0.125f));
        color = glm::vec3(1.0f, 0.3f, 0.3f);
        glUniform3fv(uniformColor, 1, glm::value_ptr(color));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        glDrawArrays(GL_TRIANGLES, 0, 36);
        // Dedo 1 - Tercera falange
        model = glm::translate(modelTemp, glm::vec3(0.5f, 0.0f, 0.0f));
        model = glm::rotate(model, glm::radians(dedo1), glm::vec3(0.0f, 0.0f, -1.0f));
        model = glm::translate(model, glm::vec3(0.375f, 0.0f, 0.0f));
        model = glm::scale(model, glm::vec3(0.75f, 0.15f, 0.125f));
        color = glm::vec3(1.0f, 0.4f, 0.4f);
        glUniform3fv(uniformColor, 1, glm::value_ptr(color));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        glDrawArrays(GL_TRIANGLES, 0, 36);

        // Dedo 2 - Primera falange (medio, arriba, dobla hacia abajo)
        model = glm::translate(modelTemp2, glm::vec3(0.25f, 0.35f, 0.04f));
        model = glm::rotate(model, glm::radians(dedo2), glm::vec3(0.0f, 0.0f, -1.0f));
        model = glm::translate(model, glm::vec3(0.5f, 0.0f, 0.0f));
        modelTemp = model;
        model = glm::scale(model, glm::vec3(1.0f, 0.15f, 0.125f));
        color = glm::vec3(0.2f, 1.0f, 0.2f);
        glUniform3fv(uniformColor, 1, glm::value_ptr(color));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        glDrawArrays(GL_TRIANGLES, 0, 36);
        // Dedo 2 - Segunda falange
        model = glm::translate(modelTemp, glm::vec3(0.5f, 0.0f, 0.0f));
        model = glm::rotate(model, glm::radians(dedo2), glm::vec3(0.0f, 0.0f, -1.0f));
        model = glm::translate(model, glm::vec3(0.5f, 0.0f, 0.0f));
        modelTemp = model;
        model = glm::scale(model, glm::vec3(1.0f, 0.15f, 0.125f));
        color = glm::vec3(0.4f, 1.0f, 0.4f);
        glUniform3fv(uniformColor, 1, glm::value_ptr(color));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        glDrawArrays(GL_TRIANGLES, 0, 36);
        // Dedo 2 - Tercera falange
        model = glm::translate(modelTemp, glm::vec3(0.5f, 0.0f, 0.0f));
        model = glm::rotate(model, glm::radians(dedo2), glm::vec3(0.0f, 0.0f, -1.0f));
        model = glm::translate(model, glm::vec3(0.375f, 0.0f, 0.0f));
        model = glm::scale(model, glm::vec3(0.75f, 0.15f, 0.125f));
        color = glm::vec3(0.6f, 1.0f, 0.6f);
        glUniform3fv(uniformColor, 1, glm::value_ptr(color));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        glDrawArrays(GL_TRIANGLES, 0, 36);

        // Dedo 3 - Primera falange (anular, arriba, dobla hacia abajo)
        model = glm::translate(modelTemp2, glm::vec3(0.25f, 0.35f, -0.27f));
        model = glm::rotate(model, glm::radians(dedo3), glm::vec3(0.0f, 0.0f, -1.0f));
        model = glm::translate(model, glm::vec3(0.5f, 0.0f, 0.0f));
        modelTemp = model;
        model = glm::scale(model, glm::vec3(1.0f, 0.15f, 0.125f));
        color = glm::vec3(0.2f, 0.2f, 1.0f);
        glUniform3fv(uniformColor, 1, glm::value_ptr(color));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        glDrawArrays(GL_TRIANGLES, 0, 36);
        // Dedo 3 - Segunda falange
        model = glm::translate(modelTemp, glm::vec3(0.5f, 0.0f, 0.0f));
        model = glm::rotate(model, glm::radians(dedo3), glm::vec3(0.0f, 0.0f, -1.0f));
        model = glm::translate(model, glm::vec3(0.5f, 0.0f, 0.0f));
        modelTemp = model;
        model = glm::scale(model, glm::vec3(1.0f, 0.15f, 0.125f));
        color = glm::vec3(0.4f, 0.4f, 1.0f);
        glUniform3fv(uniformColor, 1, glm::value_ptr(color));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        glDrawArrays(GL_TRIANGLES, 0, 36);
        // Dedo 3 - Tercera falange
        model = glm::translate(modelTemp, glm::vec3(0.5f, 0.0f, 0.0f));
        model = glm::rotate(model, glm::radians(dedo3), glm::vec3(0.0f, 0.0f, -1.0f));
        model = glm::translate(model, glm::vec3(0.375f, 0.0f, 0.0f));
        model = glm::scale(model, glm::vec3(0.75f, 0.15f, 0.125f));
        color = glm::vec3(0.6f, 0.6f, 1.0f);
        glUniform3fv(uniformColor, 1, glm::value_ptr(color));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        glDrawArrays(GL_TRIANGLES, 0, 36);

        // Dedo 4 - Primera falange (meñique, abajo, mismo movimiento que antebrazo)
        model = glm::translate(modelTemp2, glm::vec3(0.25f, -0.1f, -0.27f));
        model = glm::rotate(model, glm::radians(dedo4), glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::translate(model, glm::vec3(0.5f, 0.0f, 0.0f));
        modelTemp = model;
        model = glm::scale(model, glm::vec3(1.0f, 0.15f, 0.125f));
        color = glm::vec3(1.0f, 1.0f, 0.2f);
        glUniform3fv(uniformColor, 1, glm::value_ptr(color));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        glDrawArrays(GL_TRIANGLES, 0, 36);
        // Dedo 4 - Segunda falange
        model = glm::translate(modelTemp, glm::vec3(0.5f, 0.0f, 0.0f));
        model = glm::rotate(model, glm::radians(dedo4), glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::translate(model, glm::vec3(0.5f, 0.0f, 0.0f));
        modelTemp = model;
        model = glm::scale(model, glm::vec3(1.0f, 0.15f, 0.125f));
        color = glm::vec3(1.0f, 1.0f, 0.4f);
        glUniform3fv(uniformColor, 1, glm::value_ptr(color));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        glDrawArrays(GL_TRIANGLES, 0, 36);
        // Dedo 4 - Tercera falange
        model = glm::translate(modelTemp, glm::vec3(0.5f, 0.0f, 0.0f));
        model = glm::rotate(model, glm::radians(dedo4), glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::translate(model, glm::vec3(0.375f, 0.0f, 0.0f));
        model = glm::scale(model, glm::vec3(0.75f, 0.15f, 0.125f));
        color = glm::vec3(1.0f, 1.0f, 0.6f);
        glUniform3fv(uniformColor, 1, glm::value_ptr(color));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        glDrawArrays(GL_TRIANGLES, 0, 36);

        // Dedo 5 - Primera falange (pulgar, abajo, dobla hacia la derecha)
        model = glm::translate(modelTemp2, glm::vec3(0.25f, -0.1f, 0.357f));
        model = glm::rotate(model, glm::radians(dedo5), glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::translate(model, glm::vec3(0.375f, 0.0f, 0.0f));
        modelTemp = model;
        model = glm::scale(model, glm::vec3(0.75f, 0.15f, 0.125f));
        color = glm::vec3(1.0f, 0.2f, 1.0f);
        glUniform3fv(uniformColor, 1, glm::value_ptr(color));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        glDrawArrays(GL_TRIANGLES, 0, 36);
        // Dedo 5 - Segunda falange
        model = glm::translate(modelTemp, glm::vec3(0.375f, 0.0f, 0.0f));
        model = glm::rotate(model, glm::radians(dedo5), glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::translate(model, glm::vec3(0.375f, 0.0f, 0.0f));
        modelTemp = model;
        model = glm::scale(model, glm::vec3(0.75f, 0.15f, 0.125f));
        color = glm::vec3(1.0f, 0.4f, 1.0f);
        glUniform3fv(uniformColor, 1, glm::value_ptr(color));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        glDrawArrays(GL_TRIANGLES, 0, 36);
        // Dedo 5 - Tercera falange
        model = glm::translate(modelTemp, glm::vec3(0.375f, 0.0f, 0.0f));
        model = glm::rotate(model, glm::radians(dedo5), glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::translate(model, glm::vec3(0.375f, 0.0f, 0.0f));
        model = glm::scale(model, glm::vec3(0.75f, 0.15f, 0.125f));
        color = glm::vec3(1.0f, 0.6f, 1.0f);
        glUniform3fv(uniformColor, 1, glm::value_ptr(color));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        glDrawArrays(GL_TRIANGLES, 0, 36);

        glBindVertexArray(0);

        // Swap the screen buffers
        glfwSwapBuffers(window);
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteProgram(ourShader.Program);

    glfwTerminate();
    return EXIT_SUCCESS;
}

void Inputs(GLFWwindow* window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        movX += 0.08f;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        movX -= 0.08f;
    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
        movY += 0.08f;
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
        movY -= 0.08f;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        movZ -= 0.08f;
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        movZ += 0.08f;
    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
        rot += 0.18f;
    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
        rot -= 0.18f;
    if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS)
        hombro += 0.18f;
    if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS)
        hombro -= 0.18f;
    // Codo: dobla hacia la izquierda, [-90, 0] grados
    if (glfwGetKey(window, GLFW_KEY_T) == GLFW_PRESS) {
        codo -= 0.18f;
        if (codo < -90.0f) codo = -90.0f;
    }
    if (glfwGetKey(window, GLFW_KEY_G) == GLFW_PRESS) {
        codo += 0.18f;
        if (codo > 0.0f) codo = 0.0f;
    }
    if (glfwGetKey(window, GLFW_KEY_Y) == GLFW_PRESS)
        palma += 0.18f;
    if (glfwGetKey(window, GLFW_KEY_H) == GLFW_PRESS)
        palma -= 0.18f;
    // Dedo 1: limitar a [0, 90] grados (hacia abajo, todas las falanges)
    if (glfwGetKey(window, GLFW_KEY_U) == GLFW_PRESS) {
        dedo1 += 0.18f;
        if (dedo1 > 90.0f) dedo1 = 90.0f;
    }
    if (glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS) {
        dedo1 -= 0.18f;
        if (dedo1 < 0.0f) dedo1 = 0.0f;
    }
    // Dedo 2: limitar a [0, 90] grados (hacia abajo, todas las falanges)
    if (glfwGetKey(window, GLFW_KEY_I) == GLFW_PRESS) {
        dedo2 += 0.18f;
        if (dedo2 > 90.0f) dedo2 = 90.0f;
    }
    if (glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS) {
        dedo2 -= 0.18f;
        if (dedo2 < 0.0f) dedo2 = 0.0f;
    }
    // Dedo 3: limitar a [0, 90] grados (hacia abajo, todas las falanges)
    if (glfwGetKey(window, GLFW_KEY_O) == GLFW_PRESS) {
        dedo3 += 0.18f;
        if (dedo3 > 90.0f) dedo3 = 90.0f;
    }
    if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS) {
        dedo3 -= 0.18f;
        if (dedo3 < 0.0f) dedo3 = 0.0f;
    }
    // Dedo 4: limitar a [-90, 0] grados (mismo movimiento que antebrazo, hacia la izquierda)
    if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS) {
        dedo4 -= 0.18f;
        if (dedo4 < -90.0f) dedo4 = -90.0f;
    }
    if (glfwGetKey(window, GLFW_KEY_SEMICOLON) == GLFW_PRESS) {
        dedo4 += 0.18f;
        if (dedo4 > 0.0f) dedo4 = 0.0f;
    }
    // Dedo 5: limitar a [0, 90] grados (hacia la derecha, todas las falanges)
    if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS) {
        dedo5 += 0.18f;
        if (dedo5 > 90.0f) dedo5 = 90.0f;
    }
    if (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS) {
        dedo5 -= 0.18f;
        if (dedo5 < 0.0f) dedo5 = 0.0f;
    }
    // Cerrar todos los dedos: C (dedos 1, 2, 3, 5 a 90; dedo 4 a -90)
    if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS) {
        dedo1 += 0.18f;
        if (dedo1 > 90.0f) dedo1 = 90.0f;
        dedo2 += 0.18f;
        if (dedo2 > 90.0f) dedo2 = 90.0f;
        dedo3 += 0.18f;
        if (dedo3 > 90.0f) dedo3 = 90.0f;
        dedo4 -= 0.18f;
        if (dedo4 < -90.0f) dedo4 = -90.0f;
        dedo5 += 0.18f;
        if (dedo5 > 90.0f) dedo5 = 90.0f;
    }
    // Abrir todos los dedos: V (dedos 1, 2, 3, 5 a 0; dedo 4 a 0)
    if (glfwGetKey(window, GLFW_KEY_V) == GLFW_PRESS) {
        dedo1 -= 0.18f;
        if (dedo1 < 0.0f) dedo1 = 0.0f;
        dedo2 -= 0.18f;
        if (dedo2 < 0.0f) dedo2 = 0.0f;
        dedo3 -= 0.18f;
        if (dedo3 < 0.0f) dedo3 = 0.0f;
        dedo4 += 0.18f;
        if (dedo4 > 0.0f) dedo4 = 0.0f;
        dedo5 -= 0.18f;
        if (dedo5 < 0.0f) dedo5 = 0.0f;
    }
}