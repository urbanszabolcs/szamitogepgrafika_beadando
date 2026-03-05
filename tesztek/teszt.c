#define SDL_MAIN_HANDLED
#include <SDL2/SDL.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <stdio.h>
#include <math.h>

#define MAX_VERTICES 500000 
#define MAX_FACES 1000000   

typedef struct { float x, y, z; } Vertex;
typedef struct { int v1, v2, v3; } Face;

Vertex vertices[MAX_VERTICES];
Face faces[MAX_VERTICES];
int vertex_count = 0;
int face_count = 0;

// Kamera és egér állapot
float camAngleX = 20.0f; 
float camAngleY = 0.0f;  
float camDist = 15.0f;   
int isMousePressed = 0;  

// OBJEKTUM pozíciója és forgatása
float objX = 0.0f;
float objY = 0.0f;
float objZ = 0.0f;
float objAngleY = 0.0f;
float moveSpeed = 0.15f; // Finomabb folyamatos mozgáshoz picit kisebb érték
float rotSpeed = 3.0f;

// GOMBOK ÁLLAPOTA (0 = felengedve, 1 = lenyomva)
int keyW = 0, keyS = 0, keyA = 0, keyD = 0;

void loadObj(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (!file) { printf("Nem sikerult megnyitni a fajlt: %s\n", filename); return; }
    char line[128];
    vertex_count = 0; face_count = 0;
    while (fgets(line, sizeof(line), file)) {
        if (line[0] == 'v' && line[1] == ' ') {
            sscanf(line, "v %f %f %f", &vertices[vertex_count].x, &vertices[vertex_count].y, &vertices[vertex_count].z);
            vertex_count++;
        } else if (line[0] == 'f' && line[1] == ' ') {
            int v1, v2, v3;
            if (sscanf(line, "f %d/%*d/%*d %d/%*d/%*d %d/%*d/%*d", &v1, &v2, &v3) < 3)
                if (sscanf(line, "f %d/%*d %d/%*d %d/%*d", &v1, &v2, &v3) < 3)
                    sscanf(line, "f %d %d %d", &v1, &v2, &v3);
            faces[face_count].v1 = v1 - 1; faces[face_count].v2 = v2 - 1; faces[face_count].v3 = v3 - 1;
            face_count++;
        }
    }
    fclose(file);
    printf("Modell betoltve: %d csucspont, %d lap\n", vertex_count, face_count);
}

void drawModel() {
    glBegin(GL_TRIANGLES);
    for (int i = 0; i < face_count; i++) {
        glColor3f(0.7f, 0.7f, 0.7f);
        glVertex3f(vertices[faces[i].v1].x, vertices[faces[i].v1].y, vertices[faces[i].v1].z);
        glVertex3f(vertices[faces[i].v2].x, vertices[faces[i].v2].y, vertices[faces[i].v2].z);
        glVertex3f(vertices[faces[i].v3].x, vertices[faces[i].v3].y, vertices[faces[i].v3].z);
    }
    glEnd();
    
    glColor3f(0.0f, 0.0f, 0.0f);
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glBegin(GL_TRIANGLES);
    for (int i = 0; i < face_count; i++) {
        glVertex3f(vertices[faces[i].v1].x, vertices[faces[i].v1].y, vertices[faces[i].v1].z);
        glVertex3f(vertices[faces[i].v2].x, vertices[faces[i].v2].y, vertices[faces[i].v2].z);
        glVertex3f(vertices[faces[i].v3].x, vertices[faces[i].v3].y, vertices[faces[i].v3].z);
    }
    glEnd();
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

int main(int argc, char *argv[]) {
    SDL_Init(SDL_INIT_VIDEO);
    SDL_Window *window = SDL_CreateWindow("Hajo Szimulator - Folyamatos Mozgas", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);
    SDL_GLContext context = SDL_GL_CreateContext(window);
    glEnable(GL_DEPTH_TEST);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, 800.0 / 600.0, 0.1, 500.0);
    glMatrixMode(GL_MODELVIEW);

    loadObj("boat-row-large.obj");

    int running = 1;
    SDL_Event ev;
    while (running) {
        // 1. ESEMÉNYKEZELÉS (Csak az állapotot állítjuk)
        while (SDL_PollEvent(&ev)) {
            if (ev.type == SDL_QUIT) running = 0;

            if (ev.type == SDL_MOUSEBUTTONDOWN && ev.button.button == SDL_BUTTON_LEFT) isMousePressed = 1;
            if (ev.type == SDL_MOUSEBUTTONUP && ev.button.button == SDL_BUTTON_LEFT) isMousePressed = 0;
            
            if (ev.type == SDL_MOUSEMOTION && isMousePressed) {
                camAngleY += ev.motion.xrel * 0.5f;
                camAngleX += ev.motion.yrel * 0.5f;
                if (camAngleX > 85.0f) camAngleX = 85.0f;
                if (camAngleX < 5.0f) camAngleX = 5.0f;
            }
            if (ev.type == SDL_MOUSEWHEEL) {
                camDist -= ev.wheel.y;
                if (camDist < 2.0f) camDist = 2.0f;
            }

            // GOMB LENYOMÁSA
            if (ev.type == SDL_KEYDOWN) {
                switch (ev.key.keysym.sym) {
                    case SDLK_ESCAPE: running = 0; break;
                    case SDLK_w: keyW = 1; break;
                    case SDLK_s: keyS = 1; break;
                    case SDLK_a: keyA = 1; break;
                    case SDLK_d: keyD = 1; break;
                }
            }
            // GOMB FELENGEDÉSE
            if (ev.type == SDL_KEYUP) {
                switch (ev.key.keysym.sym) {
                    case SDLK_w: keyW = 0; break;
                    case SDLK_s: keyS = 0; break;
                    case SDLK_a: keyA = 0; break;
                    case SDLK_d: keyD = 0; break;
                }
            }
        }

        // 2. FOLYAMATOS MOZGÁS ÉS LOGIKA FRISSÍTÉSE
        if (keyA) objAngleY += rotSpeed;
        if (keyD) objAngleY -= rotSpeed;
        
        if (keyW) {
            objX += sinf(objAngleY * M_PI / 180.0f) * moveSpeed;
            objZ += cosf(objAngleY * M_PI / 180.0f) * moveSpeed;
        }
        if (keyS) {
            objX -= sinf(objAngleY * M_PI / 180.0f) * moveSpeed;
            objZ -= cosf(objAngleY * M_PI / 180.0f) * moveSpeed;
        }

        // 3. RENDERELÉS
        glClearColor(0.5f, 0.7f, 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glLoadIdentity();

        float camRadX = camAngleX * M_PI / 180.0f;
        float camRadY = camAngleY * M_PI / 180.0f;

        float eyeX = objX + camDist * cosf(camRadX) * sinf(camRadY);
        float eyeY = objY + camDist * sinf(camRadX);
        float eyeZ = objZ + camDist * cosf(camRadX) * cosf(camRadY);

        gluLookAt(eyeX, eyeY, eyeZ, objX, objY, objZ, 0, 1, 0);

        glPushMatrix();
            glTranslatef(objX, objY, objZ);
            glRotatef(objAngleY, 0, 1, 0);
            drawModel();
        glPopMatrix();

        // Rács a tájékozódáshoz
        glBegin(GL_LINES);
        glColor3f(0.3f, 0.3f, 0.3f);
        for(int i=-100; i<=100; i+=10) {
            glVertex3f(i, -1, -100); glVertex3f(i, -1, 100);
            glVertex3f(-100, -1, i); glVertex3f(100, -1, i);
        }
        glEnd();

        SDL_GL_SwapWindow(window);
        SDL_Delay(16); // ~60 FPS
    }

    SDL_GL_DeleteContext(context);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}