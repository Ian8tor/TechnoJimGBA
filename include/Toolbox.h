// === NOTES ===
//   This is a set of typedefs and #defines that can be
//   found in tonclib and might not be final.




//#pragma once equivlent
#pragma once
#ifndef TOOLBOX_H
#define TOOLBOX_H

#define ALIGN(n) __attribute__((aligned(n)))
#define PACKED   __attribute__((packed))
#define BIT(n)				( 1<<(n) )
#define INLINE   static inline

// * * * (from tonc_types.h) * * *
#define U32_MAX 0xFFFFFFFF
#define U16_MAX 0xFFFF
#define U8_MAX  0xFF
typedef unsigned char  u8 ; typedef char  s8 ; 
typedef unsigned short u16; typedef short s16;
typedef unsigned int   u32; typedef int   s32;
typedef volatile unsigned char  vu8 ; typedef volatile char  vs8 ; 
typedef volatile unsigned short vu16; typedef volatile short vs16;
typedef volatile unsigned int   vu32; typedef volatile int   vs32;

//Tiles
typedef struct {u32 data[8 ]; } TILE ; //s-tiles 32byte
typedef struct {u32 data[16]; } TILE8; //d-tiles 64byte
typedef TILE  CHARBLOCK [512];
typedef TILE8 CHARBLOCK8[256];

typedef struct OBJ_ATTR
{
	u16 attr0;
	u16 attr1;
	u16 attr2;
	s16 filler;
} ALIGN(4) OBJ_ATTR;

typedef struct OBJ_AFFLINE
{
	u16 filler0[3];
	s16 pa;
	u16 filler1[3];
	s16 pb;
	u16 filler2[3];
	s16 pc;
	u16 filler3[3];
	s16 pd;
} ALIGN(4) OBJ_AFFINE;

typedef u16 COLOR;



void oamInit(OBJ_ATTR* obj, u32 count);
void oamCopy(OBJ_ATTR* dst, const OBJ_ATTR* src, u32 count);
u32 se_index(u32 tx, u32 ty, u32 pitch);

INLINE OBJ_ATTR* obj_set_attr(OBJ_ATTR* obj, u16 a0, u16 a1, u16 a2);
INLINE void obj_set_pos(OBJ_ATTR* obj, int x, int y);
INLINE void obj_hide(OBJ_ATTR* obj);
INLINE void obj_unhide(OBJ_ATTR* obj);

INLINE int mathMod(int a, int m) { return ((a % m) + m) % m; }  


//Tile location in memory
typedef u16 SCR_ENTRY;
typedef struct
{
	s16 pa, pb;
	s16 pc, pd;
	s32 dx, dy;
} ALIGN(4) BG_AFFINE;
typedef struct { s16 x, y; } ALIGN(4) BG_POINT;
typedef SCR_ENTRY SCREENBLOCK[1024];

//Memory mapping
#define REG_BASE 0x04000000
#define se_mem          ((SCREENBLOCK*)MEM_VRAM)
#define bg_pal_mem      ((u16*)        MEM_BG_PALETTE)
#define REG_BGCNT       ((vu16*)     (REG_BASE + 0x0008))
#define REG_BG_OFS      ((BG_POINT*) (REG_BASE + 0x0010))
#define REG_BG_AFFINE   ((BG_AFFINE*)(REG_BASE + 0x0000))

#define BG_PRIO(n)     (0x0001 * (n))
#define BG_CBB(n)      (0x0004 * (n))
#define BG_MOSAIC       0x0040
#define BG_4BPP         0x0000
#define BG_8BPP         0x0080
#define BG_SBB(n)      (0x0100 * (n))
#define BG_WRAP         0x2000

#define BG_TILE_HF      0x0400
#define BG_TILE_VF      0x0800
#define BG_TILE_PB(n)   0x1000

#define BG_REG_32x32    0x0000
#define BG_REG_64x32    0x4000
#define BG_REG_32x64    0x8000
#define BG_REG_64x64    0xC000
#define BG_AFF_16x16    0x0000
#define BG_AFF_32x32    0x4000
#define BG_AFF_64x64    0x8000
#define BG_AFF_128x128  0xC000




// * * * (from tonc_memmap.h) * * *
#define MEM_IO          0x04000000
#define MEM_VRAM        0x06000000
#define MEM_OAM         0x07000000
#define MEM_TILE        0x06000000
#define MEM_BG_PALETTE  0x05000000
#define MEM_OBJ_PALETTE 0x05000200

#define tile_mem  ((CHARBLOCK *)0x06000000)
#define tile8_mem ((CHARBLOCK8*)0x06000000)

#define REG_DISPCNT  *((vu32*)(MEM_IO+0x0000))
#define REG_VCOUNT   *( vu32*)0x04000006
#define REG_KEYINPUT *( vu32*)0x04000130

#define KEY_A      0x0001
#define KEY_B      0x0002
#define KEY_SELECT 0x0004
#define KEY_START  0x0008
#define KEY_RIGHT  0x0010
#define KEY_LEFT   0x0020
#define KEY_UP     0x0040
#define KEY_DOWN   0x0080
#define KEY_R      0x0100
#define KEY_L      0x0200

#define KI_A      0
#define KI_B      1
#define KI_SELECT 2
#define KI_START  3
#define KI_RIGHT  4
#define KI_LEFT   5
#define KI_UP     6
#define KI_DOWN   7
#define KI_R      8
#define KI_L      9

#define SIGN(v) ((v > 0) - (v < 0))
#define ABS(n) (((n) > 0)*(n) - ((n) < 0)*(n)) 
#define KEY_MASK     0x03FF


// * * * (from tonc_memdef.h) * * *
#define DCNT_MODE0  0x0000
#define DCNT_MODE1  0x0001
#define DCNT_MODE2  0x0002
#define DCNT_MODE3  0x0003
#define DCNT_MODE4  0x0004
#define DCNT_MODE5  0x0005

#define DCNT_BG0    0x0100
#define DCNT_BG1    0x0200
#define DCNT_BG2    0x0400
#define DCNT_BG3    0x0800
#define DCNT_OBJ    0x1000

#define DCNT_OBJ_1D 0x0040

#define DCNT_PAGE   0x0010


//Bitfeild set and get rotuines
#define BF_PREP(x, name)   (((x) << name##_SHIFT) &  name##_MASK )
#define BF_GET(x, name)    (((x) &  name##_MASK ) >> name##_SHIFT)
#define BF_SET(y, x, name)  (y = ((y) & ~name##_MASK) | BF_PREP(x, name))

// * * * OBJECT * * *
//ATTR0
#define ATTR0_Y_MASK    0x00FF
#define ATTR0_Y_SHIFT   0x0000
#define ATTR0_Y(n)      ((n)<<ATTR0_Y_SHIFT)

#define ATTR0_OM_MASK   0x0300
#define ATTR0_OM_SHIFT  0x0008
#define ATTR0_OM(n)     ((n)<<ATTR0_OM_SHIFT)

#define ATTR0_GM_MASK   0x0C00
#define ATTR0_GM_SHIFT  0x000A
#define ATTR0_GM(n)     ((n)<<ATTR0_GM_SHIFT)

#define ATTR0_MOS_MASK  0x1000
#define ATTR0_MOS_SHIFT 0x000C
#define ATTR0_MOS(n)    ((n)<<ATTR0_MOS_SHIFT)

#define ATTR0_CM_MASK   0x2000
#define ATTR0_CM_SHIFT  0x000D
#define ATTR0_CM(n)     ((n)<<ATTR0_CM_SHIFT)

#define ATTR0_SH_MASK   0xC000
#define ATTR0_SH_SHIFT  0x000E
#define ATTR0_SH(n)     ((n)<<ATTR0_SH_SHIFT)

#define ATTR0_BUILD(sh, cm, mos, gm, om, y)               \
(                                                         \
	(((sh)&3)<<14) | (((cm)&1)<<13) | (((mos)&1  )<<12) | \
	(((gm)&3)<<10) | (((om)&3)<<8 ) | (((y  )&255)<<0 )   \
)


//ATTR1
#define ATTR1_X_MASK    0x01FF
#define ATTR1_X_SHIFT   0x0000
#define ATTR1_X(n)      ((n)<<ATTR1_X_SHIFT)

#define ATTR1_HF_MASK   0x1000
#define ATTR1_HF_SHIFT  0x000C
#define ATTR1_HF(n)     ((n)<<ATTR1_HF_SHIFT)

#define ATTR1_VF_MASK   0x2000
#define ATTR1_VF_SHIFT  0x000D
#define ATTR1_VF(n)     ((n)<<ATTR1_VF_SHIFT)

#define ATTR1_AID_MASK  0x3E00
#define ATTR1_AID_SHIFT 0x0009
#define ATTR1_AID(n)    ((n)<<ATTR1_AID_SHIFT)

#define ATTR1_SZ_MASK   0xC000
#define ATTR1_SZ_SHIFT  0x000E
#define ATTR1_SZ(n)     ((n)<<ATTR1_SZ_SHIFT)

#define ATTR1_BUILD(sz, vf, hf, x)                                     \
(                                                                      \
	(((sz)&3)<<14) | (((vf)&1)<<13) | (((hf)&1)<<12) | (((x)&255)<<0 ) \
)
#define ATTR1_BUILD_A(sz, aid, y)                      \
(                                                      \
	(((sz)&3)<<14) | (((aid)&31)<<9) | (((x)&255)<<0 ) \
)


//ATTR2
#define ATTR2_TID_MASK  0x03FF
#define ATTR2_TID_SHIFT 0x0000
#define ATTR2_TID(n)    ((n)<<ATTR2_TID_SHIFT)

#define ATTR2_PR_MASK   0x0C00
#define ATTR2_PR_SHIFT  0x000A
#define ATTR2_PR(n)     ((n)<<ATTR2_PR_SHIFT)

#define ATTR2_PB_MASK  0xF000
#define ATTR2_PB_SHIFT 0x000C
#define ATTR2_PB(n)    ((n)<<ATTR2_PB_SHIFT)

#define ATTR2_BUILD(pb, pr, tid)                        \
(                                                       \
	(((pb)&15)<<9) | (((pr)&3)<<10) | (((tid)&1023)<<0) \
)



//Sound Registers
#define REG_SND1SWEEP   *(vu16*)(REG_BASE+0x0060)
#define REG_SND1CNT     *(vu16*)(REG_BASE+0x0062)
#define REG_SND1FREQ    *(vu16*)(REG_BASE+0x0064)
#define REG_SND2CNT     *(vu16*)(REG_BASE+0x0068)
#define REG_SND2FREQ    *(vu16*)(REG_BASE+0x006C)
#define REG_SND3SEL     *(vu16*)(REG_BASE+0x0070)
#define REG_SND3CNT     *(vu16*)(REG_BASE+0x0072)
#define REG_SND3FREQ    *(vu16*)(REG_BASE+0x0074)
#define REG_SND4CNT     *(vu16*)(REG_BASE+0x0078)
#define REG_SND4FREQ    *(vu16*)(REG_BASE+0x007C)
#define REG_SNDDMGCNT   *(vu16*)(REG_BASE+0x0080)
#define REG_SNDDSCNT    *(vu16*)(REG_BASE+0x0082)
#define REG_SNDSTAT     *(vu16*)(REG_BASE+0x0084)
#define REG_SNDBIAS     *(vu16*)(REG_BASE+0x0088)


//REG_SNDDMGCNT
#define SDMG_SQR1       0x01
#define SDMG_SQR2       0x02
#define SDMG_WAVE       0x04
#define SDMG_NOISE      0x08

#define SDMG_BUILD(_lm, _rm, _lv, _rv)  (((_lv)&7) | (((_rv)&7)<<4) | ((_lm)<<8) | ((_rm)<<12))
#define SDMG_BUILD_LR(_m, _v)           SDMG_BUILD(_m, _m, _v, _v)


//REG_SNDDSCNT
#define SDS_DMG25      0x0000
#define SDS_DMG50      0x0001
#define SDS_DMG100     0x0002
#define SDS_A50        0x0000
#define SDS_A100       0x0004
#define SDS_B50        0x0000
#define SDS_B100       0x0008
#define SDS_AR         0x0100
#define SDS_AL         0x0200
#define SDS_ATMR       0x0400
#define SDS_ARESET     0x0800
#define SDS_BR         0x1000
#define SDS_BL         0x2000


//REG_SNDSTAT
#define SSTAT_SQR1     0x0001
#define SSTAT_SQR2     0x0002
#define SSTAT_WAVE     0x0004
#define SSTAT_NOISE    0x0008

#define SSTAT_DISABLE  0x0000
#define SSTAT_ENABLE   0x0080


//REG_SNDxCNT
#define SSQR_LEN(n)   (0x0001 * (n))
#define SSQR_DUTY1_8   0x0000
#define SSQR_DUTY1_4   0x0040
#define SSQR_DUTY1_2   0x0080
#define SSQR_DUTY3_4   0x00B0
#define SSQR_TIME(n)  (0x00F0 * (n))
#define SSQR_DEC       0x0000
#define SSQR_INC       0x0800
#define SSQR_IVOL(n)  (0x1000 * (n))

#define SSQR_ENV_BUILD(v, d, t) (SSQR_IVOL(v) | SSQR_TIME(t) | ((d)*0x0800))


//REG_SNDxFREQ
#define SFREQ_RATE(n)  (0x0001 * (n))
#define SFREQ_HOLD      0x0000
#define SFREQ_TIMED     0x4008
#define SFREQ_RESET     0x8000


//REG_SND1SWEEP
#define SSW_OFF         0x0000
#define SSW_SHIFT(n)   (0x0001 * (n))
#define SSW_INC         0x0000
#define SSW_DEC         0x0008
#define SSW_TIME(n)    (0x0010 * (n))


//Master sound registers
#define REG_MASTER_SOUND       *(vu16*)(0x04000084)
#define REG_SOUND_CONTROL      *(vu16*)(0x04000082)

#define SND_MASTER_ENABLE      0x0080
#define SND_A_RIGHT_CHANNEL    0x0100
#define SND_A_LEFT_CHANNEL     0x0200
#define SND_A_FIFO_RESET       0x0800
#define SND_B_RIGHT_CHANNEL    0x1000
#define SND_B_LEFT_CHANNEL     0x2000
#define SND_B_FIFO_RESET       0x8000





//Where sound samples are placed for channels
#define FIFO_BUFFER_A          (vu8 *)(0x040000A0)
#define FIFO_BUFFER_B          (vu8 *)(0x040000A4)


//DMA Registers
#define REG_DMA1_SOURCE        *(vu32*)(0x040000BC)
#define REG_DMA1_DESTINATION   *(vu32*)(0x040000C0)
#define REG_DMA1_CONTROL       *(vu32*)(0x040000C4)
#define REG_DMA2_SOURCE        *(vu32*)(0x040000C8)
#define REG_DMA2_DESTINATION   *(vu32*)(0x040000CC)
#define REG_DMA2_CONTROL       *(vu32*)(0x040000D0)


#define DMA_ENABLE         0x80000000
#define DMA_DEST_FIXED     0x00400000
#define DMA_REPEAT         0x02000000
#define DMA_32             0x04000000
#define DMA_SYNC_TO_TIMER  0x30000000
#define DMA_IRQ            0x40000000


//Timer
#define CLOCK 16777216
#define REG_TIMER0_DATA        *(vu16*)(0x04000100)
#define REG_TIMER0_CONTROL     *(vu16*)(0x04000102)
#define REG_TIMER1_DATA        *(vu16*)(0x04000104)
#define REG_TIMER1_CONTROL     *(vu16*)(0x04000106)
#define REG_TIMER2_DATA        *(vu16*)(0x04000108)
#define REG_TIMER2_CONTROL     *(vu16*)(0x0400010A)

#define TIMER_FREQ_1           0x0000
#define TIMER_FREQ_64          0x0002
#define TIMER_FREQ_256         0x0003
#define TIMER_FREQ_1024        0x0004
#define TIMER_ENABLE           0x0080
#define TIMER_CASCADE          0x0004
#define TIMER_IRQ              0x0040




//Inturupt
#define REG_IE      *(vu16*)(0x04000200)
#define REG_IF      *(vu16*)(0x04000202)
#define REG_IME     *(vu16*)(0x04000208)

//REG_IE REG_IF
#define IRQ_VBLANK  0x0001
#define IRQ_HBLANK  0x0002
#define IRQ_VCOUNT  0x0004
#define IRQ_TIMER0  0x0008
#define IRQ_TIMER1  0x0010
#define IRQ_TIMER2  0x0020
#define IRQ_TIMER3  0x0040
#define IRQ_COM     0x0080
#define IRQ_DMA0    0x0100
#define IRQ_DMA1    0x0200
#define IRQ_DMA2    0x0400
#define IRQ_DMA3    0x0800
#define IRQ_KEYPAD  0x1000
#define IRQ_GAMEPAK 0x2000

typedef void (*fnptr)(void);
#define REG_ISR_MAIN *(fnptr*)(0x03007FFC)


#define MIN(a, b) ((a)*((a)<=(b)) + ((b)<(a))*(b))
#define MAX(a, b) ((b)*((a)<=(b)) + ((b)<(a))*(a)) 


//Music notes
typedef enum
{
	NOTE_C=0, NOTE_CIS, NOTE_D  , NOTE_DIS,
	NOTE_E  , NOTE_F  , NOTE_FIS, NOTE_G  ,
	NOTE_GIS, NOTE_A  , NOTE_AIS, NOTE_B
} eSndNoteId;

extern const u32 __snd_rates[12];

#define SND_RATE(note, oct)  (2048 - (__snd_rates[note] >> (4 + (oct))))


// === INLINES ===============================================

//Sets the obj attributes
INLINE OBJ_ATTR* obj_set_attr(OBJ_ATTR* obj, u16 a0, u16 a1, u16 a2)
{
	obj->attr0 = a0; obj->attr1 = a1; obj->attr2 = a2;
	return obj;
}

//Sets the position of a obj
INLINE void obj_set_pos(OBJ_ATTR* obj, int x, int y)
{
	BF_SET(obj->attr0, y, ATTR0_Y);
	BF_SET(obj->attr1, x, ATTR1_X);
}

//Updates the 1 bit needed to hide or unhide the obj
INLINE void obj_hide  (OBJ_ATTR* obj) { obj->attr0 |= 0x0200; }
INLINE void obj_unhide(OBJ_ATTR* obj) { obj->attr0 &= 0xFDFF; }

INLINE int bit_tribool(u32 x, int plus, int minus)
{ return ((x >> plus) & 1) - ((x >> minus) & 1); } 



extern u16 __key_prev;
extern u16 __key_curr;

INLINE void key_poll()
{
	__key_prev = __key_curr;
	__key_curr = ~REG_KEYINPUT & KEY_MASK;
}

void clamp(int* x, int v, int a);


INLINE u32 key_is_down (u32 key) { return  __key_curr & key; }
INLINE u32 key_is_up   (u32 key) { return ~__key_curr & key; }
INLINE u32 key_was_down(u32 key) { return  __key_prev & key; }
INLINE u32 key_was_up  (u32 key) { return ~__key_prev & key; }

INLINE u32 key_hit     (u32 key) { return (__key_curr & ~__key_prev) & key; }
INLINE u32 key_released(u32 key) { return (__key_prev & ~__key_curr) & key; }

INLINE int key_tri_horz()    { return bit_tribool(__key_curr, KI_RIGHT, KI_LEFT); }
INLINE int key_tri_vert()    { return bit_tribool(__key_curr, KI_DOWN , KI_UP  ); }
INLINE int key_tri_sholder() { return bit_tribool(__key_curr, KI_R    , KI_L   ); }
INLINE int key_tri_fire()    { return bit_tribool(__key_curr, KI_A    , KI_B   ); }


















// * * * (from tonc_video.h) * * *
#define SCREEN_WIDTH  240
#define SCREEN_HEIGHT 160
#define vid_mem ((u16*)MEM_VRAM)

//Colors
#define CLR_BLACK  0x0000
#define CLR_RED    0x001F
#define CLR_LIME   0x03E0
#define CLR_YELLOW 0x03FF
#define CLR_BLUE   0x7C00
#define CLR_MAG    0x7C1F
#define CLR_CYAN   0x7FE0
#define CLR_WHITE  0x7FFF

//Mode 3 specific
#define M3_WIDTH 240
#define M3_SIZE 0x12C00

#define M4_WIDTH 240
#define M4_SIZE 0x9600
#define M4_VRAM_BACK 0x0600A000



extern u16* vid_page;


// * * * toolbox.h functs
void vid_vsync();
u16* vid_flip();


void bmp16_line (int x1, int y1, int x2, int y2, u32 clr, void* dstBase, u32 dstPitch);
void bmp16_rect (int x , int y , int w , int h , u32 clr, void* dstBase, u32 dstPitch);
void bmp16_frame(int x , int y , int w , int h , u32 clr, void* dstBase, u32 dstPitch);

void m3_fill(COLOR clr);
void m4_fill(u8 clr);



// * * * PROTOTYPES * * * 
INLINE COLOR RGB15(u32 red, u32 green, u32  blue);
INLINE void m3_plot (int x , int y , COLOR clr);
INLINE void m3_line (int x1, int y1, int x2, int y2, COLOR clr);
INLINE void m3_rect (int x , int y , int w , int h , COLOR clr);
INLINE void m3_frame(int x , int y , int w , int h , COLOR clr);

INLINE void m4_plot (int x , int y , u8 clrId);

// * * * INLINES * * * 
INLINE COLOR RGB15(u32 red, u32 green, u32  blue) { return red | (green<<5) | (blue<<10); }
INLINE void m3_plot (int x , int y , COLOR clr) { vid_mem[(y * SCREEN_WIDTH) + x] = clr; }
INLINE void m3_line (int x1, int y1, int x2, int y2, COLOR clr) { bmp16_line (x1, y1, x2, y2, clr, vid_mem, M3_WIDTH*2); }
INLINE void m3_rect (int x , int y , int w , int h , COLOR clr) { bmp16_rect (x , y , w , h , clr, vid_mem, M3_WIDTH*2); }
INLINE void m3_frame(int x , int y , int w , int h , COLOR clr) { bmp16_frame(x , y , w , h , clr, vid_mem, M3_WIDTH*2); }

INLINE void m4_plot (int x , int y , u8 clrId)
{
	u16 *dst = &vid_page[(y*M4_WIDTH + x)/2];
	if (x & 1) { *dst = (*dst &  0xFF) | (clrId << 8); }
	else       { *dst = (*dst & ~0xFF) |  clrId      ; }
}


//void bmp16_line(int x1, int y1, int x2, int y2, u32 clr, void* dstBase, u32 dstPitch);



#endif