#ifndef RT3D
#define RT3D
#pragma once

#include <GL/glew.h>
#include <SDL.h>
#include <iostream>
#include <fstream>
#include <string>
#include "Light.h"
#include "Material.h"


#define RT3D_VERTEX		0
#define RT3D_NORMAL		1
#define RT3D_TEXCOORD   2
#define RT3D_INDEX		3
#define RT3D_COLOUR		4

namespace rt3d {

	struct lightStruct {
		GLfloat ambient[4];
		GLfloat diffuse[4];
		GLfloat specular[4];
		GLfloat position[4];
	};

	struct materialStruct {
		GLfloat ambient[4];
		GLfloat diffuse[4];
		GLfloat specular[4];
		GLfloat shininess;
	};

	void exitFatalError(const char *message);
	char* loadFile(const char *fname, GLint &fSize);
	void printShaderError(const GLint shader);
	GLuint initShaders(const char *vertFile, const char *fragFile);
	// Some methods for creating meshes
	// ... including one for dealing with indexed meshes
	GLuint createMesh(const GLuint numVerts, const GLfloat* vertices, const GLfloat* colours, const GLfloat* normals,
		const GLfloat* texcoords, const GLuint indexCount, const GLuint* indices);
	// these three create mesh functions simply provide more basic access to the full version
	GLuint createMesh(const GLuint numVerts, const GLfloat* vertices, const GLfloat* colours, const GLfloat* normals,
		const GLfloat* texcoords);
	GLuint createMesh(const GLuint numVerts, const GLfloat* vertices);
	GLuint createColourMesh(const GLuint numVerts, const GLfloat* vertices, const GLfloat* colours);

	void drawMesh(const GLuint mesh, const GLuint numVerts, const GLuint primitive); 
	void drawIndexedMesh(const GLuint mesh, const GLuint indexCount, const GLuint primitive);

	void updateMesh(const GLuint mesh, const unsigned int bufferType, const GLfloat *data, const GLuint size);
}

#endif