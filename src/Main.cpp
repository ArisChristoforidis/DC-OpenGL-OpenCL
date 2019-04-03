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

//Project classes headers.
#include "Shader.h"
#include "Camera.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "Renderer.h"
#include "NoiseUtils.h"
#include "DualContour.h"

//#include <CL/cl.hpp>

//Function prototypes.
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void MouseCallback(GLFWwindow* window, double xPos, double yPos);
void ProcessKeyboardInput(GLFWwindow *window);
void ToggleWireframeMode();
void CalculateDeltaTime();
void InitializeImGui(GLFWwindow* window);
void ShowGUIWindow();
float CircleFunction(float x, float y, float z);
float TorusFunction(float x, float y, float z);
float NoiseFunction(float x, float y, float z);
float HelixFunction(float x, float y, float z);
//Window size.
#define WINDOW_WIDTH 1920
#define WINDOW_HEIGHT 1080

#define SIZE 128

bool isWireframeEnabled = false;

int lastKeyState = GLFW_RELEASE;
float deltaTime = 0.0f;
double lastFrame = 0.0f;
bool hasMouseMoved = false;
float lastX = WINDOW_WIDTH / 2.0f;
float lastY = WINDOW_HEIGHT / 2.0f;
Camera camera;

//GUI window variables.
unsigned int indiceCount;
unsigned int vertexCount;


int main() {
	
	//Initialize GLFW.
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	//Create a window.
	GLFWwindow* window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Procedurally Generated Terrain", NULL, NULL);
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

	//Camera.
	glfwSetCursorPosCallback(window, MouseCallback);
	//Lock and hide cursor.
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);


	//Coordinates must be normalized(Between [-1,1]).
	/*
	float vertices[] = {
		// positions        
		 1.0f,  1.0f, 1.0f,
		 1.0f, -1.0f, 1.0f,
		-1.0f, -1.0f, 1.0f,
		-1.0f,  1.0f, 1.0f,
		 1.0f,  1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f
	};

	unsigned int indices[] = {
		0,1,3,
		1,2,3,
		3,2,7,
		2,6,7,
		4,5,7,
		5,6,7,
		0,1,4,
		1,5,4,
		4,0,7,
		0,3,7,
		5,1,6,
		1,2,6
	};
	*/
	

	/*
	unsigned int indices[] = {
		3,1,2,
		1,0,2
	};*/


	DualContour dualContour;
	Mesh mesh = dualContour.ExtractSurface(TorusFunction);

	VertexBuffer vertexBuffer(&dualContour.vertArray[0], dualContour.vertArray.size() * sizeof(float));

	VertexBufferLayout bufferLayout;
	bufferLayout.Push<float>(3, true);

	VertexArray vertexArray;
	vertexArray.AddBuffer(vertexBuffer, bufferLayout);

	IndexBuffer indexBuffer(&dualContour.indices[0], dualContour.indices.size());
	indiceCount = dualContour.indices.size()/3;
	vertexCount = dualContour.vertArray.size()/3;


	Shader customShader("src/shaders/vshader.vs", "src/shaders/fshader.fs");

	//Going 3D.
	glm::mat4 model = glm::mat4(1.0f);
	//model = glm::scale(model, glm::vec3(10.0f));

	//First parameter is essentially the FOV.
	glm::mat4 projection = glm::mat4(1.0f);
	projection = glm::perspective(glm::radians(60.0f), (float)WINDOW_WIDTH / (float)WINDOW_HEIGHT, 0.01f, 1000.0f);

	glm::mat4 transform = glm::mat4(1.0f);

	glEnable(GL_DEPTH_TEST);


	Renderer renderer;

	InitializeImGui(window);

	while (!glfwWindowShouldClose(window)) {
		CalculateDeltaTime();
		
		//Input.
		ProcessKeyboardInput(window);


		customShader.setMat4("model", model);
		glm::mat4 view = camera.GetViewMatrix();
		customShader.setMat4("view", view);
		customShader.setMat4("projection", projection);
		//Render a mesh.
		renderer.Draw(vertexArray, indexBuffer, customShader);
		//Show GUI.
		ShowGUIWindow();

		
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

//Handles mouse movement.
void MouseCallback(GLFWwindow * window, double xPos, double yPos) {

	//Used for the first mouse movement,when lastX and lastY have no values.
	if (!hasMouseMoved) {
		hasMouseMoved = true;
		lastX = xPos;
		lastY = yPos;
	}

	float xOffset = (float)(xPos - lastX);
	float yOffset = (float)(lastY - yPos);

	lastX = xPos;
	lastY = yPos;

	camera.ProcessMouseLook(xOffset, yOffset);
}

//Handles keyboard input.
void ProcessKeyboardInput(GLFWwindow * window) {

	//If [ESC] is pressed, stop execution.
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
	}

	//If [TAB] is pressed,toggle wireframe mode.
	if (glfwGetKey(window, GLFW_KEY_TAB) == GLFW_PRESS && lastKeyState == GLFW_RELEASE) {
		lastKeyState = GLFW_PRESS;
	} else if (glfwGetKey(window, GLFW_KEY_TAB) == GLFW_RELEASE && lastKeyState == GLFW_PRESS) {
		ToggleWireframeMode();
		lastKeyState = GLFW_RELEASE;
	}

	//If any of the [W,A,S,D] keys are pressed,move the camera accordingly.
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) camera.ProcessKeyboardMovement(FORWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) camera.ProcessKeyboardMovement(BACKWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) camera.ProcessKeyboardMovement(LEFT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) camera.ProcessKeyboardMovement(RIGHT, deltaTime);
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
void CalculateDeltaTime() {
	double currentFrame = glfwGetTime();
	deltaTime = currentFrame - lastFrame;
	lastFrame = currentFrame;
}

//Initializes ImGui.Run once.
void InitializeImGui(GLFWwindow* window) {

	//Gui for the window.
	ImGui::CreateContext();
	ImGui::StyleColorsDark();
	ImGuiIO &io = ImGui::GetIO();
	io.BackendFlags |= ImGuiBackendFlags_HasMouseCursors;
	io.BackendFlags |= ImGuiBackendFlags_HasSetMousePos;

	ImGui_ImplGlfw_InitForOpenGL(window, true);

	//Setup keymap.
	io.KeyMap[ImGuiKey_Tab] = GLFW_KEY_TAB;
	io.KeyMap[ImGuiKey_LeftArrow] = GLFW_KEY_LEFT;
	io.KeyMap[ImGuiKey_RightArrow] = GLFW_KEY_RIGHT;
	io.KeyMap[ImGuiKey_UpArrow] = GLFW_KEY_UP;
	io.KeyMap[ImGuiKey_DownArrow] = GLFW_KEY_DOWN;
	io.KeyMap[ImGuiKey_PageUp] = GLFW_KEY_PAGE_UP;
	io.KeyMap[ImGuiKey_PageDown] = GLFW_KEY_PAGE_DOWN;
	io.KeyMap[ImGuiKey_Home] = GLFW_KEY_HOME;
	io.KeyMap[ImGuiKey_End] = GLFW_KEY_END;
	io.KeyMap[ImGuiKey_Insert] = GLFW_KEY_INSERT;
	io.KeyMap[ImGuiKey_Delete] = GLFW_KEY_DELETE;
	io.KeyMap[ImGuiKey_Backspace] = GLFW_KEY_BACKSPACE;
	io.KeyMap[ImGuiKey_Space] = GLFW_KEY_SPACE;
	io.KeyMap[ImGuiKey_Enter] = GLFW_KEY_ENTER;
	io.KeyMap[ImGuiKey_Escape] = GLFW_KEY_ESCAPE;
	io.KeyMap[ImGuiKey_A] = GLFW_KEY_A;
	io.KeyMap[ImGuiKey_C] = GLFW_KEY_C;
	io.KeyMap[ImGuiKey_V] = GLFW_KEY_V;
	io.KeyMap[ImGuiKey_X] = GLFW_KEY_X;
	io.KeyMap[ImGuiKey_Y] = GLFW_KEY_Y;
	io.KeyMap[ImGuiKey_Z] = GLFW_KEY_Z;

	ImGui_ImplOpenGL3_Init();
	io.DisplaySize = ImVec2(WINDOW_WIDTH, WINDOW_HEIGHT);
}

//Shows GUI window.Run every frame.
void ShowGUIWindow() {

	//Create a new frame for the GUI.
	ImGui_ImplOpenGL3_NewFrame();
	ImGui::NewFrame();

	//Set GUI window size.
	ImGui::SetNextWindowSize(ImVec2(250, 100));

	//GUI layout.
	bool show = true;
	ImGui::Begin("Stats", &show); {
		ImGui::Text("Framerate: %.3f (%.4f sec)", 1 / deltaTime, deltaTime);
		ImGui::Text("Vertices: %d", vertexCount);
		ImGui::Text("Triangles: %d", indiceCount);

	}
	ImGui::End();

	//Render GUI.
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

float CircleFunction(float x, float y, float z) {
	return 32.0f - sqrt(x * x + y * y + z * z);
}

float TorusFunction(float x,float y,float z){
	const float c = 50.0f;
	const float a = 6.0f;
	return (c - std::sqrt(x*x + y * y))*(c - std::sqrt(x*x + y * y)) + z * z - a*a;
}

float HelixFunction(float x,float y,float z){
	const float r = 20.0f;
	const float a = 3.0f;
	return std::pow(x - x * (z / a), 2) + std::pow(y - y * (z / a), 2) - r * r;
}

float NoiseFunction(float x,float y,float z){
	float noiseValue =  NoiseUtils::SimplexNoise(x, y, z, 1.0f);

	if (noiseValue > 0) return 1.0f;
	return -1.0f;
}