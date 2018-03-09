
#include "engine.h"

Engine::Engine(string name, int width, int height, bool gui)
{
  m_WINDOW_NAME = name;
  m_WINDOW_WIDTH = width;
  m_WINDOW_HEIGHT = height;
  m_FULLSCREEN = false;
  GUIenabled = gui;
}

Engine::Engine(string name)
{
  m_WINDOW_NAME = name;
  m_WINDOW_HEIGHT = 0;
  m_WINDOW_WIDTH = 0;
  m_FULLSCREEN = true;
  GUIenabled = false;
}

Engine::~Engine()
{
  delete m_window;
  delete m_graphics;
  m_window = NULL;
  m_graphics = NULL;
}

bool Engine::Initialize()
{
  // Start a window
  //skip window if running in headless mode
  m_window = new window(GUIenabled);
  if(!m_window->Initialize(m_WINDOW_NAME, &m_WINDOW_WIDTH, &m_WINDOW_HEIGHT))
  {
    printf("The window failed to initialize.\n");
    return false;
  }

  // Start the graphics
  m_graphics = new Graphics();
  if(!m_graphics->Initialize(m_WINDOW_WIDTH, m_WINDOW_HEIGHT, GUIenabled))
  {
    printf("The graphics failed to initialize.\n");
    return false;
  }

  // Set the time
  m_currentTimeMillis = GetCurrentTimeMillis();

  // No errors
  return true;
}

void Engine::Run()
{
  m_running = true;

  //render graphics
  cout << "beginning OpenGL render..." << endl;
  long long initialDT = GetCurrentTimeMillis();
  m_graphics->Update(m_DT);
  m_graphics->Render();
  cout << "OpenGL render finished!" << endl;
  //write to output
  if (!GUIenabled)
	  loader::writeImage(outputName, m_WINDOW_WIDTH, m_WINDOW_HEIGHT);
  long long finalDT = GetCurrentTimeMillis();
  cout << "render time: " << (float)(finalDT - initialDT)/1000.0 << "s" << endl;

  while(m_running && GUIenabled)
  {
    // Update the DT
    m_DT = getDT();

    // Check the keyboard input
    m_event.update();
    for (int i = 0; i < m_event.getSize(); i++)
    {
      Keyboard(m_event.getEvent(i));
    }

    // Update the graphics
    m_graphics->Update(m_DT);

    // Swap to the Window
    m_window->Swap();
  }
}

void Engine::Keyboard(eventType event)
{
  if(event.eventVer == SDL_QUIT)
  {
    m_running = false;
  }
  else if (event.eventVer == SDL_KEYDOWN)
  {
    // handle key down events here
    if (event.key == SDLK_ESCAPE)
    {
      m_running = false;
    }

    //regenerate from seed
    if (event.key == SDLK_SPACE)
    {
      m_graphics->Render();
    }
  }
}

unsigned int Engine::getDT()
{
  long long TimeNowMillis = GetCurrentTimeMillis();
  assert(TimeNowMillis >= m_currentTimeMillis);
  unsigned int DeltaTimeMillis = (unsigned int)(TimeNowMillis - m_currentTimeMillis);
  m_currentTimeMillis = TimeNowMillis;
  return DeltaTimeMillis;
}

long long Engine::GetCurrentTimeMillis()
{
  timeval t;
  gettimeofday(&t, NULL);
  long long ret = t.tv_sec * 1000 + t.tv_usec / 1000;
  return ret;
}

void Engine::passArgs(string filename, float seed)
{
	outputName = filename;
	m_graphics->setSeed(seed);
}







