//OpenGL general headers.
#include <glad/glad.h>
#include <GLFW/glfw3.h>

//GL math headers.
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

//ImGui headers.
#include "imgui/imgui.h"
#include "imgui/imgui_impl_opengl3.h"
#include "imgui/imgui_impl_glfw.h"

//C++ general headers.
#include <iostream>
#include <stdlib.h>
#include <time.h>
#include <thread>

//Project file headers.
#include "Shader.h"
#include "Camera.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "Renderer.h"
#include "NoiseUtils.h"
#include "DualContour.h"
#include "Mesh.h"
#include "InputManager.h"
#include "WorkerFunctions.h"
#include "GUI.h"

int main();

//Function prototypes.
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void MouseInputWrapper(GLFWwindow* window, double xPos, double yPos);
void ToggleWireframeMode();
void UpdateShaderVariables(Shader* shader,glm::mat4 model,glm::mat4 view,glm::mat4 projection);
double CalculateDeltaTime(double* pFrame);

//Window size.
#define WINDOW_WIDTH 1920
#define WINDOW_HEIGHT 1080

bool isWireframeEnabled = false;



InputManager* inputManager;
int main() {
	double previousFrame = 0.0f;
	std::vector<Mesh> objects;
	unsigned int objectCount = 0;

	//Initialize GLFW.
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	//Create a window.
	GLFWwindow* window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Isosurface Extraction in OpenGL", NULL, NULL);
	if (window == NULL) {
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);

	//Initialize GLAD.
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glEnable(GL_DEPTH_TEST);

	inputManager = new InputManager(WINDOW_WIDTH, WINDOW_HEIGHT, ToggleWireframeMode);

	//Mouse movement callback to rotate the camera.
	glfwSetCursorPosCallback(window, MouseInputWrapper);
	//Lock and hide cursor.
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);


	Work(objects,objectCount);
	//std::thread(&Work, objects, objectCount);

	Shader customShader("src/shaders/vshader.vs", "src/shaders/fshader.fs");

	//Going 3D.
	glm::mat4 model = glm::mat4(1.0f);

	//First parameter is essentially the FOV.
	glm::mat4 projection = glm::mat4(1.0f);
	projection = glm::perspective(glm::radians(60.0f), (float)WINDOW_WIDTH / (float)WINDOW_HEIGHT, 0.01f, 1000.0f);

	glm::mat4 transform = glm::mat4(1.0f);

	Camera* camera = Camera::getInstance();

	Renderer renderer;

	InitializeImGui(window,WINDOW_WIDTH,WINDOW_HEIGHT);


	while (!glfwWindowShouldClose(window)) {
		//Calculate deltaTime.
		double deltaTime = CalculateDeltaTime(&previousFrame);
		
		//Read keyboard input.
		inputManager->ProcessKeyboardInput(window,deltaTime);

		//Update shader depending on the camera position.
		UpdateShaderVariables(&customShader, model, camera->GetViewMatrix(), projection);

		unsigned int totalVertexCount = 0;
		unsigned int totalIndexCount  = 0;

		//Render meshes.
		for (int i = 0; i < objectCount;i++) {
			renderer.Draw(objects[i].GetVertexArray(), objects[i].GetIndexBuffer(), customShader);
			
			//Count total vertices and indices on the side.
			totalVertexCount += objects[i].GetVertexCount();
			totalIndexCount += objects[i].GetIndexCount();
		}

		//Show GUI.
		ShowGUIWindow(totalVertexCount, totalIndexCount,deltaTime);

		
		//Swap buffers to prevent flickering.
		glfwSwapBuffers(window);
		//Poll events.
		glfwPollEvents();
	}

	ImGui_ImplOpenGL3_Shutdown();
	glfwTerminate();
	return 0;
}


//Changes the viewport when the user resizes the window.
void framebuffer_size_callback(GLFWwindow * window, int width, int height) {
	glViewport(0, 0, width, height);
}


//Toggle wireframe mode(only shows edges of a mesh).
void ToggleWireframeMode() {
	isWireframeEnabled = !isWireframeEnabled;

	if (isWireframeEnabled) {
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	} else {
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}
}

//Calculates the time it took to process the last frame.
double CalculateDeltaTime(double* pFrame) {
	double currentFrame = glfwGetTime();
	double deltaTime = currentFrame - *pFrame;
	*pFrame = currentFrame;
	return deltaTime;
}

void MouseInputWrapper(GLFWwindow* window, double xPos, double yPos) {
	/*I cannot pass member functions as a callback to glfwSetCursorPosCallback
	so I have to call ProcessMouseInput this way.*/
	inputManager->ProcessMouseInput(xPos, yPos);
}

void UpdateShaderVariables(Shader* shader,glm::mat4 model, glm::mat4 view, glm::mat4 projection) {
	shader->setMat4("model", model);
	shader->setMat4("view", view);
	shader->setMat4("projection", projection);
}



