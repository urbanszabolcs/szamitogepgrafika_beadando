#ifndef ISLAND_H
#define ISLAND_H

#include <GL/gl.h>

// Sziget adatszerkezete
typedef struct {
    float x, z;        // Pozíció a világban
    float size;        // Alapterület
    float height;      // Maximális magasság
    int detail;        // Felbontás (rácspontok száma)
} Island;

void drawIsland(Island* island);

#endif