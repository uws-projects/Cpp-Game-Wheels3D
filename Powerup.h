#include "Object.h"
#include <GL\glew.h>
#include <glm.hpp>
#include <SDL.h>
#include "Material.h"

class Powerup :
	public Object
{
public:
	Powerup();
	void Initialize();
	void Render();
	void Update();
	void HandleEvents();
	void Create(char *type, glm::vec3 PowerPosition);

	void UseModel(GLuint Model) { model = Model; };
	void UseMaterial(Material m)	{ material = m; };
	void UseIndex(unsigned int meshCount) { indexCount = meshCount; };
	glm::vec3& Position()	{ return position; };
	glm::vec3& Scale()		{ return scale; };
	GLuint& Model()			{ return model; };
	unsigned int &Index()	{ return indexCount; };
	~Powerup();
protected:
private:
	void ApplyFog();
	void ApplyHappyWheel();
	void ApplyInstantStop();
	void ApplyJump();
	void ApplyOilDrop();
	void ApplyRepair();
	void ApplyReverseControls();
	void ApplyShield();
	void ApplySpikeStrip();
	void ApplyTurbo();
	void ApplyTwister();

	Cube *Powercube;
	GLuint model;
	GLuint texture;
	Material material;
	unsigned int indexCount;
	glm::vec3 position;
	glm::vec3 scale;
	float rotationSpeed = 0.0f;
};
