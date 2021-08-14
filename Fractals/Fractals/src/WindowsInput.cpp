#include "pch.h"
#include "WindowsInput.h"

#include "Engine.h"

Input* Input::s_instance = new WindowsInput();

bool WindowsInput::IsKeyPressedImpl(int keycode)
{
	auto window = static_cast<GLFWwindow*>(Engine::GetInstance().GetGLWindow());
	auto state = glfwGetKey(window, keycode);

	return state == GLFW_PRESS || state == GLFW_REPEAT;
}
bool WindowsInput::IsKeyReleasedImpl(int keycode)
{
	auto window = static_cast<GLFWwindow*>(Engine::GetInstance().GetGLWindow());
	auto state = glfwGetKey(window, keycode);

	return state == GLFW_RELEASE;
}
bool WindowsInput::IsMouseButtonPressedImpl(int button)
{
	auto window = static_cast<GLFWwindow*>(Engine::GetInstance().GetGLWindow());
	auto state = glfwGetMouseButton(window, button);

	return state == GLFW_PRESS;
}

std::pair<float, float> WindowsInput::GetMousePositionImpl()
{
	auto window = static_cast<GLFWwindow*>(Engine::GetInstance().GetGLWindow());
	double xPos, yPos;
	glfwGetCursorPos(window, &xPos, &yPos);

	return { xPos, yPos };
}
float WindowsInput::GetMouseXImpl()
{
	std::pair<float, float> pair = GetMousePositionImpl();
	return pair.first;
}
float WindowsInput::GetMouseYImpl()
{
	std::pair<float, float> pair = GetMousePositionImpl();
	return pair.second;
}