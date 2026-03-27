#ifndef GUIDE_H
#define GUIDE_H

#include <stdbool.h>

void Guide_Init(void);

void Guide_Toggle(void);

bool Guide_IsVisible(void);

void Guide_Render(void);

void Guide_Cleanup(void);

#endif 