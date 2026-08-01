#include "../Headers/stm32f103xe.h"
#include "songs.h"
#include "touch.h"

 // Each note has three parts:
 // Duration, Pitch, Volume

 // Duration is how long the note plays for, in ms.
 // Pitch is the period of the note, in useconds.
 // Volume is from 0 to 0x3FF, off to fully on.


 // OFF is designed to sound unpleasant if silence is not implemented
 // in the user's code.

#define OFF		100

// taken and edited from lab 5

#define C4 		3831
#define CS4 	3610
#define D4 		3401
#define DS4 	3215
#define E4 		3030
#define F4 		2857
#define FS4 	2725
#define G4 		2551
#define GS4 	2410
#define A4 		2272
#define AS4 	2146
#define B4 		2024

#define C1 		C4*8
#define CS1 	CS4*8
#define D1 		D4*8
#define DS1 	DS4*8
#define E1 		E4*8
#define F1 		F4*8
#define FS1 	FS4*8
#define G1 		G4*8
#define GS1 	GS4*8
#define A1 		A4*8
#define AS1 	AS4*8
#define B1 		B4*8

#define C2 		C4*4
#define CS2 	CS4*4
#define D2 		D4*4
#define DS2 	DS4*4
#define E2 		E4*4
#define F2 		F4*4
#define FS2 	FS4*4
#define G2 		G4*4
#define GS2 	GS4*4
#define A2 		A4*4
#define AS2 	AS4*4
#define B2 		B4*4

#define C3 		C4*2
#define CS3 	CS4*2
#define D3 		D4*2
#define DS3 	DS4*2
#define E3 		E4*2
#define F3 		F4*2
#define FS3 	FS4*2
#define G3 		G4*2
#define GS3 	GS4*2
#define A3 		A4*2
#define AS3 	AS4*2
#define B3 		B4*2

#define C5 		C4/2
#define CS5 	CS4/2
#define D5 		D4/2
#define DS5 	DS4/2
#define E5 		E4/2
#define F5 		F4/2
#define FS5 	FS4/2
#define G5 		G4/2
#define GS5 	GS4/2
#define A5 		A4/2
#define AS5 	AS4/2
#define B5 		B4/2

#define C6 		C5/2
#define CS6 	CS5/2
#define D6 		D5/2
#define DS6 	DS5/2
#define E6 		E5/2
#define F6 		F5/2
#define FS6 	FS5/2
#define G6 		G5/2
#define GS6 	GS5/2
#define A6 		A5/2
#define AS6 	AS5/2
#define B6 		B5/2

//
// Song Title: Nyanyanyanyanyanyanya! (Popularly known as 'Nyan Cat')
// Artist: daniwell
//
// Painstakingly transcribed by Christopher Hines
//
// Trimmed to a self-contained ~15 sec clip -- see comment in songs.h
//
const struct tone nyan_cat_data[] = {
    {52, DS5, 0x2FD},
    {52, DS5, 0x1A3},
    {52, E5, 0x2FD},
    {52, E5, 0x1A3},
    {52, FS5, 0x2FD},
    {52, FS5, 0x1A3},
    {104, OFF, 0},
    {52, B5, 0x2FD},
    {52, B5, 0x1A3},
    {104, OFF, 0},
    {52, DS5, 0x2FD},
    {52, DS5, 0x1A3},
    {52, E5, 0x2FD},
    {52, E5, 0x1A3},
    {52, FS5, 0x2FD},
    {52, FS5, 0x1A3},
    {52, B5, 0x2FD},
    {52, B5, 0x1A3},
    {52, CS6, 0x2FD},
    {52, CS6, 0x1A3},
    {52, DS6, 0x2FD},
    {52, DS6, 0x1A3},
    {52, CS6, 0x2FD},
    {52, CS6, 0x1A3},
    {52, AS5, 0x2FD},
    {52, AS5, 0x1A3},
    {52, B5, 0x2FD},
    {52, B5, 0x1A3},
    {104, OFF, 0},
    {52, FS5, 0x2FD},
    {52, FS5, 0x1A3},
    {104, OFF, 0},
    {52, DS5, 0x2FD},
    {52, DS5, 0x1A3},
    {52, E5, 0x2FD},
    {52, E5, 0x1A3},
    {52, FS5, 0x2FD},
    {52, FS5, 0x1A3},
    {104, OFF, 0},
    {52, AS5, 0x2FD},
    {52, AS5, 0x1A3},
    {104, OFF, 0},
    {52, B5, 0x2FD},
    {52, B5, 0x1A3},
    {52, AS5, 0x2FD},
    {52, AS5, 0x1A3},
    {52, B5, 0x2FD},
    {52, B5, 0x1A3},
    {52, CS6, 0x2FD},
    {52, CS6, 0x1A3},
    {52, E6, 0x2FD},
    {52, E6, 0x1A3},
    {52, DS6, 0x2FD},
    {52, DS6, 0x1A3},
    {52, E6, 0x2FD},
    {52, E6, 0x1A3},
    {52, CS6, 0x2FD},
    {52, CS6, 0x1A3},
    {104, FS4, 0x2FD},
    {104, OFF, 0},
    {104, GS4, 0x2FD},
    {104, OFF, 0},
    {104, CS4, 0x2FD},
    {104, DS4, 0x2FD},
    {104, OFF, 0},
    {104, B3, 0x2FD},
    {104, D4, 0x2FD},
    {104, CS4, 0x2FD},
    {104, B3, 0x2FD},
    {104, OFF, 0},
    {104, B3, 0x2FD},
    {104, OFF, 0},
    {104, CS4, 0x2FD},
    {104, OFF, 0},
    {104, D4, 0x2FD},
    {104, OFF, 0},
    {52, D4, 0x2FD},
    {52, D4, 0x204},
    {52, CS4, 0x2FD},
    {52, CS4, 0x204},
    {52, B3, 0x2FD},
    {52, B3, 0x204},
    {52, CS4, 0x2FD},
    {52, CS4, 0x204},
    {52, DS4, 0x2FD},
    {52, DS4, 0x204},
    {52, FS4, 0x2FD},
    {52, FS4, 0x204},
    {52, GS4, 0x2FD},
    {52, GS4, 0x204},
    {52, DS4, 0x2FD},
    {52, DS4, 0x204},
    {52, FS4, 0x2FD},
    {52, FS4, 0x204},
    {52, CS4, 0x2FD},
    {52, CS4, 0x204},
    {52, D4, 0x2FD},
    {52, D4, 0x204},
    {52, B3, 0x2FD},
    {52, B3, 0x204},
    {52, CS4, 0x2FD},
    {52, CS4, 0x204},
    {52, B3, 0x2FD},
    {52, B3, 0x204},
    {52, DS4, 0x2FD},
    {52, DS4, 0x204},
    {104, OFF, 0},
    {104, FS4, 0x2FD},
    {104, OFF, 0},
    {104, GS4, 0x2FD},
    {104, DS4, 0x2FD},
    {104, FS4, 0x2FD},
    {104, CS4, 0x2FD},
    {104, DS4, 0x2FD},
    {104, B3, 0x2FD},
    {104, D4, 0x2FD},
    {104, DS4, 0x2FD},
    {104, D4, 0x2FD},
    {104, CS4, 0x2FD},
    {104, B3, 0x2FD},
    {104, CS4, 0x2FD},
    {104, D4, 0x2FD},
    {104, OFF, 0},
    {104, B3, 0x2FD},
    {104, CS4, 0x2FD},
    {104, DS4, 0x2FD},
    {104, FS4, 0x2FD},
    {104, CS4, 0x2FD},
    {104, D4, 0x2FD},
    {104, CS4, 0x2FD},
    {104, B3, 0x2FD},
    {104, CS4, 0x2FD},
    {104, OFF, 0},
    {104, B3, 0x2FD},
    {104, OFF, 0},
    {104, CS4, 0x2FD},
    {104, OFF, 0},
    {104, FS4, 0x2FD},
    {104, OFF, 0},
    {104, GS4, 0x2FD},
    {104, OFF, 0},
    {104, CS4, 0x2FD},
    {104, DS4, 0x2FD},
    {104, OFF, 0},
    {104, B3, 0x2FD},
    {104, D4, 0x2FD},
    {104, CS4, 0x2FD},
    {104, B3, 0x2FD},
    {104, OFF, 0},
    {104, B3, 0x2FD},
    {104, OFF, 0},
    {104, CS4, 0x2FD},
    {104, OFF, 0},
    {104, D4, 0x2FD},
    {104, OFF, 0},
    {104, D4, 0x2FD},
    {104, CS4, 0x2FD},
    {104, B3, 0x2FD},
    {104, CS4, 0x2FD},
    {104, DS4, 0x2FD},
    {104, FS4, 0x2FD},
    {104, GS4, 0x2FD},
    {104, DS4, 0x2FD},
    {104, FS4, 0x2FD},
    {104, CS4, 0x2FD},
    {104, D4, 0x2FD},
    {104, B3, 0x2FD},
    {104, CS4, 0x2FD},
    {104, B3, 0x2FD},
    {104, DS4, 0x2FD},
    {104, OFF, 0},
    {104, FS4, 0x2FD},
    {104, OFF, 0},
    {104, GS4, 0x2FD},
    {104, DS4, 0x2FD},
    {104, FS4, 0x2FD},
    {104, CS4, 0x2FD},
    {104, DS4, 0x2FD},
    {104, B3, 0x2FD},
    {104, D4, 0x2FD},
    {104, DS4, 0x2FD},
    {104, D4, 0x2FD},
    {104, CS4, 0x2FD},
    {104, B3, 0x2FD},
    {104, CS4, 0x2FD},
    {104, D4, 0x2FD},
    {104, OFF, 0},
};
const int nyan_cat_data_len = sizeof(nyan_cat_data) / sizeof(nyan_cat_data[0]);

// meglovania, transcribed into correct form from
// https://onlinesequencer.net/1760062
const struct tone megalovania_data[] = {
    {5, OFF, 0},
    {126, D4, 0xD9},
    {16, OFF, 0},
    {122, D4, 0xC9},
    {126, D5, 0xC9},
    {125, OFF, 0},
    {126, A4, 0xC9},
    {244, OFF, 0},
    {126, GS4, 0xC9},
    {133, OFF, 0},
    {126, G4, 0xC9},
    {117, OFF, 0},
    {252, F4, 0xC9},
    {1, OFF, 0},
    {122, D4, 0xC9},
    {122, F4, 0xC9},
    {126, G4, 0xC9},
    {4, OFF, 0},
    {126, C4, 0xD9},
    {17, OFF, 0},
    {122, C4, 0xC9},
    {126, D5, 0xC9},
    {125, OFF, 0},
    {126, A4, 0xC9},
    {244, OFF, 0},
    {126, GS4, 0xC9},
    {133, OFF, 0},
    {126, G4, 0xC9},
    {117, OFF, 0},
    {252, F4, 0xC9},
    {1, OFF, 0},
    {122, D4, 0xC9},
    {122, F4, 0xC9},
    {126, G4, 0xC9},
    {4, OFF, 0},
    {126, B3, 0xD9},
    {17, OFF, 0},
    {122, B3, 0xC9},
    {126, D5, 0xC9},
    {125, OFF, 0},
    {126, A4, 0xC9},
    {244, OFF, 0},
    {126, GS4, 0xC9},
    {133, OFF, 0},
    {126, G4, 0xC9},
    {117, OFF, 0},
    {252, F4, 0xC9},
    {1, OFF, 0},
    {122, D4, 0xC9},
    {122, F4, 0xC9},
    {126, G4, 0xC9},
    {4, OFF, 0},
    {126, AS3, 0xD9},
    {17, OFF, 0},
    {122, AS3, 0xC9},
    {126, D5, 0xC9},
    {125, OFF, 0},
    {126, A4, 0xC9},
    {244, OFF, 0},
    {126, GS4, 0xC9},
    {133, OFF, 0},
    {126, G4, 0xC9},
    {117, OFF, 0},
    {252, F4, 0xC9},
    {1, OFF, 0},
    {122, D4, 0xC9},
    {122, F4, 0xC9},
    {126, G4, 0xC9},
    {4, OFF, 0},
    {126, D4, 0xD9},
    {17, OFF, 0},
    {122, D4, 0xC9},
    {126, D5, 0xC9},
    {125, OFF, 0},
    {126, A4, 0xC9},
    {244, OFF, 0},
    {126, GS4, 0xC9},
    {133, OFF, 0},
    {126, G4, 0xC9},
    {117, OFF, 0},
    {252, F4, 0xC9},
    {1, OFF, 0},
    {122, D4, 0xC9},
    {122, F4, 0xC9},
    {126, G4, 0xC9},
    {4, OFF, 0},
    {126, C4, 0xD9},
    {17, OFF, 0},
    {122, C4, 0xC9},
    {126, D5, 0xC9},
    {125, OFF, 0},
    {126, A4, 0xC9},
    {244, OFF, 0},
    {126, GS4, 0xC9},
    {133, OFF, 0},
    {126, G4, 0xC9},
    {117, OFF, 0},
    {252, F4, 0xC9},
    {1, OFF, 0},
    {122, D4, 0xC9},
    {122, F4, 0xC9},
    {126, G4, 0xC9},
    {4, OFF, 0},
    {126, B3, 0xD9},
    {17, OFF, 0},
    {122, B3, 0xC9},
    {126, D5, 0xC9},
    {125, OFF, 0},
    {126, A4, 0xC9},
    {244, OFF, 0},
    {126, GS4, 0xC9},
    {133, OFF, 0},
    {126, G4, 0xC9},
    {117, OFF, 0},
    {252, F4, 0xC9},
    {1, OFF, 0},
    {122, D4, 0xC9},
    {122, F4, 0xC9},
    {126, G4, 0xC9},
    {4, OFF, 0},
    {126, AS3, 0xD9},
    {17, OFF, 0},
    {122, AS3, 0xC9},
    {126, D5, 0xC9},
    {125, OFF, 0},
    {126, A4, 0xC9},
    {244, OFF, 0},
};
const int megalovania_data_len = sizeof(megalovania_data) / sizeof(megalovania_data[0]);

// Bad Piggies Theme, angrybirds from
// https://onlinesequencer.net/1997225#
const struct tone bad_piggies_data[] = {
    {1548, D5, 0x142},
    {95, C3, 0xFA},
    {2, C2, 0x193},
    {97, OFF, 0},
    {9, D2, 0x193},
    {97, G3, 0xFA},
    {88, OFF, 0},
    {8, C2, 0x193},
    {97, G2, 0xEA},
    {89, OFF, 0},
    {5, D2, 0x193},
    {97, G3, 0xFA},
    {92, OFF, 0},
    {4, C2, 0x193},
    {97, C3, 0xF2},
    {93, OFF, 0},
    {3, D2, 0x193},
    {97, G3, 0xFA},
    {94, OFF, 0},
    {1, C2, 0x193},
    {97, G2, 0xEA},
    {94, OFF, 0},
    {97, G3, 0xFA},
    {2, D2, 0x193},
    {95, OFF, 0},
    {97, C3, 0xFA},
    {2, C2, 0x193},
    {97, OFF, 0},
    {9, D2, 0x193},
    {97, G3, 0xFA},
    {88, OFF, 0},
    {8, C2, 0x193},
    {97, G2, 0xEA},
    {89, OFF, 0},
    {5, D2, 0x193},
    {97, G3, 0xFA},
    {92, OFF, 0},
    {4, C2, 0x193},
    {97, C3, 0xF2},
    {93, OFF, 0},
    {3, D2, 0x193},
    {97, G3, 0xFA},
    {94, OFF, 0},
    {1, C2, 0x193},
    {97, G2, 0xEA},
    {94, OFF, 0},
    {97, G3, 0xFA},
    {2, D2, 0x193},
    {95, OFF, 0},
    {387, C6, 0xFA},
    {2, OFF, 0},
    {8, C2, 0x193},
    {290, C6, 0xEA},
    {91, AS5, 0xEA},
    {2, C2, 0x193},
    {97, C6, 0xD9},
    {93, OFF, 0},
    {3, D2, 0x193},
    {97, AS5, 0xEA},
    {94, OFF, 0},
    {1, C2, 0x193},
    {97, GS5, 0xEA},
    {94, OFF, 0},
    {97, G5, 0xEA},
    {2, D2, 0x193},
    {95, OFF, 0},
    {97, AS5, 0xEA},
    {2, C2, 0x193},
    {11, OFF, 0},
    {97, GS5, 0xEA},
    {94, G5, 0xEA},
    {95, F5, 0xEA},
    {1, C2, 0x193},
    {97, DS5, 0xD9},
    {1, AS2, 0xEA},
    {89, OFF, 0},
    {4, D2, 0x193},
    {97, F5, 0xEA},
    {1, AS3, 0xFA},
    {92, OFF, 0},
    {3, C2, 0x193},
    {768, G5, 0xF2},
    {387, AS5, 0xFA},
    {3, OFF, 0},
    {7, C2, 0x193},
    {290, AS5, 0xEA},
    {91, GS5, 0xEA},
    {2, C2, 0x193},
    {97, G5, 0xD9},
    {1, AS2, 0xF2},
    {93, OFF, 0},
    {2, D2, 0x193},
    {97, F5, 0xEA},
    {1, F3, 0xFA},
    {94, OFF, 0},
    {97, DS5, 0xEA},
    {1, F2, 0xEA},
    {93, OFF, 0},
    {97, F5, 0xEA},
    {1, F3, 0xFA},
    {2, D2, 0x193},
    {94, OFF, 0},
    {97, G5, 0xEA},
    {1, GS2, 0xFA},
    {2, C2, 0x193},
    {10, OFF, 0},
    {97, DS5, 0xEA},
    {92, C5, 0xEA},
    {97, DS5, 0xEA},
    {1, C2, 0x193},
    {97, G5, 0xD9},
    {2, DS2, 0xEA},
    {89, OFF, 0},
    {3, D2, 0x193},
    {97, F5, 0xEA},
    {2, DS3, 0xFA},
    {92, OFF, 0},
    {2, C2, 0x193},
    {387, DS5, 0xF2},
    {381, D5, 0xEA},
    {387, C6, 0xFA},
    {4, OFF, 0},
    {6, C2, 0x193},
    {290, C6, 0xEA},
    {91, AS5, 0xEA},
    {2, C2, 0x193},
    {97, C6, 0xD9},
    {2, C3, 0xF2},
    {93, OFF, 0},
    {1, D2, 0x193},
    {97, AS5, 0xEA},
    {2, G3, 0xFA},
    {93, OFF, 0},
    {97, GS5, 0xEA},
    {2, G2, 0xEA},
    {92, OFF, 0},
    {97, G5, 0xEA},
    {2, G3, 0xFA},
    {2, D2, 0x193},
    {93, OFF, 0},
    {97, AS5, 0xEA},
    {2, DS3, 0xFA},
    {2, C2, 0x193},
    {9, OFF, 0},
    {97, GS5, 0xEA},
    {94, G5, 0xEA},
    {95, F5, 0xEA},
    {1, C2, 0x193},
    {97, DS5, 0xD9},
    {3, AS2, 0xEA},
    {89, OFF, 0},
    {2, D2, 0x193},
    {97, F5, 0xEA},
    {3, AS3, 0xFA},
    {92, OFF, 0},
    {1, C2, 0x193},
    {768, G5, 0xF2},
    {387, AS5, 0xFA},
    {5, OFF, 0},
    {5, C2, 0x193},
    {290, AS5, 0xEA},
    {91, GS5, 0xEA},
    {1, OFF, 0},
    {1, C2, 0x193},
    {97, G5, 0xD9},
    {3, AS2, 0xF2},
    {93, OFF, 0},
};
const int bad_piggies_data_len = sizeof(bad_piggies_data) / sizeof(bad_piggies_data[0]);

// Tetris Theme
// https://onlinesequencer.net/1297029#
const struct tone tetris_theme_data[] = {
    {4, OFF, 0},
    {400, E4, 0x193},
    {8, E2, 0x193},
    {197, B3, 0x193},
    {199, C4, 0x193},
    {199, D4, 0x193},
    {100, E4, 0x193},
    {97, D4, 0x193},
    {1, E2, 0x193},
    {200, C4, 0x193},
    {197, B3, 0x193},
    {400, A3, 0x193},
    {10, A2, 0x193},
    {197, E3, 0x193},
    {3, A3, 0x193},
    {196, A2, 0x193},
    {400, E4, 0x193},
    {197, D4, 0x193},
    {197, C4, 0x193},
    {400, B3, 0x193},
    {11, E3, 0x193},
    {196, GS1, 0x193},
    {199, C4, 0x193},
    {397, D4, 0x193},
    {400, E4, 0x193},
    {397, A3, 0x193},
    {11, A2, 0x193},
    {196, A1, 0x193},
    {199, A2, 0x193},
    {4, A3, 0x193},
    {195, A1, 0x193},
    {200, A2, 0x193},
    {195, B1, 0x193},
    {200, C2, 0x193},
    {200, D2, 0x193},
    {11, OFF, 0},
    {396, D4, 0x193},
    {199, F4, 0x193},
    {400, A4, 0x193},
    {197, G4, 0x193},
    {197, F4, 0x193},
    {400, E4, 0x193},
    {11, C2, 0x193},
    {196, OFF, 0},
    {199, C4, 0x193},
    {400, E4, 0x193},
    {197, D4, 0x193},
    {197, C4, 0x193},
    {400, B3, 0x193},
    {10, E3, 0x193},
    {197, B3, 0x193},
    {199, C4, 0x193},
    {397, D4, 0x193},
    {400, E4, 0x193},
    {397, C4, 0x193},
    {10, A2, 0x193},
    {1, E3, 0x193},
    {196, A1, 0x193},
    {199, A2, 0x193},
    {4, A3, 0x193},
    {396, A1, 0x193},
    {396, OFF, 0},
    {400, E4, 0x193},
    {8, E2, 0x193},
    {197, B3, 0x193},
    {199, C4, 0x193},
    {199, D4, 0x193},
    {100, E4, 0x193},
    {97, D4, 0x193},
    {1, E2, 0x193},
    {200, C4, 0x193},
    {197, B3, 0x193},
    {400, A3, 0x193},
    {10, A2, 0x193},
    {197, E3, 0x193},
};
const int tetris_theme_data_len = sizeof(tetris_theme_data) / sizeof(tetris_theme_data[0]);

// super mario bros theme from
// https://www.nonstop2k.com/midi-files/14522-koji-kondo-mario-bros-main-theme-midi.html
const struct tone mario_theme_data[] = {
    {124, E5, 0x284},
    {1, OFF, 0},
    {249, E5, 0x284},
    {1, OFF, 0},
    {124, E5, 0x284},
    {126, OFF, 0},
    {124, C5, 0x284},
    {1, OFF, 0},
    {124, E5, 0x284},
    {126, OFF, 0},
    {249, G5, 0x284},
    {251, OFF, 0},
    {249, G4, 0x284},
    {251, OFF, 0},
    {249, C5, 0x284},
    {126, OFF, 0},
    {249, G4, 0x284},
    {126, OFF, 0},
    {249, E4, 0x284},
    {126, OFF, 0},
    {249, A4, 0x284},
    {1, OFF, 0},
    {249, B4, 0x284},
    {1, OFF, 0},
    {124, AS4, 0x284},
    {1, OFF, 0},
    {124, A4, 0x284},
    {126, OFF, 0},
    {166, G4, 0x284},
    {1, OFF, 0},
    {166, E5, 0x284},
    {1, OFF, 0},
    {166, G5, 0x284},
    {1, OFF, 0},
    {249, A5, 0x284},
    {1, OFF, 0},
    {124, F5, 0x284},
    {1, OFF, 0},
    {124, G5, 0x284},
    {126, OFF, 0},
    {249, E5, 0x284},
    {1, OFF, 0},
    {124, C5, 0x284},
    {1, OFF, 0},
    {124, D5, 0x284},
    {1, OFF, 0},
    {124, B4, 0x284},
    {251, OFF, 0},
    {249, C5, 0x284},
    {126, OFF, 0},
    {249, G4, 0x284},
    {126, OFF, 0},
    {249, E4, 0x284},
    {126, OFF, 0},
    {249, A4, 0x284},
    {1, OFF, 0},
    {249, B4, 0x284},
    {1, OFF, 0},
    {124, AS4, 0x284},
    {1, OFF, 0},
    {124, A4, 0x284},
    {126, OFF, 0},
    {166, G4, 0x284},
    {1, OFF, 0},
    {166, E5, 0x284},
    {1, OFF, 0},
    {166, G5, 0x284},
    {1, OFF, 0},
    {249, A5, 0x284},
    {1, OFF, 0},
    {124, F5, 0x284},
    {1, OFF, 0},
    {124, G5, 0x284},
    {126, OFF, 0},
    {249, E5, 0x284},
    {1, OFF, 0},
    {124, C5, 0x284},
    {1, OFF, 0},
    {124, D5, 0x284},
    {1, OFF, 0},
    {124, B4, 0x284},
    {251, OFF, 0},
    {124, C4, 0x284},
    {126, OFF, 0},
    {124, G5, 0x284},
    {1, OFF, 0},
    {124, FS5, 0x284},
    {1, OFF, 0},
    {124, F5, 0x284},
    {1, OFF, 0},
    {124, DS5, 0x284},
    {126, OFF, 0},
    {124, E5, 0x284},
    {126, OFF, 0},
    {124, GS4, 0x284},
    {1, OFF, 0},
    {124, A4, 0x284},
    {1, OFF, 0},
    {124, C5, 0x284},
    {126, OFF, 0},
    {124, A4, 0x284},
    {1, OFF, 0},
    {124, C5, 0x284},
    {1, OFF, 0},
    {124, D5, 0x284},
    {1, OFF, 0},
    {124, C4, 0x284},
    {126, OFF, 0},
    {124, G5, 0x284},
    {1, OFF, 0},
    {124, FS5, 0x284},
    {1, OFF, 0},
    {124, F5, 0x284},
    {1, OFF, 0},
    {124, DS5, 0x284},
    {126, OFF, 0},
    {124, E5, 0x284},
    {126, OFF, 0},
    {124, C6, 0x284},
    {126, OFF, 0},
    {124, C6, 0x284},
    {1, OFF, 0},
    {124, C6, 0x284},
    {126, OFF, 0},
    {124, G4, 0x284},
    {126, OFF, 0},
    {124, C4, 0x284},
    {126, OFF, 0},
    {124, G5, 0x284},
    {1, OFF, 0},
    {124, FS5, 0x284},
    {1, OFF, 0},
    {124, F5, 0x284},
    {1, OFF, 0},
    {124, DS5, 0x284},
    {126, OFF, 0},
    {124, E5, 0x284},
    {126, OFF, 0},
};
const int mario_theme_data_len = sizeof(mario_theme_data) / sizeof(mario_theme_data[0]);

const char * const song_names[NUM_SONGS] = {
    "Nyan Cat",
    "Megalovania",
    "Bad Piggies",
    "Tetris Theme",
    "Super Mario Bros"
};

const struct tone * const song_arrays[NUM_SONGS] = {
    nyan_cat_data,
    megalovania_data,
    bad_piggies_data,
    tetris_theme_data,
    mario_theme_data
};

const int song_lengths[NUM_SONGS] = {
    nyan_cat_data_len,
    megalovania_data_len,
    bad_piggies_data_len,
    tetris_theme_data_len,
    mario_theme_data_len
};



 // 8mhz clock, so prescaled to 1mhz

static volatile int song_abort = 0;

void songs_init(void) {
    // enable dac clock, pa4 is the dac pin. tim2's clock is already
    // enabled by delay_init() in lights.c, so we don't need to touch
    // RCC for it here
    RCC->APB1ENR |= RCC_APB1ENR_DACEN;
    RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;

    // PA4 as analog so mode and cnf are 00
    GPIOA->CRL &= ~(GPIO_CRL_MODE4 | GPIO_CRL_CNF4);

    // Enable DAC channel 1, software trigger, output buffer on
    DAC->CR = DAC_CR_EN1;

    // lights alr has timer, so have to change it from 1m to 1u
}

// song delay
static void songs_udelay(unsigned int delay_in_us) {
    unsigned int chunk;

    if (delay_in_us == 0) return;

    while (delay_in_us > 0) {
        if (delay_in_us > 30000) {
            chunk = 30000;
        } else {
            chunk = delay_in_us;
        } 

        TIM2->CNT = 0;
        TIM2->CR1 |= TIM_CR1_CEN;
        while ((TIM2->CNT < chunk) && (TIM2->CNT <= 30000));
        TIM2->CR1 &= ~TIM_CR1_CEN;

        delay_in_us -= chunk;
    }
}

// attempt to quit the song, might not work bc of the cpu stressed out
static int songs_cancel_requested(void) {
    unsigned short x_num, y_num;

    if (!touch_read_xy(&x_num, &y_num)) {
        return 0;
    }

    x_num = map_pixel(x_num, 10, 234, 0, 239);
    y_num = map_pixel(y_num, 10, 242, 0, 319);

    return touch_rect(0, 0, 30, 30, x_num, y_num);
}

static void play_tone(unsigned int duration_us, int period_us, int vol) {
    int half_period;
    int cycles, i;

    // Rests
    if (vol == 0 || period_us == OFF || period_us <= 0) {
        DAC->DHR12R1 = 0;
        songs_udelay(duration_us);
        return;
    }

    half_period = period_us / 2;
    if (half_period < 1) half_period = 1;

    cycles = duration_us / period_us;

    for (i = 0; i < cycles; i++) {
        // checks to quit the song every now and then not every cycle
        if ((i & 0xF) == 0 && songs_cancel_requested()) {
            song_abort = 1;
            break;
        }

        DAC->DHR12R1 = vol;
        songs_udelay(half_period);

        DAC->DHR12R1 = 0;
        songs_udelay(half_period);
    }
}

void play_song(int index) {
    int i;
    const struct tone *notes;
    int len;

    if (index < 0 || index >= NUM_SONGS) return;

    notes = song_arrays[index];
    len = song_lengths[index];
    song_abort = 0;

    // change timer to 1u
    TIM2->CR1 &= ~TIM_CR1_CEN;
    TIM2->PSC = 8 - 1;
    TIM2->CNT = 0;
    TIM2->EGR = TIM_EGR_UG;
    TIM2->SR &= ~TIM_SR_UIF;

    for (i = 0; i < len; i++) {
        play_tone((unsigned int)notes[i].duration * 1000, notes[i].pitch, notes[i].volume);
        if (song_abort) break;
    }

    // off at the end
    DAC->DHR12R1 = 0;

    // give TIM2 back to delay_init()'s 1ms free-running tick
    TIM2->CR1 &= ~TIM_CR1_CEN;
    TIM2->PSC = 8000 - 1;
    TIM2->ARR = 0xFFFF;
    TIM2->CNT = 0;
    TIM2->EGR = TIM_EGR_UG;
    TIM2->SR &= ~TIM_SR_UIF;
    TIM2->CR1 |= TIM_CR1_CEN;
}
