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
    bool Initialize();
    void Enable();
    bool AddShader(GLenum ShaderType, string filename);
    bool Finalize();
    GLint GetUniformLocation(const char* pUniformName);

    bool buildShader(string, string);

  private:
    GLuint m_shaderProg;    
    std::vector<GLuint> m_shaderObjList;
};

#endif  /* SHADER_H */
