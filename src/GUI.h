#ifndef GUI_H
#define GUI_H

#include <GLFW/glfw3.h>
#include "imgui/imgui.h"
#include "imgui/imgui_impl_opengl3.h"
#include "imgui/imgui_impl_glfw.h"

#include "Camera.h"

void InitializeImGui(GLFWwindow* window, int width, int height);
void ShowGUIWindow(int vertexCount, int indiceCount, float deltaTime);
void ResizeUI(int width, int height);

#endif
