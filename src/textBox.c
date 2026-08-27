#include "TextBox.h"

#include "Toolbox.h"
#include "SoundPlayer.h"

#include "Music.h"

//Text box data
char* __text;
int   __cursor = 0; //24.8 fpi
int   __screX  = 0;
int   __screY  = 0;

int   __text_boX = 0;
int   __text_boY = 0;
int   __text_boW = 0;
int   __text_boH = 0;

int   __text_areX = 0;
int   __text_areY = 0;
int   __text_areW = 0;
int   __text_areH = 0;

int   __se_index = 31;
int   __font_pal = 0xF000;


//From textbox.c
#define GENDER     5
extern u32 __conv_vars[64];
char __player_name[11] = "A\0\0\0\0\0\0\0\0\0\0";
int   __player_name_i = 0;
int   __player_name_len = 1;



//Fills a rectangle in the given tilemap layer
void fillLayer(int layer, int pitch, int v, int x, int y, int w, int h)
{
	for (int i = x; i < x+w; i++)
	{
		for (int j = y; j < y+h; j++)
		{
			se_mem[layer][j*pitch + i] = __font_pal + v;
		}
	}
}

//Clears the entire textbox
void clearTextBox()
{
	fillLayer(__se_index, 32, 32, __text_boX+1, __text_boY+1, __text_boW-2, __text_boH-2);
	__text_areX = __text_boX + 1;
	__text_areY = __text_boY + 1;
	__text_areW = __text_boW - 2;
	__text_areH = __text_boH - 2;
	
	__cursor = 0;
	__screX = __text_areX;
	__screY = __text_areY;
}

//Clears only the text area of a textbox
void clearTextBoxText()
{
	fillLayer(__se_index, 32, 32, __text_areX, __text_areY, __text_areW, __text_areH);
	__screX = __text_areX;
	__screY = __text_areY;
}

//Creates a new textbox and sets all the vars
void createTextBox(int x, int y, int w, int h)
{
	//Sets the variables
	__text_boX = x;
	__text_boY = y;
	__text_boW = w;
	__text_boH = h;
	
	
	//Textbox filling
	fillLayer(__se_index, 32, 0, 0, 0, 32, 32);
	clearTextBox();
	
	// * * * Boarders * * *
	//Four corners
	se_mem[__se_index][(y    )*32 + (x    )] = (__font_pal + 29);
	se_mem[__se_index][(y    )*32 + (x+w-1)] = (__font_pal + 29) | BG_TILE_HF;
	se_mem[__se_index][(y+h-1)*32 + (x    )] = (__font_pal + 29) | BG_TILE_VF;
	se_mem[__se_index][(y+h-1)*32 + (x+w-1)] = (__font_pal + 29) | BG_TILE_HF | BG_TILE_VF;
	
	//Four edges
	for (int i = x+1; i < x+w-1; i++) 
	{
		se_mem[__se_index][(y    )*32 + i] = (__font_pal + 30);
		se_mem[__se_index][(y+h-1)*32 + i] = (__font_pal + 30) | BG_TILE_VF;
	}
	for (int i = y+1; i < y+h-1; i++) 
	{
		se_mem[__se_index][i*32 + (x    )] = (__font_pal + 31);
		se_mem[__se_index][i*32 + (x+w-1)] = (__font_pal + 31) | BG_TILE_HF;
	}
}

//-1 left, 1 right
void addTexBoxPicture(int location)
{
	//Fills in the picture
	int x = __text_areX + (__text_areW - 5)*(location == 1);
	int y = __text_areY;

	//Adjusts the text areas size
	__text_areX +=  5*(location == -1);
	__text_areW -=  5;
	__screX = __text_areX;
	__screY = __text_areY;
	

	for (int i = 0; i < 25; i++)
	{
		se_mem[__se_index][32*(y+(i/5)) + x+(i%5)] = __font_pal + 2 + i;
	}
}

//Counts the length of the word starting at text first character
int wordLength(char* text)
{
	int word_len = 0;
	int extra = 0;
	while (true) 
	{ 
		if (text[word_len] == ' ' ) { break; }
		if (text[word_len] == '\n') { break; } 
		if (text[word_len] == '\0') { break; }
		
		if (text[word_len] == '\a') { extra++; } 
		if (text[word_len] == '\b') { extra++; } 
		if (text[word_len] == '\f') { extra++; } 
		
		if (text[word_len] == '\r') { extra -= __player_name_len - 1; }
		if (text[word_len] == '\t') { extra -= __player_name_len - 1; }
		

		word_len++; 
	}
	return word_len - extra;
}

//Renders the text 1 char at a time
int renderTextBoxText(int charsToAdd)
{	
	//Text alreadly ended
	if (__text[__cursor >> 8] == '\0') { return 1; }
	
	//Finishes or progresses dialouge
	if (key_hit(PROGRESS_DIALOUGE_KEY))
	{
		if (__screY < __text_areY + __text_areH) { charsToAdd += 100000; }
		else { clearTextBoxText(); }
	}
		
	//Adjusts the cursor position
	int prev_chars = __cursor >> 8;
	__cursor += charsToAdd;
	int curr_chars = __cursor >> 8; 
	int captial = 0;
	
	for (int i = prev_chars; i < curr_chars; i++)
	{
		switch (__text[i])
		{
			case '\0': //End of text
			__cursor = i << 8;
			return 0; 	
			break;
			
			
			case '\n': //Newline
			__screX = __text_areX; 
			__screY++;
			continue;
			
			
			case '\v': //Newpage
			__screX = __text_areX; 
			__screY = __text_areY + __text_areH;
			i++;
			break;
			
			
			case '\a': //Male exclusive section
			if (__conv_vars[GENDER] == 0) { curr_chars++; }
			else
			{
				while (__text[i] != '\b' && __text[i] != '\f' && __text[i] != '\0') { i++; curr_chars++; }
			}
			__cursor = curr_chars << 8;
			continue;
			
			
			case '\b': //Female exclusive section
			if (__conv_vars[GENDER] == 1) { curr_chars++; }
			else
			{
				while (__text[i] != '\f' && __text[i] != '\a' && __text[i] != '\0') { i++; curr_chars++; }
			}
			__cursor = curr_chars << 8;
			continue;
				
				
			case '\f': //End of exclusive sections
			curr_chars++;
			__cursor = curr_chars << 8;
			continue;
			
			
			case '\t': //NAME ALL UPPER
			captial = (__player_name[__player_name_i] >= 97)*(122 >= __player_name[__player_name_i]);
			
			case '\r': //Name all lower
			if (__screY >= __text_areY + __text_areH)
			{ 
				__cursor = i << 8;
				break;
			}	
			se_mem[__se_index][__screY*32 + __screX] = __font_pal +__player_name[__player_name_i] - 32*captial;
			playSound(SFX_typing, SFX_typing_bytes, 1);
			__player_name_i++;
			__screX++;
			
			if (__player_name_i >= __player_name_len) { __player_name_i = 0; }
			else { i--; curr_chars--; __cursor = curr_chars << 8; }
			
			
			continue;
			
			
			case ' ':
			if (wordLength(&__text[i+1]) + __screX >= __text_areX + __text_areW)
			{
				__screX = __text_areX; 
				__screY++; 
				continue;
			}
			break;
		}
		if (__screY >= __text_areY + __text_areH)
		{ 
			__cursor = i << 8;
			break;
		}	
		
		se_mem[__se_index][__screY*32 + __screX] = __font_pal + __text[i];
		playSound(SFX_typing, SFX_typing_bytes, 1);
		__screX++;
	}
	
	return 0;
}


void clearScreen(int v) { fillLayer(__se_index, 32, v, 0, 0, 32, 32);	}
void renderText(int x, int y, char* text)
{
	int i = 0;
	while (text[i] != '\0')
	{
		if (text[i] == '\n') {x = 0; y++; continue; }
		se_mem[__se_index][y*32 + x] = __font_pal + text[i];
		x++; i++;
	}
}

//Gender symbols\aMale Version\bFemale Version\f
//newline \n
//newpage \v
//name    \r
//NAME    \t