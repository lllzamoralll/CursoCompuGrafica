// Sanchez Zamora Jesus
// 422084798
// Lab de compu grafica
// Grupo 7
// Keyframes - Practica final

#include <iostream>
#include <fstream>
#include <sstream>
#include <cmath>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "stb_image.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "SOIL2/SOIL2.h"
#include "Shader.h"
#include "Camera.h"
#include "Model.h"

// Prototipos
void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mode);
void MouseCallback(GLFWwindow* window, double xPos, double yPos);
void DoMovement();
void Animation();
void loadAnimationFromFile(const char* filename);

// Tamaño de ventana
const GLuint WIDTH = 800, HEIGHT = 600;
int SCREEN_WIDTH, SCREEN_HEIGHT;

// Cámara y tiempo
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
GLfloat lastX = WIDTH / 2.0;
GLfloat lastY = HEIGHT / 2.0;
GLfloat deltaTime = 0.0f;
GLfloat lastFrame = 0.0f;
bool keys[1024];
bool firstMouse = true;

// Luz y modelos
glm::vec3 lightPos(0.0f, 0.0f, 0.0f);
bool active;
glm::vec3 pointLightPositions[] = {
    glm::vec3(0.0f,2.0f, 0.0f),
    glm::vec3(0.0f,0.0f, 0.0f),
    glm::vec3(0.0f,0.0f,  0.0f),
    glm::vec3(0.0f,0.0f, 0.0f)
};

float vertices[] = {
    -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
     0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
     0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
     0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
    -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,

    -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
     0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
    -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,

    -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
    -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
    -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
    -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
    -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
    -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

     0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
     0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
     0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
     0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
     0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
     0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

    -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
     0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
     0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
     0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

    -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
     0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
    -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
    -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
};

glm::vec3 Light1 = glm::vec3(0);
float rotBall = 0.0f;
float rotDog = 0.0f;
float dogPosX = 0.0f, dogPosY = 0.0f, dogPosZ = 0.0f;
float FLegs = 0.0f, RLegs = 0.0f, RLegsB = 80.0f, FLegsB = -80.0f;
float head = 0.0f, tail = 0.0f, fLeftLeg = 0.0f, fRightLeg = 0.0f;
float bLeftLeg = 0.0f, bRightLeg = 0.0f, bodyIncline = 0.0f;

#define MAX_FRAMES 9
int i_max_steps = 100;
int i_curr_steps = 0;
typedef struct _frame {
    float rotDog, rotDogInc, dogPosX, dogPosY, dogPosZ, incX, incY, incZ;
    float head, headInc, tail, tailInc, FLegs, FLegsInc, RLegs, RLegsInc;
    float fLeftLeg, fLeftLegInc, fRightLeg, fRightLegInc, bLeftLeg, bLeftLegInc, bRightLeg, bRightLegInc, bodyIncline, bodyInclineInc;
} FRAME;

FRAME KeyFrame[MAX_FRAMES];
int FrameIndex = 0;
bool play = false;
int playIndex = 0;

void saveFrame() {
    KeyFrame[FrameIndex].dogPosX = dogPosX;
    KeyFrame[FrameIndex].dogPosY = dogPosY;
    KeyFrame[FrameIndex].dogPosZ = dogPosZ;
    KeyFrame[FrameIndex].rotDog = rotDog;
    KeyFrame[FrameIndex].head = head;
    KeyFrame[FrameIndex].tail = tail;
    KeyFrame[FrameIndex].FLegs = FLegs;
    KeyFrame[FrameIndex].RLegs = RLegs;
    KeyFrame[FrameIndex].fLeftLeg = fLeftLeg;
    KeyFrame[FrameIndex].fRightLeg = fRightLeg;
    KeyFrame[FrameIndex].bLeftLeg = bLeftLeg;
    KeyFrame[FrameIndex].bRightLeg = bRightLeg;
    KeyFrame[FrameIndex].bodyIncline = bodyIncline;
    FrameIndex++;
}

void resetElements() {
    dogPosX = KeyFrame[0].dogPosX;
    dogPosY = KeyFrame[0].dogPosY;
    dogPosZ = KeyFrame[0].dogPosZ;
    head = KeyFrame[0].head;
    tail = KeyFrame[0].tail;
    FLegs = KeyFrame[0].FLegs;
    RLegs = KeyFrame[0].RLegs;
    fLeftLeg = KeyFrame[0].fLeftLeg;
    fRightLeg = KeyFrame[0].fRightLeg;
    bLeftLeg = KeyFrame[0].bLeftLeg;
    bRightLeg = KeyFrame[0].bRightLeg;
    bodyIncline = KeyFrame[0].bodyIncline;
    rotDog = KeyFrame[0].rotDog;
}

void interpolation() {
    int pi = playIndex;
    KeyFrame[pi].incX = (KeyFrame[pi + 1].dogPosX - KeyFrame[pi].dogPosX) / i_max_steps;
    KeyFrame[pi].incY = (KeyFrame[pi + 1].dogPosY - KeyFrame[pi].dogPosY) / i_max_steps;
    KeyFrame[pi].incZ = (KeyFrame[pi + 1].dogPosZ - KeyFrame[pi].dogPosZ) / i_max_steps;
    KeyFrame[pi].headInc = (KeyFrame[pi + 1].head - KeyFrame[pi].head) / i_max_steps;
    KeyFrame[pi].tailInc = (KeyFrame[pi + 1].tail - KeyFrame[pi].tail) / i_max_steps;
    KeyFrame[pi].FLegsInc = (KeyFrame[pi + 1].FLegs - KeyFrame[pi].FLegs) / i_max_steps;
    KeyFrame[pi].RLegsInc = (KeyFrame[pi + 1].RLegs - KeyFrame[pi].RLegs) / i_max_steps;
    KeyFrame[pi].fLeftLegInc = (KeyFrame[pi + 1].fLeftLeg - KeyFrame[pi].fLeftLeg) / i_max_steps;
    KeyFrame[pi].fRightLegInc = (KeyFrame[pi + 1].fRightLeg - KeyFrame[pi].fRightLeg) / i_max_steps;
    KeyFrame[pi].bLeftLegInc = (KeyFrame[pi + 1].bLeftLeg - KeyFrame[pi].bLeftLeg) / i_max_steps;
    KeyFrame[pi].bRightLegInc = (KeyFrame[pi + 1].bRightLeg - KeyFrame[pi].bRightLeg) / i_max_steps;
    KeyFrame[pi].bodyInclineInc = (KeyFrame[pi + 1].bodyIncline - KeyFrame[pi].bodyIncline) / i_max_steps;
    KeyFrame[pi].rotDogInc = (KeyFrame[pi + 1].rotDog - KeyFrame[pi].rotDog) / i_max_steps;
}

void loadAnimationFromFile(const char* filename) {
    std::ifstream inFile(filename);
    if (!inFile) { std::cout << "No se pudo abrir el archivo de animación." << std::endl; return; }
    FrameIndex = 0;
    std::string line;
    while (std::getline(inFile, line) && FrameIndex < MAX_FRAMES) {
        std::istringstream iss(line);
        iss >> KeyFrame[FrameIndex].dogPosX >> KeyFrame[FrameIndex].dogPosY >> KeyFrame[FrameIndex].dogPosZ
            >> KeyFrame[FrameIndex].rotDog >> KeyFrame[FrameIndex].head >> KeyFrame[FrameIndex].tail >> KeyFrame[FrameIndex].FLegs
            >> KeyFrame[FrameIndex].RLegs >> KeyFrame[FrameIndex].fLeftLeg >> KeyFrame[FrameIndex].fRightLeg
            >> KeyFrame[FrameIndex].bLeftLeg >> KeyFrame[FrameIndex].bRightLeg >> KeyFrame[FrameIndex].bodyIncline;
        FrameIndex++;
    }
    inFile.close();
    std::cout << "Animación cargada desde archivo." << std::endl;
}

void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mode) {
    if (key == GLFW_KEY_K && action == GLFW_PRESS) {
        if (FrameIndex < MAX_FRAMES) saveFrame();
    }
    if (key == GLFW_KEY_L && action == GLFW_PRESS) {
        if (!play && FrameIndex > 1) { resetElements(); interpolation(); play = true; playIndex = 0; i_curr_steps = 0; }
        else play = !play;
    }
    if (key == GLFW_KEY_M && action == GLFW_PRESS) {
        loadAnimationFromFile("animacion_sentarse_saludar.txt");
    }
    if (key == GLFW_KEY_SPACE && action == GLFW_PRESS) {
        active = !active;
        Light1 = active ? glm::vec3(0.2f, 0.8f, 1.0f) : glm::vec3(0.0f);
    }
    if (key >= 0 && key < 1024) {
        if (action == GLFW_PRESS)   keys[key] = true;
        else if (action == GLFW_RELEASE) keys[key] = false;
    }
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);
}

void DoMovement() {
    // Movimiento libre de cámara
    if (keys[GLFW_KEY_W]) { camera.ProcessKeyboard(FORWARD, deltaTime); }
    if (keys[GLFW_KEY_S]) { camera.ProcessKeyboard(BACKWARD, deltaTime); }
    if (keys[GLFW_KEY_A]) { camera.ProcessKeyboard(LEFT, deltaTime); }
    if (keys[GLFW_KEY_D]) { camera.ProcessKeyboard(RIGHT, deltaTime); }

    // Movimiento partes del cuerpo
    if (keys[GLFW_KEY_4]) { head += 0.25f; }
    if (keys[GLFW_KEY_5]) { head -= 0.25f; }
    if (keys[GLFW_KEY_U]) {
        bLeftLeg += 0.25f; bRightLeg -= 0.25f;
        if (bLeftLeg >= RLegsB && bRightLeg <= FLegsB) { bLeftLeg = RLegsB; bRightLeg = FLegsB; }
    }
    if (keys[GLFW_KEY_J]) {
        bLeftLeg -= 0.25f; bRightLeg += 0.25f;
        if (bLeftLeg <= FLegsB && bRightLeg >= RLegsB) { bLeftLeg = FLegsB; bRightLeg = RLegsB; }
    }
    if (keys[GLFW_KEY_C]) { fLeftLeg += 0.25f; if (fLeftLeg > 20.0f) fLeftLeg = 20.0f; }
    if (keys[GLFW_KEY_V]) { fLeftLeg -= 0.25f; if (fLeftLeg < 0.0f) fLeftLeg = 0.0f; }
    if (keys[GLFW_KEY_Z]) { fRightLeg += 0.25f; if (fRightLeg > 90.0f) fRightLeg = 90.0f; }
    if (keys[GLFW_KEY_X]) { fRightLeg -= 0.25f; if (fRightLeg < 0.0f) fRightLeg = 0.0f; }
    if (keys[GLFW_KEY_0]) { bodyIncline += 0.5f; if (bodyIncline > 25.0f) bodyIncline = 25.0f; }
    if (keys[GLFW_KEY_9]) { bodyIncline -= 0.5f; if (bodyIncline < 0.0f) bodyIncline = 0.0f; }
}

void Animation() {
    if (play) {
        if (i_curr_steps >= i_max_steps) {
            playIndex++;
            if (playIndex > FrameIndex - 2) { playIndex = 0; play = false; }
            else { i_curr_steps = 0; interpolation(); }
        }
        else {
            dogPosX += KeyFrame[playIndex].incX;
            dogPosY += KeyFrame[playIndex].incY;
            dogPosZ += KeyFrame[playIndex].incZ;
            head += KeyFrame[playIndex].headInc;
            tail += KeyFrame[playIndex].tailInc;
            RLegs += KeyFrame[playIndex].RLegsInc; FLegs += KeyFrame[playIndex].FLegsInc;
            fLeftLeg += KeyFrame[playIndex].fLeftLegInc; fRightLeg += KeyFrame[playIndex].fRightLegInc;
            bLeftLeg += KeyFrame[playIndex].bLeftLegInc; bRightLeg += KeyFrame[playIndex].bRightLegInc;
            bodyIncline += KeyFrame[playIndex].bodyInclineInc;
            rotDog += KeyFrame[playIndex].rotDogInc;
            i_curr_steps++;
        }
    }
}

void MouseCallback(GLFWwindow* window, double xPos, double yPos) {
    if (firstMouse) { lastX = xPos; lastY = yPos; firstMouse = false; }
    GLfloat xOffset = xPos - lastX; GLfloat yOffset = lastY - yPos;
    lastX = xPos; lastY = yPos;
    camera.ProcessMouseMovement(xOffset, yOffset);
}

int main() {
    glfwInit();
    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "KeyFrames, Sanchez Zamora Jesus", nullptr, nullptr);
    if (!window) { std::cout << "Failed to create GLFW window" << std::endl; glfwTerminate(); return EXIT_FAILURE; }
    glfwMakeContextCurrent(window);
    glfwGetFramebufferSize(window, &SCREEN_WIDTH, &SCREEN_HEIGHT);
    glfwSetKeyCallback(window, KeyCallback);
    glfwSetCursorPosCallback(window, MouseCallback);
    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK) { std::cout << "Failed to initialize GLEW" << std::endl; return EXIT_FAILURE; }
    glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

    Shader lightingShader("Shader/lighting.vs", "Shader/lighting.frag");
    Shader lampShader("Shader/lamp.vs", "Shader/lamp.frag");
    Model DogBody((char*)"Models/DogBody.obj");
    Model HeadDog((char*)"Models/HeadDog.obj");
    Model DogTail((char*)"Models/TailDog.obj");
    Model F_RightLeg((char*)"Models/F_RightLegDog.obj");
    Model F_LeftLeg((char*)"Models/F_LeftLegDog.obj");
    Model B_RightLeg((char*)"Models/B_RightLegDog.obj");
    Model B_LeftLeg((char*)"Models/B_LeftLegDog.obj");
    Model Piso((char*)"Models/piso.obj");
    Model Ball((char*)"Models/ball.obj");

    GLuint VBO, VAO;
    glGenVertexArrays(1, &VAO); glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    lightingShader.Use();
    glUniform1i(glGetUniformLocation(lightingShader.Program, "Material.difuse"), 0);
    glUniform1i(glGetUniformLocation(lightingShader.Program, "Material.specular"), 1);
    glm::mat4 projection = glm::perspective(camera.GetZoom(), (GLfloat)SCREEN_WIDTH / (GLfloat)SCREEN_HEIGHT, 0.1f, 100.0f);

    for (int i = 0; i < MAX_FRAMES; ++i) {
        KeyFrame[i] = FRAME();
    }

    while (!glfwWindowShouldClose(window)) {
        GLfloat currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        glfwPollEvents();
        DoMovement();
        Animation();
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glEnable(GL_DEPTH_TEST);

        lightingShader.Use();
        glUniform1i(glGetUniformLocation(lightingShader.Program, "diffuse"), 0);
        GLint viewPosLoc = glGetUniformLocation(lightingShader.Program, "viewPos");
        glUniform3f(viewPosLoc, camera.GetPosition().x, camera.GetPosition().y, camera.GetPosition().z);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.direction"), -0.2f, -1.0f, -0.3f);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.ambient"), 0.6f, 0.6f, 0.6f);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.diffuse"), 0.6f, 0.6f, 0.6f);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.specular"), 0.3f, 0.3f, 0.3f);

        glm::vec3 lightColor;
        lightColor.x = std::abs(std::sin(glfwGetTime() * Light1.x));
        lightColor.y = std::abs(std::sin(glfwGetTime() * Light1.y));
        lightColor.z = std::sin(glfwGetTime() * Light1.z);

        glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].position"), pointLightPositions[0].x, pointLightPositions[0].y, pointLightPositions[0].z);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].ambient"), lightColor.x, lightColor.y, lightColor.z);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].diffuse"), lightColor.x, lightColor.y, lightColor.z);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].specular"), 1.0f, 0.2f, 0.2f);
        glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[0].constant"), 1.0f);
        glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[0].linear"), 0.045f);
        glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[0].quadratic"), 0.075f);

        glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.position"), camera.GetPosition().x, camera.GetPosition().y, camera.GetPosition().z);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.direction"), camera.GetFront().x, camera.GetFront().y, camera.GetFront().z);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.ambient"), 0.2f, 0.2f, 0.8f);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.diffuse"), 0.2f, 0.2f, 0.8f);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.specular"), 0.0f, 0.0f, 0.0f);
        glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.constant"), 1.0f);
        glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.linear"), 0.3f);
        glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.quadratic"), 0.7f);
        glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.cutOff"), glm::cos(glm::radians(12.0f)));
        glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.outerCutOff"), glm::cos(glm::radians(18.0f)));
        glUniform1f(glGetUniformLocation(lightingShader.Program, "material.shininess"), 5.0f);

        glm::mat4 view = camera.GetViewMatrix();
        GLint modelLoc = glGetUniformLocation(lightingShader.Program, "model");
        GLint viewLoc = glGetUniformLocation(lightingShader.Program, "view");
        GLint projLoc = glGetUniformLocation(lightingShader.Program, "projection");
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

        glm::mat4 model(1);

        // Piso
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        Piso.Draw(lightingShader);

        // Cuerpo del perro
        glm::mat4 modelTemp = glm::translate(model, glm::vec3(dogPosX, dogPosY, dogPosZ));
        model = glm::rotate(modelTemp, glm::radians(-bodyIncline), glm::vec3(1.0f, 0.0f, 0.0f));
        modelTemp = glm::rotate(model, glm::radians(rotDog), glm::vec3(0.0f, 1.0f, 0.0f));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        DogBody.Draw(lightingShader);

        // Cabeza
        model = modelTemp;
        model = glm::translate(model, glm::vec3(0.0f, 0.093f, 0.208f));
        model = glm::rotate(model, glm::radians(head), glm::vec3(0.0f, 0.0f, 1.0f));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        HeadDog.Draw(lightingShader);

        // Cola
        model = modelTemp;
        model = glm::translate(model, glm::vec3(0.0f, 0.026f, -0.288f));
        model = glm::rotate(model, glm::radians(tail), glm::vec3(0.0f, 0.0f, -1.0f));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        DogTail.Draw(lightingShader);

        // Back Left Leg
        model = modelTemp;
        model = glm::translate(model, glm::vec3(0.082f, -0.046f, -0.218f));
        model = glm::rotate(model, glm::radians(bLeftLeg), glm::vec3(-1.0f, 0.0f, 0.0f));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        B_LeftLeg.Draw(lightingShader);

        // Back Right Leg
        model = modelTemp;
        model = glm::translate(model, glm::vec3(-0.083f, -0.057f, -0.231f));
        model = glm::rotate(model, glm::radians(bRightLeg), glm::vec3(-1.0f, 0.0f, 0.0f));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        B_RightLeg.Draw(lightingShader);

        // Front Left Leg
        model = modelTemp;
        model = glm::translate(model, glm::vec3(0.112f, -0.044f, 0.074f));
        model = glm::rotate(model, glm::radians(fLeftLeg), glm::vec3(-1.0f, 0.0f, 0.0f));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        F_LeftLeg.Draw(lightingShader);

        // Front Right Leg
        model = modelTemp;
        model = glm::translate(model, glm::vec3(-0.111f, -0.055f, 0.074f));
        model = glm::rotate(model, glm::radians(fRightLeg), glm::vec3(-1.0f, 0.0f, 0.0f));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        F_RightLeg.Draw(lightingShader);

        // Ball
        model = glm::mat4(1);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        glUniform1i(glGetUniformLocation(lightingShader.Program, "transparency"), 1);
        model = glm::rotate(model, glm::radians(rotBall), glm::vec3(0.0f, 1.0f, 0.0f));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        Ball.Draw(lightingShader);
        glDisable(GL_BLEND);
        glBindVertexArray(0);

        glfwSwapBuffers(window);
    }
    glfwTerminate();
    return 0;
}
