#include <iostream>

#include "engine.h"


int main(int argc, char **argv)
{
  // Start an engine and run it then cleanup after
  Engine *engine;

  //set in smaller mode for rendering
  if (argc < 3)
  {
	  engine = new Engine("Voyager1", 800, 900);
  }
  else
  {
	  engine = new Engine("Voyager1", 1700, 1920);
  }

  if(!engine->Initialize())
  {
    printf("The engine failed to start.\n");
    delete engine;
    engine = NULL;
    return 1;
  }

  //get arguments
  if (argc < 3)
  {
	  //no arguments
	  //run default
	  engine->passArgs("DEFUALT", 1.0);
  }
  else{
	  engine->passArgs(argv[1], std::atof(argv[2]));
  }
  engine->Run();
  delete engine;
  engine = NULL;
  return 0;
}
