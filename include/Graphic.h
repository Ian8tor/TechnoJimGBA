#ifndef GRAPHIC_H
#define GRAPHIC_H

#define TIL_font_bytes 4096
extern const unsigned short TIL_font_palette[16];
extern const unsigned char  TIL_font[TIL_font_bytes];

#define TIL_sand_bytes 544
extern const unsigned short TIL_sand_palette[16];
extern const unsigned char  TIL_sand[TIL_sand_bytes];

#define TIL_sand2_bytes 544
extern const unsigned short TIL_sand2_palette[16];
extern const unsigned char  TIL_sand2[TIL_sand2_bytes];



#define MAP_map_w 64
#define MAP_map_h 32
extern const unsigned short MAP_map[MAP_map_w * MAP_map_h]; 

#define MAP_64x64_w 64
#define MAP_64x64_h 64
extern const unsigned short MAP_64x64[MAP_64x64_w * MAP_64x64_h];



#define GFX_sadSlime_bytes 128
extern const unsigned short GFX_sadSlime_palette[48];
extern const unsigned char  GFX_sadSlime[GFX_sadSlime_bytes];


//HeadShots
#define LEFT_SIDE  0x0000
#define RIGHT_SIDE 0x8000

#define EXPRESSION_LEN 800
#define ARISYA_HAPPY   0
#define ARISYA_SHOCK   1
#define ARISYA_SAD     2
#define NO_FACE        127

#define GFX_headshots_bytes 2400
extern const unsigned short GFX_headshots_palette[16];
extern const unsigned char  GFX_headshots[GFX_headshots_bytes];




#endif