#ifndef INPUTMANAGER_H
#define INPUTMANAGER_H

//OpenGL general headers.
#include <GLFW/glfw3.h>

//Project file headers.
#include "Camera.h"


class InputManager
{
public:
	InputManager(int width,int height, void(*wireFrameFunction)(void));
	~InputManager();

	void ProcessMouseInput(double xPos, double yPos);
	void ProcessKeyboardInput(GLFWwindow *window,float deltaTime);

private:
	bool hasMouseMoved = false;
	int lastKeyState = GLFW_RELEASE;
	
	float lastX;
	float lastY;

	Camera* camera;

	void (*WireFrameFunctionCallback)();

};
#endif

