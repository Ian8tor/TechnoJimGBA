#ifndef CONVERSATION_H
#define CONVERSATION_H

#include "Toolbox.h"

#define CONVERSATION_COUNT 100

//Var value branches
#define VAR(n)       (0x04000000 * (n))
#define VAL(n)       (0x00010000 * (n))
#define JPT(n)       (0x00000100 * (n))
#define JPF(n)       (0x00000001 * (n))

//Operators
#define EQUAL_TO     0x00000000
#define NOT_EQUAL_TO 0x01000000
#define GREATER_THAN 0x02000000
#define LESS_THAN    0x03000000
#define DO_NOTHING   0x00000000
#define SET_VAL      0x01000000
#define ADD_TO_VAL   0x02000000
#define SUB_TO_VAL   0x03000000

//Convenence
#define DEFAULT_DECISION 0x0000003D
#define DEFAULT_LR       0x08010000

//Variables index
#define CONV_NUM   1
#define LR_INPUT   2
#define CONV_AUTO  3
#define START      4
#define GENDER     5



//Allows conv_vars to be modified from a different file
extern u32 __conv_vars[64];
extern int __running_conv;


struct Conversation
{
	//Messages
	int messageCount;
	char** messages;
	u32*   decisions;
	u32*   sets;
	
	u32 trigger;

	int currentMessage;
};
extern struct Conversation __all_convs[CONVERSATION_COUNT];


//Setup conversation
void createConversation(int c, int m, u32 t1);
void addMessageToConversation(int c, int i, u32 decision, u32 set, char* message);

//Run conversation
int  triggered(u32 trigger);
void setConvVals(u32 set);
int  runConversations();
void createAllConversations();


#endif
