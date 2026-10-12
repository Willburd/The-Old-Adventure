#include "../tools.h"
#include "../assets.h"
#include "../actor_factory.h"
#include "../scene_entry.h"
#include "../game_draw.h"
#include "../collision.h"
#include "json_properties.h"
#include "actor_musicplayer.h"
#include "../audio.h"

// private header
ACTOR_JSON_INIT(musicplayer);
ACTOR_PRELOADASSETS(musicplayer);
ACTOR_CLEANUP(musicplayer);

typedef struct {
	char* asset_path;
	Music* music_data;
	float volume;
	int priority;
} MusicData;

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Public functions
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

ACTOR_INIT(musicplayer)
{
	actor->actor_flags = ACTOR_FLAG_DOES_NOT_TICK | ACTOR_FLAG_IS_INVISIBLE;
	ACTOR_REGISTER_JSON_INIT(musicplayer);
	ACTOR_REGISTER_PRELOADASSETS(musicplayer);
	ACTOR_REGISTER_CLEANUP(musicplayer);

	MALLOC_ACTOR_DATA(MusicData, actor->data);
	MusicData* music_data = (MusicData*)actor->data;
	music_data->asset_path = NULL;
	music_data->music_data = NULL;
	music_data->volume = AUDIO_VOLUME_HALF;
	music_data->priority = 0;
}

void MusicPlayerPlay(struct Actor* actor)
{
	if (actor->is_destroying)
		return;
	MusicData* music_data = (MusicData*)actor->data;
	ChannelPlayMusic(music_data->music_data, music_data->volume, AUDIO_PAN_CENTER, 1.0f);
}

void MusicPlayerStop(struct Actor* actor)
{
	if (!MusicPlayerIsPlaying(actor))
		return;
	MusicData* music_data = (MusicData*)actor->data;
	ChannelStopMusic(music_data->music_data);
}

int MusicPlayerIsPlaying(struct Actor* actor)
{
	MusicData* music_data = (MusicData*)actor->data;
	return IsMusicStreamPlaying(*music_data->music_data);
}

#define SEARCH_COUNT 32
void MusicPlayerStopAll()
{
	struct Actor* music_players[SEARCH_COUNT] = { NULL };
	int found_count = FINDACTORGROUP_BYTYPE(music_players, SEARCH_COUNT, act_musicplayer);
	for (int i = 0; i < found_count; i++)
	{
		struct Actor* target = music_players[i];
		MusicPlayerStop(target);
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
		MusicPlayerPlay(highest_player);
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
	const char* music_path = TextFormat("%s%s%s", ASSET_MUSIC"/", music_data->asset_path, ".ogg");
	LoadAsset_Music(music_path, TRUE);
	music_data->music_data = ChannelStartMusic(music_path, music_data->volume, AUDIO_PAN_CENTER, 1.0f);
	RELEASE(music_data->asset_path);
}

ACTOR_CLEANUP(musicplayer)
{
	MusicPlayerStop(actor);
	MusicData* music_data = (MusicData*)actor->data;
	music_data->music_data = NULL; // Do not release it, that's for the asset loader code to handle.
}