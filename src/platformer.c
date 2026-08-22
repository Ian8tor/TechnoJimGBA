#include "Platformer.h"

#include "Toolbox.h"
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
		if (se_mem[30][tile] == 0x0000) { continue; }
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
	

/*
void adjustPlayerVelocity(struct Player* p)
{
	//Friction
	clamp(&p.vel[0], 0, 20);
	//clamp(&blob.veY, 0, 20);
	
	blob.vel[0] += blob.accel[0] * key_tri_horz();
	
	//Gravity
	blob.vel[1] += 15;
	if (key_is_up(KEY_B))                { blob.vel[1] += 2 + (13)*(blob.vel[1] < 0);  }
	if (blob.grounded && key_hit(KEY_B)) { blob.vel[1] -= 500; playSound(SFX_jump, SFX_jump_bytes, 1); }
	
	//blob.veY += blob.acceY * key_tri_vert();
	
	if (blob.vel[0] >  blob.maxVel[0]) { blob.vel[0] =  blob.maxVel[0]; }
	if (blob.vel[1] >  blob.maxVel[1]) { blob.vel[1] =  blob.maxVel[1]; }
	if (blob.vel[0] < -blob.maxVel[0]) { blob.vel[0] = -blob.maxVel[0]; }
	if (blob.vel[1] < -blob.maxVel[1]) { blob.vel[1] = -blob.maxVel[1]; }
	
	blob.pos[0] += blob.vel[0];  
	tileCollision(&blob, 0);
	blob.pos[1] += blob.vel[1];
	int hard = 0;
	if (blob.vel[1] >= 502) {hard = 1; } 
	if (blob.vel[1] >= 0)             { blob.grounded = 1; }
	if (tileCollision(&blob, 1) == 0) { blob.grounded = 0; }
	else if (hard) { playSound(SFX_landing, SFX_landing_bytes, 1); }
	
}
*/

void movePlayer(struct Player* p)
{
	//LR movement
	clamp(&p->vel[0], 0                         , (p->gFriction)*(p->touching[1] == 0) + (p->aFriction)*(p->touching[1] != 0));
	p->vel[0] += key_tri_horz()*((p->accel[0])*(p->touching[1] == 0) + (p->aControl)*(p->touching[1] != 0));
	if (p->vel[0] >  p->maxVel[0]) { p->vel[0] =  p->maxVel[0]; }
	if (p->vel[0] < -p->maxVel[0]) { p->vel[0] = -p->maxVel[0]; }
	//clamp(&p->vel[0], p->maxVel[0]*key_tri_horz(), p->accel[0]                                                               );
	
	//UD movement
	clamp(&p->vel[1], p->maxVel[1], p->gravity + (p->gravityInc)*(p->vel[1] < 0)*(key_is_up(KEY_B)));
	if ((p->touching[1] <= p->cyote) && key_hit(KEY_B)) { p->vel[1] -= p->jump; playSound(SFX_jump, SFX_jump_bytes, 1); }
	
	//Impact sound
	int impact = 0;
	if (p->vel[1] >= 600) { impact = 1; } 

	//Moves character and collides
	p->pos[0] += p->vel[0];  tileCollision(p, 0);
	p->pos[1] += p->vel[1];  tileCollision(p, 1);
	
	//Plays impact sound
	if (impact & !p->touching[1]) { playSound(SFX_landing, SFX_landing_bytes, 1); }	
}
