#ifndef __ACTOR_MUSICPLAYER_HEADER__
#define __ACTOR_MUSICPLAYER_HEADER__

void MusicPlayerPlay(struct Actor* actor);
void MusicPlayerStop(struct Actor* actor);
int MusicPlayerIsPlaying(struct Actor* actor);
void MusicPlayerStopAll();
void PlayerHighestPriorityMusic();

#endif