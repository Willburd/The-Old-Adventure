#ifndef __ACTOR_MUSICPLAYER_HEADER__
#define __ACTOR_MUSICPLAYER_HEADER__

void MusicPlay(struct Actor* actor);
void MusicStop(struct Actor* actor);
int MusicIsPlaying(struct Actor* actor);
void MusicStopAll();
void PlayerHighestPriorityMusic();

#endif