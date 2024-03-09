#ifndef ICONS_H
#define ICONS_H

extern const byte heartbeat[][288] PROGMEM;
extern const byte wifi[][128] PROGMEM;
extern const byte tick[][128] PROGMEM;
extern const byte cloud[][128] PROGMEM;
extern const byte cross[][128] PROGMEM;

void play_animaton(int loops,const byte PROGMEM frames[][288], int speed, int size,int x, int y);

void play_animaton(int loops,const byte PROGMEM frames[][128], int speed, int size,int x, int y);

#endif