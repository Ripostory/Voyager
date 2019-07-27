/*
 * textureComponent.cpp
 *
 *  Created on: Jul 26, 2019
 *      Author: Ronn Quijada
 */

#include "textureComponent.h"

TextureComponent::TextureComponent() : Component("Texture") {
	texturePointer = 0;
	bindLocation = GL_TEXTURE0;
}

TextureComponent::~TextureComponent() {
	texturePointer = 0;
	bindLocation = 0;
}

void TextureComponent::loadTexture(string filename) {
	  loader fileLoader;
	  Texture texture(0,0,NULL);
	  glGenTextures(1, &texturePointer);
	  glActiveTexture(bindLocation);
	  glBindTexture(GL_TEXTURE_2D, texturePointer);

	  try {
		  texture = fileLoader.loadTexture(filename);
		  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	  } catch (exception& e) {
		  cerr << e.what() << endl;

		  //load failed, use a default color
		  unsigned char bytes[] = {1,0,1,1};
		  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 1, 1, 0, GL_RGBA, GL_UNSIGNED_BYTE, bytes);
	  }
}

void TextureComponent::setBindLocation(GLuint location) {
	bindLocation = location;
}

void TextureComponent::render() {
	  glActiveTexture(bindLocation);
	  glBindTexture(GL_TEXTURE_2D, texturePointer);
}


