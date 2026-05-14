CC = gcc
CFLAGS = -Wall -O2
LIBS = -lmingw32 -lSDL2main -lSDL2 -lSDL2_ttf -lSDL2_image -lopengl32 -lglu32 -mconsole
TARGET = game.exe
OBJ_DIR = obj

SRCS = main.c \
       src/input/controls/controls.c \
       src/input/camera/camera.c \
       src/models/model.c \
       src/terrain/terrain.c \
       src/terrain/island.c \
       src/terrain/waves.c \
       src/input/movement/movement.c \
       src/hud/hud.c \
       src/hud/guide.c \
       src/weather/weather.c \
       src/config/config.c

OBJS = $(addprefix $(OBJ_DIR)/, $(SRCS:.c=.o))

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) -o $@ $^ $(LIBS)

$(OBJ_DIR)/%.o: %.c
	@if not exist $(subst /,\,$(dir $@)) mkdir $(subst /,\,$(dir $@))
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	@if exist $(OBJ_DIR) rmdir /s /q $(OBJ_DIR)
	@if exist $(TARGET) del /q $(TARGET)

help:
	@echo Available commands:
	@echo   make        - Build the project (incremental)
	@echo   make clean  - Remove build artifacts (obj folder and exe)
	@echo   make help   - Show this help message