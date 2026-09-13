#include "Sprite.h"

#include "Toolbox.h"
#include "Graphic.h"


//Takes in a sprite and creates it
void spriteCreate(struct Sprite* spr, int objID, int tID, int pb, struct Animation* ani)
{
	//Sets the struct values
	spr->objID = objID; 
	spr->tID   = tID; 
	spr->pb    = pb;
	spr->ani   = ani;
	spr->frame = 0;
	spr->ticks = 0;
	spr->serie = 0;
	spr->queued = -1;
	
	//Creates the object in memory at pos 0, 0
	obj_set_attr(&obj_buffer[objID],
				 ATTR0_BUILD(0 , 0, 0, 0, 0, 0),
				 ATTR1_BUILD(1 , 0, 0, 0),
				 ATTR2_BUILD(pb, 0, tID));
				 
	spriteUpdateFrame(spr);
}


//Runs the animation
void spriteTick(struct Sprite* spr)
{
	//Updates tick
	spr->ticks = (spr->ticks + 1) % (spr->ani->animations[spr->serie][spr->frame] % 0xFFFF);
	if (spr->ticks == 0)
	{
		//Updates frame
		spr->frame = spr->frame + 1; 
		if (spr->ani->animations[spr->serie][spr->frame] == 0) 
		{ 
			spr->frame = 0; 
			if (spr->queued != -1)
			{
				spr->serie  = spr->queued;
				spr->queued = -1;
			}
				
		}
		spriteUpdateFrame(spr);
	}
}
