#ifndef SHADER_H
#define SHADER_H

#include <vector>
#include "loader.h"
#include "graphics_headers.h"

class Shader
{
  public:
    Shader();
    ~Shader();
    void Enable();
    GLint GetUniformLocation(const char* pUniformName);
    bool buildShader(string, string);

  private:
    GLuint shaderProg;    
    std::vector<GLuint> shaderObjList;
    void Initialize();
    void AddShader(GLenum ShaderType, string filename);
    void Finalize();
};

#endif  /* SHADER_H */
