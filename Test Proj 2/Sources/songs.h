#ifndef SONGS_H
#define SONGS_H

/*
 * A full song would freeze the board for too long, so we have 15 sec clips.
 * Theres a back arrow that trys to quit the song.
 */

#define NUM_SONGS 5
struct tone {
    int duration_ms;	// how long this note plays
    int period_us;
    int volume;
};

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

void play_song(int index);

#endif
