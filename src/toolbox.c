#include "Toolbox.h"

//Object storage;
OBJ_ATTR obj_buffer[128];
OBJ_AFFINE* obj_aff_buffer = (OBJ_AFFINE*)obj_buffer;


u16 __key_prev = 0;
u16 __key_curr = 0;


const u32 __snd_rates[12] = 
{
	8013, 7566, 7144, 6742,   // C , C#, D , D#
	6362, 6005, 5666, 5346,   // E , F , F#, G
	5048, 4766, 4499, 4246    // G#, A , A#, B
};

void clamp(int* x, int v, int a)
{
	if      (*x > v) { *x -= a; if (*x < v) { *x = v; }}
	else if (*x < v) { *x += a; if (*x > v) { *x = v; }}
}

u32 se_index(u32 tx, u32 ty, u32 pitch)
{
	u32 sbb = (ty/32)*(pitch/32) + (tx/32);
	return sbb*1024 + (ty%32)*32 + tx%32;
}



//Initalizes the oam
void oamInit(OBJ_ATTR* obj, u32 count)
{
	u32* dst = (u32*)obj;
	
	//Hides all objects
	for (int i = 0; i < count; i++)
	{
		(*dst++) = 0x00000200;
		(*dst++) = 0;
	}
	
	//Init oam
	oamCopy((OBJ_ATTR*)MEM_OAM, obj, count);
}

//Copies from oam shadow to actual oam
void oamCopy(OBJ_ATTR* dst, const OBJ_ATTR* src, u32 count)
{
	//Steict copy (perfered but if it fails switch
#if 1
	while (count--) { (*dst++) = (*src++); }
#else
	u32* dstwrd = (u32*)dst;
	u32* srcwrd = (u32*)src;
	while (count--)
	{
		(*dstwrd++) = (*srcwrd++);
		(*dstwrd++) = (*srcwrd++);
	}
#endif
}




























u16* vid_page = vid_mem;

void bmp16_line(int x1, int y1, int x2, int y2, u32 clr, void* dstBase, u32 dstPitch)
{
	int i, dx, dy, x_step, y_step, d;
	u16 *dst = (u16*)(dstBase + y1*dstPitch + x1*2);
	dstPitch /= 2;
	
	//Normalizes
	if (x1 > x2) { x_step = -1; dx = x1 - x2; }
	else         { x_step =  1; dx = x2 - x1; }
	if (y1 > y2) { y_step = -dstPitch; dy = y1 - y2; }
	else         { y_step =  dstPitch; dy = y2 - y1; }
	
	// * * * Drawing * * *
	//Horizontal
	if (dy == 0)  
	{
		for (i = 0; i <= dx; i++) { dst[i * x_step] = clr; }
		return;
	}
	
	//Vertical
	if (dx == 0)
	{
		for (i = 0; i <= dy; i++) { dst[i * y_step] = clr; }
		return;
	}
	
	//Slope <= 1
	if (dx >= dy)
	{
		d = 2*dy - dx;
		for (i = 0; i <= dx; i++)
		{
			*dst = clr;
			if (d >= 0) { d -= 2 *dx; dst += y_step; }
			d += 2*dy;
			dst += x_step;
		}
		return;
	}
	
	//Slope > 1
	d = 2*dx - dy;
	for (i = 0; i <= dy; i++)
	{
		*dst = clr;
		if (d >= 0) { d -= 2 *dy; dst += x_step; }
		d += 2*dx;
		dst += y_step;
	}
}


void bmp16_rect(int x, int y, int w, int h, u32 clr, void* dstBase, u32 dstPitch)
{
	int ix,  iy;
	u16* dst = (u16*)(dstBase + y*dstPitch + x * 2);
	dstPitch /= 2;
	
	//Draw
	for (ix = 0; ix < w; ix++)
	{
		for (iy = 0; iy < h; iy++) { dst[iy*dstPitch + ix] = clr; }
	}
}


void bmp16_frame(int x, int y, int w, int h, u32 clr, void* dstBase, u32 dstPitch)
{
	w--; h--;
	bmp16_line(x    , y    , x + w, y    , clr, dstBase, dstPitch);
	bmp16_line(x    , y + h, x + w, y + h, clr, dstBase, dstPitch);
	bmp16_line(x    , y    , x    , y + h, clr, dstBase, dstPitch);
	bmp16_line(x + w, y    , x + w, y + h, clr, dstBase, dstPitch);
}


void m3_fill(COLOR clr)
{
	int i;
	u32* dst = (u32*)vid_mem;
	u32  wrd = (clr << 16) | clr;
	
	for (i = 0; i < M3_SIZE/4; i++)
	{
		(*dst++) = wrd;
	}
}

void m4_fill(u8 clr)
{
	int i;
	u32* dst = (u32*)vid_page;
	u32  wrd = (clr << 24) | (clr << 16) | (clr << 8) | clr;
	
	for (i = 0; i < M4_SIZE/4; i++)
	{
		(*dst++) = wrd;
	}
}


u16* vid_flip()
{
	vid_page = (u16*)((u32)vid_page ^ 0xA000);
	REG_DISPCNT ^= DCNT_PAGE;
	return vid_page;
}

void vid_vsync()
{
	while(REG_VCOUNT % 256 >= 160) {  }
	while(REG_VCOUNT % 256 <  160) {  }
}
