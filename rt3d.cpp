#include "rt3d.h"
#include <map>

using namespace std;

namespace rt3d {

// struct vaoBuffers will be used inside the rt3d library
// clients should not need to know about this data structure
struct vaoBuffers {
	GLuint vertex_buffer;
	GLuint colour_buffer;
	GLuint normal_buffer;
	GLuint texcoord_buffer;
	GLuint index_buffer;
};

static map<GLuint, GLuint *> vertexArrayMap;

// Something went wrong - print error message and quit
void exitFatalError(const char *message) {
    cout << message << " ";
    exit(1);
}

// loadFile - loads text file from file fname as a char* 
// Allocates memory - so remember to delete after use
// size of file returned in fSize
char* loadFile(const char *fname, GLint &fSize) {
	int size;
	char * memblock;

	// file read based on example in cplusplus.com tutorial
	// ios::ate opens file at the end
	ifstream file (fname, ios::in|ios::binary|ios::ate);
	if (file.is_open()) {
		size = (int) file.tellg(); // get location of file pointer i.e. file size
		fSize = (GLint) size;
		memblock = new char [size];
		file.seekg (0, ios::beg);
		file.read (memblock, size);
		file.close();
		cout << "file " << fname << " loaded" << endl;
	}
	else {
		cout << "Unable to open file " << fname << endl;
		fSize = 0;
		// should ideally set a flag or use exception handling
		// so that calling function can decide what to do now
		return nullptr;
	}
	return memblock;
}

// printShaderError
// Display (hopefully) useful error messages if shader fails to compile or link
void printShaderError(const GLint shader) {
	int maxLength = 0;
	int logLength = 0;
	GLchar *logMessage;

	// Find out how long the error message is
	if (!glIsShader(shader))
		glGetProgramiv(shader, GL_INFO_LOG_LENGTH, &maxLength);
	else
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);

	if (maxLength > 0) { // If message has some contents
		logMessage = new GLchar[maxLength];
		if (!glIsShader(shader))
			glGetProgramInfoLog(shader, maxLength, &logLength, logMessage);
		else
			glGetShaderInfoLog(shader,maxLength, &logLength, logMessage);
		cout << "Shader Info Log:" << endl << logMessage << endl;
		delete [] logMessage;
	}
	// should additionally check for OpenGL errors here
}


GLuint initShaders(const char *vertFile, const char *fragFile) {
	GLuint p, f, v;

	char *vs,*fs;

	v = glCreateShader(GL_VERTEX_SHADER);
	f = glCreateShader(GL_FRAGMENT_SHADER);	

	// load shaders & get length of each
	GLint vlen;
	GLint flen;
	vs = loadFile(vertFile,vlen);
	fs = loadFile(fragFile,flen);
	
	const char * vv = vs;
	const char * ff = fs;

	glShaderSource(v, 1, &vv,&vlen);
	glShaderSource(f, 1, &ff,&flen);
	
	GLint compiled;

	glCompileShader(v);
	glGetShaderiv(v, GL_COMPILE_STATUS, &compiled);
	if (!compiled) {
		cout << "Vertex shader not compiled." << endl;
		rt3d::printShaderError(v);
	} 

	glCompileShader(f);
	glGetShaderiv(f, GL_COMPILE_STATUS, &compiled);
	if (!compiled) {
		cout << "Fragment shader not compiled." << endl;
		rt3d::printShaderError(f);
	} 
	
	p = glCreateProgram();
		
	glAttachShader(p,v);
	glAttachShader(p,f);

	glBindAttribLocation(p,RT3D_VERTEX,"in_Position");
	glBindAttribLocation(p,RT3D_NORMAL,"in_Normal");
	glBindAttribLocation(p,RT3D_TEXCOORD,"in_TexCoord");
	glBindAttribLocation(p, RT3D_COLOUR, "in_Colour");


	glLinkProgram(p);
	glUseProgram(p);

	delete [] vs; // dont forget to free allocated memory
	delete [] fs; // we allocated this in the loadFile function...

	return p;
}

GLuint createMesh(const GLuint numVerts, const GLfloat* vertices, const GLfloat* colours, 
	const GLfloat* normals, const GLfloat* texcoords, const GLuint indexCount, const GLuint* indices) {
	GLuint VAO;
	// generate and set up a VAO for the mesh
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	GLuint *pMeshBuffers = new GLuint[5];


	if (vertices == nullptr) {
		// cant create a mesh without vertices... oops
		exitFatalError("Attempt to create a mesh with no vertices");
	}

	// generate and set up the VBOs for the data
	GLuint VBO;
	glGenBuffers(1, &VBO);
	
	// VBO for vertex data
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, 3*numVerts*sizeof(GLfloat), vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0); 
	glEnableVertexAttribArray(0);
	pMeshBuffers[0] = VBO;


	// VBO for colour data
	if (colours != nullptr) {
		glGenBuffers(1, &VBO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, 3*numVerts*sizeof(GLfloat), colours, GL_STATIC_DRAW);
		glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(4);
		pMeshBuffers[4] = VBO;
	}

	// VBO for normal data
	if (normals != nullptr) {
		glGenBuffers(1, &VBO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, 3*numVerts*sizeof(GLfloat), normals, GL_STATIC_DRAW);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(1);
		pMeshBuffers[1] = VBO;
	}

	// VBO for tex-coord data
	if (texcoords != nullptr) {
		glGenBuffers(1, &VBO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, 2*numVerts*sizeof(GLfloat), texcoords, GL_STATIC_DRAW);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(2);
		pMeshBuffers[2] = VBO;
	}

	if (indices != nullptr && indexCount > 0) {
		glGenBuffers(1, &VBO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, VBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexCount * sizeof(GLuint), indices, GL_STATIC_DRAW);
		pMeshBuffers[RT3D_INDEX] = VBO;
	}
	// unbind vertex array
	glBindVertexArray(0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	// return the identifier needed to draw this mesh

	vertexArrayMap.insert( pair<GLuint, GLuint *>(VAO, pMeshBuffers) );

	return VAO;
}

GLuint createMesh(const GLuint numVerts, const GLfloat* vertices, const GLfloat* colours, 
	const GLfloat* normals, const GLfloat* texcoords) {
	return createMesh(numVerts, vertices, colours, normals, texcoords, 0, nullptr);
}

GLuint createMesh(const GLuint numVerts, const GLfloat* vertices) {
	return createMesh(numVerts, vertices, nullptr, nullptr, nullptr);
}

GLuint createColourMesh(const GLuint numVerts, const GLfloat* vertices, const GLfloat* colours) {
	return createMesh(numVerts, vertices, colours, nullptr, nullptr);
}

void setUniformMatrix4fv(const GLuint program, const char* uniformName, const GLfloat *data) {
	int uniformIndex = glGetUniformLocation(program, uniformName);
	glUniformMatrix4fv(uniformIndex, 1, GL_FALSE, data); 
}

// set matrices untested... likely to change - not totally happy with this for now.
void setMatrices(const GLuint program, const GLfloat *proj, const GLfloat *mv, const GLfloat *mvp) {
	int uniformIndex = glGetUniformLocation(program, "modelview");
	glUniformMatrix4fv(uniformIndex, 1, GL_FALSE, mv);
	uniformIndex = glGetUniformLocation(program, "projection");
	glUniformMatrix4fv(uniformIndex, 1, GL_FALSE, proj);
	uniformIndex = glGetUniformLocation(program, "MVP");
	glUniformMatrix4fv(uniformIndex, 1, GL_FALSE, mvp);
	uniformIndex = glGetUniformLocation(program, "normalmatrix");
	GLfloat nm[9] = {	mvp[0], mvp[1], mvp[2],
						mvp[4], mvp[5], mvp[6],
						mvp[8], mvp[8], mvp[20] };	
	glUniformMatrix4fv(uniformIndex, 1, GL_FALSE, nm);
}

void setLightPos(const GLuint program, const GLfloat *lightPos) {
	int uniformIndex = glGetUniformLocation(program, "lightPosition");
	glUniform4fv(uniformIndex, 1, lightPos);
}

void setProjection(const GLuint program, const GLfloat *data) {
	int uniformIndex = glGetUniformLocation(program, "projection");
	glUniformMatrix4fv(uniformIndex, 1, GL_FALSE, data); 
}


void setLight(const GLuint program, const lightStruct light) {
	// pass in light data to shader
	int uniformIndex = glGetUniformLocation(program, "light.ambient");
	glUniform4fv(uniformIndex, 1, light.ambient);
	uniformIndex = glGetUniformLocation(program, "light.diffuse");
	glUniform4fv(uniformIndex, 1, light.diffuse);
	uniformIndex = glGetUniformLocation(program, "light.specular");
	glUniform4fv(uniformIndex, 1, light.specular);
	uniformIndex = glGetUniformLocation(program, "light.position");
	glUniform4fv(uniformIndex, 1, light.position);
}

void setLight(const GLuint program, const Light l) {
	// pass in light data to shader
	int uniformIndex = glGetUniformLocation(program, "light.ambient");
	glUniform4fv(uniformIndex, 1, &l.La[0]);
	uniformIndex = glGetUniformLocation(program, "light.diffuse");
	glUniform4fv(uniformIndex, 1, &l.Ld[0]);
	uniformIndex = glGetUniformLocation(program, "light.specular");
	glUniform4fv(uniformIndex, 1, &l.Ls[0]);
	uniformIndex = glGetUniformLocation(program, "light.position");
	glUniform4fv(uniformIndex, 1, &l.position[0]);
}


void setMaterial(const GLuint program, const materialStruct material) {
	// pass in material data to shader 
	int uniformIndex = glGetUniformLocation(program, "material.ambient");
	glUniform4fv(uniformIndex, 1, material.ambient);
	uniformIndex = glGetUniformLocation(program, "material.diffuse");
	glUniform4fv(uniformIndex, 1, material.diffuse);
	uniformIndex = glGetUniformLocation(program, "material.specular");
	glUniform4fv(uniformIndex, 1, material.specular);
	uniformIndex = glGetUniformLocation(program, "material.shininess");
	glUniform1f(uniformIndex, material.shininess);
}


void setMaterial(const GLuint program, const Material m) {
	int uniformIndex = glGetUniformLocation(program, "material.ambient");
	glUniform4fv(uniformIndex, 1, &m.Ka[0]);
	uniformIndex = glGetUniformLocation(program, "material.diffuse");
	glUniform4fv(uniformIndex, 1, &m.Kd[0]);
	uniformIndex = glGetUniformLocation(program, "material.specular");
	glUniform4fv(uniformIndex, 1, &m.Ks[0]);
	uniformIndex = glGetUniformLocation(program, "material.shininess");
	glUniform1f(uniformIndex, m.shininess);
}


void drawMesh(const GLuint mesh, const GLuint numVerts, const GLuint primitive) {
	glBindVertexArray(mesh);	// Bind mesh VAO
	glDrawArrays(primitive, 0, numVerts);	// draw first vertex array object
	glBindVertexArray(0);
}


void drawIndexedMesh(const GLuint mesh, const GLuint indexCount, const GLuint primitive) {
	glBindVertexArray(mesh);	// Bind mesh VAO
	glDrawElements(primitive, indexCount,  GL_UNSIGNED_INT, 0);	// draw VAO 
	glBindVertexArray(0);
}


void updateMesh(const GLuint mesh, const unsigned int bufferType, const GLfloat *data, const GLuint size) {
	GLuint * pMeshBuffers = vertexArrayMap[mesh];
	glBindVertexArray(mesh);

	// Delete the old buffer data
	glDeleteBuffers(1, &pMeshBuffers[bufferType]);

	// generate and set up the VBOs for the new data
	GLuint VBO;
	glGenBuffers(1, &VBO);
		// VBO for the data
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, size*sizeof(GLfloat), data, GL_STATIC_DRAW);
	glVertexAttribPointer((GLuint)bufferType, 3, GL_FLOAT, GL_FALSE, 0, 0); 
	glEnableVertexAttribArray(bufferType);
	pMeshBuffers[RT3D_VERTEX] = VBO;

	glBindVertexArray(0);

}

} // namespace rt3d