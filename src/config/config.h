#ifndef CONFIG_H
#define CONFIG_H

void Config_Init(void);
void Config_Toggle(void);
int Config_IsOpen(void);                        // True or False value
void Config_HandleInput(SDL_Event* ev);
void Config_Render(void);

#endif