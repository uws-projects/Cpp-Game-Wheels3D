#include "Load.h"
#include <algorithm>
#include "rt3dObjLoader.h"
#include "rt3d.h"
#include <SDL_image.h>

std::vector<GLfloat> Load::verts;
std::vector<GLfloat> Load::norms;
std::vector<GLfloat> Load::tex_coords;
std::vector<GLuint> Load::indices;
std::vector<GLfloat> Load::tangents;

static std::vector<float> coordinates;

GLuint Load::m_fileSize;
GLuint Load::m_meshCount;


void generateTangents(){

	using namespace std;
	// Code taken from http://www.terathon.com/code/tangent.html and modified slightly to use vectors instead of arrays
	// Lengyel, Eric. “Computing Tangent Space Basis Vectors for an Arbitrary Mesh”. Terathon Software 3D Graphics Library, 2001. 

	// This is a little messy because my vectors are of type GLfloat:
	// should have made them glm::vec2 and glm::vec3 - life, would be much easier!

	auto verts = Load::verts;
	auto indices = Load::indices;
	auto tex_coords = Load::tex_coords;
	auto normals = Load::norms;
	auto tangents = Load::tangents;


	vector<glm::vec3> tan1(verts.size() / 3, glm::vec3(0.0f));
	vector<glm::vec3> tan2(verts.size() / 3, glm::vec3(0.0f));

	int triCount = indices.size() / 3;

	for (unsigned int c = 0; c < indices.size(); c += 3)
	{
		int i1 = indices[c];
		int i2 = indices[c + 1];
		int i3 = indices[c + 2];

		glm::vec3 v1(verts[i1 * 3], verts[i1 * 3 + 1], verts[i1 * 3 + 2]);
		glm::vec3 v2(verts[i2 * 3], verts[i2 * 3 + 1], verts[i2 * 3 + 2]);
		glm::vec3 v3(verts[i3 * 3], verts[i3 * 3 + 1], verts[i3 * 3 + 2]);

		glm::vec2 w1(tex_coords[i1 * 2], tex_coords[i1 * 2 + 1]);
		glm::vec2 w2(tex_coords[i2 * 2], tex_coords[i2 * 2 + 1]);
		glm::vec2 w3(tex_coords[i3 * 2], tex_coords[i3 * 2 + 1]);

		float x1 = v2.x - v1.x;
		float x2 = v3.x - v1.x;
		float y1 = v2.y - v1.y;
		float y2 = v3.y - v1.y;
		float z1 = v2.z - v1.z;
		float z2 = v3.z - v1.z;

		float s1 = w2.x - w1.x;
		float s2 = w3.x - w1.x;
		float t1 = w2.y - w1.y;
		float t2 = w3.y - w1.y;

		float r = 1.0F / (s1 * t2 - s2 * t1);
		glm::vec3 sdir((t2 * x1 - t1 * x2) * r, (t2 * y1 - t1 * y2) * r,
			(t2 * z1 - t1 * z2) * r);
		glm::vec3 tdir((s1 * x2 - s2 * x1) * r, (s1 * y2 - s2 * y1) * r,
			(s1 * z2 - s2 * z1) * r);

		tan1[i1] += sdir;
		tan1[i2] += sdir;
		tan1[i3] += sdir;

		tan2[i1] += tdir;
		tan2[i2] += tdir;
		tan2[i3] += tdir;
	}

	for (unsigned int a = 0; a < verts.size(); a += 3)
	{
		glm::vec3 n(normals[a], normals[a + 1], normals[a + 2]);
		glm::vec3 t = tan1[a / 3];

		glm::vec3 tangent;
		tangent = (t - n * glm::normalize(glm::dot(n, t)));

		// handedness
		GLfloat w = (glm::dot(glm::cross(n, t), tan2[a / 3]) < 0.0f) ? -1.0f : 1.0f;

		tangents.push_back(tangent.x);
		tangents.push_back(tangent.y);
		tangents.push_back(tangent.z);
		tangents.push_back(w);
	}
	Load::tangents = tangents;
}

static void CheckShaderError(GLuint shader, GLuint flag, bool isProgram, const std::string& errorMessage)
{
	GLint ok = 0;
	GLchar error[512] = { 0 };

	if (isProgram)
		glGetProgramiv(shader, flag, &ok);
	else
		glGetShaderiv(shader, flag, &ok);

	if (ok == GL_FALSE)
	{
		if (isProgram)
			glGetProgramInfoLog(shader, sizeof(error), nullptr, error);
		else
			glGetShaderInfoLog(shader, sizeof(error), nullptr, error);

		std::cerr << errorMessage << ": " << error << "\n";
	}
}
static GLuint CreateShader(const char* fileName, GLenum typeOfShader)
{
	GLuint shader = glCreateShader(typeOfShader);

	if (shader == 0)
	{
		std::cerr << "Failed to create shader \n";
	}
	
	const char* source = Load::File(fileName);
	GLint fileSize = Load::FileSize();

	glShaderSource(shader, 1, &source, &fileSize);
	glCompileShader(shader);

	CheckShaderError(shader, GL_COMPILE_STATUS, false, "Failed to compile shader\n");

	return shader;

}
Load::Load()
{
}

std::vector<float> Load::CollisionMap(const char* fileName){

	std::vector<float> coordinates;
	std::vector<float> tempCoordinates;
	unsigned int loadedCoordinates = 0;

	std::ifstream in(fileName);		// file to load
	char buffer[128];				// buffer to read lines

	if (!in.is_open())
	{
		std::cout << "File has not been loaded ... \n";
		//return false;
	}
	while (!in.eof())
		// read all data in a string
	{
		in.getline(buffer, 128);

		// ========================= vertex loading =================================
		if (buffer[0] == 'v' && buffer[1] == ' ')
			// if it starts with a v followed by space
			// it is a vertex, so store it
		{
			float localX, localY, localZ;
			sscanf_s(buffer, "v %f %f %f", &localX, &localY, &localZ);
			tempCoordinates.push_back(localX);
			loadedCoordinates++;
			tempCoordinates.push_back(localZ);
			loadedCoordinates++;
		}
	}


	float min(0.0f), max(0.0f);


	for (unsigned int i = 0; i != loadedCoordinates; i += 4)
		// so the collision files store lines which represent the diagonal
		// of a rectangle that is the base of each building, so basically for 
		// each building we will have two points: xy coordinates for each one
		// the difference between x of both points is the width while the difference
		// between both y is the height
		// this way using two points we actually have the coordinates of all 4 edges
		// of the rectangle.
		// this way we find out which x is on the left edge and which x is on the right edge
		// which y is on the top edge and which y is on the bottom edge
		// this way we will store our points in the right order to be easier checked for 
		// collision later on when we check if the object suspect for collision is higher than
		// the bottom and lower than the top, on the right side of the left edge and on the left 
		// side of the right edge. If so, then there is a collision

		// so using the minimum and maximum functions we find the min x (left edge), max x (right edge)
		// store them and the proceed with the same thing for the y values
	{
		if (tempCoordinates[i] < tempCoordinates[i + 2])
		{
			min = tempCoordinates[i];
			max = tempCoordinates[i + 2];
		}
		else
		{
			min = tempCoordinates[i + 2];
			max = tempCoordinates[i];
		}

		coordinates.push_back(min);
		coordinates.push_back(max);

		if (tempCoordinates[i+1] < tempCoordinates[i + 3])
		{
			min = tempCoordinates[i + 1];
			max = tempCoordinates[i + 3];
		}
		else
		{
			min = tempCoordinates[i + 3];
			max = tempCoordinates[i + 1];
		}
		
		coordinates.push_back(min);
		coordinates.push_back(max);

	}
	return coordinates;
}

void Clean()
{
	Load::verts.clear();
	Load::norms.clear();
	Load::tex_coords.clear();
	Load::indices.clear();
}

GLuint Load::Text(const char * str) {
	if (TTF_Init() == -1)
		std::cerr << "TTF failed to initialise.\n";

	TTF_Font *font = TTF_OpenFont("MavenPro-Regular.ttf", 24);
	if (font == NULL)
		std::cerr << "Failed to open font.\n";

	SDL_Color colour = { 255, 255, 255 };
	SDL_Color bg = { 0, 0, 0 };

	SDL_Surface *stringImage;
	stringImage = TTF_RenderText_Blended(font, str, colour);

	if (stringImage == NULL)
		//exitFatalError("String surface not created.");
		std::cout << "String surface not created." << std::endl;

	GLuint w = stringImage->w;
	GLuint h = stringImage->h;
	GLuint colours = stringImage->format->BytesPerPixel;

	GLuint format, internalFormat;
	if (colours == 4) {   // alpha
		if (stringImage->format->Rmask == 0x000000ff)
			format = GL_RGBA;
		else
			format = GL_BGRA;
	}
	else {             // no alpha
		if (stringImage->format->Rmask == 0x000000ff)
			format = GL_RGB;
		else
			format = GL_BGR;
	}
	internalFormat = (colours == 4) ? GL_RGBA : GL_RGB;

	GLuint texture;

	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, w, h, 0,
		format, GL_UNSIGNED_BYTE, stringImage->pixels);

	// SDL surface was used to generate the texture but is no longer
	// required. Release it to free memory
	SDL_FreeSurface(stringImage);
	return texture;
}

GLuint Load::CubeMap(const char *fname[6], GLuint *texID)
{
	glGenTextures(1, texID); // generate texture ID
	GLenum sides[6] = { 
		GL_TEXTURE_CUBE_MAP_POSITIVE_Z,
		GL_TEXTURE_CUBE_MAP_NEGATIVE_Z,
		GL_TEXTURE_CUBE_MAP_POSITIVE_X,
		GL_TEXTURE_CUBE_MAP_NEGATIVE_X,
		GL_TEXTURE_CUBE_MAP_POSITIVE_Y,
		GL_TEXTURE_CUBE_MAP_NEGATIVE_Y };
	SDL_Surface *tmpSurface;

	glBindTexture(GL_TEXTURE_CUBE_MAP, *texID); // bind texture and set parameters
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	GLuint externalFormat;
	for (int i = 0; i<6; i++)
	{
		// load file - using core SDL library
		tmpSurface = SDL_LoadBMP(fname[i]);
		if (!tmpSurface)
		{
			std::cout << "\nError loading bitmap " << fname[i] << " when Load::CubeMap function \n";
			return *texID;
		}

		// skybox textures should not have alpha (assuming this is true!)
		SDL_PixelFormat *format = tmpSurface->format;
		externalFormat = (format->Rmask < format->Bmask) ? GL_RGB : GL_BGR;

		glTexImage2D(sides[i], 0, GL_RGB, tmpSurface->w, tmpSurface->h, 0,
			externalFormat, GL_UNSIGNED_BYTE, tmpSurface->pixels);
		// texture loaded, free the temporary buffer
		SDL_FreeSurface(tmpSurface);
	}
	return *texID;	// return value of texure ID, redundant really
}

GLuint Load::BMP(char *fname) {
	GLuint texID;
	glGenTextures(1, &texID); // generate texture ID

	// load file - using core SDL library
	SDL_Surface *tmpSurface;
	tmpSurface = SDL_LoadBMP(fname);
	if (!tmpSurface) {
		std::cout << "\nError loading bitmap " << fname << " when calling Load::BMP function \n";
	}

	// bind texture and set parameters
	glBindTexture(GL_TEXTURE_2D, texID);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	SDL_PixelFormat *format = tmpSurface->format;

	GLuint externalFormat, internalFormat;
	if (format->Amask) {
		internalFormat = GL_RGBA;
		externalFormat = (format->Rmask < format->Bmask) ? GL_RGBA : GL_BGRA;
	}
	else {
		internalFormat = GL_RGB;
		externalFormat = (format->Rmask < format->Bmask) ? GL_RGB : GL_BGR;
	}

	glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, tmpSurface->w, tmpSurface->h, 0,
		externalFormat, GL_UNSIGNED_BYTE, tmpSurface->pixels);
	glGenerateMipmap(GL_TEXTURE_2D);

	SDL_FreeSurface(tmpSurface); // texture loaded, free the temporary buffer
	if (texID != 0) std::cout << "Loaded texture " << fname << " successfully\n";
	return texID;	// return value of texture ID
}

GLuint Load::PNG(char *fname) {
	// load file - using core SDL library
	int imgFlags = IMG_INIT_PNG;
	if (!(IMG_Init(imgFlags) & imgFlags))
	{
		printf("SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError());
		return 0;
	}
	else {
		GLuint texID;
		glGenTextures(1, &texID); // generate texture ID

		SDL_Surface *tmpSurface;
		tmpSurface = IMG_Load(fname);
		if (!tmpSurface) {
			std::cout << "\nError loading image file " << fname << " when calling Load::PNG function \n";
		}

		// bind texture and set parameters
		glBindTexture(GL_TEXTURE_2D, texID);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

		SDL_PixelFormat *format = tmpSurface->format;

		GLuint externalFormat, internalFormat;
		if (format->Amask) {
			internalFormat = GL_RGBA;
			externalFormat = (format->Rmask < format->Bmask) ? GL_RGBA : GL_BGRA;
		}
		else {
			internalFormat = GL_RGB;
			externalFormat = (format->Rmask < format->Bmask) ? GL_RGB : GL_BGR;
		}

		glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, tmpSurface->w, tmpSurface->h, 0,
			externalFormat, GL_UNSIGNED_BYTE, tmpSurface->pixels);
		glGenerateMipmap(GL_TEXTURE_2D);

		SDL_FreeSurface(tmpSurface); // texture loaded, free the temporary buffer
		if (texID != 0) std::cout << "\nLoaded image " << fname;
		return texID;	// return value of texture ID
	}
}

// just a wrapper for Daniel Livingstone`s loader to which I added Pablo Casaseca`s tangent loader;
GLuint Load::Obj(const char* filename)
{
	Clean();
	rt3d::loadObj(filename,
		Load::verts,
		Load::norms,
		Load::tex_coords,
		Load::indices);

	Load::m_meshCount = Load::indices.size();

	GLuint result = rt3d::createMesh(Load::verts.size() / 3,
		Load::verts.data(), nullptr,
		Load::norms.data(),
		Load::tex_coords.data(),
		Load::m_meshCount,
		Load::indices.data());

	// only generate tangents if we have the coordinates
	if (tex_coords.size()) generateTangents();

	glBindVertexArray(result);
	{
		GLuint tangentVBO;
		glGenBuffers(1, &tangentVBO);
		// VBO for tangent data
		glBindBuffer(GL_ARRAY_BUFFER, tangentVBO);
		glBufferData(GL_ARRAY_BUFFER, Load::tangents.size()*sizeof(GLfloat), Load::tangents.data(), GL_STATIC_DRAW);
		glEnableVertexAttribArray(3);
		glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, 0, 0);

	}
	glBindVertexArray(0);
	return result;
}

char* Load::File(const char* filename)
{
	int size;
	char * memblock;

	std::ifstream file(filename, std::ios::in | std::ios::binary | std::ios::ate);
	if (file.is_open()) {
		size = (int)file.tellg(); // get location of file pointer i.e. file size
		m_fileSize = (GLint)size;
		memblock = new char[size];
		file.seekg(0, std::ios::beg);
		file.read(memblock, size);
		file.close();
		std::cout << "file " << filename << " loaded\n";
	}
	else {
		std::cerr << "Unable to open file " << filename << "\n";
		m_fileSize = 0;
		// should ideally set a flag or use exception handling
		// so that calling function can decide what to do now
		return nullptr;
	}
	return memblock;
}

GLuint Load::Shader(const char* filename)
{
	GLuint toReturn = glCreateProgram();
	GLuint vertexShader;
	GLuint fragmentShader;

	// create respective file paths
	std::string vertShaderCompleteFileName = filename;
	vertShaderCompleteFileName.append(".vert");
	std::string fragShaderCompleteFileName = filename;
	fragShaderCompleteFileName.append(".frag");
	vertexShader = CreateShader(vertShaderCompleteFileName.c_str(), GL_VERTEX_SHADER);
	fragmentShader = CreateShader(fragShaderCompleteFileName.c_str(), GL_FRAGMENT_SHADER);

	glAttachShader(toReturn, vertexShader);
	glAttachShader(toReturn, fragmentShader);
	
	glBindAttribLocation(toReturn, 0, "VertexPosition");
	glBindAttribLocation(toReturn, 1, "VertexNormal");
	glBindAttribLocation(toReturn, 2, "VertexTexCoord");
	glBindAttribLocation(toReturn, 3, "VertexTangent");
	glBindAttribLocation(toReturn, 4, "VertexColour");
	
	// link the program and check for linking errors and validation errors
	glLinkProgram(toReturn);
	CheckShaderError(toReturn, GL_LINK_STATUS, true, "Failed to link program...\n");
	glValidateProgram(toReturn);
	CheckShaderError(toReturn, GL_VALIDATE_STATUS, true, "Failed to validate program... Invalid shader program\n");

	return toReturn;
}