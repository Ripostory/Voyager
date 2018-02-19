#ifndef WINDOW_H
#define WINDOW_H

#include <SDL2/SDL.h>
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
