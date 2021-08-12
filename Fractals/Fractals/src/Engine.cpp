#include "pch.h"
#include "Engine.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

Engine* Engine::s_instance = nullptr;

int Engine::Init()
{
    if (InitWindow() == -1)
        return -1;

    InitShader();

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
        L_SYSTEM_ERROR("Error: {0}", glewGetErrorString(err));

    glViewport(0, 0, m_width, m_height);
    glfwSetFramebufferSizeCallback(m_window, framebuffer_size_callback);
    glfwSetWindowTitle(m_window, m_name);
    glfwSetInputMode(m_window, GLFW_STICKY_KEYS, GL_TRUE);
    glfwSetWindowPos(m_window, m_xpos, m_ypos);

    glEnable(GL_DEPTH_TEST);
}

void Engine::InitShader()
{
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
