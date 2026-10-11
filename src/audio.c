#include "audio.h"

Vector3 listener_forward = { 0.0f };
Vector3 listener_position = { 0.0f };

float channel_volume[ChannelLowPiority] = { 1.0f, 1.0f, 1.0f, 1.0f, 1.0f };

void SetListener(Vector3 pos, Vector3 facing)
{
	listener_position = pos;
	listener_forward = facing;
}

void SetListenerPosition(Vector3 pos)
{
	listener_position = pos;
}

void SetListenerFacing(Vector3 facing)
{
	listener_forward = facing;
}

Vector3 GetListenerPosition()
{
	return listener_position;
}

Vector3 GetListenerFacing()
{
	return listener_forward;
}


void TOA_PlaySound(char* path, float volume, float pan, float pitch)
{
	Sound* base_sound = AssetGet_Sound(path);
	Sound alias = LoadSoundAlias(*base_sound);
	SetSoundVolume(alias, volume);
	SetSoundPan(alias, pan);
	SetSoundPitch(alias, pitch);
	PlaySound(alias);
}

void TOA_Play3DSound(char* path, Vector3 position, float range, float volume, float pitch)
{
	// Solve to listener
	Vector3 dir = VEC3DIRECTION(listener_position, position);
	float dist = Vector3Distance(listener_position, position);
	if (dist > range)
		return;

	// Solve panning
	float pan = 0.0f;
	float perc_dist = 1.0f -(dist / range);
	Vector3 listener_perpen = Vector3Perpendicular(listener_forward);
	pan = Vector3DotProduct(listener_perpen, dir) * AUDIO_PAN_LIMIT;
	if (dist < AUDIO_PAN_NEAREST_DIST) // Being this close to the audio reduce its panning
		pan *= (dist / AUDIO_PAN_NEAREST_DIST);

	TOA_PlaySound(path, perc_dist * volume, pan, pitch);
}

void ChannelPlaySound(char* path, int channel, float volume, float pan, float pitch)
{
	TOA_PlaySound(path, volume * channel_volume[channel], pan, pitch);
}

void ChannelPlay3DSound(char* path, Vector3 position, int channel, float range, float volume, float pitch)
{
	TOA_Play3DSound(path, position, range, volume * channel_volume[channel], pitch);
}