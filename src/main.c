//First demo from :
//https://www.coranac.com/tonc/text/first.htm


#include <stdlib.h>
#include <String.h>

#include "Conversation.h"
#include "SoundPlayer.h"
#include "Platformer.h"
#include "TextBox.h"
#include "Toolbox.h"
#include "Sprite.h"


#include "Music.h"
#include "Graphic.h"



//Converts a number to a string
int numToString(char* str, int num, int max_len)
{
	int len = 0;
	for (int i = max_len - 1; i >= 0; i--)
	{
		str[i] = (num % 10) + '0';
		num /= 10;
		len++;
		if (num == 0) { break; }
	}
	
	return len;
	
}

//Shows victory score with highscores			
int displayHighscore(int score)
{
	clearScreen(1);
	renderText(7, 3, "Congratulations!");
	renderText(7, 5, "Enter\001your\001name:");
	renderText(6, 10, "Best\001Four\001WARRIORS");
	
	int position = 12 + ((score <= 1000000) + (score <= 50000) + (score <= 4000));
	char scoreText[8] = {1, 1, 1, 1, 1, 1, 1, 0};
	numToString(scoreText, score, 7);
	
	
	char* leaders[3] = {"\001\0011000000\001\001zIan", 
						"\001\001\001\00150000\001\001AlpineK8",
						"\001\001\001\001\0014000\001\001BigSkiier"};
	char* positionPrefix[4] = {"1st", "2nd", "3rd", "4th"};
	for (int y = 12; y <= 15; y++)
	{
		renderText(3, y, positionPrefix[y - 12]);
		if (y == position) { renderText(8, y, scoreText); continue; }
		renderText(6, y, leaders[y - 12 - (y >= position)]);
	}
	
	return position;
}

//Allows player to select name
int selectName(int y)
{	
	if (key_hit(KEY_A) && __player_name_len < 10)
	{
		__player_name[__player_name_len] = 'a';
		__player_name_len++;
	}
	
	if (key_hit(KEY_B) && __player_name_len > 1)
	{
		__player_name_len--;
		__player_name[__player_name_len] = '\0';
	}
	
	//Updates letters
	if (key_hit(KEY_UP  )) { __player_name[__player_name_len - 1]++;     }
	if (key_hit(KEY_DOWN)) { __player_name[__player_name_len - 1]--;     }	
	if (key_hit(KEY_L   )) { __player_name[__player_name_len - 1] += 32; }
	if (key_hit(KEY_R   )) { __player_name[__player_name_len - 1] -= 32; }


	//Keeps name in bound
	int v = __player_name[__player_name_len - 1];
	if (v%32 == 0 ) { __player_name[__player_name_len - 1] = (v/32)*32 + 26; }
	if (v%32 == 27) { __player_name[__player_name_len - 1] = (v/32)*32 + 1 ; }
	if (v >= 128  ) { __player_name[__player_name_len - 1] -= 96; }
	if (v <= 32   ) { __player_name[__player_name_len - 1] += 96; }
	
	renderText(17, y, __player_name);
	if (key_hit(KEY_SELECT)) { return 1; }
	return 0;
}	

//This will take a larger map and pull 1 32x32 screen chunk to the screen
void loadMapTo32x32(const unsigned short* map, int w, int h, int sbb)
{
	for (int y = 0; y < 32; y++)
	{
		memcpy(&se_mem[sbb][y*32], &map[y*w], 32 * 2);
	}
}


int __screen_x = 0;
int __screen_y = 0;
void moveScreen(const unsigned short* map, int w, int h, int sbb, int bg, int new_x, int new_y)
{	
	//FxV = First X or Y Visable
	int old_FXV = __screen_x / 8;    int new_FXV = new_x / 8   ;
	int old_FYV = __screen_y / 8;    int new_FYV = new_y / 8   ;
	int old_LXV = old_FXV + 30  ;    int new_LXV = new_FXV + 30;
	int old_LYV = old_FYV + 20  ;    int new_LYV = new_FYV + 20;
	
	//Starting positions for the loops
	int xStart = (old_LXV + 1)*(new_FXV > old_FXV) + (new_FXV)*(new_FXV <= old_FXV);
	int xThick = ABS(new_FXV - old_FXV);
	int yStart = (old_LYV + 1)*(new_FYV > old_FYV) + (new_FYV)*(new_FYV <= old_FYV);
	int yThick = ABS(new_FYV - old_FYV);
	
	//Updates the grid
	for (int x = xStart; x < xStart + xThick; x++) // X
	{
		for (int y = new_FYV; y <= new_LYV; y++) { se_mem[sbb][(x%32) + 32*(y%32)] = map[(x%w) + w*(y%h)]; }
	}
	for (int y = yStart; y < yStart + yThick; y++) // Y
	{
		for (int x = new_FXV; x <= new_LXV; x++) { se_mem[sbb][(x%32) + 32*(y%32)] = map[(x%w) + w*(y%h)]; }
	}
	
	//Updates the registers
	__screen_x = new_x;    __screen_y = new_y;
	REG_BG_OFS[0].x = new_x;
	REG_BG_OFS[0].y = new_y;

}



int main()
{	
	REG_DISPCNT = DCNT_MODE0 | DCNT_BG0 | DCNT_BG3 | DCNT_OBJ | DCNT_OBJ_1D;
	
	//Sets up bg to render
	REG_BGCNT[3] = BG_CBB(0) | BG_SBB(__se_index) | BG_4BPP | BG_REG_32x32 | BG_PRIO(0); //Text
	REG_BGCNT[0] = BG_CBB(1) | BG_SBB(30)         | BG_4BPP | BG_REG_32x32 | BG_PRIO(1); //Platformer

	//Enables all sounds i am using
	enableSound();
	setSampleRate(8000);
	
	// * * * BG * * *
	//Copies the entries to memory
	memcpy(bg_pal_mem, TIL_badTiles_palette, 32);
	//memcpy(bg_pal_mem, TIL_sand2_palette, 16);
	
	memcpy(&tile_mem[1][0], TIL_badTiles, TIL_badTiles_bytes);
	//memcpy(&tile_mem[1][0], TIL_sand2, TIL_sand2_bytes);
	//memcpy(&se_mem[30][0], MAP_map, 2 * MAP_map_w * MAP_map_h);
	
	memcpy(bg_pal_mem + ((__font_pal >> 0x8)), TIL_font_palette, 32);
	memcpy(&tile_mem[0][0], TIL_font, TIL_font_bytes);
	
	
	loadMapTo32x32(MAP_64x200, MAP_64x200_w, 32, 30);
	
	// * * * SPRITE * * * 
	//Places the tiles into block 4
	memcpy(&tile_mem[4][0], GFX_cloakGuySheet, GFX_cloakGuySheet_bytes);
	memcpy((u32*)MEM_OBJ_PALETTE, GFX_cloakGuySheet_palette, 32);
	//memcpy(&tile_mem[4][0], GFX_sadSlimeSheet, GFX_sadSlimeSheet_bytes);
	//memcpy((u32*)MEM_OBJ_PALETTE, GFX_sadSlimeSheet_palette, 32);
	
	//Initalizes sprite
	oamInit(obj_buffer, 128);
	
	//Slime
	struct Player blob = {{318<<8, 3109<<8},   //pos
						  {7, 9, 8, 8},       //hitbox
						  {0  , 0   },        //vel
						  {35 , 35  },        //accel
						  {500, 2000},        //max
						  20, 10, 20, 510, 15, 15, 6, 1, 0, {0, 0, 0, 0}};
	spriteCreate(&blob.spr, 0, 0, 0, &ANI_cloakGuySheet);
						  
	//int gFriction;
	//int aFriction;
	//int aControl;
	//int jump;
	//int gravity;
	//int gravityInc;


	createAllConversations();
	createTextBox(1, 13, 28, 7);
	
	int scrX = 0;
	int scrY = 0;
	
	int time = 0;
	int dia = 0;
	
	while(1)
	{
		//Score
		renderText(0, 0, "Time > ");
		char timeChar[7] = {48, 48, 48, 48, 48, 48, 48};
		numToString(timeChar, time, 7);
		renderText(7, 0, timeChar);
		
		if ((dia < 3) || (blob.pos[1] < (300  << 8))) { time++; }
		
		
		if ((dia == 2) && (blob.pos[1] < (300  << 8))) { dia++; __conv_vars[START] = (u32)0; }
		if ((dia == 1) && (blob.pos[1] < (1140 << 8))) { dia++; __conv_vars[START] = (u32)0; }
		if ((dia == 0) && (blob.pos[1] < (2400 << 8))) { dia++; __conv_vars[START] = (u32)0; }
		
		
		if (!isPlaying(0)) { playSound(MUSCSong_BlueSkies, MUSCSong_BlueSkies_bytes, 0); }
		key_poll();
		vid_vsync();
		spriteTick(&blob.spr);
		spriteSetR(&blob.spr, 16*(blob.gravDir % 2 == 0));
		
		runConversations();
		
		/*	
		if (__conv_vars[START] == 0 && __running_conv == -1)
		{
			__conv_vars[START] = selectName(displayHighscore(526732));
			if (__conv_vars[START]) { createTextBox(1, 12, 28, 7); }
		}
		*/
		
		// * * * SPRITE * * 
		adjustPlayerVelocityGrav(&blob, key_tri_horz(), key_tri_vert(), key_is_down(KEY_B), key_hit(KEY_B));
		movePlayer(&blob);
		//obj_set_pos(slime, (blob.pos[0] - scrX) >> 8, (blob.pos[1] - scrY) >> 8);
		int reX = blob.pos[0] - scrX - (10 << 8)*(blob.facing      )*(blob.gravDir % 2 == 1) - (7 << 8)*(blob.gravDir % 2 == 0) + (3 << 8)*(blob.gravDir == 0) - (12 - (24*blob.facing) << 8)*(blob.gravDir == 3);
		int reY = blob.pos[1] - scrY - (5  << 8)*(blob.gravDir == 3)*(blob.gravDir % 2 == 1) - (9 << 8)*(blob.gravDir % 2 == 0)*(!blob.facing) + (4 << 8)*(blob.gravDir % 2 == 0)*(blob.facing) + ((12 - (24*blob.facing)) << 8)*(blob.gravDir == 0);
		spriteUpdatePos(&blob.spr, reX, reY);
		
		//Update oam
		oamCopy((OBJ_ATTR*)MEM_OAM, obj_buffer, 1);

		//Camera Positioning
		if (blob.pos[0] - scrX        < (60   << 8)) { scrX = blob.pos[0] - (60  << 8); }
		if (scrX        - blob.pos[0] < (-164 << 8)) { scrX = blob.pos[0] - (164 << 8); }
		if (blob.pos[1] - scrY        < (60   << 8)) { scrY = blob.pos[1] - (60  << 8); }
		if (scrY        - blob.pos[1] < (-84  << 8)) { scrY = blob.pos[1] - (84  << 8); }
		if (scrY < 0 ) { scrY = 0 ; }
		if (scrX < 0 ) { scrX = 0 ; }
		if (scrX > (MAP_64x200_w - 30) << 11) { scrX = (MAP_64x200_w - 30) << 11; }
		moveScreen(MAP_64x200, MAP_64x200_w, MAP_64x200_h, 30, 0, scrX >> 8, scrY >> 8);
	}
	
	return 0;
}

