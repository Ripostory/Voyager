#ifndef WINDOW_H
#define WINDOW_H

#ifdef __linux__
#include <SDL2/SDL.h>
#endif

#ifdef _WIN32
#include <SDL.h>
#endif

#include <string>

using namespace std;

class window
{
  public:
    window(bool);
    ~window();
    bool Initialize(const string &name, int* width, int* height);
    void Swap();

  private:
    SDL_Window* gWindow;
    SDL_GLContext gContext;
    bool GUIenabled;
};

#endif /* WINDOW_H */
