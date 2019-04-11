#include "GUI.h"

//Initializes ImGui.Run once.
void InitializeImGui(GLFWwindow* window, int width, int height) {

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
	ResizeUI(width,height);
}

void ResizeUI(int width, int height) {
	ImGui::GetIO().DisplaySize = ImVec2(width, height);
}

//Shows GUI window.Run every frame.
void ShowGUIWindow(int vertexCount, int indiceCount, float deltaTime) {

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

