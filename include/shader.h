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
    void Initialize();
    void Enable();
    void AddShader(GLenum ShaderType, string filename);
    void Finalize();
    GLint GetUniformLocation(const char* pUniformName);

    bool buildShader(string, string);

  private:
    GLuint shaderProg;    
    std::vector<GLuint> shaderObjList;
};

#endif  /* SHADER_H */
