TARGET = aasml
CC = gcc
CCFLAGS = \
        -ggdb \
        -Wall \
        -Wextra \

LDFLAGS = 
SRC_DIR = ./src
OBJ_DIR = ./obj


SRC_FILES = $(shell find $(SRC_DIR) -type f -name '*.c')
OBJ_FILES = $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(SRC_FILES))

all: $(TARGET)


$(OBJ_FILES): $(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@echo "$<"
	@$(CC) $(CCFLAGS) -c $< -o $@

$(TARGET): $(OBJ_FILES)
	@$(CC) $(LDFLAGS) $(OBJ_FILES) -o $@


clean:
	@rm -v $(OBJ_FILES)

.PHONY: all clean
