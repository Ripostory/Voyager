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
	isEOF = false;
}

loader::~loader()
{
	isEOF = false;
}

bool loader::loadObject(string filename, obj &inputObj)
{
	aiMesh *mesh;
	aiVector3D vert;
	aiVector3D texture;
	aiFace face;
	glm::vec3 tempVec;
	glm::vec3 tempColor;
	glm::vec2 tempTex;
	int indice;

	//load object from file
	Assimp::Importer import;
	const aiScene *scene = import.ReadFile(filename, \
			aiProcess_CalcTangentSpace      |  \
			aiProcess_GenNormals            |  \
			aiProcess_JoinIdenticalVertices |  \
			aiProcess_Triangulate           |  \
			aiProcess_GenUVCoords           |  \
			aiProcess_FlipUVs	            |  \
			0
			);

	//if fail, throw error
	if (scene == NULL)
	{
		//read failed, return empty object
		cout << "FILE " << filename << " UNABLE TO BE READ" << endl;
		return false;
	}

	//otherwise, pack object data into the OBJ ADT and send to inputObj
	//get mesh (should be at index 0 since it's the only mesh)
	mesh = scene->mMeshes[0];

	obj final;
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
			//if fails, default to 0,0 coords
		}
		else
		{
			tempTex = glm::vec2(0,0);
		}

		//push into object
		final.addVert(Vertex(tempVec, tempColor, tempTex));
	}

	//load indices
	for (int i = 0; i < mesh->mNumFaces; i++)
	{
		face = mesh->mFaces[i];

		//made to only work with triangles
		final.addIndice(face.mIndices[0]+1);
		final.addIndice(face.mIndices[1]+1);
		final.addIndice(face.mIndices[2]+1);
	}

	mesh = NULL;
	scene = NULL;

	inputObj = final;
	return true;
}

bool loader::loadShader(string filename, string& output) {
	isEOF = false;
	string final = "";
	char temp;
	filein.open(filename.c_str());

	if (filein.fail())
	{
		//read failed, return empty object
		cout << "FILE " << filename << " UNABLE TO BE READ" << endl;
		return false;
	}

	//load file
	while(!isEOF) {
		temp = filein.get();
		if (filein.eof())
		{
			isEOF = true;
			//throw away EOF bit
		} else
		{
			final += temp;
		}
	}

	//return file
	output = final;

	filein.close();
	return true;
}

bool loader::loadTexture(string filename, Texture &output)
{
	Magick::Blob blob;
	Magick::Image *tex;

	// load texture
	try
	{
		tex = new Magick::Image(filename);
		tex->write(&blob, "RGBA");
	}
	catch (Magick::Error& Error)
	{
		cout << "Error loading Texture " << filename << ": " << Error.what() << endl;
		return false;
	}

	//extract data
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, tex->columns(), tex->rows(), 0, GL_RGBA, GL_UNSIGNED_BYTE, blob.data());
	delete tex;
	return true;
}





//TODO
/*
 *    char *buffer; // needs to be big enough
    glReadPixels( 0, 0, width, length, GL_BGR, GL_BYTE, buffer );
    Blob b( buffer, 3 * width * length );
    Image i( b, 3 * width * length, 3 );
    i.write( "img.jpg" );
 */
