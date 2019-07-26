#ifndef OBJECT_H
#define OBJECT_H

#include <vector>
#include "entity.h"
#include "loader.h"
#include "event.h"

#define SPEED_STEP 0.2f

class Object : public Entity
{
  public:
    Object();
    virtual ~Object();
    virtual void Update(unsigned int dt);
    void loadNewModel(string filename);
    void loadNewTexture(string filename);
    void loadNewNormal(string filename);
    virtual void Render();

  private:
    std::vector<Vertex> Vertices;
    std::vector<unsigned int> Indices;
    GLuint VB;
    GLuint IB;
    GLuint tex;
    GLuint normal;

};

#endif /* OBJECT_H */
