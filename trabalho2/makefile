ASAN_FLAGS = -fsanitize=address -fno-omit-frame-pointer -Wno-format-security
ASAN_LIBS  = -static-libasan
CFLAGS    := -Wall -Wextra -Werror --std=gnu99 -g3
LDFLAGS   += -lpthread
CC := gcc
RM := rm -rf

.PHONY: all clean

VPATH := ./src/ ./obj/ ./include/

# Paths for .c, .h, and .o Files 
SRC_PATH := ./src/
OBJ_PATH := ./obj/
INC_PATH := -I ./include

# Executable Names
TARGET := main

# Source files
SRC := main.c server.c queue_server.c iterative_server.c concurrent_server.c thread_server.c

# Object files
OBJ := $(OBJ_PATH)server.o $(OBJ_PATH)queue_server.o $(OBJ_PATH)iterative_server.o $(OBJ_PATH)concurrent_server.o $(OBJ_PATH)thread_server.o $(OBJ_PATH)main.o

# Build all
all: $(TARGET)

# Build .o files
$(OBJ_PATH)%.o: $(SRC_PATH)%.c
	@mkdir -p $(OBJ_PATH)
	@echo [CC] $<
	@$(CC) $(C_FLAGS) -o $@ -c $< $(INC_PATH)                  

# Build server binary
$(TARGET): $(OBJ)
	@echo [INFO] Creating Server Binary Executable [$(TARGET)]
	@$(CC) -o $@ $^

# Clean all the object files and the binaries
clean:
	@echo "[Cleaning]"
	@$(RM) $(OBJ_PATH)*.o $(TARGET)
