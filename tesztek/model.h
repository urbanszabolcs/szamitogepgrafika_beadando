#ifndef MODEL_H
#define MODEL_h

#include <GL/gl.h>

typedef struct {
    float x, y, z;
}Vertex;

void loadModel(const char* path);
void renderModel();
#endif