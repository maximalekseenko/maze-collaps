EXECUTABLE  = maze-collapse

CC          = g++
LDFLAGS     =
CFLAGS      = -fsanitize=address -fdiagnostics-color=always -std=c++20 -Wall

CODE_DIR 	= ./src/code/
DATA_DIR 	= ./src/data/

BIN_DIR     = ./bin/
OBJ_DIR 	= ./bin/obj/

SOURCES 	= $(wildcard $(CODE_DIR)/*.cpp)
LIBS	    = -lncurses

OBJ_PATTERN = $(OBJ_DIR)%.o
SRC_PATTERN = $(CODE_DIR)%.cpp
OBJECTS		= $(SOURCES:$(SRC_PATTERN)=$(OBJ_PATTERN))


# ------------------------- commands -------------------------
all: build-debug


build-debug: create_directories create_executable create_data
build-prod: create_directories create_executable create_data


# ------------------------- create directories -------------------------
create_directories: $(BIN_DIR) $(OBJ_DIR)


$(BIN_DIR): # +++ create binary directory +++
	mkdir -p $(BIN_DIR)


$(OBJ_DIR): # +++ create object directory +++
	mkdir -p $(OBJ_DIR)


# ------------------------- create executable -------------------------
create_executable: $(SOURCES) $(EXECUTABLE)


$(OBJECTS): $(OBJ_PATTERN) : $(SRC_PATTERN) # +++ compile +++
	$(CC) $(CFLAGS) -c $< -o $@


$(EXECUTABLE): $(OBJECTS) # +++ link +++
	# $(OBJECTS)
	$(CC) $(CFLAGS) $(LIBS) $(OBJECTS) -o $(BIN_DIR)/$(EXECUTABLE)


# ------------------------- create data -------------------------
create_data:
	# cp -Rf $(DATA_DIR) $(BIN_DIR)
	rsync -rupE $(DATA_DIR) $(BIN_DIR)/data


# ------------------------- extra -------------------------
clean: # +++ cleanup +++
	$(RM) -rf $(OBJ_DIR)


.PHONY: create_directories create_executable create_data clean