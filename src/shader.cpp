#include "shader.h"

Shader::Shader()
{
  m_shaderProg = 0;
}

Shader::~Shader()
{
  for (std::vector<GLuint>::iterator it = m_shaderObjList.begin() ; it != m_shaderObjList.end() ; it++)
  {
    glDeleteShader(*it);
  }

  if (m_shaderProg != 0)
  {
    glDeleteProgram(m_shaderProg);
    m_shaderProg = 0;
  }
}

bool Shader::Initialize()
{
  m_shaderProg = glCreateProgram();
  if (m_shaderProg == 0) 
  {
    std::cerr << "Error creating shader program\n";
    return false;
  }

  return true;
}


bool Shader::AddShader(GLenum ShaderType, string filename)
{
  std::string shaderCode;
  loader loadShader;

  shaderCode = loadShader.loadShader(filename);
  GLuint ShaderObj = glCreateShader(ShaderType);

  if (ShaderObj == 0) 
  {
    std::cerr << "Error creating shader type " << ShaderType << std::endl;
    return false;
  }

  // Save the shader object - will be deleted in the destructor
  m_shaderObjList.push_back(ShaderObj);

  const GLchar* rawShaderCodeData[1];
  rawShaderCodeData[0] = shaderCode.c_str();
  GLint shaderCodeSize[1] = { (GLint)shaderCode.size() };

  glShaderSource(ShaderObj, 1, rawShaderCodeData, shaderCodeSize);

  glCompileShader(ShaderObj);

  GLint success;
  glGetShaderiv(ShaderObj, GL_COMPILE_STATUS, &success);

  if (!success) 
  {
    GLchar InfoLog[1024];
    glGetShaderInfoLog(ShaderObj, 1024, NULL, InfoLog);
    std::cerr << "Error compiling: " << InfoLog << std::endl;
    return false;
  }

  glAttachShader(m_shaderProg, ShaderObj);

  return true;
}


// After all the shaders have been added to the program call this function
// to link and validate the program.
bool Shader::Finalize()
{
  GLint Success = 0;
  GLchar ErrorLog[1024] = { 0 };

  glLinkProgram(m_shaderProg);

  glGetProgramiv(m_shaderProg, GL_LINK_STATUS, &Success);
  if (Success == 0)
  {
    glGetProgramInfoLog(m_shaderProg, sizeof(ErrorLog), NULL, ErrorLog);
    std::cerr << "Error linking shader program: " << ErrorLog << std::endl;
    return false;
  }

  glValidateProgram(m_shaderProg);
  glGetProgramiv(m_shaderProg, GL_VALIDATE_STATUS, &Success);
  if (!Success)
  {
    glGetProgramInfoLog(m_shaderProg, sizeof(ErrorLog), NULL, ErrorLog);
    std::cerr << "Invalid shader program: " << ErrorLog << std::endl;
    return false;
  }

  // Delete the intermediate shader objects that have been added to the program
  for (std::vector<GLuint>::iterator it = m_shaderObjList.begin(); it != m_shaderObjList.end(); it++)
  {
    glDeleteShader(*it);
  }

  m_shaderObjList.clear();

  return true;
}


void Shader::Enable()
{
    glUseProgram(m_shaderProg);
    //bind texture locations
    glUniform1i(GetUniformLocation("texture"), 0);
    glUniform1i(GetUniformLocation("normalMap"), 1);
}


GLint Shader::GetUniformLocation(const char* pUniformName)
{
    GLuint Location = glGetUniformLocation(m_shaderProg, pUniformName);

    if (Location == INVALID_UNIFORM_LOCATION) {
        fprintf(stderr, "Warning! Unable to get the location of uniform '%s'\n", pUniformName);
    }

    return Location;
}

bool Shader::buildShader(string vertFilename, string fragFilename)
{
	  if(!Initialize())
	  {
	    printf("Shader Failed to Initialize\n");
	    return false;
	  }

	  // Add the vertex shader
	  if(!AddShader(GL_VERTEX_SHADER, vertFilename))
	  {
	    printf("Vertex Shader failed to Initialize\n");
	    return false;
	  }

	  // Add the fragment shader
	  if(!AddShader(GL_FRAGMENT_SHADER, fragFilename))
	  {
	    printf("Fragment Shader failed to Initialize\n");
	    return false;
	  }

	  // Connect the program
	  if(!Finalize())
	  {
	    printf("Program to Finalize\n");
	    return false;
	  }

	  return true;
}




