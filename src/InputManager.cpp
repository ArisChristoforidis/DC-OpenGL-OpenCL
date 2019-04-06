#include "InputManager.h"



InputManager::InputManager(int width,int height,void(*wireFrameFunction)(void)) {
		
	camera = Camera::getInstance();

	lastX = width / 2.0f;
	lastY = height / 2.0f;
	WireFrameFunctionCallback = wireFrameFunction;
}


InputManager::~InputManager() {
}

//Handles mouse movement.
void InputManager::ProcessMouseInput(double xPos, double yPos) {

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

	camera->ProcessMouseLook(xOffset, yOffset);
}

//Handles keyboard input.
void InputManager::ProcessKeyboardInput(GLFWwindow * window,float deltaTime) {

	//If [ESC] is pressed, stop execution.
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
	}

	//If [TAB] is pressed,toggle wireframe mode.
	if (glfwGetKey(window, GLFW_KEY_TAB) == GLFW_PRESS && lastKeyState == GLFW_RELEASE) {
		lastKeyState = GLFW_PRESS;
	}
	else if (glfwGetKey(window, GLFW_KEY_TAB) == GLFW_RELEASE && lastKeyState == GLFW_PRESS) {
		WireFrameFunctionCallback();
		lastKeyState = GLFW_RELEASE;
	}

	//If any of the [W,A,S,D] keys are pressed,move the camera accordingly.
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) camera->ProcessKeyboardMovement(FORWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) camera->ProcessKeyboardMovement(BACKWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) camera->ProcessKeyboardMovement(LEFT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) camera->ProcessKeyboardMovement(RIGHT, deltaTime);
}