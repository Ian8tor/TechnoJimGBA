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
			memcpy(&tile_mem[1][2], &GFX_headshots[(c & ~RIGHT_SIDE) * EXPRESSION_LEN], EXPRESSION_LEN);
			addTexBoxPicture(((c & RIGHT_SIDE) != 0) - ((c & RIGHT_SIDE) == 0));
		}
		
		__message_displayed = 1;
	}
	
	if (renderTextBoxText(100))
	{
		//Finds if it should make a decision
		int LR = (conv->decisions[conv->currentMessage] & 0xFFFF0000) == DEFAULT_LR;
		if ((key_hit(PROGRESS_DIALOUGE_KEY) && !LR) || (LR && __conv_vars[LR_INPUT] != 0))
		{
			//Jumps to next conv
			trigger = conv->decisions[conv->currentMessage];
			if (triggered(trigger)) { conv->currentMessage = (trigger & 0x0000FF00) >> 8; }
			else                    { conv->currentMessage = (trigger & 0x000000FF)     ; }
			
			
			if (conv->currentMessage >= conv->messageCount)
			{
				conv->currentMessage = 0;
				__running_conv = -1;
			}
			
			__message_displayed = 0;
		}
	}
	
	return 1;
}




void createAllConversations()
{
	__conv_vars[START] = 0;

	createConversation(0, 6, VAR(START) | GREATER_THAN | VAL(0));
	addMessageToConversation(0, 0, DEFAULT_LR | JPT(1)   | JPF(2), VAR(START)    | SET_VAL | VAL(0) | NO_FACE, "What gender are you\n\nL > Man\nR > Women");
	addMessageToConversation(0, 1, DEFAULT_LR | JPT(250) | JPF(3), VAR(GENDER)   | SET_VAL | VAL(0) | NO_FACE, "You are a \aboy\bgirl\f named \r.\n\nL > Rename\nR > Confirm");   
	addMessageToConversation(0, 2, DEFAULT_LR | JPT(250) | JPF(3), VAR(GENDER)   | SET_VAL | VAL(1) | NO_FACE, "You are a \aboy\bgirl\f named \r.\n\nL > Rename\nR > Confirm");   
	addMessageToConversation(0, 3, DEFAULT_DECISION              , VAR(START)    | SET_VAL | VAL(1) | NO_FACE, "Shutting down...");
	addMessageToConversation(0, 4, DEFAULT_DECISION              , VAR(CONV_NUM) | SET_VAL | VAL(1) | NO_FACE, "Progress not saved...");
	addMessageToConversation(0, 5, DEFAULT_DECISION              ,                                    NO_FACE, "\r...\v\r...\v\t WAKE UP!!!");

	createConversation(1, 18, 0);
	addMessageToConversation(1, 0,  DEFAULT_DECISION              , ARISYA_HAPPY | RIGHT_SIDE, "Wondering all alone kid? Hand over all you have and you wont be hurt");
	addMessageToConversation(1, 1,  DEFAULT_LR | JPT(2)  | JPF(12), NO_FACE                  , "What Would you like to do?\n\nL > Give him everything\nR > Fight back");
	addMessageToConversation(1, 2,  DEFAULT_DECISION              , NO_FACE                  , "You toss him your coin pouch");
	addMessageToConversation(1, 3,  DEFAULT_DECISION              , ARISYA_HAPPY | RIGHT_SIDE, "I said everything");
	addMessageToConversation(1, 4,  DEFAULT_DECISION              , ARISYA_SHOCK | RIGHT_SIDE, "You're- the \aprince\bprincess\f!");
	addMessageToConversation(1, 5,  DEFAULT_DECISION              , ARISYA_HAPPY | RIGHT_SIDE, "Come with me quietly and I'll ransom you off to our dear king before he croaks... Otherwise. He can bury a child first.");
	addMessageToConversation(1, 6,  DEFAULT_DECISION              , ARISYA_SHOCK | LEFT_SIDE , "Lay one hand on my \abrother\bsister\f and you'll be spending the night in a healing pod");
	addMessageToConversation(1, 7,  DEFAULT_DECISION              , ARISYA_SAD   | RIGHT_SIDE, "I don't want trouble. A man needs to eat, you understand...");
	addMessageToConversation(1, 8,  DEFAULT_DECISION              , ARISYA_HAPPY | LEFT_SIDE , "Big \abrother\bsister\f, are you okay? He didn't hurt you, did he");
	addMessageToConversation(1, 9,  DEFAULT_LR | JPT(10) | JPF(11), NO_FACE                  , "You respone\n\nL > I'm fine thank you for the help\nR > I didn't need help");
	addMessageToConversation(1, 10,              JPT(250)         , ARISYA_HAPPY | LEFT_SIDE , "Anytime, I've always got your back just as you have mine.");
	addMessageToConversation(1, 11,              JPT(250)         , ARISYA_SHOCK | LEFT_SIDE , "Of course not! You could have taken him with a single punch!");
	addMessageToConversation(1, 12, DEFAULT_DECISION              , NO_FACE                  , "You charge at him, you have never been the strongest or the best fighter, but maybe you can take this man. Maybe you can prove yourself. The first hit knocks you off your feet and as you stand he pulls a weapon. You don't even have time to scream as he fires, the air splashes red as you fall, a pool growing around you.");
	addMessageToConversation(1, 13, DEFAULT_DECISION              , ARISYA_SAD   | LEFT_SIDE , "You're awake. I was so worried");
	addMessageToConversation(1, 14, DEFAULT_DECISION              , ARISYA_HAPPY | LEFT_SIDE , "How do you feel?");
	addMessageToConversation(1, 15, DEFAULT_LR | JPT(16) | JPF(17), NO_FACE                  , "You feel?\n\nL > Fine\nR > Like I was hit by a hover bus");
	addMessageToConversation(1, 16,              JPT(250)         , ARISYA_SAD   | LEFT_SIDE , "The healing pods are effective, but you still need to be careful, you could have died");
	addMessageToConversation(1, 17,              JPT(250)         , ARISYA_SAD   | LEFT_SIDE , "You still look pretty beat up. That guy really did a number on you. But I'm glad you're safe now.");
	
}