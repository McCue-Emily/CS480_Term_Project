#pragma once
#include "graphics_headers.h"
#include <SOIL2/SOIL2.h>

using namespace std;

class Texture
{

public:
	Texture();
	Texture(const char* fileName);
	/*Texture(const char* right,
		const char* left,
		const char* top,
		const char* bottom,
		const char* front,
		const char* back);*/
	bool loadTexture(const char* texFile);
	/*bool loadCubeTexture(const char* right,
		const char* left,
		const char* top,
		const char* bottom,
		const char* front,
		const char* back);*/

	GLuint getTextureID() { return m_TextureID; }

	//GLuint getCubeTextureID() { return m_cubeTextureID; }

private:
	GLuint m_TextureID;
	/*GLuint m_cubeTextureID;

	const char* m_fileNames[6];*/

	bool initializeTexture();
	//bool initializeCubeTexture();

	//const char* right = "asset\\right.png";
	//const char* left = "asset\\left.png";
	//const char* top = "asset\\top.png";
	//const char* bottom = "asset\\bottom.png";
	//const char* front = "asset\\front.png";
	//const char* back = "asset\\back.png";

};

