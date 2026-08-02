#ifndef SONGS_H
#define SONGS_H

//Note datatype
struct tone {
	int duration;
	int pitch;
	int volume;
};

#define NUM_SONGS 5

enum song_id {
	SONG_NYAN_CAT = 0,
	SONG_MEGALOVANIA,
	SONG_BAD_PIGGIES,
	SONG_TETRIS_THEME,
	SONG_SUPER_MARIO
};

extern const char * const song_names[NUM_SONGS];
extern const struct tone * const song_arrays[NUM_SONGS];
extern const int song_lengths[NUM_SONGS];

// initialise board for songs
void songs_init(void);

// plays 15s songs
void play_song(int index);

// plays short sound effects
void play_doorbell(void);
void play_alarm_sound(void);
void play_alcohol_alarm_sound(void);

#endif
