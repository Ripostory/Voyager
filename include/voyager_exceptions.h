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

