/*
 * textureComponent.h
 *
 *  Created on: Jul 26, 2019
 *      Author: nog
 */

#ifndef INCLUDE_TEXTURECOMPONENT_H_
#define INCLUDE_TEXTURECOMPONENT_H_

#include "component.h"
#include "loader.h"

class TextureComponent : public Component {
private:
	GLuint texturePointer;
	GLuint bindLocation;
public:
	TextureComponent();
	~TextureComponent();
	void render() override;
	void loadTexture(string filename);
	void setBindLocation(GLuint location);
};



#endif /* INCLUDE_TEXTURECOMPONENT_H_ */
