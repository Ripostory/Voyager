/*
 * renderer.h
 *
 *  Created on: Jul 25, 2019
 *      Author: nog
 */

#ifndef INCLUDE_MODELRENDERER_H_
#define INCLUDE_MODELRENDERER_H_

#include "component.h"
#include "loader.h"

class ModelRenderer : public Component {
private:
    GLuint VB;
    GLuint IB;
    GLuint indiceSize;
	vector<Vertex> Vertices;
	vector<unsigned int> Indices;
    bool renderable;
public:
    ModelRenderer();
    ~ModelRenderer();
	virtual void update() override;
	virtual void remove() override;
	void render() override;
	void loadModel(string model);
	void setRenderable(bool isRenderable);
};



#endif /* INCLUDE_MODELRENDERER_H_ */
