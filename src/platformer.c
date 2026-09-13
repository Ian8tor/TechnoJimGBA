#include "Platformer.h"

#include "Toolbox.h"
#include "Sprite.h"
#include "SoundPlayer.h"
#include "Music.h"


void print(int pos, int v, int l)
{
	for (int p = pos + l - 1; p >= pos; p--)
	{
		se_mem[31][p] = (v%10) + 48;
		v /= 10;
	}
}

//If dirY = 1 it is y collision, else it is x collision
int tileCollision(struct Player* p, int diY)
{
	//increments the touching directions to say it is being checked
	p->touching[diY]++; p->touching[diY + 2]++;
	
	//If no movement no tile collision
	if (p->vel[diY] == 0) { return 0; }
	
	//Vars
	u32 tile;
	int o_diY = (diY == 0);
	int foward = p->vel[diY] > 0;
	int box[2];
	int pos[2] = {p->pos[0] >> 8, p->pos[1] >> 8};
	
	//Gets the specific boxes to check
	box[  diY] = (pos[  diY] + p->hitbox[  diY] + p->hitbox[diY + 2]*foward) / 8;
	box[o_diY] = (pos[o_diY] + p->hitbox[o_diY]                            ) / 8;
	
	//Loops through all possible colliding boxes
	for (box[o_diY] = box[o_diY]; box[o_diY] <= (pos[o_diY]	+ p->hitbox[o_diY] + p->hitbox[o_diY + 2] - 1) / 8; box[o_diY]++)
	{
		tile = 32*(box[1] % 32) + (box[0] % 32);
		if ((se_mem[30][tile] % 0x0400) <= 0x0002) { continue; }
		pos[diY] = (box[diY]*8 - p->hitbox[diY]) - p->hitbox[diY + 2]*foward + 8*(1 - foward);
		
		//resets the values
		p->touching[diY + 2*(p->vel[diY] < 0)] = 0;
		p->pos[diY] = pos[diY] << 8;
		p->vel[diY] = 0;
		
		//Collision
		return 1;
	}
	
	//No collision
	return 0;
}

int getGravityDir(struct Player* p)
{
	//Gets the center of the player
	int cenX = ((p->pos[0] >> 8) + p->hitbox[0] + (p->hitbox[2] / 2)) / 8;
	int cenY = ((p->pos[1] >> 8) + p->hitbox[1] + (p->hitbox[3] / 2)) / 8;
	int tile = se_mem[30][32*(cenY % 32) + (cenX % 32)];
	
	//L, D, R, U
	switch (tile)
	{
		case 0x0001: return 0;
		case 0x0401: return 2;
		case 0x0002: return 3;
		default: return 1;
	}
}

	
//Moves player who jumps with gravity
void adjustPlayerVelocityGrav(struct Player* p, int LRtribool, int UDtribool, int flt, int jump)
{
	//Sets the gravity direction
	p->gravDir = getGravityDir(p);
	int gravSign = (((p->gravDir / 2) == 0) - ((p->gravDir / 2) != 0));
	//int gravSign = -(p->gravDir == 0) + (p->gravDir == 1) + (p->gravDir == 2) - (p->gravDir == 3);
	int gravAxis = p->gravDir % 2;
	int runAxis = (gravAxis == 0);
	
	int touching = p->touching[p->gravDir];
	
	//LR velocity
	clamp(&p->vel[runAxis], 0, (p->gFriction)*(touching == 0) + (p->aFriction)*(touching != 0));
	p->vel[runAxis] += (LRtribool*gravAxis + UDtribool*runAxis)*((p->accel[runAxis])*(touching == 0) + (p->aControl)*(touching != 0));
	if (p->vel[runAxis] >  p->maxVel[0]) { p->vel[runAxis] =  p->maxVel[0]; }
	if (p->vel[runAxis] < -p->maxVel[0]) { p->vel[runAxis] = -p->maxVel[0]; }
	
	//UD velocity
	//p->vel[p->gravDir % 2] += p->gravity * (((p->gravDir / 2) == 0) - (p->gravDir / 2) != 0);
	int maxGrav =  p->maxVel[gravAxis] * gravSign;
	int gravPow = p->gravity + (p->gravityInc)*((gravSign*p->vel[gravAxis]) < 0)*(!flt);
	clamp(&p->vel[gravAxis], maxGrav, gravPow);
	se_mem[31][0] = 48 + p->gravDir;
	se_mem[31][1] = 48 + gravSign;
	if ((touching <= p->cyote) && jump) 
	{ 
		p->vel[gravAxis] -= p->jump * gravSign; 
		spriteSetAni(&p->spr, 2);
		playSound(SFX_jump, SFX_jump_bytes, 1);
	}
	
	//Updates animation
	if (p->vel[0] == 0) { p->spr.queued = 0; }
	else
	{ 
		spriteFlip(&p->spr, (p->vel[0] > 0), 0); 
		p->spr.queued = 1;
	}
}

//Moves player without gravity or jumping
void adjustPlayerVelocityNoGrav(struct Player* p, int LRtribool, int UDtribool)
{
	//LR velocity
	clamp(&p->vel[0], 0                         , (p->gFriction)*(p->touching[1] == 0) + (p->aFriction)*(p->touching[1] != 0));
	p->vel[0] += LRtribool*((p->accel[0])*(p->touching[1] == 0) + (p->aControl)*(p->touching[1] != 0));
	if (p->vel[0] >  p->maxVel[0]) { p->vel[0] =  p->maxVel[0]; }
	if (p->vel[0] < -p->maxVel[0]) { p->vel[0] = -p->maxVel[0]; }
	
	//UD velocity
	clamp(&p->vel[1], 0                         , (p->gFriction)*(p->touching[1] == 0) + (p->aFriction)*(p->touching[1] != 0));
	p->vel[1] += LRtribool*((p->accel[1])*(p->touching[1] == 0) + (p->aControl)*(p->touching[1] != 0));
	if (p->vel[1] >  p->maxVel[1]) { p->vel[1] =  p->maxVel[1]; }
	if (p->vel[1] < -p->maxVel[1]) { p->vel[1] = -p->maxVel[1]; }
}


void movePlayer(struct Player* p)
{
	
	
	
	//Impact sound
	int impact = (p->vel[1] >= 600);
	int prevTouch = p->touching[1];

	//Moves character and collides
	p->pos[0] += p->vel[0];  tileCollision(p, 0);
	p->pos[1] += p->vel[1];  tileCollision(p, 1);
	
	//Force plays the queued animation if landing
	if (prevTouch && !p->touching[1]) { spriteSetAni(&p->spr, p->spr.queued); }
	
	//Plays impact sound
	if (impact && !p->touching[1]) 
	{ 
		spriteSetAni(&p->spr, 3);
		playSound(SFX_landing, SFX_landing_bytes, 1);
	}	
}
/*
void movePlayer(struct Player* p)
{
	
	//LR velocity
	clamp(&p->vel[0], 0                         , (p->gFriction)*(p->touching[1] == 0) + (p->aFriction)*(p->touching[1] != 0));
	p->vel[0] += key_tri_horz()*((p->accel[0])*(p->touching[1] == 0) + (p->aControl)*(p->touching[1] != 0));
	if (p->vel[0] >  p->maxVel[0]) { p->vel[0] =  p->maxVel[0]; }
	if (p->vel[0] < -p->maxVel[0]) { p->vel[0] = -p->maxVel[0]; }
	
	//UD movement
	clamp(&p->vel[1], p->maxVel[1], p->gravity + (p->gravityInc)*(p->vel[1] < 0)*(key_is_up(KEY_B)));
	if ((p->touching[1] <= p->cyote) && key_hit(KEY_B)) 
	{ 
		p->vel[1] -= p->jump; 
		spriteSetAni(&p->spr, 2);
		playSound(SFX_jump, SFX_jump_bytes, 1);
	}
	
	//Impact sound
	int impact = (p->vel[1] >= 600);
	int prevTouch = p->touching[1];

	//Moves character and collides
	p->pos[0] += p->vel[0];  tileCollision(p, 0);
	p->pos[1] += p->vel[1];  tileCollision(p, 1);
	
	//Force plays the queued animation if landing
	if (prevTouch && !p->touching[1]) { spriteSetAni(&p->spr, p->spr.queued); }
	
	//Plays impact sound
	if (impact && !p->touching[1]) 
	{ 
		spriteSetAni(&p->spr, 3);
		playSound(SFX_landing, SFX_landing_bytes, 1);
	}	
}
*/