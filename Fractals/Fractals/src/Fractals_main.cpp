#include "pch.h"
#include "Engine.h"

#include "Log.h"

int main(int argc, char* argv[])
{
    HANDLE mutex = CreateMutex(NULL, TRUE, L"Fractals");
    if (GetLastError() == ERROR_ALREADY_EXISTS)
    {
        MessageBox(NULL, L"Multiple Instances of Application", NULL, MB_ICONERROR | MB_OK);
        return 0;
    }

    _CrtMemState sOld;
    _CrtMemState sNew;
    _CrtMemState sDiff;
    _CrtMemCheckpoint(&sOld); //take a snapchot

    bool isInitialized = true;

    Engine* engine = &Engine::GetInstance();

    Log::Log::Init();
    L_SYSTEM_INFO("Initializing Log");

    if (engine->Init() == -1) 
    {
        L_SYSTEM_ERROR("Error has occured when initialize the engine");
        isInitialized = false;
    }

    glfwSetTime(0);

    do
    {
        glClearColor(0.2f, 0.0f, 0.2f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        engine->Draw();
        engine->Update();

    } while (glfwGetKey(engine->GetGLWindow(), GLFW_KEY_ESCAPE) != GLFW_PRESS && glfwWindowShouldClose(engine->GetGLWindow()) == 0 && isInitialized);

    delete engine;

    _CrtMemCheckpoint(&sNew); //take a snapshot 
    if (_CrtMemDifference(&sDiff, &sOld, &sNew)) // if there is a difference
    {
        OutputDebugString(L"-----------_CrtMemDumpStatistics ---------");
        _CrtMemDumpStatistics(&sDiff);
        OutputDebugString(L"-----------_CrtMemDumpAllObjectsSince ---------");
        _CrtMemDumpAllObjectsSince(&sOld);
        OutputDebugString(L"-----------_CrtDumpMemoryLeaks ---------");
        _CrtDumpMemoryLeaks();
    }

    return 0;
}
