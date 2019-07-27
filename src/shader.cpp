#include "shader.h"

Shader::Shader() {
	shaderProg = 0;
}

Shader::~Shader() {
	for (std::vector<GLuint>::iterator it = shaderObjList.begin();
			it != shaderObjList.end(); it++) {
		glDeleteShader(*it);
	}

	if (shaderProg != 0) {
		glDeleteProgram(shaderProg);
		shaderProg = 0;
	}
}

void Shader::Initialize() {
	shaderProg = glCreateProgram();
	if (shaderProg == 0) {
		throw GlException::GlShaderException(
				"Unable to create shader program.");
	}
}

void Shader::AddShader(GLenum ShaderType, string filename) {
	std::string shaderCode;
	loader loadShader;

	shaderCode = loadShader.loadShader(filename);
	GLuint ShaderObj = glCreateShader(ShaderType);

	if (ShaderObj == 0) {
		throw GlException::GlShaderException(
				"Error creating shader type " + ShaderType);
	}

	// Save the shader object - will be deleted in the destructor
	shaderObjList.push_back(ShaderObj);

	const GLchar* rawShaderCodeData[1];
	rawShaderCodeData[0] = shaderCode.c_str();
	GLint shaderCodeSize[1] = { (GLint) shaderCode.size() };

	glShaderSource(ShaderObj, 1, rawShaderCodeData, shaderCodeSize);

	glCompileShader(ShaderObj);

	GLint success;
	glGetShaderiv(ShaderObj, GL_COMPILE_STATUS, &success);

	if (!success) {
		GLchar InfoLog[1024];
		glGetShaderInfoLog(ShaderObj, 1024, NULL, InfoLog);
		throw GlException::GlShaderException(
				"Error compiling: " + string(InfoLog));
	}

	glAttachShader(shaderProg, ShaderObj);
}

void Shader::Finalize() {
	GLint Success = 0;
	GLchar ErrorLog[1024] = { 0 };

	glLinkProgram(shaderProg);

	glGetProgramiv(shaderProg, GL_LINK_STATUS, &Success);
	if (!Success) {
		glGetProgramInfoLog(shaderProg, sizeof(ErrorLog), NULL, ErrorLog);
		throw GlException::GlShaderException(
				"Error linking shader program: " + string(ErrorLog));
	}

	glValidateProgram(shaderProg);
	glGetProgramiv(shaderProg, GL_VALIDATE_STATUS, &Success);
	if (!Success) {
		glGetProgramInfoLog(shaderProg, sizeof(ErrorLog), NULL, ErrorLog);
		throw GlException::GlShaderException(
				"Invalid shader program: " + string(ErrorLog));
	}

	// Delete the intermediate shader objects that have been added to the program
	for (std::vector<GLuint>::iterator it = shaderObjList.begin();
			it != shaderObjList.end(); it++) {
		glDeleteShader(*it);
	}

	shaderObjList.clear();
}

void Shader::Enable() {
	glUseProgram(shaderProg);
	//bind texture locations
	glUniform1i(GetUniformLocation("texture"), 0);
	glUniform1i(GetUniformLocation("normalMap"), 1);
}

GLint Shader::GetUniformLocation(const char* pUniformName) {
	GLuint Location = glGetUniformLocation(shaderProg, pUniformName);

	if (Location == INVALID_UNIFORM_LOCATION) {
		fprintf(stderr, "Warning! Unable to get the location of uniform '%s'\n",
				pUniformName);
	}

	return Location;
}

bool Shader::buildShader(string vertFilename, string fragFilename) {
	try {
		Initialize();
		AddShader(GL_VERTEX_SHADER, vertFilename);
		AddShader(GL_FRAGMENT_SHADER, fragFilename);
		Finalize();
	} catch (GlException::GlShaderException& e) {
		cerr << e.what() << endl;
		return false;
	}
	return true;
}

