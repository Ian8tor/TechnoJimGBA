#ifndef TEXTBOX_H
#define TEXTBOX_H

//KeyBindings
#define PROGRESS_DIALOUGE_KEY KEY_A

//The text variable
extern char* __text;
extern int   __se_index;

extern char __player_name[11];
extern int   __player_name_len;

//Set up textbox
void fillLayer(int layer, int pitch, int v, int x, int y, int w, int h);
void clearTextBox();
void clearTextBoxText();
void createTextBox(int x, int y, int w, int h);
void addTexBoxPicture(int location);

//Render textbox
int wordLength(char* text);
int renderTextBoxText(int charsToAdd);

//Render Text
void clearScreen(int v);
void renderText(int x, int y, char* text);


#endif
