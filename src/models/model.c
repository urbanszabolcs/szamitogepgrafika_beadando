#include "model.h"
#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h> // Required to load .png files

#define MAX_V 500000

Vertex vertices[MAX_V];
TexCoord texCoords[MAX_V];

int vertex_indices[MAX_V * 3];
int tex_indices[MAX_V * 3];

int index_count = 0;
int has_texcoords = 0; // Flag to check if the model actually has UV mapping

void loadModel(const char *path)
{
    FILE *f = fopen(path, "r");
    if (!f)
    {
        printf("Failed to open model: %s\n", path);
        return;
    }

    char line[256];
    int v_idx = 0;
    int t_idx = 0;
    index_count = 0;
    has_texcoords = 0;

    while (fgets(line, sizeof(line), f))
    {
        // Parse Vertices
        if (line[0] == 'v' && line[1] == ' ')
        {
            sscanf(line, "v %f %f %f", &vertices[v_idx].x, &vertices[v_idx].y, &vertices[v_idx].z);
            v_idx++;
        }
        // Parse Texture Coordinates (UVs)
        else if (line[0] == 'v' && line[1] == 't' && line[2] == ' ')
        {
            sscanf(line, "vt %f %f", &texCoords[t_idx].u, &texCoords[t_idx].v);
            t_idx++;
            has_texcoords = 1; // Model has texture data
        }
        // Parse Faces
        else if (line[0] == 'f' && line[1] == ' ')
        {
            int v[4], t[4], n[4];

            // 1. Try Quad format: v/vt/vn (4 points, with normals)
            if (sscanf(line, "f %d/%d/%d %d/%d/%d %d/%d/%d %d/%d/%d",
                       &v[0], &t[0], &n[0], &v[1], &t[1], &n[1], &v[2], &t[2], &n[2], &v[3], &t[3], &n[3]) == 12)
            {
                // Slice into Triangle 1 (Points 0, 1, 2)
                vertex_indices[index_count] = v[0] - 1;
                tex_indices[index_count++] = t[0] - 1;
                vertex_indices[index_count] = v[1] - 1;
                tex_indices[index_count++] = t[1] - 1;
                vertex_indices[index_count] = v[2] - 1;
                tex_indices[index_count++] = t[2] - 1;
                // Slice into Triangle 2 (Points 0, 2, 3)
                vertex_indices[index_count] = v[0] - 1;
                tex_indices[index_count++] = t[0] - 1;
                vertex_indices[index_count] = v[2] - 1;
                tex_indices[index_count++] = t[2] - 1;
                vertex_indices[index_count] = v[3] - 1;
                tex_indices[index_count++] = t[3] - 1;
            }
            // 2. Try Triangle format: v/vt/vn (3 points, with normals)
            else if (sscanf(line, "f %d/%d/%d %d/%d/%d %d/%d/%d",
                            &v[0], &t[0], &n[0], &v[1], &t[1], &n[1], &v[2], &t[2], &n[2]) == 9)
            {
                vertex_indices[index_count] = v[0] - 1;
                tex_indices[index_count++] = t[0] - 1;
                vertex_indices[index_count] = v[1] - 1;
                tex_indices[index_count++] = t[1] - 1;
                vertex_indices[index_count] = v[2] - 1;
                tex_indices[index_count++] = t[2] - 1;
            }
            // 3. Try Quad format: v/vt (4 points, no normals)
            else if (sscanf(line, "f %d/%d %d/%d %d/%d %d/%d",
                            &v[0], &t[0], &v[1], &t[1], &v[2], &t[2], &v[3], &t[3]) == 8)
            {
                // Slice into Triangle 1
                vertex_indices[index_count] = v[0] - 1;
                tex_indices[index_count++] = t[0] - 1;
                vertex_indices[index_count] = v[1] - 1;
                tex_indices[index_count++] = t[1] - 1;
                vertex_indices[index_count] = v[2] - 1;
                tex_indices[index_count++] = t[2] - 1;
                // Slice into Triangle 2
                vertex_indices[index_count] = v[0] - 1;
                tex_indices[index_count++] = t[0] - 1;
                vertex_indices[index_count] = v[2] - 1;
                tex_indices[index_count++] = t[2] - 1;
                vertex_indices[index_count] = v[3] - 1;
                tex_indices[index_count++] = t[3] - 1;
            }
            // 4. Try Triangle format: v/vt (3 points, no normals)
            else if (sscanf(line, "f %d/%d %d/%d %d/%d",
                            &v[0], &t[0], &v[1], &t[1], &v[2], &t[2]) == 6)
            {
                vertex_indices[index_count] = v[0] - 1;
                tex_indices[index_count++] = t[0] - 1;
                vertex_indices[index_count] = v[1] - 1;
                tex_indices[index_count++] = t[1] - 1;
                vertex_indices[index_count] = v[2] - 1;
                tex_indices[index_count++] = t[2] - 1;
            }
        }
    }
    fclose(f);
}

GLuint loadModelTexture(const char *path)
{
    // IMG_Load handles PNGs, JPGs, etc.
    SDL_Surface *originalSurface = IMG_Load(path);
    if (!originalSurface)
    {
        printf("Failed to load texture %s: %s\n", path, IMG_GetError());
        return 0;
    }

    // Convert to standard RGBA format to prevent OpenGL pitch/color issues
    SDL_Surface *surface = SDL_ConvertSurfaceFormat(originalSurface, SDL_PIXELFORMAT_ABGR8888, 0);
    SDL_FreeSurface(originalSurface);

    if (!surface)
        return 0;

    GLuint textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, surface->w, surface->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, surface->pixels);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    SDL_FreeSurface(surface);
    return textureID;
}

void renderModel(GLuint textureID, float scale)
{
    glPushMatrix();

    // Apply the scaling factor
    glScalef(scale, scale, scale);

    // Enable texturing if the model supports it and a texture was passed
    if (has_texcoords && textureID != 0)
    {
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, textureID);
        glColor4f(1.0f, 1.0f, 1.0f, 1.0f); // Reset color to pure white so the texture isn't tinted
    }
    else
    {
        glDisable(GL_TEXTURE_2D);
    }

    glBegin(GL_TRIANGLES);
    for (int i = 0; i < index_count; i++)
    {
        if (has_texcoords && textureID != 0)
        {
            TexCoord t = texCoords[tex_indices[i]];
            // 1.0f - t.v is often required because OpenGL reads textures bottom-up,
            // while OBJ files often export them top-down.
            glTexCoord2f(t.u, 1.0f - t.v);
        }
        Vertex v = vertices[vertex_indices[i]];
        glVertex3f(v.x, v.y, v.z);
    }
    glEnd();

    // Clean up state
    if (has_texcoords && textureID != 0)
    {
        glDisable(GL_TEXTURE_2D);
    }

    glPopMatrix();
}