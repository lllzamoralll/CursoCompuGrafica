//Capistran Ponce Manuel Emiliano
//115006564
//practica 5
//entrega: 07/03/2025


#include<iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


// Shaders
#include "Shader.h"

void Inputs(GLFWwindow *window);


const GLint WIDTH = 1200, HEIGHT = 800;

//para modifiicar nuestra vista 
//For Keyboard
float	movX = 0.0f,
movY = 0.0f,
movZ = -5.0f,
rot = 0.0f;

//For model
float	hombro = 0.0f,
codo = 0.0f,
palma = 0.0f,
dedo1 = 0.0f,
dedo1b = 0.0f,
dedo2 = 0.0f,
dedo2b = 0.0f,
dedo2c = 0.0f,
dedo3b = 0.0f,
dedo4b = 0.0f,
dedo5b = 0.0f;



int main() {
	glfwInit();
	//Verificación de compatibilidad 
	// Set all the required options for GLFW
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	GLFWwindow *window = glfwCreateWindow(WIDTH, HEIGHT, "Practica 5 , Capistran Ponce Manuel Emiliano", nullptr, nullptr);

	int screenWidth, screenHeight;

	glfwGetFramebufferSize(window, &screenWidth, &screenHeight);

	//Verificación de errores de creacion  ventana
	if (nullptr == window)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();

		return EXIT_FAILURE;
	}

	glfwMakeContextCurrent(window);
	glewExperimental = GL_TRUE;

	//Verificación de errores de inicialización de glew

	if (GLEW_OK != glewInit()) {
		std::cout << "Failed to initialise GLEW" << std::endl;
		return EXIT_FAILURE;
	}


	// Define las dimensiones del viewport
	glViewport(0, 0, screenWidth, screenHeight);


	// Setup OpenGL options
	glEnable(GL_DEPTH_TEST);

	// enable alpha support
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


	// Build and compile our shader program
	Shader ourShader("Shader/core.vs", "Shader/core.frag");


	// Set up vertex data (and buffer(s)) and attribute pointers
	// use with Orthographic Projection


	

	// use with Perspective Projection
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
		 0.5f,  -0.5f, 0.5f,
      
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
	//glGenBuffers(1, &EBO);

	// Enlazar  Vertex Array Object
	glBindVertexArray(VAO);

	//2.- Copiamos nuestros arreglo de vertices en un buffer de vertices para que OpenGL lo use
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	

	//Posicion
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3* sizeof(GLfloat), (GLvoid *)0);
	glEnableVertexAttribArray(0);

	

	glBindBuffer(GL_ARRAY_BUFFER, 0);


	glBindVertexArray(0); // Unbind VAO (it's always a good thing to unbind any buffer/array to prevent strange bugs)

	
	glm::mat4 projection=glm::mat4(1);

	projection = glm::perspective(glm::radians(45.0f), (GLfloat)screenWidth / (GLfloat)screenHeight, 0.1f, 100.0f);//FOV, Radio de aspecto,znear,zfar
	glm::vec3 color= glm::vec3(0.0f, 0.0f, 1.0f);
	while (!glfwWindowShouldClose(window))
	{

		Inputs(window);
		// Check if any events have been activiated (key pressed, mouse moved etc.) and call corresponding response functions
		glfwPollEvents();

		// Render
		// Clear the colorbuffer
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);



		ourShader.Use();
		glm::mat4 model = glm::mat4(1);
		glm::mat4 view = glm::mat4(1);
		//se agregaron 2 elementos nuevos y ayudan con la informacion de la posicion de los elementos 
		glm::mat4 modelTemp = glm::mat4(1.0f); //Temp
		glm::mat4 modelTemp2 = glm::mat4(1.0f); //Temp
		glm::mat4 modelTemp3 = glm::mat4(1.0f); //Temp
		glm::mat4 modelTemp4 = glm::mat4(1.0f); //Temp
		glm::mat4 modelTemp5 = glm::mat4(1.0f); //Temp



		//View set up 
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

		//Model bicep
		model = glm::rotate(model, glm::radians(hombro), glm::vec3(0.0f, 0.0, 1.0f)); //hombro
		//s emueve la mitad por que se supone que estamos en la mitad del objeto 
		modelTemp = model = glm::translate(model, glm::vec3(1.5f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(3.0f, 1.0f, 1.0f));
		//con la variable color podemos modificar el color de cada cuadro
		color = glm::vec3(0.0f, 1.0f, 0.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);//A


		//Model Antebrazo
		//primero en donde va la rotacion
		model = glm::translate(modelTemp, glm::vec3(1.5f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(codo), glm::vec3(0.0f, 1.0f, 0.0f));
		//el centro de la figura para dibujarla 
		modelTemp = model = glm::translate(model, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(2.0f, 1.0f, 1.0f));
		color = glm::vec3(1.0f, 1.0f, 0.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);//B

		//model palma 
		model = glm::translate(modelTemp, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(palma), glm::vec3(1.0f, 0.0f, 0.0f));
		//el centro de la figura para dibujarla 
		modelTemp2 = modelTemp = model = glm::translate(model, glm::vec3(0.25f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.5f, 1.0f, 1.0f));
		color = glm::vec3(1.0f, 1.0f, 1.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);//C

		//model dedo1 A
		// Dedo 1 - Primera falange
		model = glm::translate(modelTemp2, glm::vec3(0.25f, 0.35f, 0.357f)); // Posicionar en la palma
		model = glm::rotate(model, glm::radians(dedo1), glm::vec3(0.0f, 0.0f, 1.0f)); // Rotar en su pivote
		model = glm::translate(model, glm::vec3(0.5f, 0.0f, 0.0f)); // Alinear al siguiente segmento
		modelTemp = model; // Guardar posición para el siguiente segmento
		model = glm::scale(model, glm::vec3(1.0f, 0.15f, 0.125f)); // Escalar
		color = glm::vec3(1.0f, 0.2f, 0.2f); // Rojo
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);
		// Dedo 1 - Segunda falange
		model = glm::translate(modelTemp, glm::vec3(0.5f, 0.0f, 0.0f)); // Posicionar en la primera falange
		model = glm::rotate(model, glm::radians(dedo1b), glm::vec3(0.0f, 0.0f, 1.0f)); // Rotar en su pivote
		model = glm::translate(model, glm::vec3(0.5f, 0.0f, 0.0f)); // Alinear al siguiente segmento
		modelTemp = model; // Guardar posición para la siguiente falange
		model = glm::scale(model, glm::vec3(1.0f, 0.15f, 0.125f)); // Escalar
		color = glm::vec3(1.0f, 0.3f, 0.3f); // Rojo
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);
		// Dedo 1 - Tercera falange
		model = glm::translate(modelTemp, glm::vec3(0.5f, 0.0f, 0.0f)); // Posicionar en la segunda falange
		model = glm::rotate(model, glm::radians(dedo1b), glm::vec3(0.0f, 0.0f, 1.0f)); // Rotar en su pivote
		model = glm::translate(model, glm::vec3(0.375f, 0.0f, 0.0f)); // Alinear al siguiente segmento
		model = glm::scale(model, glm::vec3(0.75f, 0.15f, 0.125f)); // Escalar
		color = glm::vec3(1.0f, 0.4f, 0.4f); // Rojo
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);


		//vamos a hacer la practica 5
		// Dedo 2 - Primera falange
		model = glm::translate(modelTemp2, glm::vec3(0.25f, 0.35f, 0.15f)); // Cambio en Y para separarlo
		model = glm::rotate(model, glm::radians(dedo1), glm::vec3(0.0f, 0.0f, 1.0f)); // Rotación propia del dedo
		model = glm::translate(model, glm::vec3(0.5f, 0.0f, 0.0f)); // Alinear la siguiente falange
		modelTemp = model; // Guardar posición base para la siguiente parte
		model = glm::scale(model, glm::vec3(1.0f, 0.15f, 0.125f));
		color = glm::vec3(0.2f, 1.0f, 0.2f); // Verde
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);
		// Dedo 2 - Segunda falange
		model = glm::translate(modelTemp, glm::vec3(0.5f, 0.0f, 0.0f)); // Partir de la falange anterior
		model = glm::rotate(model, glm::radians(dedo2b), glm::vec3(0.0f, 0.0f, 1.0f)); // Rotación de esta articulación
		model = glm::translate(model, glm::vec3(0.5f, 0.0f, 0.0f)); // Alinear la siguiente falange
		modelTemp = model; // Guardar posición para la tercera falange
		model = glm::scale(model, glm::vec3(1.0f, 0.15f, 0.125f));
		color = glm::vec3(0.4f, 1.0f, 0.4f); // Verde
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);
		// Dedo 2 - Tercera falange
		model = glm::translate(modelTemp, glm::vec3(0.5f, 0.0f, 0.0f)); // Partir de la segunda falange
		model = glm::rotate(model, glm::radians(dedo2b), glm::vec3(0.0f, 0.0f, 1.0f)); // Rotación de la última articulación
		model = glm::translate(model, glm::vec3(0.375f, 0.0f, 0.0f)); // Alinear al siguiente segmento
		model = glm::scale(model, glm::vec3(0.75f, 0.15f, 0.125f)); // Escalar
		color = glm::vec3(0.6f, 1.0f, 0.6f); // Verde
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);


		// Dedo 3 - Primera falange
		model = glm::translate(modelTemp2, glm::vec3(0.25f, 0.35f, -0.07f)); // Cambio en Y para separarlo
		model = glm::rotate(model, glm::radians(dedo1), glm::vec3(0.0f, 0.0f, 1.0f)); // Rotación propia del dedo
		model = glm::translate(model, glm::vec3(0.5f, 0.0f, 0.0f)); // Alinear la siguiente falange
		modelTemp = model; // Guardar posición base para la siguiente parte
		model = glm::scale(model, glm::vec3(1.0f, 0.15f, 0.125f));
		color = glm::vec3(0.2f, 0.2f, 1.0f); // Azul
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);
		// Dedo 3 - Segunda falange
		model = glm::translate(modelTemp, glm::vec3(0.5f, 0.0f, 0.0f)); // Partir de la falange anterior
		model = glm::rotate(model, glm::radians(dedo3b), glm::vec3(0.0f, 0.0f, 1.0f)); // Rotación de esta articulación
		model = glm::translate(model, glm::vec3(0.5f, 0.0f, 0.0f)); // Alinear la siguiente falange
		modelTemp = model; // Guardar posición para la tercera falange
		model = glm::scale(model, glm::vec3(1.0f, 0.15f, 0.125f));
		color = glm::vec3(0.4f, 0.4f, 1.0f); // Azul
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);
		// Dedo 3 - Tercera falange
		model = glm::translate(modelTemp, glm::vec3(0.5f, 0.0f, 0.0f)); // Partir de la segunda falange
		model = glm::rotate(model, glm::radians(dedo3b), glm::vec3(0.0f, 0.0f, 1.0f)); // Rotación de la última articulación
		model = glm::translate(model, glm::vec3(0.375f, 0.0f, 0.0f)); // Alinear al siguiente segmento
		model = glm::scale(model, glm::vec3(0.75f, 0.15f, 0.125f)); // Escalar
		color = glm::vec3(0.6f, 0.6f, 1.0f); // Azul
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);


		// Dedo 4 - Primera falange
		model = glm::translate(modelTemp2, glm::vec3(0.25f, 0.35f, -0.27f)); // Cambio en Y para separarlo
		model = glm::rotate(model, glm::radians(dedo1), glm::vec3(0.0f, 0.0f, 1.0f)); // Rotación propia del dedo
		model = glm::translate(model, glm::vec3(0.5f, 0.0f, 0.0f)); // Alinear la siguiente falange
		modelTemp = model; // Guardar posición base para la siguiente parte
		model = glm::scale(model, glm::vec3(1.0f, 0.15f, 0.125f));
		color = glm::vec3(1.0f, 1.0f, 0.2f); // Amarillo
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);
		// Dedo 4 - Segunda falange
		model = glm::translate(modelTemp, glm::vec3(0.5f, 0.0f, 0.0f)); // Partir de la falange anterior
		model = glm::rotate(model, glm::radians(dedo4b), glm::vec3(0.0f, 0.0f, 1.0f)); // Rotación de esta articulación
		model = glm::translate(model, glm::vec3(0.5f, 0.0f, 0.0f)); // Alinear la siguiente falange
		modelTemp = model; // Guardar posición para la tercera falange
		model = glm::scale(model, glm::vec3(1.0f, 0.15f, 0.125f));
		color = glm::vec3(1.0f, 1.0f, 0.4f); // Amarillo
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);
		// Dedo 4 - Tercera falange
		model = glm::translate(modelTemp, glm::vec3(0.5f, 0.0f, 0.0f)); // Partir de la segunda falange
		model = glm::rotate(model, glm::radians(dedo4b), glm::vec3(0.0f, 0.0f, 1.0f)); // Rotación de la última articulación
		model = glm::translate(model, glm::vec3(0.375f, 0.0f, 0.0f)); // Alinear al siguiente segmento
		model = glm::scale(model, glm::vec3(0.75f, 0.15f, 0.125f)); // Escalar
		color = glm::vec3(1.0f, 1.0f, 0.6f); // Amarillo
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);


		// Dedo 5 - Primera falange
		model = glm::translate(modelTemp2, glm::vec3(0.25f, -0.1f, 0.357f)); // Posicionar en la palma
		model = glm::rotate(model, glm::radians(dedo5b), glm::vec3(0.0f, 1.0f, 0.0f)); // Rotar en su pivote
		model = glm::translate(model, glm::vec3(0.375f, 0.0f, 0.0f)); // Alinear al siguiente segmento
		modelTemp = model; // Guardar posición para el siguiente segmento
		model = glm::scale(model, glm::vec3(0.75f, 0.15f, 0.125f)); // Escalar
		color = glm::vec3(1.0f, 0.2f, 1.0f); // Magenta
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);
		// Dedo 5 - Segunda falange
		model = glm::translate(modelTemp, glm::vec3(0.375f, 0.0f, 0.0f)); // Posicionar en la primera falange
		model = glm::rotate(model, glm::radians(dedo5b), glm::vec3(0.0f,1.0f, 0.0f)); // Rotar en su pivote
		model = glm::translate(model, glm::vec3(0.375f, 0.0f, 0.0f)); // Alinear al siguiente segmento
		modelTemp = model; // Guardar posición para la siguiente falange
		model = glm::scale(model, glm::vec3(0.75f, 0.15f, 0.125f)); // Escalar
		color = glm::vec3(1.0f, 0.4f, 1.0f); // Magenta
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);
		//prueba de subir al repositorio
		


		glBindVertexArray(0);

		
		// Swap the screen buffers
		glfwSwapBuffers(window);
	
	}
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);


	glfwTerminate();
	return EXIT_SUCCESS;
 }

 void Inputs(GLFWwindow *window) {
	 if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)  //GLFW_RELEASE
		 glfwSetWindowShouldClose(window, true);
	 if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		 movX += 0.08f;
	 if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		 movX -= 0.08f;
	 if (glfwGetKey(window,GLFW_KEY_UP) == GLFW_PRESS)
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
	 //se agrego el como se mueve el codo
	 if (glfwGetKey(window, GLFW_KEY_T) == GLFW_PRESS)
		 codo += 0.18f;
	 if (glfwGetKey(window, GLFW_KEY_G) == GLFW_PRESS)
		 codo -= 0.18f;
	 //se agrega el como se mueve la mano
	 if (glfwGetKey(window, GLFW_KEY_Y) == GLFW_PRESS)
		 palma += 0.18f;
	 if (glfwGetKey(window, GLFW_KEY_H) == GLFW_PRESS)
		 palma -= 0.18f;
	 //se agrega como se mueve el dedo1
	 if (glfwGetKey(window, GLFW_KEY_U) == GLFW_PRESS)
		 dedo1 += 0.18f;
	 if (glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS)
		 dedo1 -= 0.18f;
	 //se agrega como se mueve el dedo1b
	 if (glfwGetKey(window, GLFW_KEY_I) == GLFW_PRESS)
		 dedo1b += 0.18f;
	 if (glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS)
		 dedo1b -= 0.18f;
	 // se agrega la velocidad del dedo 2
	 if (glfwGetKey(window, GLFW_KEY_I) == GLFW_PRESS)
		 dedo2b += 0.19f;
	 if (glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS)
		 dedo2b -= 0.19f;
	 // se agrega la velocidad del dedo 3
	 if (glfwGetKey(window, GLFW_KEY_I) == GLFW_PRESS)
		 dedo3b += 0.20f;
	 if (glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS)
		 dedo3b -= 0.20f;
	 // se agrega la velocidad del dedo 4
	 if (glfwGetKey(window, GLFW_KEY_I) == GLFW_PRESS)
		 dedo4b += 0.21f;
	 if (glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS)
		 dedo4b -= 0.21f;
	 // se agrega la velocidad del dedo 5
	 if (glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS)
		 dedo5b += 0.19f;
	 if (glfwGetKey(window, GLFW_KEY_I) == GLFW_PRESS)
		 dedo5b -= 0.19f;
	
 }


