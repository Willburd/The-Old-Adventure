#ifndef __AUDIO_SYSTEM_HEADER__
#define __AUDIO_SYSTEM_HEADER__

#include <raylib.h>
#include "assets.h"
#include "tools.h"

#define AUDIO_PAN_NEAREST_DIST 0.25f
#define AUDIO_PAN_LIMIT 0.6f

#define AUDIO_VOLUME_FULL 1.0f
#define AUDIO_VOLUME_HIGH 0.75f
#define AUDIO_VOLUME_HALF 0.5f
#define AUDIO_VOLUME_LOW 0.25f
#define AUDIO_VOLUME_MUTE 0.0f

#define AUDIO_RANGE_ALL 1000.0f
#define AUDIO_RANGE_HUGE 40.0f
#define AUDIO_RANGE_BIG 30.0f
#define AUDIO_RANGE_AVERAGE 20.0f
#define AUDIO_RANGE_SMALL 10.0f
#define AUDIO_RANGE_TINY 6.0f

enum AudioChannels
{
	ChannelHighPriority,
	ChannelUI,
	ChannelSoundEffects,
	ChannelSoundscape,
	ChannelMusic,
	ChannelLowPiority
};

void SetListener(Vector3 pos, Vector3 facing);
void SetListenerPosition(Vector3 pos);
void SetListenerFacing(Vector3 facing);
Vector3 GetListenerPosition();
Vector3 GetListenerFacing();

void TOA_PlaySound(char* path, float volume, float pan, float pitch);
void TOA_Play3DSound(char* path, Vector3 position, float range, float volume, float pitch);

void ChannelPlaySound(char* path, int channel, float volume, float pan, float pitch);
void ChannelPlay3DSound(char* path, Vector3 position, int channel, float range, float volume, float pitch);

#endif