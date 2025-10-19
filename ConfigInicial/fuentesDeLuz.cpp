// Sanchez Zamora Jesus
// 422084798
// fuentes de luz
// practica 9
// entrega: 19/10/2025
#include <string>
#include <iostream>
#include <vector>
#include <cmath>
// GLEW
#include <GL/glew.h>
// GLFW
#include <GLFW/glfw3.h>
// GL includes
#include "Shader.h"
#include "Camera.h"
#include "Model.h"
// GLM Mathematics
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
// Other Libs
#include "SOIL2/SOIL2.h"
#include "stb_image.h"
// Properties
const GLuint WIDTH = 800, HEIGHT = 600;
int SCREEN_WIDTH, SCREEN_HEIGHT;
// Buffer objects
GLuint modelVAO, modelVBO, sphereVAO, sphereVBO, sphereEBO;
// Function prototypes
void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mode);
void MouseCallback(GLFWwindow* window, double xPos, double yPos);
void DoMovement();
// Camera
Camera camera(glm::vec3(0.0f, 1.0f, 5.0f));
bool keys[1024];
GLfloat lastX = 400, lastY = 300;
bool firstMouse = true;
// Light attributes (modifica estas posiciones manualmente para mover las fuentes de luz)
glm::vec3 dirLightPos(0.0f, 0.15f, -0.95f); // Posición de la luz direccional (esfera)
glm::vec3 lightPos1(-1.386f, 1.2f, -1.0f);   // Posición de spotlight 1 (esfera)
glm::vec3 lightPos2(-1.386f, 1.2f, 1.0f);  // Posición de spotlight 2 (esfera)
glm::vec3 dirLightDir(0.0f, 0.0f, 2.0f); // Dirección de la luz direccional
double deltaTime = 0.0; // Cambiado a double
double lastFrame = 0.0; // Cambiado a double
// Light scales (modifica estos valores para cambiar el tamaño de las fuentes de luz)
GLfloat dirLightScale = 0.135f;  // Escala de la esfera de la luz direccional
GLfloat spotLight1Scale = 0.18f; // Escala de la esfera de spotlight 1
GLfloat spotLight2Scale = 0.18f; // Escala de la esfera de spotlight 2
// Function to generate sphere vertices and indices
void generateSphere(std::vector<float>& vertices, std::vector<unsigned int>& indices, float radius, int stacks, int sectors) {
    const float PI = 3.14159265359f;
    vertices.clear();
    indices.clear();
    for (int i = 0; i <= stacks; ++i) {
        float phi = PI * float(i) / stacks;
        float sinPhi = sin(phi);
        float cosPhi = cos(phi);
        for (int j = 0; j <= sectors; ++j) {
            float theta = 2.0f * PI * float(j) / sectors;
            float sinTheta = sin(theta);
            float cosTheta = cos(theta);
            float x = radius * sinPhi * cosTheta;
            float y = radius * sinPhi * sinTheta;
            float z = radius * cosPhi;
            float nx = x / radius;
            float ny = y / radius;
            float nz = z / radius;
            vertices.push_back(x);
            vertices.push_back(y);
            vertices.push_back(z);
            vertices.push_back(nx);
            vertices.push_back(ny);
            vertices.push_back(nz);
        }
    }
    for (int i = 0; i < stacks; ++i) {
        for (int j = 0; j < sectors; ++j) {
            int k1 = i * (sectors + 1) + j;
            int k2 = k1 + sectors + 1;
            indices.push_back(k1);
            indices.push_back(k2);
            indices.push_back(k1 + 1);
            indices.push_back(k2);
            indices.push_back(k2 + 1);
            indices.push_back(k1 + 1);
        }
    }
}
int main() {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Practica 9, Sanchez Zamora Jesus", nullptr, nullptr);
    if (nullptr == window) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return EXIT_FAILURE;
    }
    glfwMakeContextCurrent(window);
    glfwGetFramebufferSize(window, &SCREEN_WIDTH, &SCREEN_HEIGHT);
    glfwSetKeyCallback(window, KeyCallback);
    glfwSetCursorPosCallback(window, MouseCallback);
    glewExperimental = GL_TRUE;
    if (GLEW_OK != glewInit()) {
        std::cout << "Failed to initialize GLEW" << std::endl;
        return EXIT_FAILURE;
    }
    glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
    glEnable(GL_DEPTH_TEST);
    // Setup and compile our shaders
    Shader shader("Shader/modelLoading.vs", "Shader/modelLoading.frag");
    Shader lampshader("Shader/lamp.vs", "Shader/lamp.frag");
    Shader lightingShader("Shader/lighting.vs", "Shader/lighting.frag");
    // Load models (solo como objetos iluminados, no fuentes de luz)
    Model dog((char*)"Models/RedDog.obj");
    Model repisa((char*)"Models/mesa/mesa.obj");
    Model canasta((char*)"Models/alfombra/10417_Rectangular_rug_v1_iterations-2.obj");
    Model mesa((char*)"Models/plantita/plantita.obj");
    Model silla((char*)"Models/mesita/10256_TV_Cabinet_v1_max2011.obj");
    Model sillon((char*)"Models/tv/MI SMART TV.obj");
    Model dirLightModel((char*)"Models/Lampara1/Flashlight.obj");    // Reemplaza con el nombre correcto de la lámpara direccional
    Model spotLight1Model((char*)"Models/Lampara1/sconce_02.obj"); // Reemplaza con el nombre correcto de la lámpara spotlight 1
    Model spotLight2Model((char*)"Models/Lampara1/sconce_02.obj"); // Reemplaza con el nombre correcto de la lámpara spotlight 2
    glm::mat4 projection = glm::perspective(camera.GetZoom(), (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, 0.1f, 100.0f);
    // Set up model VAO/VBO for .obj models
    float vertices[] = {
        -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f,
         0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f,
         0.5f,  0.5f, -0.5f, 0.0f, 0.0f, -1.0f,
         0.5f,  0.5f, -0.5f, 0.0f, 0.0f, -1.0f,
        -0.5f,  0.5f, -0.5f, 0.0f, 0.0f, -1.0f,
        -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f,
        -0.5f, -0.5f,  0.5f, 0.0f, 0.0f,  1.0f,
         0.5f, -0.5f,  0.5f, 0.0f, 0.0f,  1.0f,
         0.5f,  0.5f,  0.5f, 0.0f, 0.0f,  1.0f,
         0.5f,  0.5f,  0.5f, 0.0f, 0.0f,  1.0f,
        -0.5f,  0.5f,  0.5f, 0.0f, 0.0f,  1.0f,
        -0.5f, -0.5f,  0.5f, 0.0f, 0.0f,  1.0f,
        -0.5f,  0.5f,  0.5f, -1.0f, 0.0f,  0.0f,
        -0.5f,  0.5f, -0.5f, -1.0f, 0.0f,  0.0f,
        -0.5f, -0.5f, -0.5f, -1.0f, 0.0f,  0.0f,
        -0.5f, -0.5f, -0.5f, -1.0f, 0.0f,  0.0f,
        -0.5f, -0.5f,  0.5f, -1.0f, 0.0f,  0.0f,
        -0.5f,  0.5f,  0.5f, -1.0f, 0.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,  0.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 0.0f,  0.0f,
         0.5f, -0.5f, -0.5f,  1.0f, 0.0f,  0.0f,
         0.5f, -0.5f, -0.5f,  1.0f, 0.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,  0.0f,
        -0.5f, -0.5f, -0.5f, 0.0f, -1.0f,  0.0f,
         0.5f, -0.5f, -0.5f, 0.0f, -1.0f,  0.0f,
         0.5f, -0.5f,  0.5f, 0.0f, -1.0f,  0.0f,
         0.5f, -0.5f,  0.5f, 0.0f, -1.0f,  0.0f,
        -0.5f, -0.5f,  0.5f, 0.0f, -1.0f,  0.0f,
        -0.5f, -0.5f, -0.5f, 0.0f, -1.0f,  0.0f,
        -0.5f,  0.5f, -0.5f, 0.0f,  1.0f,  0.0f,
         0.5f,  0.5f, -0.5f, 0.0f,  1.0f,  0.0f,
         0.5f,  0.5f,  0.5f, 0.0f,  1.0f,  0.0f,
         0.5f,  0.5f,  0.5f, 0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f,  0.5f, 0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f, -0.5f, 0.0f,  1.0f,  0.0f
    };
    glGenVertexArrays(1, &modelVAO);
    glGenBuffers(1, &modelVBO);
    glBindVertexArray(modelVAO);
    glBindBuffer(GL_ARRAY_BUFFER, modelVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);
    glBindVertexArray(0);
    // Set up sphere VAO/VBO for lights
    std::vector<float> sphereVertices;
    std::vector<unsigned int> sphereIndices;
    generateSphere(sphereVertices, sphereIndices, 0.5f, 20, 20);
    glGenVertexArrays(1, &sphereVAO);
    glGenBuffers(1, &sphereVBO);
    glGenBuffers(1, &sphereEBO);
    glBindVertexArray(sphereVAO);
    glBindBuffer(GL_ARRAY_BUFFER, sphereVBO);
    glBufferData(GL_ARRAY_BUFFER, sphereVertices.size() * sizeof(float), sphereVertices.data(), GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, sphereEBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sphereIndices.size() * sizeof(unsigned int), sphereIndices.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glBindVertexArray(0);
    // Load textures
    GLuint texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    int textureWidth, textureHeight, nrChannels;
    stbi_set_flip_vertically_on_load(true);
    unsigned char* image = stbi_load("Models/Texture_albedo.jpg", &textureWidth, &textureHeight, &nrChannels, 0);
    if (image) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, textureWidth, textureHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
        glGenerateMipmap(GL_TEXTURE_2D);
        std::cout << "Textura cargada exitosamente." << std::endl;
    }
    else {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(image);
    // Game loop
    while (!glfwWindowShouldClose(window)) {
        double currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        glfwPollEvents();
        DoMovement();
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        lightingShader.Use();
        glm::mat4 view = camera.GetViewMatrix();
        glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
        glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "view"), 1, GL_FALSE, glm::value_ptr(view));
        GLint viewPosLoc = glGetUniformLocation(lightingShader.Program, "viewPos");
        glUniform3f(viewPosLoc, camera.GetPosition().x, camera.GetPosition().y, camera.GetPosition().z);
        // Set material properties
        glUniform1i(glGetUniformLocation(lightingShader.Program, "material.diffuse"), 0);
        glUniform1i(glGetUniformLocation(lightingShader.Program, "material.specular"), 1); // Placeholder
        glUniform1f(glGetUniformLocation(lightingShader.Program, "material.shininess"), 0.6f);
        // Bind texture
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture);
        // Directional light (apunta hacia +z, atrás del perro)
        glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.direction"), 0.0f, 0.0f, 1.0f); // Normalizado, hacia +z
        glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.ambient"), 0.2f, 0.2f, 0.2f);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.diffuse"), 0.5f, 0.5f, 0.5f);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.specular"), 0.3f, 0.3f, 0.3f);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.position"), dirLightPos.x, dirLightPos.y, dirLightPos.z);
        // Spotlight 1 (luz de arriba, apunta hacia -y)
        glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLights[0].position"), lightPos1.x, lightPos1.y, lightPos1.z);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLights[0].direction"), 0.0f, -1.0f, 0.0f); // Hacia abajo
        glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLights[0].ambient"), 0.1f, 0.1f, 0.1f);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLights[0].diffuse"), 1.0f, 0.5f, 0.0f);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLights[0].specular"), 1.0f, 0.5f, 0.0f);
        glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLights[0].constant"), 1.0f);
        glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLights[0].linear"), 0.09f);
        glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLights[0].quadratic"), 0.032f);
        glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLights[0].cutOff"), glm::cos(glm::radians(12.5f)));
        glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLights[0].outerCutOff"), glm::cos(glm::radians(17.5f)));
        // Spotlight 2 (luz de arriba, apunta hacia -y)
        glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLights[1].position"), lightPos2.x, lightPos2.y, lightPos2.z);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLights[1].direction"), 0.0f, -1.0f, 0.0f); // Hacia abajo
        glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLights[1].ambient"), 0.1f, 0.1f, 0.1f);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLights[1].diffuse"), 0.0f, 0.5f, 1.0f);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLights[1].specular"), 0.0f, 0.5f, 1.0f);
        glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLights[1].constant"), 1.0f);
        glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLights[1].linear"), 0.09f);
        glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLights[1].quadratic"), 0.032f);
        glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLights[1].cutOff"), glm::cos(glm::radians(12.5f)));
        glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLights[1].outerCutOff"), glm::cos(glm::radians(17.5f)));
        // Transparency
        glUniform1i(glGetUniformLocation(lightingShader.Program, "transparency"), 0);
        // Draw the loaded models (solo iluminados, no fuentes de luz)
        glBindVertexArray(modelVAO);
        // dibujando perro
        glm::mat4 model(1.0f);
        model = glm::translate(model, glm::vec3(0.0f, 0.35f, 0.0f));
        glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        dog.Draw(lightingShader);
        // dibujando la repisa
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(1.0f, 0.0f, 1.0f));
        model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
        glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        repisa.Draw(lightingShader);
        // dibujando el canasta
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
        model = glm::rotate(model, glm::radians(90.0f), glm::vec3(-1.0f, 0.0f, 0.0f));
        model = glm::scale(model, glm::vec3(0.015f, 0.015f, 0.015f));
        glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        canasta.Draw(lightingShader);
        // dibujando el mesa (plantita con transparencia)
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.9f, 0.0f, -0.5f));
        model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
        glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        glUniform1i(glGetUniformLocation(lightingShader.Program, "transparency"), 1);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        mesa.Draw(lightingShader);
        glDisable(GL_BLEND);
        // dibujando la silla
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(-1.0f, 0.2f, -0.75f));
        model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 2.0f, -0.0f));
        model = glm::scale(model, glm::vec3(0.0060f, 0.0060f, 0.0060f));
        glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        silla.Draw(lightingShader);
        // dibujando el sillon
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(1.0f, 0.49f, 1.15f));
        model = glm::scale(model, glm::vec3(0.25f, 0.25f, 0.25f));
        glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        sillon.Draw(lightingShader);
        // dibujando la lámpara direccional
        model = glm::mat4(1.0f);
        model = glm::scale(model, glm::vec3(0.05f, 0.05f, 0.05f)); // Ajusta la escala según el modelo
        model = glm::translate(model, glm::vec3(0.0f, 3.0f, -25.0f));
        glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        dirLightModel.Draw(lightingShader);
        // dibujando la lámpara spotlight 1
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(-1.5f, 1.0f, 1.0f));
        model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::scale(model, glm::vec3(2.1f, 2.1f, 2.1f)); // Ajusta la escala según el modelo
        glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        spotLight1Model.Draw(lightingShader);
        // dibujando la lámpara spotlight 2
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(-1.5f, 1.0f, -1.0f));
        model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 4.0f, 0.0f));
        model = glm::scale(model, glm::vec3(2.1f, 2.1f, 2.1f)); // Ajusta la escala según el modelo
        glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        spotLight2Model.Draw(lightingShader);
        glBindVertexArray(0);
        // Render light spheres (solo fuentes de luz)
        lampshader.Use();
        glUniformMatrix4fv(glGetUniformLocation(lampshader.Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
        glUniformMatrix4fv(glGetUniformLocation(lampshader.Program, "view"), 1, GL_FALSE, glm::value_ptr(view));
        // Directional light (sphere)
        model = glm::mat4(1.0f);
        model = glm::translate(model, dirLightPos); // Modifica dirLightPos para mover
        model = glm::scale(model, glm::vec3(dirLightScale)); // Modifica dirLightScale para escalar
        glUniformMatrix4fv(glGetUniformLocation(lampshader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        glUniform3f(glGetUniformLocation(lampshader.Program, "overrideColor"), 0.5f, 0.5f, 0.5f); // Gray
        glBindVertexArray(sphereVAO);
        glDrawElements(GL_TRIANGLES, sphereIndices.size(), GL_UNSIGNED_INT, 0);
        // Spotlight 1 (sphere)
        model = glm::mat4(1.0f);
        model = glm::translate(model, lightPos1); // Modifica lightPos1 para mover
        model = glm::scale(model, glm::vec3(spotLight1Scale)); // Modifica spotLight1Scale para escalar
        glUniformMatrix4fv(glGetUniformLocation(lampshader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        glUniform3f(glGetUniformLocation(lampshader.Program, "overrideColor"), 1.0f, 0.5f, 0.0f); // Orange
        glBindVertexArray(sphereVAO);
        glDrawElements(GL_TRIANGLES, sphereIndices.size(), GL_UNSIGNED_INT, 0);
        // Spotlight 2 (sphere)
        model = glm::mat4(1.0f);
        model = glm::translate(model, lightPos2); // Modifica lightPos2 para mover
        model = glm::scale(model, glm::vec3(spotLight2Scale)); // Modifica spotLight2Scale para escalar
        glUniformMatrix4fv(glGetUniformLocation(lampshader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        glUniform3f(glGetUniformLocation(lampshader.Program, "overrideColor"), 0.0f, 0.5f, 1.0f); // Blue
        glBindVertexArray(sphereVAO);
        glDrawElements(GL_TRIANGLES, sphereIndices.size(), GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
        glfwSwapBuffers(window);
    }
    glDeleteVertexArrays(1, &modelVAO);
    glDeleteBuffers(1, &modelVBO);
    glDeleteVertexArrays(1, &sphereVAO);
    glDeleteBuffers(1, &sphereVBO);
    glDeleteBuffers(1, &sphereEBO);
    glfwTerminate();
    return 0;
}
void DoMovement() {
    if (keys[GLFW_KEY_W] || keys[GLFW_KEY_UP]) {
        camera.ProcessKeyboard(FORWARD, (GLfloat)deltaTime);
    }
    if (keys[GLFW_KEY_S] || keys[GLFW_KEY_DOWN]) {
        camera.ProcessKeyboard(BACKWARD, (GLfloat)deltaTime);
    }
    if (keys[GLFW_KEY_A] || keys[GLFW_KEY_LEFT]) {
        camera.ProcessKeyboard(LEFT, (GLfloat)deltaTime);
    }
    if (keys[GLFW_KEY_D] || keys[GLFW_KEY_RIGHT]) {
        camera.ProcessKeyboard(RIGHT, (GLfloat)deltaTime);
    }
}
void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mode) {
    if (GLFW_KEY_ESCAPE == key && GLFW_PRESS == action) {
        glfwSetWindowShouldClose(window, GL_TRUE);
    }
    if (key >= 0 && key < 1024) {
        if (action == GLFW_PRESS) keys[key] = true;
        else if (action == GLFW_RELEASE) keys[key] = false;
    }
}
void MouseCallback(GLFWwindow* window, double xPos, double yPos) {
    if (firstMouse) {
        lastX = (GLfloat)xPos;
        lastY = (GLfloat)yPos;
        firstMouse = false;
    }
    GLfloat xOffset = (GLfloat)(xPos - lastX);
    GLfloat yOffset = (GLfloat)(lastY - yPos);
    lastX = (GLfloat)xPos;
    lastY = (GLfloat)yPos;
    camera.ProcessMouseMovement(xOffset, yOffset);
}