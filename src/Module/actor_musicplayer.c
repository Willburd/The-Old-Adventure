#include "../tools.h"
#include "../assets.h"
#include "../actor_factory.h"
#include "../actor_scene.h"
#include "../game_draw.h"
#include "../collision.h"
#include "json_properties.h"
#include "actor_musicplayer.h"
#include "../audio.h"

// private header
ACTOR_JSON_INIT(musicplayer);
ACTOR_PRELOADASSETS(musicplayer);
ACTOR_PREDRAWWORLD(musicplayer);
ACTOR_CLEANUP(musicplayer);

typedef struct {
	char* asset_path;
	Asset* music_asset;
	float volume;
	int priority;
} MusicData;

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Public functions
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

ACTOR_INIT(musicplayer)
{
	actor->actor_flags = ACTOR_FLAG_DOES_NOT_TICK;
	ACTOR_REGISTER_JSON_INIT(musicplayer);
	ACTOR_REGISTER_PRELOADASSETS(musicplayer);
	ACTOR_REGISTER_PREDRAWWORLD(musicplayer);
	ACTOR_REGISTER_CLEANUP(musicplayer);

	MALLOC_ACTOR_DATA(MusicData, actor->data);
	MusicData* music_data = (MusicData*)actor->data;
	music_data->asset_path = NULL;
	music_data->music_asset = NULL;
	music_data->volume = AUDIO_VOLUME_HALF;
	music_data->priority = 0;
}

void MusicPlay(struct Actor* actor)
{
	if (actor->is_destroying)
		return;
	if (MusicIsPlaying(actor))
		return;
	MusicData* music_data = (MusicData*)actor->data;
	SetMusicVolume(*music_data->music_asset->mus, music_data->volume);
	SetMusicPan(*music_data->music_asset->mus, 0.0f);
	PlayMusicStream(*music_data->music_asset->mus);
}

void MusicStop(struct Actor* actor)
{
	if (!MusicIsPlaying(actor))
		return;
	MusicData* music_data = (MusicData*)actor->data;
	StopMusicStream(*music_data->music_asset->mus);
}

int MusicIsPlaying(struct Actor* actor)
{
	if (actor->is_destroying)
		return FALSE;
	MusicData* music_data = (MusicData*)actor->data;
	return IsMusicStreamPlaying(*music_data->music_asset->mus);
}

#define SEARCH_COUNT 32
void MusicStopAll()
{
	struct Actor* music_players[SEARCH_COUNT] = { NULL };
	int found_count = FINDACTORGROUP_BYTYPE(music_players, SEARCH_COUNT, act_musicplayer);
	for (int i = 0; i < found_count; i++)
	{
		struct Actor* target = music_players[i];
		MusicStop(target);
	}
}

void PlayerHighestPriorityMusic()
{
	int highest_priority = -INFINITY;
	struct Actor* highest_player = NULL;

	struct Actor* music_players[SEARCH_COUNT] = { NULL };
	int found_count = FINDACTORGROUP_BYTYPE(music_players, SEARCH_COUNT, act_musicplayer);
	for (int i = 0; i < found_count; i++)
	{
		struct Actor* target = music_players[i];
		MusicData* music_data = (MusicData*)target->data;
		if (music_data->priority <= highest_priority)
			return;
		highest_player = music_data->priority;
		highest_player = target;
	}
	// Play ours
	if (ACTOR_EXISTS(highest_player))
		MusicPlay(highest_player);
}
#undef SEARCH_COUNT

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Private functions
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

ACTOR_JSON_INIT(musicplayer) 
{
	MusicData* music_data = (MusicData*)actor->data;
	JSON_GET_STRING(music_data->asset_path, file_data, PROP_MUSIC_ID, NULL);
	JSON_GET_INT(music_data->priority, file_data, PROP_MUSIC_PRIORITY, 0);
}

ACTOR_PRELOADASSETS(musicplayer)
{
	MusicData* music_data = (MusicData*)actor->data;
	music_data->music_asset = LoadAsset_Music(TextFormat("%s%s%s", ASSET_MUSIC"/", music_data->asset_path, ".ogg"), FALSE);
	RELEASE(music_data->asset_path);
}

ACTOR_PREDRAWWORLD(musicplayer)
{
	// Using this to update every render, not just gameticks
	MusicData* music_data = (MusicData*)actor->data;
	if(music_data->music_asset != NULL)
		UpdateMusicStream(*music_data->music_asset->mus);
}

ACTOR_CLEANUP(musicplayer)
{
	MusicStop(actor);
	MusicData* music_data = (MusicData*)actor->data;
	music_data->music_asset = NULL; // Do not release it, that's for the asset loader code to handle.
}