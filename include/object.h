#ifndef OBJECT_H
#define OBJECT_H

#include <vector>
#include "graphics_headers.h"
#include "loader.h"
#include "event.h"

#define SPEED_STEP 0.2f

class Object
{
  public:
    Object();
    virtual ~Object();
    virtual void Update(unsigned int dt);
    void loadNewModel(string filename);
    void loadNewTexture(string filename);
    void loadNewNormal(string filename);
    virtual void Render();

    void translate(glm::vec3);
    void scale(float);
    void rotate(float, glm::vec3);

    glm::mat4 GetModel();

  protected:
    glm::mat4 model;

  private:
    void updateMatrix();

    std::vector<Vertex> Vertices;
    std::vector<unsigned int> Indices;
    GLuint VB;
    GLuint IB;
    GLuint tex;
    GLuint normal;

    glm::mat4 m_translate;
    glm::mat4 m_rotate;
    glm::mat4 m_scale;
};

#endif /* OBJECT_H */
