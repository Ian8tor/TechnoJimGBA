#include "Conversation.h"

#include "TextBox.h"

#include "Graphic.h"

#include <String.h>
#include <stdlib.h>


u32 __conv_vars[64];

struct Conversation __all_convs[CONVERSATION_COUNT];
int __running_conv = -1;
int __message_displayed = 0;


//Creates a new conversation and allocates the space
void createConversation(int c, int m, u32 t1)
{
	//resets the vars
	__all_convs[c].messageCount = m;
	__all_convs[c].currentMessage = 0;
	__all_convs[c].trigger = t1;
	
	//Allocates the space for the messages
	__all_convs[c].messages  = (char**)malloc(sizeof(char*) * m);
	__all_convs[c].decisions = (u32*  )malloc(sizeof(u32  ) * m);
	__all_convs[c].sets      = (u32*  )malloc(sizeof(u32  ) * m);
}

void addMessageToConversation(int c, int i, u32 decision, u32 set, char* message)
{
	__all_convs[c].messages [i] = message ;
	__all_convs[c].decisions[i] = decision;
	__all_convs[c].sets     [i] = set     ;
	
	if (decision == DEFAULT_DECISION) { __all_convs[c].decisions[i] = JPT(i + 1); }
}

int triggered(u32 trigger)
{
	int var = (trigger & 0xFC000000) >> 26;
	int op  = (trigger & 0x03000000);
	int val = (trigger & 0x00FF0000) >> 16;
	
	switch(op)
	{
		case EQUAL_TO    : return __conv_vars[var] == val; break;
		case NOT_EQUAL_TO: return __conv_vars[var] != val; break;
		case GREATER_THAN: return __conv_vars[var]  > val; break;
		case LESS_THAN   : return __conv_vars[var]  < val; break;
	}
	
	return 0;
}

void setConvVals(u32 set)
{
	int var = (set & 0xFC000000) >> 26;
	int op  = (set & 0x03000000);
	int val = (set & 0x00FF0000) >> 16;

	switch(op)
	{
		case DO_NOTHING: break;
		case SET_VAL   : __conv_vars[var]  = val; break;
		case ADD_TO_VAL: __conv_vars[var] += val; break;
		case SUB_TO_VAL: __conv_vars[var] -= val; break;
	}
	__conv_vars[0] = 0;
}

int runConversations()
{	
	u32 trigger;
	if (__running_conv == -1)
	{
		trigger = __all_convs[__conv_vars[CONV_NUM]].trigger;
		if (triggered(trigger)) { __running_conv = __conv_vars[CONV_NUM]; }
		if (__running_conv == -1) { return 0; }
		createTextBox(1, 13, 28, 7);
	}
	
	struct Conversation* conv = &__all_convs[__running_conv];
	__conv_vars[LR_INPUT] = (key_hit(KEY_L) != 0) + 2*(key_hit(KEY_R) != 0);
	
	if (!__message_displayed)
	{
		//Runs sets and loads text
		clearTextBox();
		setConvVals(conv->sets[conv->currentMessage]);
		__text = conv->messages[conv->currentMessage];
		
		//Loads head
		int c = conv->sets[conv->currentMessage] & 0x0000FFFF;
		if (c != NO_FACE)
		{
			memcpy(&tile_mem[0][2], &GFX_headshots[(c & ~RIGHT_SIDE) * EXPRESSION_LEN], EXPRESSION_LEN);
			addTexBoxPicture(((c & RIGHT_SIDE) != 0) - ((c & RIGHT_SIDE) == 0));
		}
		
		__message_displayed = 1;
	}
	
	if (renderTextBoxText(100))
	{
		//Finds if it should make a decision
		int LR = (conv->decisions[conv->currentMessage] & 0xFFFF0000) == DEFAULT_LR;
		if ((key_hit(PROGRESS_DIALOUGE_KEY) && !LR) || 
		    (LR && __conv_vars[LR_INPUT] != 0)      ||
			(conv->messages[conv->currentMessage][0] == 0))
		{
			//Jumps to next conv
			trigger = conv->decisions[conv->currentMessage];
			if (triggered(trigger)) { conv->currentMessage = (trigger & 0x0000FF00) >> 8; }
			else                    { conv->currentMessage = (trigger & 0x000000FF)     ; }
			
			
			if (conv->currentMessage >= conv->messageCount)
			{
				conv->currentMessage = 0;
				
				__running_conv = -1;
				clearScreen(0);
			}
			
			__message_displayed = 0;
		}
	}
	
	return 1;
}

