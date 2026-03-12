#include "model.h"
#include <stdio.h>

#define MAX_V 500000
Vertex vertices[MAX_V];
int vertex_indices[MAX_V * 3];
int index_count = 0;

void loadModel(const char* path) {
    FILE* f = fopen(path, "r");
    if (!f) return;
    char line[128];
    int v_idx = 0;
    index_count = 0;

    while (fgets(line, sizeof(line), f)) {
        if (line[0] == 'v' && line[1] == ' ') {
            sscanf(line, "v %f %f %f", &vertices[v_idx].x, &vertices[v_idx].y, &vertices[v_idx].z);
            v_idx++;
        } else if (line[0] == 'f') {
            int v1, v2, v3;
            sscanf(line, "f %d/%*d/%*d %d/%*d/%*d %d/%*d/%*d", &v1, &v2, &v3);
            vertex_indices[index_count++] = v1 - 1;
            vertex_indices[index_count++] = v2 - 1;
            vertex_indices[index_count++] = v3 - 1;
        }
    }
    fclose(f);
}

void renderModel() {
    glBegin(GL_TRIANGLES);
    for (int i = 0; i < index_count; i++) {
        Vertex v = vertices[vertex_indices[i]];
        glVertex3f(v.x, v.y, v.z);
    }
    glEnd();
}