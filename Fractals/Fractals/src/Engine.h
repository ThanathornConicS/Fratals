#pragma once

#include <GL\glew.h>
#include <GLFW\glfw3.h>

#include "Utilities/Shader.h"
#include "WindowsInput.h"

struct Time 
{
	int num_frames{ 0 };
	float last_time{ 0.0f };

	void InitTime() 
	{
		last_time = glfwGetTime();
	}

	inline void CountFPS() 
	{
		double current_time = glfwGetTime();

		num_frames++;
		if (current_time - last_time >= 1.0)
		{
			//L_SYSTEM_INFO("{0} ms / frame", 1000.0 / num_frames);
			num_frames = 0;
			last_time += 1.0;
		}
	}
};

struct UniformLocation 
{
	int gl_zoomfactor = -1;
	int gl_centerX = -1;
	int gl_centerY = -1;

	inline void FindUniformLocation(uint32_t progID, const char* u_name, int& value) 
	{
		value = glGetUniformLocation(progID, u_name);
	}
};

class Engine
{
public:		//Functions
	inline static Engine& GetInstance() 
	{
		if (!s_instance)
			s_instance = new Engine();

		return *s_instance;
	}

	int Init();
	void Draw();
	void Update();
	void Clean();

	virtual ~Engine();

	int InitWindow(int w = 800, int h = 800, const char* name = "Fractals");
	void InitShader();

	void SetScreenName(const char* name);
	void SetScreenSize(int w, int h);
	void SetScreenPos(int x, int y);

	const char* GetScreenName() const;
	std::pair<int, int> GetScreenSize() const;
	std::pair<int, int> GetScreenPos() const;

	GLFWwindow* GetGLWindow() const;

	inline uint32_t GetShaderProgID() const 
	{
		return m_shader.program_ID;
	}

public:		//Variables
	UniformLocation m_uniformL;

protected:	
	Engine();

private:	//Variables
	static Engine* s_instance;
	Time m_time;
	GLFWwindow* m_window = nullptr;

	Shader m_shader;
	WindowsInput m_input;

	const char* m_name;
	int m_width, m_height;
	int m_xpos, m_ypos;

	uint32_t VAO, VBO, EBO;

	float vertices[12] =
	{
		//    x      y      z   
			-1.0f, -1.0f, -0.0f,
			 1.0f,  1.0f, -0.0f,
			-1.0f,  1.0f, -0.0f,
			 1.0f, -1.0f, -0.0f
	};
	uint32_t indices[6] =
	{
		//  2---,1
		//  | .' |
		//  0'---3
			0, 1, 2,
			0, 3, 1
	};

};

