#include "object.h"

Object::Object()
{  
	//init matrices
	model = glm::mat4(1.0f);
	m_translate = glm::mat4(1.0f);
	m_rotate = glm::mat4(1.0f);
	m_scale = glm::mat4(1.0f);
}

Object::~Object()
{
	  Vertices.clear();
	  Indices.clear();

	  //clean buffers
	  glDeleteBuffers(1, &VB);
	  glDeleteBuffers(1, &IB);
	  glDeleteTextures(1, &tex);
	  glDeleteTextures(1, &normal);
}

void Object::loadNewModel(string filename)
{
	  loader fileLoader;
	  ObjectCDM object;

	  try {
		  object = fileLoader.loadObject(filename);
		  Vertices = object.getVerts();
		  Indices = object.getIndices();
	  } catch (exception& e) {
		  cerr << e.what() << endl;
	  }

	  //model loading
	  glGenBuffers(1, &VB);
	  glBindBuffer(GL_ARRAY_BUFFER, VB);
	  glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * (Vertices.size()+1), &Vertices[0], GL_STATIC_DRAW);

	  glGenBuffers(1, &IB);
	  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IB);
	  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * Indices.size(), &Indices[0], GL_STATIC_DRAW);
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

glm::mat4 Object::GetModel()
{
  return model;
}

void Object::Render()
{
	  //note: draws only the model itself, does not bind textures
	  glEnableVertexAttribArray(0);
	  glEnableVertexAttribArray(1);
	  glEnableVertexAttribArray(2);

	  glBindBuffer(GL_ARRAY_BUFFER, VB);
	  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
	  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex,color));
	  glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex,texCoord));

	  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IB);

	  glDrawElements(GL_TRIANGLES, Indices.size(), GL_UNSIGNED_INT, 0);

	  glDisableVertexAttribArray(0);
	  glDisableVertexAttribArray(1);
	  glDisableVertexAttribArray(2);
}

void Object::translate(glm::vec3 move)
{
	m_translate = glm::translate(move);
	updateMatrix();
}

void Object::scale(float size)
{
	m_scale = glm::scale(glm::vec3(size));
	updateMatrix();
}

void Object::rotate(float degree, glm::vec3 axis)
{
	m_rotate = glm::rotate(degree, axis);
	updateMatrix();
}

void Object::updateMatrix()
{
	model = m_translate * m_scale * m_rotate;
}











