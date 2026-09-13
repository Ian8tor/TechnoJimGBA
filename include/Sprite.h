#ifndef SPRITE_H
#define SPRITE_H

#include <string.h>

#include "Toolbox.h"
#include "Graphic.h"

struct Sprite
{
	//Sprite data
	int objID;
	int tID;
	int pb;
	
	//Animation controls
	struct Animation* ani;
	int frame;
	int ticks;
	int serie;
	int queued;
	int r;
};

//Sprite functions
void spriteCreate(struct Sprite* spr, int objID, int tID, int pb, struct Animation* ani);
void spriteTick(struct Sprite* spr);


//===== INLINES =============================================
//Takes in 8 bit fpi positions
//Updates the tID area with the proper frame
INLINE void spriteUpdateFrame(struct Sprite* spr)
{
	struct Animation* a = spr->ani;
	memcpy(&tile_mem[4][spr->tID], &a->spr[a->frameSize * ((a->animations[spr->serie][spr->frame] / 0xFFFF) + spr->r)] , a->frameSize);
}

INLINE void spriteSetR(struct Sprite* spr, int r)
{
	if (r == spr->r) { return; }
	spr->r = r;
	spriteUpdateFrame(spr);
}

INLINE void spriteUpdatePos(struct Sprite* spr, int x, int y)
{
	BF_SET(obj_buffer[spr->objID].attr0, y >> 8, ATTR0_Y);
	BF_SET(obj_buffer[spr->objID].attr1, x >> 8, ATTR1_X);
}

//Sets the flip flags for the sprite
INLINE void spriteFlip(struct Sprite* spr, int xFlip, int yFlip)
{
	BF_SET(obj_buffer[spr->objID].attr1, xFlip, ATTR1_HF);
	BF_SET(obj_buffer[spr->objID].attr1, yFlip, ATTR1_VF);
}

//Sets the flip flags for the sprite
INLINE void spriteFlipX(struct Sprite* spr, int xFlip)
{
	BF_SET(obj_buffer[spr->objID].attr1, xFlip, ATTR1_HF);
}

//Sets the flip flags for the sprite
INLINE void spriteFlipY(struct Sprite* spr, int yFlip)
{
	BF_SET(obj_buffer[spr->objID].attr1, yFlip, ATTR1_VF);
}

//Sets the animation to be used
INLINE void spriteSetAni(struct Sprite* spr, int serie)
{
	if (spr->serie != serie)
	{
		spr->frame = 0;
		spr->ticks = 0;
		spr->serie = serie;
	}
}

//Returns weather or not the animation is on the last frame
INLINE int spriteAniLastFrame(struct Sprite* spr)
{
	return (spr->ani->animations[spr->serie][spr->frame + 1] == 0);
}


#endif