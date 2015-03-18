#pragma once
#include "Object.h"
#include "Wheel.h"

#ifndef COMPLETE
#define COMPLETE true
#endif
#ifndef INCOMPELTE
#define INCOMPLETE false
#endif

#define CALL_MEMBER_FN(object, pointerToMember) ((object).*(pointerToMwember))

class Track :
	public Object
{
public:
	Track() {}
	Track(Wheel * player) { wheel = player; }
	void Initialize();
	void Render() {};
	void Update();
	void HandleEvents() {};

	void StartTimer()		{ startTime = SDL_GetTicks(); }
	double getStartTime()	{ return startTime; }

	bool ContactRightX()	{ return collidedRightX; }
	bool ContactLeftX()		{ return collidedLeftX; }
	bool ContactRightZ()	{ return collidedRightZ; }
	bool ContactLeftZ()		{ return collidedLeftZ; }
	~Track() {}

private:
	bool raceComplete;	// switch for complete race
	bool lapComplete;	// switch for complete lap
	double startTime;	// timer to count lap time
	bool complete;		// temp flag to check if sector is complete
	bool lap[32];		// flags for each complete section of the track
	Wheel* wheel;		// object to check collisions against

	void(Track::*pointerArray[32])();	// array of pointer to member function with each
	// element of the array representing one sector of the track that has
	// to be checked

	unsigned int section;

	void checkTrack();
	bool collidedRightX, collidedLeftX, collidedRightZ, collidedLeftZ;
	std::string finalTime;

	void sector00()
	{
		if (wheel->Position().x < 35.9921f + wheel->Radius()) { wheel->Position().x = 35.9921f + wheel->Radius(); wheel->AddSpeedPenalty(); collidedRightX = true; wheel->AddDamage(); }
		if (wheel->Position().x > 38.4921f - wheel->Radius()) { wheel->Position().x = 38.4921f - wheel->Radius(); wheel->AddSpeedPenalty(); collidedLeftX = true; wheel->AddDamage(); }
		if (wheel->Position().z < 4.73671f) { section = 31; }
		if (wheel->Position().z > 104.737f) { lap[section] = COMPLETE; section = 1; }
		// only in this section, check if the lap is complete
		// a lap is complete when is over the finish line and ALL sections are complete
		if (wheel->Position().z > 19.7211f - wheel->Radius())
		{

			for (int section = 0; section < 32; section++)
			{
				if (lap[section] == INCOMPLETE)
				{
					complete = INCOMPLETE;
					break;
				}
				else complete = COMPLETE;
			}

			if (complete) {
				startTime = SDL_GetTicks() - startTime;
				int minutes = (int)startTime / 60000;
				int seconds = (int)(startTime - (minutes * 60000)) / 1000;
				int mili = (int)(startTime - minutes * 60000 - seconds * 1000);
				std::cout << "\nRace completed in ";
				if (minutes < 10) std::cout << "0" << minutes << ":";
				else std::cout << minutes << ":";
				if (seconds < 10) std::cout << "0" << seconds << ":";
				else std::cout << seconds << ":";
				std::cout << mili;
			}
		}
	}
	void sector01() {
		if (wheel->Position().z > 108.237f - wheel->Radius()) { wheel->Position().z = 108.237f - wheel->Radius(); wheel->AddSpeedPenalty(); collidedLeftZ = true; wheel->AddDamage(); }
		if (wheel->Position().x > 38.4921f - wheel->Radius()) { wheel->Position().x = 38.4921f - wheel->Radius(); wheel->AddSpeedPenalty(); collidedLeftX = true; wheel->AddDamage(); }
		if (wheel->Position().z < 104.737f) { section = 0; }
		if (wheel->Position().x < 35.9921f) { lap[section] = COMPLETE; section = 2; }
	}
	void sector02() {
		if (wheel->Position().z < 104.737f + wheel->Radius()) { wheel->Position().z = 104.737f + wheel->Radius(); wheel->AddSpeedPenalty(); collidedRightZ = true; wheel->AddDamage(); }
		if (wheel->Position().z > 108.237f - wheel->Radius())	{ wheel->Position().z = 108.237f - wheel->Radius(); wheel->AddSpeedPenalty(); collidedLeftZ = true; wheel->AddDamage(); }
		if (wheel->Position().x < 2.99211f) { lap[section] = COMPLETE; section = 3; }
		if (wheel->Position().x > 35.9921f) { section = 1; }
	}
	void sector03() {
		if (wheel->Position().z < 104.737f + wheel->Radius()) { wheel->Position().z = 104.737f + wheel->Radius(); wheel->AddSpeedPenalty(); collidedRightZ = true; wheel->AddDamage(); }
		if (wheel->Position().x < 1.49211f + wheel->Radius()) { wheel->Position().x = 1.49211f + wheel->Radius(); wheel->AddSpeedPenalty(); collidedRightX = true; wheel->AddDamage(); }
		if (wheel->Position().z > 108.237f) { lap[section] = COMPLETE; section = 4; }
		if (wheel->Position().x > 2.99211f) { section = 2; }
	}
	void sector04() {
		if (wheel->Position().x > 2.99211f - wheel->Radius()) { wheel->Position().x = 2.99211f - wheel->Radius(); wheel->AddSpeedPenalty(); collidedLeftX = true; wheel->AddDamage(); }
		if (wheel->Position().x < 1.49211f + wheel->Radius()) { wheel->Position().x = 1.49211f + wheel->Radius(); wheel->AddSpeedPenalty(); collidedRightX = true; wheel->AddDamage(); }
		if (wheel->Position().z < 108.237f) { section = 3; }
		if (wheel->Position().z > 158.237f) { lap[section] = COMPLETE; section = 5; }
	}
	void sector05() {
		if (wheel->Position().x < 1.49211f + wheel->Radius()) { wheel->Position().x = 1.49211f + wheel->Radius(); wheel->AddSpeedPenalty(); collidedRightX = true; wheel->AddDamage(); }
		if (wheel->Position().z > 160.737f - wheel->Radius()) { wheel->Position().z = 160.737f - wheel->Radius(); wheel->AddSpeedPenalty(); collidedLeftZ = true; wheel->AddDamage(); }
		if (wheel->Position().x > 2.99211f) { lap[section] = COMPLETE; section = 6; }
		if (wheel->Position().z < 158.237f) { section = 4; }
	}
	void sector06() {
		if (wheel->Position().z > 160.737f - wheel->Radius()) { wheel->Position().z = 160.737f - wheel->Radius(); wheel->AddSpeedPenalty(); collidedLeftZ = true; wheel->AddDamage(); }
		if (wheel->Position().z < 158.237f + wheel->Radius()) { wheel->Position().z = 158.237f + wheel->Radius(); wheel->AddSpeedPenalty(); collidedRightZ = true; wheel->AddDamage(); }
		if (wheel->Position().x < 2.99211f) { section = 5; }
		if (wheel->Position().x > 44.9921f) { lap[section] = COMPLETE; section = 7; }
	}
	void sector07() {
		if (wheel->Position().z > 160.737f - wheel->Radius()) { wheel->Position().z = 160.737f - wheel->Radius(); wheel->AddSpeedPenalty(); collidedLeftZ = true; wheel->AddDamage(); }
		if (wheel->Position().x > 49.4921f - wheel->Radius()) { wheel->Position().x = 49.4921f - wheel->Radius(); wheel->AddSpeedPenalty(); collidedLeftX = true; wheel->AddDamage(); }
		if (wheel->Position().x < 44.9921f) { section = 6; }
		if (wheel->Position().z < 158.237f) { lap[section] = COMPLETE; section = 8; }
	}
	void sector08() {
		if (wheel->Position().x > 49.4921f - wheel->Radius()) { wheel->Position().x = 49.4921f - wheel->Radius(); wheel->AddSpeedPenalty(); collidedLeftX = true; wheel->AddDamage(); }
		if (wheel->Position().x < 44.9921f + wheel->Radius()) { wheel->Position().x = 44.9921f + wheel->Radius(); wheel->AddSpeedPenalty(); collidedRightX = true; wheel->AddDamage(); }
		if (wheel->Position().z > 158.237f) { section = 7; }
		if (wheel->Position().z < 152.237f) { lap[section] = COMPLETE; section = 9; }
	}
	void sector09() {
		if (wheel->Position().x < 44.9921f + wheel->Radius()) { wheel->Position().x = 44.9921f + wheel->Radius(); wheel->AddSpeedPenalty(); collidedRightX = true; wheel->AddDamage(); }
		if (wheel->Position().z < 144.737f + wheel->Radius()) { wheel->Position().z = 144.737f + wheel->Radius(); wheel->AddSpeedPenalty(); collidedRightZ = true; wheel->AddDamage(); }
		if (wheel->Position().x > 49.4921f) { lap[section] = COMPLETE; section = 10; }
		if (wheel->Position().z > 152.237f) { section = 8; }
	}
	void sector10() {
		if (wheel->Position().z < 144.737f + wheel->Radius()) { wheel->Position().z = 144.737f + wheel->Radius(); wheel->AddSpeedPenalty(); collidedRightZ = true; wheel->AddDamage(); }
		if (wheel->Position().z > 152.237f - wheel->Radius()) { wheel->Position().z = 152.237f - wheel->Radius(); wheel->AddSpeedPenalty(); collidedLeftZ = true; wheel->AddDamage(); }
		if (wheel->Position().x < 49.4921f) { section = 9; }
		if (wheel->Position().x > 66.4921f) { lap[section] = COMPLETE; section = 11; }
	}
	void sector11() {
		if (wheel->Position().z > 152.237f - wheel->Radius()) { wheel->Position().z = 152.237f - wheel->Radius(); wheel->AddSpeedPenalty(); collidedLeftZ = true; wheel->AddDamage(); }
		if (wheel->Position().x > 68.9921f - wheel->Radius()) { wheel->Position().x = 68.9921f - wheel->Radius(); wheel->AddSpeedPenalty(); collidedLeftX = true; wheel->AddDamage(); }
		if (wheel->Position().x < 66.4921f) { section = 10; }
		if (wheel->Position().z < 144.737f) { lap[section] = COMPLETE; section = 12; }
	}
	void sector12() {
		if (wheel->Position().x > 68.9921f - wheel->Radius()) { wheel->Position().x = 68.9921f - wheel->Radius(); wheel->AddSpeedPenalty(); collidedLeftX = true; wheel->AddDamage(); }
		if (wheel->Position().x < 66.4921f + wheel->Radius()) { wheel->Position().x = 66.4921f + wheel->Radius(); wheel->AddSpeedPenalty(); collidedRightX = true; wheel->AddDamage(); }
		if (wheel->Position().z > 144.737f) { section = 11; }
		if (wheel->Position().z < 111.737f) { lap[section] = COMPLETE; section = 13; }
	}
	void sector13() {
		if (wheel->Position().x < 66.4921f + wheel->Radius()) { wheel->Position().x = 66.4921f + wheel->Radius(); wheel->AddSpeedPenalty(); collidedRightX = true; wheel->AddDamage(); }
		if (wheel->Position().z < 109.237f + wheel->Radius()) { wheel->Position().z = 109.237f + wheel->Radius(); wheel->AddSpeedPenalty(); collidedRightZ = true; wheel->AddDamage(); }
		if (wheel->Position().z > 111.737f) { section = 12; }
		if (wheel->Position().x > 68.9921f) { lap[section] = COMPLETE; section = 14; }
	}
	void sector14() {
		if (wheel->Position().z < 109.237f + wheel->Radius()) { wheel->Position().z = 109.237f + wheel->Radius(); wheel->AddSpeedPenalty(); collidedRightZ = true; wheel->AddDamage(); }
		if (wheel->Position().z > 111.737f - wheel->Radius()) { wheel->Position().z = 111.737f - wheel->Radius(); wheel->AddSpeedPenalty(); collidedLeftZ = true; wheel->AddDamage(); }
		if (wheel->Position().x < 68.9921f) { section = 13; }
		if (wheel->Position().x > 126.992f) { lap[section] = COMPLETE; section = 15; }
	}
	void sector15() {
		if (wheel->Position().z > 111.737f - wheel->Radius()) { wheel->Position().z = 111.737f - wheel->Radius(); wheel->AddSpeedPenalty(); collidedLeftZ = true; wheel->AddDamage(); }
		if (wheel->Position().x > 137.492f - wheel->Radius()) { wheel->Position().x = 137.492f - wheel->Radius(); wheel->AddSpeedPenalty(); collidedLeftX = true; wheel->AddDamage(); }
		if (wheel->Position().x < 126.992f) { section = 14; }
		if (wheel->Position().z < 109.237f) { lap[section] = COMPLETE; section = 16; }
	}
	void sector16() {
		if (wheel->Position().x > 137.492f - wheel->Radius()) { wheel->Position().x = 137.492f - wheel->Radius(); wheel->AddSpeedPenalty(); collidedLeftX = true; wheel->AddDamage(); }
		if (wheel->Position().x < 126.992f + wheel->Radius()) { wheel->Position().x = 126.992f + wheel->Radius(); wheel->AddSpeedPenalty(); collidedRightX = true; wheel->AddDamage(); }
		if (wheel->Position().z > 109.237f) { section = 15; }
		if (wheel->Position().z < 90.2367f) { lap[section] = COMPLETE; section = 17; }
	}
	void sector17() {
		if (wheel->Position().x > 137.492f - wheel->Radius()) { wheel->Position().x = 137.492f - wheel->Radius(); wheel->AddSpeedPenalty(); collidedLeftX = true; wheel->AddDamage(); }
		if (wheel->Position().z < 87.7367f + wheel->Radius()) { wheel->Position().z = 87.7367f + wheel->Radius(); wheel->AddSpeedPenalty(); collidedRightZ = true; wheel->AddDamage(); }
		if (wheel->Position().z > 90.2367f) { section = 16; }
		if (wheel->Position().x < 126.992f) { lap[section] = COMPLETE; section = 18; }
	}
	void sector18() {
		if (wheel->Position().z < 87.7367f + wheel->Radius()) { wheel->Position().z = 87.7367f + wheel->Radius(); wheel->AddSpeedPenalty(); collidedRightZ = true; wheel->AddDamage(); }
		if (wheel->Position().z > 90.2367f - wheel->Radius()) { wheel->Position().z = 90.2367f - wheel->Radius(); wheel->AddSpeedPenalty(); collidedLeftZ = true; wheel->AddDamage(); }
		if (wheel->Position().x > 126.992f) { section = 17; }
		if (wheel->Position().x < 90.9921f) { lap[section] = COMPLETE; section = 19; }
	}
	void sector19() {
		if (wheel->Position().z > 90.2367f - wheel->Radius()) { wheel->Position().z = 90.2367f - wheel->Radius(); wheel->AddSpeedPenalty(); collidedLeftZ = true; wheel->AddDamage(); }
		if (wheel->Position().x < 88.4921f + wheel->Radius()) { wheel->Position().x = 88.4921f + wheel->Radius(); wheel->AddSpeedPenalty(); collidedRightX = true; wheel->AddDamage(); }
		if (wheel->Position().x > 90.9921f) { section = 18; }
		if (wheel->Position().z < 87.7367f) { lap[section] = COMPLETE; section = 20; }
	}
	void sector20() {
		if (wheel->Position().x < 88.4921f + wheel->Radius()) { wheel->Position().x = 88.4921f + wheel->Radius(); wheel->AddSpeedPenalty(); collidedRightX = true; wheel->AddDamage(); }
		if (wheel->Position().x > 90.9921f - wheel->Radius()) { wheel->Position().x = 90.9921f - wheel->Radius(); wheel->AddSpeedPenalty(); collidedLeftX = true; wheel->AddDamage(); }
		if (wheel->Position().z > 87.7367f) { section = 19; }
		if (wheel->Position().z < 65.7367f) { lap[section] = COMPLETE; section = 21; }
	}
	void sector21() {
		if (wheel->Position().x < 88.4921f + wheel->Radius()) { wheel->Position().x = 88.4921f + wheel->Radius(); wheel->AddSpeedPenalty(); collidedRightX = true; wheel->AddDamage(); }
		if (wheel->Position().z < 63.2367f + wheel->Radius()) { wheel->Position().z = 63.2367f + wheel->Radius(); wheel->AddSpeedPenalty(); collidedRightZ = true; wheel->AddDamage(); }
		if (wheel->Position().z > 65.7367f) { section = 20; }
		if (wheel->Position().x > 90.9921f) { lap[section] = COMPLETE; section = 22; }
	}
	void sector22() {
		if (wheel->Position().z < 63.2367f + wheel->Radius()) { wheel->Position().z = 63.2367f + wheel->Radius(); wheel->AddSpeedPenalty(); collidedRightZ = true; wheel->AddDamage(); }
		if (wheel->Position().z > 65.7367f - wheel->Radius()) { wheel->Position().z = 65.7367f - wheel->Radius(); wheel->AddSpeedPenalty(); collidedLeftZ = true; wheel->AddDamage(); }
		if (wheel->Position().x < 90.9921f) { section = 21; }
		if (wheel->Position().x > 140.992f) { lap[section] = COMPLETE; section = 23; }
	}
	void sector23() {
		if (wheel->Position().z > 65.7367f - wheel->Radius()) { wheel->Position().z = 65.7367f - wheel->Radius(); wheel->AddSpeedPenalty(); collidedLeftZ = true; wheel->AddDamage(); }
		if (wheel->Position().x > 142.492f - wheel->Radius()) { wheel->Position().x = 142.492f - wheel->Radius(); wheel->AddSpeedPenalty(); collidedLeftX = true; wheel->AddDamage(); }
		if (wheel->Position().x < 140.992f) { section = 22; }
		if (wheel->Position().z < 63.2367f) { lap[section] = COMPLETE; section = 24; }
	}
	void sector24() {
		if (wheel->Position().x > 142.492f - wheel->Radius()) { wheel->Position().x = 142.492f - wheel->Radius(); wheel->AddSpeedPenalty(); collidedLeftX = true; wheel->AddDamage(); }
		if (wheel->Position().x < 140.992f + wheel->Radius()) { wheel->Position().x = 140.992f + wheel->Radius(); wheel->AddSpeedPenalty(); collidedRightX = true; wheel->AddDamage(); }
		if (wheel->Position().z > 63.2367f) { section = 23; }
		if (wheel->Position().z < 10.2367f) { lap[section] = COMPLETE; section = 25; }
	}
	void sector25() {
		if (wheel->Position().x > 142.492f - wheel->Radius()) { wheel->Position().x = 142.492f - wheel->Radius(); wheel->AddSpeedPenalty(); collidedLeftX = true; wheel->AddDamage(); }
		if (wheel->Position().z < 5.73671f + wheel->Radius()) { wheel->Position().z = 5.73671f + wheel->Radius(); wheel->AddSpeedPenalty(); collidedRightZ = true; wheel->AddDamage(); }
		if (wheel->Position().z > 10.2367f) { section = 24; }
		if (wheel->Position().x < 140.992f) { lap[section] = COMPLETE; section = 26; }
	}
	void sector26() {
		if (wheel->Position().z < 5.73671f + wheel->Radius()) { wheel->Position().z = 5.73671f + wheel->Radius(); wheel->AddSpeedPenalty(); collidedRightZ = true; wheel->AddDamage(); }
		if (wheel->Position().z > 10.2367f - wheel->Radius()) { wheel->Position().z = 10.2367f - wheel->Radius(); wheel->AddSpeedPenalty(); collidedLeftZ = true; wheel->AddDamage(); }
		if (wheel->Position().x > 140.992f) { section = 25; }
		if (wheel->Position().x < 93.9921f) { lap[section] = COMPLETE; section = 27; }
	}
	void sector27() {
		if (wheel->Position().z > 10.2367f - wheel->Radius()) { wheel->Position().z = 10.2367f - wheel->Radius(); wheel->AddSpeedPenalty(); collidedLeftZ = true; wheel->AddDamage(); }
		if (wheel->Position().x < 91.4921f + wheel->Radius()) { wheel->Position().x = 91.4921f + wheel->Radius(); wheel->AddSpeedPenalty(); collidedRightX = true; wheel->AddDamage(); }
		if (wheel->Position().x > 93.9921f) { section = 26; }
		if (wheel->Position().z < 5.73671f) { lap[section] = COMPLETE; section = 28; }
	}
	void sector28() {
		if (wheel->Position().x < 91.4921f + wheel->Radius()) { wheel->Position().x = 91.4921f + wheel->Radius(); wheel->AddSpeedPenalty(); collidedRightX = true; wheel->AddDamage(); }
		if (wheel->Position().x > 93.9921f - wheel->Radius()) { wheel->Position().x = 93.9921f - wheel->Radius(); wheel->AddSpeedPenalty(); collidedLeftX = true; wheel->AddDamage(); }
		if (wheel->Position().z > 5.73671f) { section = 27; }
		if (wheel->Position().z < 4.73671f) { lap[section] = COMPLETE; section = 29; }
	}
	void sector29() {
		if (wheel->Position().x > 93.9921f - wheel->Radius()) { wheel->Position().x = 93.9921f - wheel->Radius(); wheel->AddSpeedPenalty(); collidedLeftX = true; wheel->AddDamage(); }
		if (wheel->Position().z < 2.23671f + wheel->Radius()) { wheel->Position().z = 2.23671f + wheel->Radius(); wheel->AddSpeedPenalty(); collidedRightZ = true; wheel->AddDamage(); }
		if (wheel->Position().z > 4.73671f) { section = 28; }
		if (wheel->Position().x < 91.4921f) { lap[section] = COMPLETE; section = 30; }
	}
	void sector30() {
		if (wheel->Position().z < 2.23671f + wheel->Radius()) { wheel->Position().z = 2.23671f + wheel->Radius(); wheel->AddSpeedPenalty(); collidedRightZ = true; wheel->AddDamage(); }
		if (wheel->Position().z > 4.73671f - wheel->Radius()) { wheel->Position().z = 4.73671f - wheel->Radius(); wheel->AddSpeedPenalty(); collidedLeftZ = true; wheel->AddDamage(); }
		if (wheel->Position().x > 91.4921f) { section = 29; }
		if (wheel->Position().x < 38.4921f) { lap[section] = COMPLETE; section = 31; }
	}
	void sector31() {
		if (wheel->Position().z < 2.23671f + wheel->Radius()) { wheel->Position().z = 2.23671f + wheel->Radius(); wheel->AddSpeedPenalty(); collidedRightZ = true; wheel->AddDamage(); }
		if (wheel->Position().x < 35.9921f + wheel->Radius()) { wheel->Position().x = 35.9921f + wheel->Radius(); wheel->AddSpeedPenalty(); collidedRightX = true; wheel->AddDamage(); }
		if (wheel->Position().x > 38.4921f) { section = 30; }
		if (wheel->Position().z > 4.73671f) { lap[section] = COMPLETE; section = 0; }
	}
};

