// This nice tool is created by Constantin Toader by gathering different methods
// from Daniel Livingstone and organising them into one stand alone class
#pragma once
#include <GL\glew.h>
#include <SDL_ttf.h>
#include <iostream>
#include <vector>

class Load
{
public:
	Load();
	static GLuint Text(const char * str);
	static GLuint CubeMap(const char *fname[6], GLuint *texID);
	static GLuint BMP(char *fname);
	static GLuint PNG(char *fname);
	static GLuint Obj(const char* fileName);
	static std::vector<float> CollisionMap(const char* fileName);
	static char* File(const char* fileName);
	static GLuint Shader(const char* fileName);
	static unsigned int meshCount() { return m_meshCount; }
	static GLuint FileSize() { return m_fileSize; }
	
	static std::vector<GLfloat> verts;
	static std::vector<GLfloat> norms;
	static std::vector<GLfloat> tex_coords;
	static std::vector<GLuint> indices;
	static std::vector<GLfloat> tangents;

	

	private:
	static GLuint m_texture;
	static GLuint m_fileSize;
	static GLuint m_meshCount;
};

