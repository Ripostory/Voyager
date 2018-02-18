/*
 * loader.h
 *
 *  Created on: Aug 30, 2017
 *      Author: Ronn Quijada
 */

#ifndef LOADER_H_
#define LOADER_H_

#include <iostream>
#include <fstream>
#include <assimp/Importer.hpp> //includes the importer, which is used to read our obj file
#include <assimp/scene.h> //includes the aiScene object
#include <assimp/postprocess.h> //includes the postprocessing variables for the importer
#include <assimp/color4.h> //includes the aiColor4 object, which is used to handle the colors from the mesh objects
#include <Magick++.h>
#include <magick/blob.h>
#include <vector>
#include "graphics_headers.h"

using namespace std;

class obj {
private:
	vector<glm::vec3> vertRaw;
    vector<Vertex> vertices;
    vector<unsigned int> indices;
public:
    obj();
    ~obj();

    void addVert(Vertex);
    void addIndice(unsigned int);
    void addRaw(glm::vec3);

    vector<Vertex> getVerts();
    vector<unsigned int> getIndices();
    vector<glm::vec3> getRawVerts();
};

class loader {
private:
	fstream filein;
	bool isEOF;

	bool isHeader(string);
	unsigned int* getIndices(string parse);
public:
	loader();
	~loader();

	bool loadObject(string, obj&);
	bool loadShader(string, string&);
	bool loadTexture(string, Texture&);
	static bool writeImage(string, int, int);

};

#endif /* LOADER_H_ */
