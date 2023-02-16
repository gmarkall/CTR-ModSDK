#include <common.h>

// budget: 0x208 (520)
// curr:

void Player_Driving_Init(struct Thread* t, struct Driver* d);

void DECOMP_Player_Blasted_Update(struct Thread *thread, struct Driver *driver)
{
	if (driver->NoInputTimer != 0) return;
	
	driver->animationIndex = 0;
	driver->animationFrame = 0;
	Player_Driving_Init(thread, driver);
}

void DECOMP_Player_Driving_PhysLinear(struct Thread* thread, struct Driver* driver);
void DECOMP_Player_Blasted_PhysLinear(struct Thread *thread, struct Driver *driver)
{
	int NoInputTimer;
	
	driver->NoInputTimer -= sdata->gGT->elapsedTimeMS;
	if (driver->NoInputTimer < 0)
		driver->NoInputTimer = 0;
	
	DECOMP_Player_Driving_PhysLinear(thread, driver);
	
	driver->jump_ForcedMS = 0x60;
	driver->baseSpeed = 0;
	driver->fireSpeed = 0;
	driver->actionsFlagSet |= 0x5808;
	driver->jump_InitialVelY = driver->NoInputTimer * 2 + 6000;
	
	return;
}

int InterpBySpeed(int currentRot, int rotSpeed, int destinedRot);
void Rot_AxisAngle(MATRIX* m, short* normVec, short angle);

void DECOMP_Player_Blasted_PhysAngular(struct Thread *thread, struct Driver *driver)
{
	int elapsedTimeMS = sdata->gGT->elapsedTimeMS;
	
	driver->numFramesSpentSteering = 10000;
	driver->unk3D4[0] -= (driver->unk3D4[0] >> 3);
	driver->rotationSpinRate -= (driver->rotationSpinRate >> 3);
	driver->unk_LerpToForwards -= (driver->unk_LerpToForwards >> 3);
	driver->ampTurnState = driver->rotationSpinRate;
	
	driver->unknownDimension2Curr += driver->unk_LerpToForwards;
	driver->unknownDimension2Curr += 0x800;
	driver->unknownDimension2Curr &= 0xfff;
	driver->unknownDimension2Curr -= 0x800;
	
	driver->angle += (short)((int)driver->rotationSpinRate * elapsedTimeMS >> 0xd);
	driver->angle &= 0xfff;
	
	(driver->rotCurr).y = 
		driver->unk3D4[0] + driver->angle + driver->unknownDimension2Curr;
	
	(driver->rotCurr).w = 
		InterpBySpeed((int)(driver->rotCurr).w, (elapsedTimeMS << 5) >> 5, 0);
	
	Rot_AxisAngle(&driver->matrix310, driver->AxisAngle1_normalVec, driver->angle);
	
	return;
}

void DECOMP_Player_Blasted_Animate(struct Thread *thread,struct Driver *driver)
{
	u_char animationFrame;
	int iVar2;
	
	driver->animationIndex = 6;
	iVar2 = (int)((u_int)(u_short)driver->NoInputTimer << 0x10) >> 0x15;
	
	// what???
	animationFrame = (u_char)(iVar2 % data.betweenIDsAndMeta[0x1094]);
	
	if (driver->KartStates.Blasted.boolPlayBackwards != 0)
	{
		// what???
		animationFrame = (char)data.betweenIDsAndMeta[0x1094] - (animationFrame + 1);
	}
	
	driver->animationFrame = animationFrame;
	
	return;
}