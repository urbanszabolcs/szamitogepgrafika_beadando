#define SDL_MAIN_HANDLED
#include <SDL2/SDL.h>
#include <GL/gl.h>
#include <stdio.h>

int main(int argc, char* argv[]) {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("SDL hiba: %s\n", SDL_GetError());
        return 1;
    }

    SDL_Window* window = SDL_CreateWindow("Mozgo Haromszog Teszt", 
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);

    if (!window) {
        SDL_Quit();
        return 1;
    }

    SDL_GLContext context = SDL_GL_CreateContext(window);

    // Változók a mozgáshoz és a mérethez
    float triX = 0.0f;
    float triY = 0.0f;
    float size = 0.2f;  // Kisebb méret (eredetileg 0.5 volt)
    float speed = 0.09f; // Mozgási sebesség

    int running = 1;
    SDL_Event ev;
    while (running) {
        while (SDL_PollEvent(&ev)) {
            if (ev.type == SDL_QUIT) running = 0;

            // Billentyűzet kezelése
            if (ev.type == SDL_KEYDOWN) {
                switch (ev.key.keysym.sym) {
                    case SDLK_w:    triY += speed; break;
                    case SDLK_s:  triY -= speed; break;
                    case SDLK_a:  triX -= speed; break;
                    case SDLK_d: triX += speed; break;
                    case SDLK_ESCAPE: running = 0;  break;
                }
            }
        }

        glClearColor(0.1f, 0.2f, 0.4f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // Rajzolás előtt "eltoljuk" a koordináta-rendszert
        glLoadIdentity(); 
        glTranslatef(triX, triY, 0.0f); // Ez mozgatja el a tárgyat

        glBegin(GL_TRIANGLES);
            glColor3f(1.0f, 0.5f, 0.0f); // Narancssárga 1.0f, 0.5f, 0.0f
            glVertex2f(-size, -size);
            glVertex2f( size, -size);
            glVertex2f( 0.0f,  size);
        glEnd();

        SDL_GL_SwapWindow(window);
        
        // Egy kis késleltetés, hogy ne pörögjön túl a processzor
        SDL_Delay(60); // Kb. 60 FPS
    }

    SDL_GL_DeleteContext(context);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}