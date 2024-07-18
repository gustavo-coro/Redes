C_FLAGS := -g -Wall -Wextra
CC := gcc
RM := rm -rf

.PHONY: all clean

VPATH := ./src/ ./obj/ ./include/

# Paths for .c, .h, and .o Files 
SRC_PATH := ./src/
OBJ_PATH := ./obj/
INC_PATH := -I ./include

# Executable Names
SERVER_TARGET := servidor
CLIENT_TARGET := cliente

# Source files
SERVER_SRC := servidor.c
CLIENT_SRC := cliente.c

# Object files
SERVER_OBJ := $(OBJ_PATH)servidor.o
CLIENT_OBJ := $(OBJ_PATH)cliente.o

# Build all
all: $(SERVER_TARGET) $(CLIENT_TARGET)

# Build .o files
$(OBJ_PATH)%.o: $(SRC_PATH)%.c
	@mkdir -p $(OBJ_PATH)
	@echo [CC] $<
	@$(CC) $(C_FLAGS) -o $@ -c $< $(INC_PATH)                  

# Build server binary
$(SERVER_TARGET): $(SERVER_OBJ)
	@echo [INFO] Creating Server Binary Executable [$(SERVER_TARGET)]
	@$(CC) -o $@ $^

# Build client binary
$(CLIENT_TARGET): $(CLIENT_OBJ)
	@echo [INFO] Creating Client Binary Executable [$(CLIENT_TARGET)]
	@$(CC) -o $@ $^

# Clean all the object files and the binaries
clean:
	@echo "[Cleaning]"
	@$(RM) $(OBJ_PATH)*.o $(SERVER_TARGET) $(CLIENT_TARGET)
