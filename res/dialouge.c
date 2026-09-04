#include "Conversation.h"

#include "Graphic.h"


void createAllConversations()
{
	//VAR(START) | GREATER_THAN | VAL(0)
	createConversation(0,9,VAR(START) | GREATER_THAN | VAL(0));
	addMessageToConversation(0,0,DEFAULT_DECISION,ARISYA_HAPPY|LEFT_SIDE,"Hi there! We can still be friends...\nCant we?\n");
	addMessageToConversation(0,1,DEFAULT_DECISION,ARISYA_SAD|RIGHT_SIDE,"Yellow, you can stand can't you.");
	addMessageToConversation(0,2,DEFAULT_DECISION,ARISYA_SHOCK|LEFT_SIDE,"I...");
	addMessageToConversation(0,3,DEFAULT_DECISION,ARISYA_SAD|RIGHT_SIDE, "Take purple and get out of here.");
	addMessageToConversation(0,4,DEFAULT_DECISION,ARISYA_HAPPY|LEFT_SIDE, "Wait! We can talk this out. I'm sure deep down they are nice...\vRight?");
	addMessageToConversation(0,5,DEFAULT_DECISION,ARISYA_SAD|RIGHT_SIDE, "LEAVE NOW!");
	addMessageToConversation(0,6,DEFAULT_DECISION,ARISYA_HAPPY|LEFT_SIDE, "Wait! We can't just leave her.");
	addMessageToConversation(0,7,DEFAULT_DECISION,ARISYA_SHOCK|LEFT_SIDE, "...I'm\n...Sorry.");
	addMessageToConversation(0,8,DEFAULT_DECISION,ARISYA_SAD|RIGHT_SIDE, "Go ahead\nFinish me. I'll gladly die to see my sisters safe.\vCoward.");
	
	
	
	/*
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
	
	createConversation(2, 18, 0);
	addMessageToConversation(2, 0,  DEFAULT_DECISION              , ARISYA_HAPPY   | LEFT_SIDE,  "Arisya enters the room, her smile fades, turning downcast."
	addMessageToConversation(2, 1,  DEFAULT_DECISION              , ARISYA_SAD     | RIGHT_SIDE, "I actually came to get you… It’s father… He is getting worse. The doctors said it will be soon. We need to hurry.";
	addMessageToConversation(2, 2,  DEFAULT_DECISION              , NO_FACE                    , "You get up and head out of the room, Arisya follows a few steps behind.";
	addMessageToConversation(2, 3,  DEFAULT_DECISION              , NO_FACE                    , "You are in a large manor standing outside of a door. Arisya paces the hallway, but stops when a doctor steps out.";
	addMessageToConversation(2, 4,  DEFAULT_DECISION              , DOCTOR_CONCERN | LEFT_SIDE,  "Your father wishes to speak with you, my \aprince\bprincess\f!";
	addMessageToConversation(2, 5,  DEFAULT_DECISION              , FATHER_WEAK    | RIGHT_SIDE, "Arisya steps to the side allowing you to pass. You step into the room, a healing pod taking up most of the space, other medical supplies dot the sterile room. The sound of labored breaths and beeping machinery fills the space.";
	addMessageToConversation(2, 6,  DEFAULT_DECISION              , FATHER_WEAK    | RIGHT_SIDE, "My child."
	addMessageToConversation(2, 7,  DEFAULT_DECISION              , FATHER_WEAK    | LEFT_SIDE,  "You step closer, your fathers labored breaths become louder, his eyes slowly opening and he reaches for you with shaking hands.";
	addMessageToConversation(2, 8,  DEFAULT_LR | JPT(9) | JPF(11) , NO_FACE                     ,"You respond by\n\nL > Taking his hand\nR > Keeping your distance");
	addMessageToConversation(2, 9,  DEFAULT_DECISION              , FATHER_SMILE   | LEFT_SIDE,  "You take his hand gently as if not to break him.";
	addMessageToConversation(2, 10,               JPT(13)         , FATHER_SMILE   | LEFT_SIDE,  "My \aboy\bgirl\f, I wish I could see the \aman\bwoman\f you will become…";
	addMessageToConversation(2, 11, DEFAULT_DECISION              , FATHER_SAD     | RIGHT_SIDE, "You remain where you are, unwilling to get closer. Father lowers his hand.";
	addMessageToConversation(2, 12, DEFAULT_DECISION              , FATHER_SAD     | RIGHT_SIDE, "It's ok, child, neither of us are comforted by touch.";
	addMessageToConversation(2, 13, DEFAULT_DECISION              , NO_FACE        | RIGHT_SIDE, "Father takes a gurgling breath";
	addMessageToConversation(2, 14, DEFAULT_DECISION              , FATHER_WEAK    | LEFT_SIDE,  “But know I love you. Even if I’m not there to watch you grow or see the future you help create. Help your sister, be her sword. Arisya will receive the GameBoy, but together is the only way you can create a better world.";
	
	*/
	
}

/*

	*/
