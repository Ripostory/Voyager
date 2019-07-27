#include "object.h"

Object::Object()
{  
	renderer = new ModelRenderer();
	addComponent(renderer);
	texture = new TextureComponent();
	addComponent(texture);
	normal = new TextureComponent();
	addComponent(texture);
}

Object::~Object()
{
}

void Object::loadNewModel(string filename)
{
	renderer->loadModel(filename);
}

void Object::loadNewTexture(string filename)
{
	texture->loadTexture(filename);
}

void Object::loadNewNormal(string filename)
{
	normal->loadTexture(filename);
	normal->setBindLocation(GL_TEXTURE1);
}

void Object::Update(unsigned int dt)
{
}

void Object::Render()
{
	renderer->render();
}
