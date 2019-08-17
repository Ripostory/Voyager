/*
 *  Created by Ronn
 *  7-25-2019
 *
 *  Implements various common exceptions related to the program
 *
 */

#include <exception>
#include <stdio.h>
#include <string.h>
#include <GL/glew.h>
using namespace std;

namespace VoyagerException {

struct LoaderException: public exception {
	const char* what() const throw () {
		return "There was an issue when trying to load a resource";
	}
};

struct ShaderFileException: public LoaderException {
	const char* what() const throw () {
		return "A shader file failed to load";
	}
};

struct ObjectFileException: public LoaderException {
	const char* what() const throw () {
		return "An object file failed to load";
	}
};

struct TextureFileException: public LoaderException {
	const char* what() const throw () {
		return "A texture filed failed to load";
	}
};

}

namespace GlException {

struct GlException {
	GLenum exception;
	GlException(GLenum e): exception(e) {}
	const char* what() throw () {
		string message;
		switch (exception) {
		case GL_INVALID_ENUM:
			message = "GL_INVALID_ENUM: An unacceptable value is specified for an enumerated argument.";
			break;
		case GL_INVALID_VALUE:
			message = "GL_INVALID_VALUE: A numeric argument is out of range.";
			break;
		case GL_INVALID_OPERATION:
			message = "GL_INVALID_OPERATION: The specified operation is not allowed in the current state.";
			break;
		case GL_INVALID_FRAMEBUFFER_OPERATION:
			message = "GL_INVALID_FRAMEBUFFER_OPERATION: The framebuffer object is not complete.";
			break;
		case GL_OUT_OF_MEMORY:
			message = "GL_OUT_OF_MEMORY: There is not enough memory left to execute the command.";
			break;
		default:
			message = "An unknown GL Exception has occured.";
		}

		return message.insert(0,"An Opengl Exception has occured: ").c_str();
	}
};

struct GlErrorException : public exception {
	string message;
	GlErrorException(string e) : message(e) {}

	const char* what() throw () {
		return message.insert(0,"An Opengl Exception has occured: ").c_str();
	}
};

struct GlShaderException : public GlErrorException {
	GlShaderException(string e) : GlErrorException(e) {}
	const char* what() throw () {
		return message.insert(0,"An Opengl Shader Exception has occured: ").c_str();
	}
};

}

