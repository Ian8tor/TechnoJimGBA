#ifndef PLATFORMER_H
#define PLATFORMER_H

#include "Toolbox.h"
#include "Sprite.h"

//#define GET_PLAYER_TOUCH_L(n) (((n) & 0x000000FF) >> 0 )
//#define GET_PLAYER_TOUCH_D(n) (((n) & 0x0000FF00) >> 8 )
//#define GET_PLAYER_TOUCH_R(n) (((n) & 0x00FF0000) >> 16)
//#define GET_PLAYER_TOUCH_U(n) (((n) & 0xFF000000) >> 24)
//
//#define SET_PLAYER_TOUCH_L(n, v) (((n) & 0xFFFFFF00) + (((v) & 0xFF) << 0 ))
//#define SET_PLAYER_TOUCH_L(n, v) (((n) & 0xFFFF00FF) + (((v) & 0xFF) << 8 ))
//#define SET_PLAYER_TOUCH_L(n, v) (((n) & 0xFF00FFFF) + (((v) & 0xFF) << 16))
//#define SET_PLAYER_TOUCH_L(n, v) (((n) & 0x00FFFFFF) + (((v) & 0xFF) << 24))

/*
//A struct to hold the players position and velocity
struct Player
{			
	//8 bit fixed point position
	int x;
	int y;         
	
	int hoX;  //Hitbox offset x
	int hoY;  //Hitbox offset y
	int hoW;  //Hitbox offset w
	int hoH;  //Hitbox offset h

	int veX;  //Velocity x
	int veY;  //Velocity y
	
	int acceX;
	int acceY;
	int maX;
	int maY;
	
	int grounded;
};
*/

struct Player
{			
	//index 0 = x | index 1 = y
	int pos   [2];    //8 bit fixed point position   
	int hitbox[4];    //The offset and size of the hitbox
	int vel   [2];    //The velocity pixel per frame >> 8
	int accel [2];    //The acceleration speeds
	int maxVel[2];    //The speed caps
	
	//Control modifiers
	int gFriction;
	int aFriction;
	int aControl;
	int jump;
	int gravity;
	int gravityInc;
	
	//Cyote Jump
	int cyote;
	int gravDir;
	
	//Character states
	u32 touching[4];  //L, D, R, U
	struct Sprite spr;
};

//Deals with tile collision
int tileCollision (struct Player* p, int diY);

void movePlayer(struct Player* p);

//Velocity adjustment
void adjustPlayerVelocityGrav  (struct Player* p, int LRtribool, int UDtribool, int flt, int jump);
void adjustPlayerVelocityNoGrav(struct Player* p, int LRtribool, int UDtribool);


#endif