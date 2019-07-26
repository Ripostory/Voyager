/*
 * renderer.cpp
 *
 *  Created on: Jul 25, 2019
 *      Author: Ronn Quijada
 */

#include "modelRenderer.h"

ModelRenderer::ModelRenderer() : Component("Model Renderer") {
	VB = -1;
	IB = -1;
	indiceSize = 0;
	renderable = true;
}

ModelRenderer::~ModelRenderer() {
	VB = 0;
	IB = 0;
	indiceSize = 0;
	renderable = false;
}

void ModelRenderer::update() {
}

void ModelRenderer::remove() {
	//clean buffers
	glDeleteBuffers(1, &VB);
	glDeleteBuffers(1, &IB);
}

void ModelRenderer::render() {
	if (renderable) {
		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		glEnableVertexAttribArray(2);

		glBindBuffer(GL_ARRAY_BUFFER, VB);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
				(void*) offsetof(Vertex, color));
		glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
				(void*) offsetof(Vertex, texCoord));

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IB);

		glDrawElements(GL_TRIANGLES, Indices.size(), GL_UNSIGNED_INT, 0);

		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);
		glDisableVertexAttribArray(2);
	}
}

void ModelRenderer::loadModel(string filename) {
	  loader fileLoader;
	  Model object;

	  try {
		  object = fileLoader.loadObject(filename);
		  Vertices = object.vertices;
		  Indices = object.indices;
		  indiceSize = Indices.size();

		  //Load models into the GPU
		  glGenBuffers(1, &VB);
		  glBindBuffer(GL_ARRAY_BUFFER, VB);
		  glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * (Vertices.size()+1), &Vertices[0], GL_STATIC_DRAW);

		  glGenBuffers(1, &IB);
		  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IB);
		  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * Indices.size(), &Indices[0], GL_STATIC_DRAW);
	  } catch (exception& e) {
		  cerr << e.what() << endl;
	  }
}

void ModelRenderer::setRenderable(bool isRenderable) {
	renderable = isRenderable;
}
