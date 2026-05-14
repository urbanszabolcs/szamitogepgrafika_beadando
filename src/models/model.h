#ifndef MODEL_H
#define MODEL_H

#include <GL/gl.h>

// Represents a 3D point
typedef struct {
    float x;
    float y;
    float z;
} Vertex;

// Represents a 2D texture coordinate (UV)
typedef struct {
    float u;
    float v;
} TexCoord;

// Load the 3D geometry
void loadModel(const char* path);

// Load the PNG texture and convert it to OpenGL
GLuint loadModelTexture(const char* path);

// Render the model with a specific texture and scale
void renderModel(GLuint textureID, float scale);

#endif // MODEL_H