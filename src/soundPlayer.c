#include "SoundPlayer.h"

#include "Toolbox.h"

//Controls for playing sounds
u32   __sounds_remain  [2] = {U32_MAX, U32_MAX};
vu32* __sounds_dma_ctrl[2] = {(vu32*)0x040000C4, (vu32*)0x040000D0};
vu32* __sounds_dma_dest[2] = {(vu32*)0x040000C0, (vu32*)0x040000CC};
vu32* __sounds_dma_sorc[2] = {(vu32*)0x040000BC, (vu32*)0x040000C8};
vu8*  __sounds_fifo    [2] = {FIFO_BUFFER_A    , FIFO_BUFFER_B    };

//For timer 1 only
void timerInturupt()
{
	if (REG_IF & IRQ_TIMER1)
	{
		//Turns off the timers
		REG_TIMER0_CONTROL &= ~TIMER_ENABLE;
		REG_TIMER1_CONTROL &= ~TIMER_ENABLE;
		
		//Disables music
		if (__sounds_remain[0] == 0) { __sounds_remain[0] = U32_MAX;   *__sounds_dma_ctrl[0] &= ~DMA_ENABLE; }
		if (__sounds_remain[1] == 0) { __sounds_remain[1] = U32_MAX;   *__sounds_dma_ctrl[1] &= ~DMA_ENABLE; }
		
		//Reenables timer if one of the sounds is still running
		int shortest_remain = MIN(__sounds_remain[0], __sounds_remain[1]);
		if (shortest_remain != U32_MAX)
		{
			int play_len = MIN(shortest_remain, 0xFFFF);
			REG_TIMER1_DATA = 0xFFFF - play_len;
			
			if (__sounds_remain[0] != U32_MAX) { __sounds_remain[0] -= play_len; }
			if (__sounds_remain[1] != U32_MAX) { __sounds_remain[1] -= play_len; }

			REG_TIMER1_CONTROL |= TIMER_ENABLE;
		}
		
		//Restarts the timers
		REG_TIMER0_CONTROL |= TIMER_ENABLE;
		
		//Tells the GBA inturupt has been handeled
		REG_IF = IRQ_TIMER1;
	}
}


//Setup the inturupt to go to the given function
void setUpInturupt(fnptr function, u32 inturupts)
{
	REG_ISR_MAIN = function;
	REG_IE |= inturupts;
	REG_IME = 1;
}


//Turns on all the sound related functions
void enableSound()
{
	//Turns on timer 1's inturupt for controlling length of sound
	setUpInturupt(timerInturupt, IRQ_TIMER1);
	REG_TIMER1_CONTROL = TIMER_IRQ | TIMER_CASCADE;
	
	//Resets all DMA and TIMERS needed
	REG_TIMER0_CONTROL = 0;
	REG_DMA1_CONTROL = 0;
	REG_DMA2_CONTROL = 0;
	
	//Turns on channel 1 and 2 for L and R
	REG_SOUND_CONTROL |= SND_A_RIGHT_CHANNEL | SND_A_LEFT_CHANNEL | SND_A_FIFO_RESET;
	REG_SOUND_CONTROL |= SND_B_RIGHT_CHANNEL | SND_B_LEFT_CHANNEL | SND_B_FIFO_RESET;
	REG_MASTER_SOUND = SND_MASTER_ENABLE;
}


//Sets the sample rate for timer 0
void setSampleRate(int rate)
{
	unsigned short ticks_per_sample = CLOCK / (rate);
	REG_TIMER0_CONTROL &= ~TIMER_ENABLE;
	REG_TIMER0_DATA    = 65536 - ticks_per_sample;
	REG_TIMER0_CONTROL = TIMER_ENABLE | TIMER_FREQ_1;
}


//Plays a sound on the given channel for the given time
void playSound(const signed char* sound, int length, int channel)
{
	int o_channel = (channel == 0);
	__sounds_remain[channel] = length - 18;  //The -18 is because of a weird pop sound that plays at the end
	
	//Turns off the timers
	REG_TIMER0_CONTROL &= ~TIMER_ENABLE;
	REG_TIMER1_CONTROL &= ~TIMER_ENABLE;
	if (__sounds_remain[o_channel] != U32_MAX)
	{
		__sounds_remain[o_channel] += 0xFFFF - (REG_TIMER1_DATA & 0xFFFF);  //Adds the skipped time back
	}
	
	//Sets up sound
	*__sounds_dma_ctrl[channel] &= ~DMA_ENABLE;  //Disables previous sound
	*__sounds_dma_sorc[channel]  = (unsigned int) sound;
	*__sounds_dma_dest[channel]  = (unsigned int) __sounds_fifo[channel];
	*__sounds_dma_ctrl[channel]  = DMA_DEST_FIXED | DMA_REPEAT | DMA_32 | DMA_SYNC_TO_TIMER | DMA_ENABLE;
	
	//Gets the shortest sounds remaining time
	int play_len = MIN(MIN(__sounds_remain[channel], __sounds_remain[o_channel]), 0xFFFF);
	REG_TIMER1_DATA = 0xFFFF - play_len;
	if (__sounds_remain[0] != U32_MAX) { __sounds_remain[0] -= play_len; }
	if (__sounds_remain[1] != U32_MAX) { __sounds_remain[1] -= play_len; }
	
	//Restarts the timers
	REG_TIMER1_CONTROL |= TIMER_ENABLE;
	REG_TIMER0_CONTROL |= TIMER_ENABLE;
}


int isPlaying(int channel)
{
	return __sounds_remain[channel] != U32_MAX;
}
