#pragma once
#include <GL\glew.h>
#include <stack>
#include "rt3d.h"
#include "Material.h"


class Shader
{
public:
	Shader();
	static void Start();
	// shader methods
	static void Use(GLuint program)			{ glUseProgram(0); activeProgram = program;  glUseProgram(activeProgram); }
	static GLuint& DiffusePerVertex()		{ return DPVSL; }
	static GLuint& Bump()					{ return BUMP; }
	static GLuint& ScreenHUD()				{ return SCRELEM; }
	static GLuint& Skybox()					{ return SKYBOX; }
	static GLuint& Current()				{ return activeProgram; }
	
	// transformation methods
	
	static glm::mat4& Identity()			{ return m_IdentityMatrix; }	
	static glm::mat3& NormalMatrix()		{ return m_NormalMatrix; }
	static glm::mat4& ProjectionMatrix()	{ return m_ProjectionMatrix; }
	static glm::mat4& ModelViewMatrix()		{ return m_matrix.top(); }
	static glm::mat4& MVP()					{ return m_ProjectionMatrix*Shader::Top(); }

	// matrix methods
	static void Pop()						{ m_matrix.pop(); if (m_matrix.size()) m_ModelViewMatrix = Shader::Top(); }
	static void Push(glm::mat4 mat)			{ m_matrix.push(mat); m_ModelViewMatrix = Shader::Top(); }
	static void Push()						{ m_matrix.push(Shader::Top()); m_ModelViewMatrix = Shader::Top(); }
	static glm::mat4& Top()					{ return m_matrix.top(); }
	
	// uniform methods
	static void AddLight(Light l);
	static void AddLight(Light l, std::string name);
	static void AddMaterial(Material m);
	
	static void SetUniform(const char *name, float value1, float value2, float value3);
	static void SetUniform(const char *name, const glm::vec2& vector2float);
	static void SetUniform(const char *name, const glm::vec3& vector3float);
	static void SetUniform(const char *name, const glm::vec4& vector4float);
	static void SetUniform(const char *name, const glm::mat4& matrix4x4);
	static void SetUniform(const char *name, const glm::mat3& matrix3x3);
	static void SetUniform(const char *name, float FloatValue);
	static void SetUniform(const char *name, int IntegerValue);
	static void SetUniform(const char *name, GLuint unsignedInteger);
	static void SetUniform(const char *name, bool boolean);
	static void Bind(int textureUnit, const char *name, int textureHandle);

private:

	static GLuint DPVSL;			// diffuse per vertex shading lighting
	static GLuint BUMP;				// parallax
	static GLuint SCRELEM;			// HUD shader
	static GLuint SKYBOX;			// skybox shader
	static GLuint activeProgram;
	static std::stack<glm::mat4> m_matrix;

	static glm::mat3	m_NormalMatrix;
	static glm::mat4	m_IdentityMatrix;
	static glm::mat4	m_ProjectionMatrix;
	static glm::mat4	m_ViewMatrix;
	static glm::mat4	m_ModelViewMatrix;
};

