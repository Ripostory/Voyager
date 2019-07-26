#include "object.h"

Object::Object()
{  
	renderer = new ModelRenderer();
	addComponent(renderer);
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
	  loader fileLoader;
	  Texture texture(0,0,NULL);
	  //TODO try to decouple object class from opengl
	  glGenTextures(1, &tex);
	  glActiveTexture(GL_TEXTURE0);
	  glBindTexture(GL_TEXTURE_2D, tex);

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

void Object::loadNewNormal(string filename)
{
	  loader fileLoader;

	  Texture texture(0,0,NULL);
	  try {
		  texture = fileLoader.loadTexture(filename);
	  } catch (exception& e) {
		  cerr << e.what() << endl;
	  }
	  //TODO try to decouple object class from opengl
	  //texture loading
	  glGenTextures(1, &normal);
	  glActiveTexture(GL_TEXTURE0 +1);
	  glBindTexture(GL_TEXTURE_2D, normal);
	  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, texture.column, texture.row, 0, GL_RGBA, GL_UNSIGNED_BYTE, texture.data);
	  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}

void Object::Update(unsigned int dt)
{
}

void Object::Render()
{
	renderer->render();
}
