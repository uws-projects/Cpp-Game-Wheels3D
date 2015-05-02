#pragma once
#include "Object.h"
#include "Camera.h"
#include "Sound.h"
#include <vector>

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
	
	void AddLeftDamage()				{ leftDamage += damageAmount; }
	void AddRightDamage()				{ rightDamage += damageAmount; }
	void AddSpeedPenalty()				{ velocity *= velocityPenalty; }
	void DetachCamera()					{ detachedCamera = true; }
	void DetachControl()				{ currentGear = NEUTRAL; canRace = false; }
	void FinishRace()					{ raceComplete = true; }
	bool IsRaceFinished()				{ return raceComplete; }
	void SetFinalTime(float result)		{ finalTime = result; }
	float RaceResult()					{ return finalTime; }
	int &GetLeftLevel()					{ return leftDamageLevel; }
	int &GetRightLevel()				{ return rightDamageLevel; }
	int GetDamage()						{ return (int)damage; }
	int &GetGearStatus()				{ return gearStatus; }
	void ApplySensibility()				{ damageAmount = 0.3f;			SOUND->Sensibility(SampleVolume); sensibility = true; }
	void ApplyHappyWheel()				{ boostTurn = 2.0f;				SOUND->Happy(SampleVolume); happyWheel = true; }
	void ApplyInstantStop()				{ velocity = 0.0f;				SOUND->Stop(SampleVolume); }
	void ApplyRepair()					{ leftDamage = rightDamage = 0; SOUND->Repair(SampleVolume); }
	void ApplyReverseControls()			{ reverseControls = true;		SOUND->Reverse(SampleVolume); }
	void ApplyShield()					{ damageAmount = 0;	velocityPenalty = 1.0f;	SOUND->Shield(SampleVolume); shield = true; }
	void ApplyTurbo()					{ boostTurbo = 2.0f;			SOUND->Turbo(SampleVolume); turbo = true; }

	void UndoSensibility()		{ damageAmount = 0.05f; sensibility = false; }
	void UndoHappyWheel()		{ boostTurn = 1.0f; happyWheel = false; }
	void UndoReverseControls()	{ reverseControls = false; }
	void UndoShield()			{ damageAmount = 0.05f; velocityPenalty = 0.95; shield = false; }
	void UndoTurbo()			{ boostTurbo = 1.0f; turbo = false; }

	~Wheel();

	bool sensibility, happyWheel, shield, turbo, reverseControls;

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
	float velocityPenalty;
	float damage;
	float leftDamage;
	float rightDamage;
	int leftDamageLevel, rightDamageLevel;
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

	int gearStatus;
	bool canGearUp;
	bool canGearDown;
	bool detachedCamera;			// used when passing the finish line
	bool raceComplete;				// used to stop updating timer
	float finalTime;				// result of the race

	void initializePhysics();
	void updatePhysics();
	void gearUp();
	void gearDown();

	float boostTurbo = 1.0f;
	float boostTurn = 1.0f;
	float damageAmount = 0.05f;

	bool automaticGearBox;
	bool canControl;
	bool canRace;
	bool raceStarted;

};

