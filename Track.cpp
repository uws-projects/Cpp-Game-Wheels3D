#include "Track.h"

// TO DO:: fix pointer function!


void Track::Initialize()
{
	raceComplete = false;
	lapComplete = false;
	startTime = 0.0;
	complete = false;
	for (int i = 0; i < 32; i++)
	{
		lap[32] = false;
	}

	section = 0; // very important, track should always start with section 0;
	collidedRightX = collidedLeftX = collidedRightZ = collidedLeftZ = false;
	finalTime = "";
	
	pointerArray[0] = &Track::sector00;
	pointerArray[1] = &Track::sector01;
	pointerArray[2] = &Track::sector02;
	pointerArray[3] = &Track::sector03;
	pointerArray[4] = &Track::sector04;
	pointerArray[5] = &Track::sector05;
	pointerArray[6] = &Track::sector06;
	pointerArray[7] = &Track::sector07;
	pointerArray[8] = &Track::sector08;
	pointerArray[9] = &Track::sector09;
	pointerArray[10] = &Track::sector10;
	pointerArray[11] = &Track::sector11;
	pointerArray[12] = &Track::sector12;
	pointerArray[13] = &Track::sector13;
	pointerArray[14] = &Track::sector14;
	pointerArray[15] = &Track::sector15;
	pointerArray[16] = &Track::sector16;
	pointerArray[17] = &Track::sector17;
	pointerArray[18] = &Track::sector18;
	pointerArray[19] = &Track::sector19;
	pointerArray[20] = &Track::sector20;
	pointerArray[21] = &Track::sector21;
	pointerArray[22] = &Track::sector22;
	pointerArray[23] = &Track::sector23;
	pointerArray[24] = &Track::sector24;
	pointerArray[25] = &Track::sector25;
	pointerArray[26] = &Track::sector26;
	pointerArray[27] = &Track::sector27;
	pointerArray[28] = &Track::sector28;
	pointerArray[29] = &Track::sector29;
	pointerArray[30] = &Track::sector30;
	pointerArray[31] = &Track::sector31;	
}

void Track::Update()
{
	collidedRightX = collidedLeftX = collidedRightZ = collidedLeftZ = false;
	(this->*pointerArray[section])();
}