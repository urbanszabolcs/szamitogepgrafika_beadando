#include "terrain.h"
#include <math.h>

# define WATER_SIZE 50
# define WATER_STEP 2.0f
# define WATER_WERTEX_COUNT ((WATER_SIZE*2+1) * (WATER_SIZE*2+1))
# define WATER_INDEX_COUNT (WATER_SIZE * 2 * WATER_SIZE*2 * 6)

static GLuint waterVBO = 0;
static GLuint waterIBO = 0;
