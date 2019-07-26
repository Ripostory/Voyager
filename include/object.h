#ifndef OBJECT_H
#define OBJECT_H

#include <vector>
#include "entity.h"
#include "loader.h"
#include "event.h"
#include "modelRenderer.h"

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
    GLuint tex;
    GLuint normal;
    ModelRenderer *renderer;

};

#endif /* OBJECT_H */
