#pragma once
#include "Object.h"
#include "Camera.h"
#include "Sound.h"

#define KG 1.0f
#define S 0.01666666666f
#define M 1.0f;

class Track;

class Wheel :
	public Object
{
public:
	Wheel();

	void Initialize();
	void Render();
	void Update();
	void HandleEvents();

	void UseModel(GLuint Model) { model = Model; };
	void UseMaterial(Material m)	{ material = m; };
	void UseIndex(unsigned int meshCount) { indexCount = meshCount; };
	void UseLight(Light& l) {light = l; };

	glm::vec3 CameraPosition() {return cameraPosition; };
	glm::vec3 CameraAt()		{ return cameraAt; }

	glm::vec3& Position()	{ return position; };
	glm::vec3& Scale()		{ return scale; };
	GLuint& Model()			{ return model; };
	unsigned int &Index()	{ return indexCount; };
	float Radius()			{ return RADIUS; }
	float& Velocity()		{ return velocity; }
	double& StartTimer()	{ return startTime; }
	int& Gear()				{ return currentGear; }
	
	void AddDamage()		{ damage += damageAmount; }
	void AddSpeedPenalty()	{ velocity *= VelocityPenalty; }
	void DetachCamera()		{ detachedCamera = true; }
	void DetachControl()	{ currentGear = NEUTRAL; canRace = false; }

	void ApplySensibility()		{ damageAmount = 0.3f; }
	void ApplyHappyWheel()		{ boostTurn = 2.0f; }
	void ApplyInstantStop()		{ velocity = 0.0f; }
	void ApplyRepair()			{ damage = 0; }
	void ApplyReverseControls() { reverseControls = true; }
	void ApplyShield()			{ damageAmount = 0; }
	void ApplyTurbo()			{ boostTurbo = 2.0f; }

	void UndoSensibility()		{ damageAmount = 0.01f; }
	void UndoHappyWheel()		{ boostTurn = 1.0f; }
	void UndoReverseControls()	{ reverseControls = false; }
	void UndoShield()			{ damageAmount = 0.01f; }
	void UndoTurbo()			{ boostTurbo = 1.0f; }

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
	
	Track *raceTrack;

	const float Cdrag = 0.9f;	// drag coefficient
	const float Crolling = Cdrag * 10.0f; // rolling coefficient
	const float Cbraking = 10.0f;
	
	const float RHOAIR = 1.29f; // density of air
	const float DT = 0.016f;
	const unsigned int NEUTRAL = 0;
	const unsigned int REVERSE = 7;
	const float DIFFERENTIALRATIO = 3.42f;
	const float CAMERACOLLISIONSPEED = 0.05f;
	const float CAMERAMAXIMUMDISPLACEMENT = 0.5f;

	float gears[8];		// 6 gears + reverse + neutral
	int currentGear;
	int EnginePower;
	float F_long;				// longitudinal force
	float F_traction;			// traction force
	float F_braking;			// braking force
	float F_drag;				// drag force
	float F_rr;					// rolling resistance force
	float u;					// car heading

	float Trigger;
	float rotationAngle;
	float velocity;				// Velocity
	float Acceleration;			// Acceleration

	float damage;
	
	GLuint model;
	GLuint rubber;
	Light light;
	Material material;
	unsigned int indexCount;
	glm::vec3 position;
	glm::vec3 scale;
	
	float direction;
	float cameraTurn;

	glm::vec3 cameraPosition;
	glm::vec3 cameraAt;
	float cameraHeight;
	float cameraZoom;
	float cameraWidthX;
	float cameraWidthZ;
	bool hugeZoom;
	float zoomAmount;

	double startTime;

	bool canGearUp;
	bool canGearDown;
	bool detachedCamera;			// used when passing the finish line


	void initializePhysics();
	void updatePhysics();
	void gearUp();
	void gearDown();

	float boostTurbo = 1.0f;
	float boostTurn = 1.0f;
	float damageAmount = 0.01f;
	bool reverseControls;
	bool automaticGearBox;
	bool canControl;
	bool canRace;
};

