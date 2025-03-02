//Actividad Modelado jerárquico                             	Mendoza Espinosa Ricardo
//Fecha de entrega : 01 - 03 - 2025    	 	319018370

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

//For Keyboard´para editar la vista
float	movX = 0.0f,
movY = 0.0f,
movZ = -5.0f,
rot = 0.0f;

//For model esta es para manipular el modelo,esto permite generar la articulzacion
float	hombro = 0.0f,
codo = 0.0f,
muneca=0.0f,
dedo1=0.0f,
dedo2 = 0.0f;



int main() {
	glfwInit();
	//Verificación de compatibilidad 
	// Set all the required options for GLFW
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	GLFWwindow *window = glfwCreateWindow(WIDTH, HEIGHT, "Modelado jerarquico-Ricardo Mendoza", nullptr, nullptr);

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


	

	// use with Perspective Projection.Componentes en : X Y Z
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

	//Esto genera la matriz de proyeccion en perspectiva
	projection = glm::perspective(glm::radians(45.0f), (GLfloat)screenWidth / (GLfloat)screenHeight, 0.1f, 100.0f);//FOV, Radio de aspecto,znear,zfar
	glm::vec3 color= glm::vec3(0.0f, 0.0f, 1.0f);
	while (!glfwWindowShouldClose(window))
	{
		//Funciones para el control de teclado

		Inputs(window);
		// Check if any events have been activiated (key pressed, mouse moved etc.) and call corresponding response functions
		glfwPollEvents();

		// Render
		// Clear the colorbuffer
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT| GL_DEPTH_BUFFER_BIT);


		//Aqui se comienza a dibujar 
		ourShader.Use();
		glm::mat4 model=glm::mat4(1);//Funciones para manipular el modelo
		glm::mat4 view=glm::mat4(1);//Funciones para manipular la vista
		//Elementos nuevos para ayudar a controlar la informacion de la posicion de cada uno de los elementos
		glm::mat4 modelTemp = glm::mat4(1.0f); //Temp
		glm::mat4 modelTemp2 = glm::mat4(1.0f); //Temp



		//View set up 
		view = glm::translate(view, glm::vec3(movX,movY, movZ));//Se incializan los valores  para la vista
		view = glm::rotate(view, glm::radians(rot), glm::vec3(0.0f, 1.0f, 0.0f));
		
		GLint modelLoc = glGetUniformLocation(ourShader.Program, "model");
		GLint viewLoc = glGetUniformLocation(ourShader.Program, "view");
		GLint projecLoc = glGetUniformLocation(ourShader.Program, "projection");
		GLint uniformColor = ourShader.uniformColor;
		//SAE MANDAN A LA MATRIZ
		glUniformMatrix4fv(projecLoc, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
	

		glBindVertexArray(VAO);
		
		//Model Bicep
		model = glm::rotate(model, glm::radians(hombro), glm::vec3(0.0f, 0.0, 1.0f)); //Aplicamos la rotacion para la variable "hombro" en la componente Z
		modelTemp = model = glm::translate(model, glm::vec3(1.5f, 0.0f, 0.0f));//Se guarada lo de la matriz "model" en la "matriz Temporal",para una matriz auxiliar,GENERA LA SENSACION DE MOVIMIENTO,el pivote se situa a 1.5 de 3.0 distancia total en X
		//Se recorre 1.5 unidades para poder generar la articulacion del hombro
		model = glm::scale(model, glm::vec3(3.0f, 1.0f, 1.0f));//Se aplica una escala a la matriz model en X Y Z, en donde se tiene un elemento de 3 unidades
		color = glm::vec3(0.0f, 1.0f, 0.0f); //Variable para el color,en cada caja que tenemos 
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);//La informacion se envia a los SHADERS A
		
		//Modelo Antebrazo
		model = glm::translate(modelTemp, glm::vec3(1.5f, 0.0f, 0.0f));//informacion de la posicion del nuevo elemento,nos recorrimos otras 1.5 unidades para llegar a la articulacion
		model = glm::rotate(model, glm::radians(codo), glm::vec3(0.0f, 1.0f, 0.0f));
		modelTemp = model = glm::translate(model, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(2.0f, 1.0f, 1.0f));//Se aplica una escala a la matriz model en X Y Z
		color = glm::vec3(1.0f, 0.0f, 0.0f); //Variable para el color,en cada caja que tenemos 
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);//La informacion se envia a los SHADERS B



		//Modelo Palma

		model = glm::translate(modelTemp, glm::vec3(1.0f, 0.0f, 0.0f));//Nos basamos de la matriz temporal,debido a que guardamos 
		//el valor de la matriz modelo antes de aplicar la escala y de esta manera regreso al valor original de mi matriz justo antes de aplicar la escala 
		//esto para no volver a tomar los datos desde el origen inicial
		model = glm::rotate(model, glm::radians(muneca), glm::vec3(1.0f, 0.0f, 0.0f));
		modelTemp2 = modelTemp = model = glm::translate(model, glm::vec3(0.25f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.5f, 1.0f, 1.0f));//Se aplica una escala a la matriz model en X Y Z
		color = glm::vec3(1.0f, 1.0f, 1.0f); //Variable para el color,en cada caja que tenemos 
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);//La informacion se envia a los SHADERS C


		//Modelo Dedo1

		model = glm::translate(modelTemp, glm::vec3(0.25f, 0.35f, 0.375f));//Nos basamos de la matriz temporal,debido a que guardamos 
		//el valor de la matriz modelo antes de aplicar la escala y de esta manera regreso al valor original de mi matriz justo antes de aplicar la escala 
		//esto para no volver a tomar los datos desde el origen inicial
		model = glm::rotate(model, glm::radians(dedo1), glm::vec3(0.0f, 0.0f, 1.0f));
		modelTemp = model = glm::translate(model, glm::vec3(0.5f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.0f, 0.3f, 0.25f));//Se aplica una escala a la matriz model en X Y Z
		color = glm::vec3(0.0f, 1.0f, 1.0f); //Variable para el color,en cada caja que tenemos 
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);//La informacion se envia a los SHADERS D




		//Modelo Dedo2

		model = glm::translate(modelTemp, glm::vec3(0.5f, 0.0f, 0.0f));//Nos basamos de la matriz temporal,debido a que guardamos 
		//el valor de la matriz modelo antes de aplicar la escala y de esta manera regreso al valor original de mi matriz justo antes de aplicar la escala 
		//esto para no volver a tomar los datos desde el origen inicial
		model = glm::rotate(model, glm::radians(dedo2), glm::vec3(0.0f, 0.0f, 1.0f));
		modelTemp = model = glm::translate(model, glm::vec3(0.5f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.0f, 0.3f, 0.25f));//Se aplica una escala a la matriz model en X Y Z
		color = glm::vec3(1.0f, 0.0f, 1.0f); //Variable para el color,en cada caja que tenemos 
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);//La informacion se envia a los SHADERS E









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
	 if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		 movX += 0.002f;
	 if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		 movX -= 0.002f;
	 if (glfwGetKey(window,GLFW_KEY_UP) == GLFW_PRESS)
		 movY += 0.002f;
	 if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
		 movY -= 0.002f;
	 if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		 movZ -= 0.002f;
	 if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		 movZ += 0.002f;
	 if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
		 rot += 0.09f;
	 if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
		 rot -= 0.07f;
	 if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS)
		 hombro += 0.07f;
	 if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS)
		 hombro -= 0.07f;
	 //nuevas variables 
	 if (glfwGetKey(window, GLFW_KEY_G) == GLFW_PRESS)
		 codo += 0.07f;
	 if (glfwGetKey(window, GLFW_KEY_T) == GLFW_PRESS)
		 codo -= 0.07f;
	 if (glfwGetKey(window, GLFW_KEY_H) == GLFW_PRESS)
		 muneca += 0.07f;
	 if (glfwGetKey(window, GLFW_KEY_Y) == GLFW_PRESS)
		 muneca -= 0.07f;
	 if (glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS)
		 dedo1 += 0.07f;
	 if (glfwGetKey(window, GLFW_KEY_U) == GLFW_PRESS)
		 dedo1 -= 0.07f;
	 if (glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS)
		 dedo2 += 0.07f;
	 if (glfwGetKey(window, GLFW_KEY_I) == GLFW_PRESS)
		 dedo2 -= 0.07f;

 }


