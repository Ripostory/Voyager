
#include "Camera.h"

camera::camera()
{

}

camera::~camera()
{

}

bool camera::Initialize(int w, int h)
{
  view = glm::lookAt( glm::vec3(0.0, 0.0, -70.0), //Eye Position
                      glm::vec3(0.0, 0.0, 0.0), //Focus point
                      glm::vec3(0.0, 1.0, 0.0)); //Positive Y is up

  projection = glm::perspective( 44.3f, //the FoV typically 90 degrees is good which is what this is set to
                                 float(w)/float(h), //Aspect Ratio, so Circles stay Circular
                                 0.01f, //Distance to the near plane, normally a small value like this
                                 100.0f); //Distance to the far plane, 
  return true;
}

glm::mat4 camera::GetProjection()
{
  return projection;
}

glm::mat4 camera::GetView()
{
  return view;
}
