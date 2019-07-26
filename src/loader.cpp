/*
 * loader.cpp
 *
 *  Created on: Aug 30, 2017
 *      Author: Ronn Quijada
 */

#include <loader.h>

obj::obj()
{
}

obj::~obj()
{
	vertRaw.empty();
	vertices.empty();
	indices.empty();
}

void obj::addVert(Vertex vert)
{
	vertices.push_back(vert);
}

void obj::addIndice(unsigned int ind)
{
	//add indice and auto fix indexing
	indices.push_back(ind-1);
}

void obj::addRaw(glm::vec3 raw)
{
	vertRaw.push_back(raw);

	//allocate size for new vertex in vertices
	glm::vec3 item;
	glm::vec2 item2;
	Vertex temp(item,item, item2);
	vertices.push_back(temp);
}

vector<Vertex> obj::getVerts()
{
	return vertices;
}

vector<unsigned int> obj::getIndices()
{
	return indices;
}

vector<glm::vec3> obj::getRawVerts() {
	return vertRaw;
}







loader::loader()
{
}

loader::~loader()
{
}

obj loader::loadObject(string filename)
{
	aiVector3D vert;
	aiVector3D texture;
	aiFace face;
	glm::vec3 tempVec;
	glm::vec3 tempColor;
	glm::vec2 tempTex;
	int indice;

	Assimp::Importer import;
	const aiScene *rawSceneData = import.ReadFile(filename, \
			aiProcess_CalcTangentSpace      |  \
			aiProcess_GenNormals            |  \
			aiProcess_JoinIdenticalVertices |  \
			aiProcess_Triangulate           |  \
			aiProcess_GenUVCoords           |  \
			aiProcess_FlipUVs	            |  \
			0
			);

	if (rawSceneData == NULL)
	{
		cout << "FILE " << filename << " UNABLE TO BE READ" << endl;
		throw VoyagerException::ObjectFileException();
	}

	aiMesh *mesh;
	mesh = rawSceneData->mMeshes[0];

	obj outputObjectCDM;
	for (int i = 0; i < mesh->mNumVertices; i++)
	{
		//get vertices
		vert = mesh->mVertices[i];
		tempVec.x = vert.x;
		tempVec.y = vert.y;
		tempVec.z = vert.z;

		//get color (get them from normals)
		vert = mesh->mNormals[i];
		tempColor.r = vert.x;
		tempColor.g = vert.y;
		tempColor.b = vert.z;

		//get texture coordinates
		if (mesh->mTextureCoords[0] != NULL)
		{
			texture = mesh->mTextureCoords[0][i];
			tempTex.x = texture.x;
			tempTex.y = texture.y;
		}
		else
		{
			tempTex = glm::vec2(0,0);
		}

		outputObjectCDM.addVert(Vertex(tempVec, tempColor, tempTex));
	}

	//load indices
	for (int i = 0; i < mesh->mNumFaces; i++)
	{
		face = mesh->mFaces[i];

		//made to only work with triangles
		outputObjectCDM.addIndice(face.mIndices[0]+1);
		outputObjectCDM.addIndice(face.mIndices[1]+1);
		outputObjectCDM.addIndice(face.mIndices[2]+1);
	}

	mesh = NULL;
	rawSceneData = NULL;

	return outputObjectCDM;
}

string loader::loadShader(string filename) {
	bool isEOF = false;
	string outputString = "";

	filein.open(filename.c_str());

	if (filein.fail())
	{
		cerr << "FILE " << filename << " UNABLE TO BE READ" << endl;
		throw VoyagerException::ShaderFileException();
	}

	//load file
	char currentChar;
	while(!isEOF) {
		currentChar = filein.get();
		if (filein.eof())
		{
			isEOF = true;
		} else
		{
			outputString += currentChar;
		}
	}

	filein.close();
	return outputString;
}

Texture loader::loadTexture(string filename)
{
	Magick::Blob blob;
	Magick::Image *tex;

	try
	{
		tex = new Magick::Image(filename);
		tex->write(&blob, "RGBA");
	}
	catch (Magick::Error& Error)
	{
		cout << "Error loading Texture " << filename << ": " << Error.what() << endl;
		throw VoyagerException::TextureFileException();
	}

	//TODO cleanup to decouple opengl from loader
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, tex->columns(), tex->rows(), 0, GL_RGBA, GL_UNSIGNED_BYTE, blob.data());
	delete tex;
	return Texture(tex->rows(), tex->columns(), NULL);
}

bool loader::writeImage(string filename, int width, int height)
{
	//move contents of default render buffer to blob
	char* buffer = new char[3*width*height];
	glReadPixels(0,0,width,height, GL_RGB, GL_BYTE, buffer);
	Magick::Blob data(buffer, 3 * width * height);

	//create image from BLOB
	Magick::Image bloom;
	Magick::Image finalBloom;
	Magick::Image final;
	final.magick("RGB");
	final.size(Magick::Geometry(width, height, 0, 0));
	final.depth(8);
	final.read(data);
	bloom.magick("RGB");
	bloom.size(Magick::Geometry(width, height, 0, 0));
	bloom.depth(8);
	bloom.read(data);
	finalBloom.magick("RGB");
	finalBloom.size(Magick::Geometry(width, height, 0, 0));
	finalBloom.depth(8);
	finalBloom.read(data);

	//post processing
	finalBloom.flip();
	bloom.flip();
	final.flip();
	final.opacity(0);
	final.modulate(200.0,100.0,100.0);

	float ratio = (float) width/1920.0; //make blur resolution independent
	//blur bloom and apply it over final

	//set base
	bloom.sigmoidalContrast(1,40.0);
	finalBloom.sigmoidalContrast(1,40.0);
	finalBloom.gaussianBlur(0, 10.0*ratio);

	//convolution
	if (true)
	{
		float power = 10.0f;
		float size = 0.8;
		for (int i = 0; i < 3; i++)
		{
			cout << "applying blur convolution " << i+1;
			cout << " using buffer size " << size*width << "x" <<size*height << "..." << endl;

			//resize for each convolution step
			bloom.resize(Magick::Geometry(width*size, height*size, 0, 0));
			bloom.gaussianBlur(0.0, power*ratio);

			//stretch to fit
			bloom.resize(Magick::Geometry(width, height, 0, 0));
			finalBloom.composite(bloom, 0, 0, Magick::PlusCompositeOp);
			size /= 1.5;
		}
	}

	//perform screen operation 1 - (1-picture1) * (1-picture2)
	cout << "performing final composition..." << endl;
	final.negate();
	finalBloom.negate();
	final.composite(finalBloom, 0, 0, Magick::MultiplyCompositeOp);
	final.negate();

	//final image adjustments
	final.modulate(100.0, 110.0, 100.0);

	//use LUT to color correct
	Magick::Image lut;
	lut.read("assets/planetLUT.png");
	final.haldClut(lut);

	cout << "writing image to " << filename <<".png..." << endl;
	final.write(filename + ".png");
	delete[] buffer;

	return true;
}
