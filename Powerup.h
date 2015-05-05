#include "Object.h"
#include "Wheel.h"

class Powerup :
	public Object
{
public:
	Powerup() {};
	Powerup(Wheel * w, int type, glm::vec3 Position, int (&tex)[7]);
	void AttachToWheel(Wheel * w) { wheel = w; }
	void Initialize();
	void Render();
	void Update();
	void HandleEvents();

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
	bool contact();
	void ApplySensibility();
	void ApplyHappyWheel();
	void ApplyInstantStop();
	void ApplyRepair();
	void ApplyReverseControls();
	void ApplyShield();
	void ApplyTurbo();


	Wheel *wheel;
	Cube *Powercube;
	GLuint model;
	GLuint texture[7];

	Material material;
	unsigned int indexCount;
	glm::vec3 position;
	glm::vec3 scale;
	float rotationSpeed = 0.0f;
	unsigned alarm;
	int timer;
	int type;
	bool visible;
};
