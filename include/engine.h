#ifndef ENGINE_H
#define ENGINE_H

#include <sys/time.h>
#include <stdio.h>
#include <assert.h>
#include "window.h"
#include "graphics.h"
#include "event.h"

class Engine
{
  public:
    Engine(string name, int width, int height);
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
