#include "Shader.h"
#include "Load.h"
#include "settings.h"

GLuint Shader::DPVSL;
GLuint Shader::BUMP;
GLuint Shader::SCRELEM;
GLuint Shader::SKYBOX;
GLuint Shader::activeProgram = Shader::DPVSL;
std::stack<glm::mat4> Shader::m_matrix;

glm::mat3	Shader::m_NormalMatrix;
glm::mat4	Shader::m_IdentityMatrix;
glm::mat4	Shader::m_ProjectionMatrix;
glm::mat4	Shader::m_ViewMatrix;
glm::mat4	Shader::m_ModelViewMatrix;


Shader::Shader()
{
	Shader::Start();
}

void Shader::Start()
{
	BUMP		= Load::Shader(".\\shaders\\parallax");
	SCRELEM		= Load::Shader(".\\shaders\\texture");
	DPVSL		= Load::Shader(".\\shaders\\diffuse");
	SKYBOX		= Load::Shader(".\\shaders\\skybox");
	// set default shader here:
	activeProgram = SKYBOX;
	Use(activeProgram);

	m_IdentityMatrix = glm::mat4(1.0);
	m_ViewMatrix = m_IdentityMatrix;
	m_ModelViewMatrix = m_IdentityMatrix;
	m_ProjectionMatrix = glm::perspective(60.0f, WIDTH/ HEIGHT, 0.001f, 1500.0f);
	m_NormalMatrix = glm::transpose(glm::inverse(glm::mat3(m_ModelViewMatrix)));
}

void Shader::AddMaterial(Material m)
{
	GLuint uniformLocation;
	uniformLocation = glGetUniformLocation(activeProgram, "material.shininess");
	glUniform4fv(uniformLocation, 1, &m.shininess);
	uniformLocation = glGetUniformLocation(activeProgram, "material.Ka");
	glUniform4fv(uniformLocation, 1, &m.Ka[0]);
	uniformLocation = glGetUniformLocation(activeProgram, "material.Kd");
	glUniform4fv(uniformLocation, 1, &m.Kd[0]);
	uniformLocation = glGetUniformLocation(activeProgram, "material.Ks");
	glUniform4fv(uniformLocation, 1, &m.Ks[0]);
}

void Shader::AddLight(Light l)
{
	GLuint uniformLocation;
	uniformLocation = glGetUniformLocation(activeProgram, "light.position");
	glUniform4fv(uniformLocation, 1, &l.position[0]);
	uniformLocation = glGetUniformLocation(activeProgram, "light.La");
	glUniform4fv(uniformLocation, 1, &l.La[0]);
	uniformLocation = glGetUniformLocation(activeProgram, "light.Ld");
	glUniform4fv(uniformLocation, 1, &l.Ld[0]);
	uniformLocation = glGetUniformLocation(activeProgram, "light.Ls");
	glUniform4fv(uniformLocation, 1, &l.Ls[0]);
}

void Shader::AddLight(Light l, std::string name)
{	
	GLuint uniformLocation;
	uniformLocation = glGetUniformLocation(activeProgram, (name+".position").c_str());
	glUniform4fv(uniformLocation, 1, &l.position[0]);
	uniformLocation = glGetUniformLocation(activeProgram, (name + ".La").c_str());
	glUniform4fv(uniformLocation, 1, &l.La[0]);
	uniformLocation = glGetUniformLocation(activeProgram, (name + ".Ld").c_str());
	glUniform4fv(uniformLocation, 1, &l.Ld[0]);
	uniformLocation = glGetUniformLocation(activeProgram, (name + ".Ls").c_str());
	glUniform4fv(uniformLocation, 1, &l.Ls[0]);
	uniformLocation = glGetUniformLocation(activeProgram, (name + ".constant").c_str());
	glUniform1f(uniformLocation, l.constant);
	uniformLocation = glGetUniformLocation(activeProgram, (name + ".linear").c_str());
	glUniform1f(uniformLocation, l.linear);
	uniformLocation = glGetUniformLocation(activeProgram, (name + ".quadratic").c_str());
	glUniform1f(uniformLocation, l.quadratic);
}

void Shader::SetUniform(const char *name, float x, float y, float z)
{
	GLuint loc = glGetUniformLocation(activeProgram, name);
	glUniform3f(loc, x, y, z);
}

void Shader::SetUniform(const char *name, const glm::vec3& v)
{
	GLuint loc = glGetUniformLocation(activeProgram, name);
	glUniform3f(loc, v.x, v.y, v.z);
}

void Shader::SetUniform(const char *name, const glm::vec4& v)
{
	GLuint loc = glGetUniformLocation(activeProgram, name);
	glUniform4f(loc, v.x, v.y, v.z, v.w);
}

void Shader::SetUniform(const char *name, const glm::vec2& v)
{
	GLuint loc = glGetUniformLocation(activeProgram, name);
	glUniform2f(loc, v.x, v.y);
}

void Shader::SetUniform(const char *name, const glm::mat4& m)
{
	GLuint loc = glGetUniformLocation(activeProgram, name);
	glUniformMatrix4fv(loc, 1, GL_FALSE, &m[0][0]);
}

void Shader::SetUniform(const char *name, const glm::mat3& m)
{
	GLuint loc = glGetUniformLocation(activeProgram, name);
	glUniformMatrix3fv(loc, 1, GL_FALSE, &m[0][0]);
}

void Shader::SetUniform(const char *name, float val)
{
	GLuint loc = glGetUniformLocation(activeProgram, name);
	glUniform1f(loc, val);
}

void Shader::SetUniform(const char *name, int val)
{
	GLuint loc = glGetUniformLocation(activeProgram, name);
	glUniform1i(loc, val);
}

void Shader::SetUniform(const char *name, GLuint val)
{
	GLuint loc = glGetUniformLocation(activeProgram, name);
	glUniform1ui(loc, val);
}

void Shader::SetUniform(const char *name, bool val)
{
	GLuint loc = glGetUniformLocation(activeProgram, name);
	glUniform1i(loc, val);
}

void Shader::Bind(int textureUnit, const char *name, int textureHandle)
{
	int uniformIndex = glGetUniformLocation(activeProgram, name);
	glUniform1i(uniformIndex, textureUnit);
	glActiveTexture(GL_TEXTURE0+ textureUnit);
	glBindTexture(GL_TEXTURE_2D, textureHandle);
}