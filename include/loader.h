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
#include "graphics_headers.h"

using namespace std;

class loader {
private:
	bool isHeader(string);
	unsigned int* getIndices(string parse);
public:
	loader();
	~loader();

	Model loadObject(string filename);
	string loadShader(string filename);
	Texture loadTexture(string filename);
	static bool writeImage(string, int, int);

};

#endif /* LOADER_H_ */
