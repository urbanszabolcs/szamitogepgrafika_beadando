#define SDL_MAIN_HANDLED
#include <SDL2/SDL.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <stdio.h>
#include <math.h>
#include "island.h" // Beemeljük a szigetet

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

#define MAX_VERTICES 500000 
#define MAX_FACES 1000000   

typedef struct { float x, y, z; } Vertex;
typedef struct { int v1, v2, v3; } Face;

Vertex vertices[MAX_VERTICES];
Face faces[MAX_VERTICES];
int vertex_count = 0, face_count = 0;

// Mozgás és Kamera változók
float objX = 0, objY = 0, objZ = 0, objAngleY = 0;
float camAngleX = 20, camAngleY = 0, camDist = 10;
int keyW = 0, keyS = 0, keyA = 0, keyD = 0, isMousePressed = 0;

// Sziget inicializálása
Island myIsland = {20.0f, -20.0f, 10.0f, 5.0f, 30};

void loadObj(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (!file) { printf("Hiba: %s nem talalhato!\n", filename); return; }
    char line[128];
    while (fgets(line, sizeof(line), file)) {
        if (line[0] == 'v' && line[1] == ' ') {
            sscanf(line, "v %f %f %f", &vertices[vertex_count].x, &vertices[vertex_count].y, &vertices[vertex_count].z);
            vertex_count++;
        } else if (line[0] == 'f' && line[1] == ' ') {
            int v1, v2, v3;
            sscanf(line, "f %d/%*d/%*d %d/%*d/%*d %d/%*d/%*d", &v1, &v2, &v3);
            faces[face_count].v1 = v1 - 1; faces[face_count].v2 = v2 - 1; faces[face_count].v3 = v3 - 1;
            face_count++;
        }
    }
    fclose(file);
}

void drawModel() {
    glBegin(GL_TRIANGLES);
    for (int i = 0; i < face_count; i++) {
        glColor3f(0.6f, 0.4f, 0.2f); // Barna hajó
        glVertex3f(vertices[faces[i].v1].x, vertices[faces[i].v1].y, vertices[faces[i].v1].z);
        glVertex3f(vertices[faces[i].v2].x, vertices[faces[i].v2].y, vertices[faces[i].v2].z);
        glVertex3f(vertices[faces[i].v3].x, vertices[faces[i].v3].y, vertices[faces[i].v3].z);
    }
    glEnd();
}

int main(int argc, char *argv[]) {
    SDL_Init(SDL_INIT_VIDEO);
    SDL_Window *win = SDL_CreateWindow("Hajo Szimulacio", 100, 100, 800, 600, SDL_WINDOW_OPENGL);
    SDL_GL_CreateContext(win);
    glEnable(GL_DEPTH_TEST);

    glMatrixMode(GL_PROJECTION);
    gluPerspective(45.0, 800.0/600.0, 0.1, 500.0);
    glMatrixMode(GL_MODELVIEW);

    // Kijavított elérési út az image_304e9f.png alapján
    loadObj("assets/boat-row-large.obj");

    int running = 1;
    SDL_Event ev;
    while (running) {
        while (SDL_PollEvent(&ev)) {
            if (ev.type == SDL_QUIT) running = 0;
            if (ev.type == SDL_MOUSEBUTTONDOWN) isMousePressed = 1;
            if (ev.type == SDL_MOUSEBUTTONUP) isMousePressed = 0;
            if (ev.type == SDL_MOUSEMOTION && isMousePressed) {
                camAngleY += ev.motion.xrel * 0.5f;
                camAngleX += ev.motion.yrel * 0.5f;
            }
            if (ev.type == SDL_KEYDOWN) {
                if (ev.key.keysym.sym == SDLK_w) keyW = 1;
                if (ev.key.keysym.sym == SDLK_s) keyS = 1;
                if (ev.key.keysym.sym == SDLK_a) keyA = 1;
                if (ev.key.keysym.sym == SDLK_d) keyD = 1;
            }
            if (ev.type == SDL_KEYUP) {
                if (ev.key.keysym.sym == SDLK_w) keyW = 0;
                if (ev.key.keysym.sym == SDLK_s) keyS = 0;
                if (ev.key.keysym.sym == SDLK_a) keyA = 0;
                if (ev.key.keysym.sym == SDLK_d) keyD = 0;
            }
        }

        // Mozgás számítása
        if (keyW) { objX += sinf(objAngleY * M_PI / 180.0f) * 0.1f; objZ += cosf(objAngleY * M_PI / 180.0f) * 0.1f; }
        if (keyS) { objX -= sinf(objAngleY * M_PI / 180.0f) * 0.1f; objZ -= cosf(objAngleY * M_PI / 180.0f) * 0.1f; }
        if (keyA) objAngleY += 2.0f;
        if (keyD) objAngleY -= 2.0f;

        glClearColor(0.5f, 0.8f, 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glLoadIdentity();

        // Kamera beállítása
        float camRadX = camAngleX * M_PI / 180.0f;
        float camRadY = camAngleY * M_PI / 180.0f;
        float ex = objX + camDist * cosf(camRadX) * sinf(camRadY);
        float ey = objY + camDist * sinf(camRadX);
        float ez = objZ + camDist * cosf(camRadX) * cosf(camRadY);
        gluLookAt(ex, ey, ez, objX, objY, objZ, 0, 1, 0);

        // HAJÓ (Push/Pop védi)
        glPushMatrix();
            glTranslatef(objX, objY, objZ);
            glRotatef(objAngleY, 0, 1, 0);
            drawModel();
        glPopMatrix();

        // SZIGET (Push/Pop védi)
        glPushMatrix();
            drawIsland(&myIsland);
        glPopMatrix();

        SDL_GL_SwapWindow(win);
        SDL_Delay(16);
    }
    SDL_Quit();
    return 0;
}