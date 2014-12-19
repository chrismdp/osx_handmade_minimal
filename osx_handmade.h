#pragma once

#include <stdint.h>
#include <math.h>


// NOTE(jeff): These typedefs are the same as in win32
#define internal static
#define local_persist static
#define global_variable static

#define Pi32 3.14159265359f

typedef int8_t int8;
typedef int16_t int16;
typedef int32_t int32;
typedef int64_t int64;
typedef int32 bool32;

typedef uint8_t uint8;
typedef uint16_t uint16;
typedef uint32_t uint32;
typedef uint64_t uint64;

typedef float real32;
typedef double real64;


struct osx_offscreen_buffer
{
	// NOTE: Pixels are always 32-bits wide. BB GG RR XX
	//BITMAPINFO Info;
	void* Memory;
	int Width;
	int Height;
	int Pitch;
	int BytesPerPixel;
};


struct osx_window_dimension
{
	int Width;
	int Height;
};


struct osx_sound_output
{
	int SamplesPerSecond;
	uint32 RunningSampleIndex;
	int BytesPerSample;
	uint32 SafetyBytes;
	real32 tSine;
	int LatencySampleCount;
};


struct osx_game_code
{
	void* GameCodeDL;
	time_t DLLastWriteTime;

	game_update_and_render* UpdateAndRender;
    game_get_sound_samples *GetSoundSamples;

    bool32 IsValid;
};


//#define OSX_STATE_FILENAME_COUNT PROC_PIDPATHINFO_MAXSIZE
#define OSX_STATE_FILENAME_COUNT FILENAME_MAX

struct osx_state
{
	uint64 TotalSize;
	void* GameMemoryBlock;

	int RecordingHandle;
	int InputRecordingIndex;

	int PlaybackHandle;
	int InputPlayingIndex;

	char AppFilename[OSX_STATE_FILENAME_COUNT];
	char* OnePastLastAppFilenameSlash;
};


#ifndef HANDMADE_USE_ASM_RDTSC
// NOTE(jeff): Thanks to @visitect for this suggestion
#define rdtsc __builtin_readcyclecounter
#else
internal inline uint64
rdtsc()
{
	uint32 eax = 0;
	uint32 edx;

	__asm__ __volatile__("cpuid;"
			     "rdtsc;"
				: "+a" (eax), "=d" (edx)
				:
				: "%rcx", "%rbx", "memory");

	__asm__ __volatile__("xorl %%eax, %%eax;"
			     "cpuid;"
				:
				:
				: "%rax", "%rbx", "%rcx", "%rdx", "memory");

	return (((uint64)edx << 32) | eax);
}
#endif


void OSXGetAppFilename(osx_state *State);

void OSXBuildAppPathFilename(osx_state *State, char *Filename,
                             int DestCount, char *Dest);

DEBUG_PLATFORM_FREE_FILE_MEMORY(DEBUGPlatformFreeFileMemory);
DEBUG_PLATFORM_READ_ENTIRE_FILE(DEBUGPlatformReadEntireFile);
DEBUG_PLATFORM_WRITE_ENTIRE_FILE(DEBUGPlatformWriteEntireFile);

time_t OSXGetLastWriteTime(const char* Filename);

osx_game_code OSXLoadGameCode(const char* SourceDLName, const char* TempDLName);
void OSXUnloadGameCode(osx_game_code* GameCode);


void OSXGetInputFileLocation(osx_state* State, int SlotIndex, int DestCount, char* Dest);
void OSXBeginRecordingInput(osx_state* State, int InputRecordingIndex);
void OSXEndRecordingInput(osx_state* State);
void OSXBeginInputPlayback(osx_state* State, int InputPlayingIndex);
void OSXEndInputPlayback(osx_state* State);
void OSXRecordInput(osx_state* State, game_input* NewInput);
void OSXPlaybackInput(osx_state* State, game_input* NewInput);


