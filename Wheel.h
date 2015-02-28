#pragma once
#include "Object.h"
#include "Camera.h"

#define KG 1.0f
#define S 0.01666666666f
#define M 1.0f;


class Wheel :
	public Object
{
public:
	Wheel();

	void Initialize();
	void Render();
	void Update();
	void HandleEvents();

	void useModel(GLuint Model) { model = Model; };
	void useMaterial(Material m)	{ material = m; };
	void useIndex(unsigned int meshCount) { indexCount = meshCount; };
	void useLight(Light& l) {light = l; };

	glm::vec3 CameraPosition() {return cameraPosition; };
	glm::vec3 CameraAt()		{ return cameraAt; }

	glm::vec3& Position()	{ return position; };
	glm::vec3& Scale()		{ return scale; };
	GLuint& Model()			{ return model; };
	unsigned int &Index()	{ return indexCount; };
	float Radius()			{ return RADIUS; }

	~Wheel();

protected:
private:
	/* 
	wheel is 205/55/R16
		tyre width = 205 mm = 0.205 m;
		tyre aspect ratio = 55 mm = 0.055 m;
		rim diameter = 16" = 40.64 cm = 0.4064 m;
	*/
	const float tyreWidth = 0.205f;
	const float tyreAspectRatio = 0.055f;
	const float rimDiameter = 0.4064f;
	const float tyreDiameter = rimDiameter + 2.0f * tyreAspectRatio;
	const float RADIUS = tyreDiameter/2;
	const float AREA = tyreWidth * tyreDiameter;
	const float MASS = 8.0f;
		
	
	const float Cdrag = 0.9f;	// drag coefficient
	const float Crolling = Cdrag * 10.0f; // rolling coefficient
	const float Cbraking = 1.0f;
	
	const float RHOAIR = 1.29f; // density of air
	const float DT = 0.016f;
	const float NEUTRAL = 0;
	const float REVERSE = 7;
	const float DIFFERENTIALRATIO = 3.42f;

	float gears[8];		// 6 gears + reverse + neutral
	unsigned currentGear;
	float EnginePower;
	float F_long;				// longitudinal force
	float F_traction;			// traction force
	float F_braking;			// braking force
	float F_drag;				// drag force
	float F_rr;					// rolling resistance force
	float u;					// car heading

	float Trigger;
	float rotationAngle;
	float Velocity;				// Velocity
	float Acceleration;			// Acceleration
	
	GLuint model;
	GLuint rubber;
	Light light;
	Material material;
	unsigned int indexCount;
	glm::vec3 position;
	glm::vec3 scale;
	glm::vec3 lastPosition;
	
	float direction;
	float cameraTurn;

	glm::vec3 cameraPosition;
	glm::vec3 cameraAt;
	float cameraHeight;
	float cameraZoom;
	bool editingCameraPosition;
	bool hugeZoom;
	bool collidedLeftX = false;
	bool collidedRightX = false;
	bool collidedLeftZ = false;
	bool collidedRightZ = false;
	float zoomAmount;

	bool canGearUp;
	bool canGearDown;

	Camera firstPersonCamera;
	
	int trackSection;

	void checkTrack(int &section);
	void checkCollision();
	void initializePhysics();
	void updatePhysics();
	void gearUp();
	void gearDown();
	bool contacts(float& left, float& right, float& top, float &bottom, float& x, float &z);

	std::vector<float> leftCollisionMap;
	std::vector<float> rightCollisionMap;
};

