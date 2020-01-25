#ifndef ENGINE_H
#define ENGINE_H

#include <stdio.h>
#include <assert.h>
#include "window.h"
#include "graphics.h"
#include "event.h"
#ifdef __linux__
#include <SDL2/SDL.h>
#endif
#ifdef _WIN32
#include <SDL.h>
#endif

class Engine
{
  public:
    Engine(string name, int width, int height, bool gui);
    Engine(string name);
    ~Engine();
    bool Initialize();
    void Run();
    void Keyboard(eventType);
    unsigned int getDT();
    long long GetCurrentTimeMillis();
    void passArgs(string, float);
  
  private:
    // Window related variables
    window *m_window;
    string m_WINDOW_NAME;
    int m_WINDOW_WIDTH;
    int m_WINDOW_HEIGHT;
    bool m_FULLSCREEN;
    event m_event;

    Graphics *m_graphics;
    unsigned int m_DT;
    long long m_currentTimeMillis;
    bool m_running;
    string outputName;

    bool GUIenabled;
};

#endif // ENGINE_H
