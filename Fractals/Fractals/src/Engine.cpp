#include "pch.h"
#include "Engine.h"
#include "GLM/glm.hpp"

double zoom_factor = 0.1;
float center_x = 0;
float center_y = 0;

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}
void mouse_scrollwheel_callback(GLFWwindow* window, double xOffset, double yOffset) 
{
    //L_TRACE("X: {0}     Y: {1}", xOffset, yOffset);
    zoom_factor = zoom_factor - ((yOffset / 100.0f));
    glUniform1f(Engine::GetInstance().m_uniformL.gl_zoomfactor, zoom_factor);
    L_TRACE("Zoom: {0}", zoom_factor);
}

Engine* Engine::s_instance = nullptr;

int Engine::Init()
{
    L_SYSTEM_INFO("Initializing Engine");

    if (InitWindow() == -1)
        return -1;

    InitShader();
    if (m_shader.IsSuccess() == false)
        return -1;

    m_time.InitTime();
}

void Engine::Draw()
{
    m_time.CountFPS();

    m_shader.use_shader();

    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

void Engine::Update()
{
    if (m_input.IsKeyPressed(GLFW_KEY_A)) 
    {
        center_x -= 0.01f * abs(1 + zoom_factor);
        glUniform1f(Engine::GetInstance().m_uniformL.gl_centerX, center_x);
        //L_TRACE("lol");
    }
    else if (m_input.IsKeyPressed(GLFW_KEY_D))  
    {
        center_x += 0.01f * abs(1 + zoom_factor);
        glUniform1f(Engine::GetInstance().m_uniformL.gl_centerX, center_x);
    }
    else if (m_input.IsKeyPressed(GLFW_KEY_W))
    {
        center_y += 0.01f * abs(1 + zoom_factor);
        glUniform1f(Engine::GetInstance().m_uniformL.gl_centerY, center_y);
    }
    else if (m_input.IsKeyPressed(GLFW_KEY_S))
    {
        center_y -= 0.01f * abs(1 + zoom_factor);
        glUniform1f(Engine::GetInstance().m_uniformL.gl_centerY, center_y);
    }
    else if (m_input.IsKeyPressed(GLFW_KEY_R))
    {
        center_x = 0; center_y = 0;
        glUniform1f(Engine::GetInstance().m_uniformL.gl_centerX, center_x);
        glUniform1f(Engine::GetInstance().m_uniformL.gl_centerY, center_y);
    }

    glfwSwapBuffers(m_window);
    glfwPollEvents();
}

void Engine::Clean()
{
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);

    glfwTerminate();
}

Engine::Engine()
{}
Engine::~Engine()
{}

int Engine::InitWindow(int w, int h, const char* name)
{
    L_SYSTEM_INFO("Initializing Window");

    SetScreenSize(w, h);
    SetScreenPos(100, 100);
    SetScreenName(name);

    glfwInit();
    glfwWindowHint(GLFW_SAMPLES, 4); // 4x antialiasing
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    m_window = glfwCreateWindow(m_width, m_height, m_name, NULL, NULL);

    if (m_window == nullptr)
    {
        L_SYSTEM_ERROR("Error: Failed to open GLFW window, check available version with GLView");
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(m_window);

    glewExperimental = GL_TRUE;
    GLenum err = glewInit();
    if (err != GLEW_OK) 
    {
        L_SYSTEM_ERROR("Error: {0}", glewGetErrorString(err));
        return -1;
    }
        
    glViewport(0, 0, m_width, m_height);

    glfwSetFramebufferSizeCallback(m_window, framebuffer_size_callback);
    glfwSetWindowTitle(m_window, m_name);
    glfwSetInputMode(m_window, GLFW_STICKY_KEYS, GL_TRUE);
    glfwSetScrollCallback(m_window, mouse_scrollwheel_callback);
    glfwSetWindowPos(m_window, m_xpos, m_ypos);

    glEnable(GL_DEPTH_TEST);
}

void Engine::InitShader()
{
    L_SYSTEM_INFO("Initializing Shader");

    m_shader.Create_Program("src/Utilities/Shader_vert.vert", "src/Utilities/Shader_frag.frag");

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    m_uniformL.FindUniformLocation(m_shader.program_ID, "zoom", m_uniformL.gl_zoomfactor);
    m_uniformL.FindUniformLocation(m_shader.program_ID, "center_x", m_uniformL.gl_centerX);
    m_uniformL.FindUniformLocation(m_shader.program_ID, "center_y", m_uniformL.gl_centerY);

    glUniform1f(Engine::GetInstance().m_uniformL.gl_zoomfactor, zoom_factor);
    glUniform1f(Engine::GetInstance().m_uniformL.gl_centerX, center_x);
    glUniform1f(Engine::GetInstance().m_uniformL.gl_centerY, center_y);
}

void Engine::SetScreenName(const char* name)
{
    m_name = name;
}
void Engine::SetScreenSize(int w, int h)
{
    m_width = w; m_height = h;
}
void Engine::SetScreenPos(int x, int y)
{
    m_xpos = x; m_ypos = y;
}

const char* Engine::GetScreenName() const
{
	return m_name;
}
std::pair<int, int> Engine::GetScreenSize() const
{
	return std::pair<int, int>(m_width, m_height);
}
std::pair<int, int> Engine::GetScreenPos() const
{
	return std::pair<int, int>(m_xpos, m_ypos);
}

GLFWwindow* Engine::GetGLWindow() const
{
	return m_window;
}
