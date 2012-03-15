#ifndef KMAIN_H__
#define KMAIN_H__

#include <stdint.h>
#include <attributes.h>
#include <devices/videoram.h>

#define BG_NORMAL (VR_GREEN|VR_BRIGHT)
#define BG_INFO (VR_BLACK|VR_BRIGHT)
#define BG_ERROR BG_INFO
#define FG_NORMAL BG_INFO
#define FG_INFO BG_NORMAL
#define FG_ERROR (VR_RED|VR_BRIGHT)

#define COLOR_NORMAL (VR_COLOR (BG_NORMAL, FG_NORMAL))
#define COLOR_INFO (VR_COLOR (BG_INFO, FG_INFO))
#define COLOR_ERROR (VR_COLOR (BG_ERROR, FG_ERROR))

void _start (void) NO_RETURN;

void khalt (void) NO_RETURN;

#endif
