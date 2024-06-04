APPLICATION_EXECUTABLE  = maze-collapse
EDITOR_EXECUTABLE = editor
 
CODE_DIR 	= ./src/code/
DATA_DIR 	= ./src/data/

BIN_DIR     = ./bin/
OBJ_DIR 	= ./bin/obj/
OUT_PREF	?= build
OUT_DIR		= $(BIN_DIR)/$(OUT_PREF)

SOURCES 	= $(shell find $(CODE_DIR)/ -type f -name '*.cpp')
OBJ_PATTERN = $(OBJ_DIR)%.o
SRC_PATTERN = $(CODE_DIR)%.cpp

CC          = g++
LDFLAGS     =
CFLAGS      = -std=c++20 -I$(CODE_DIR)
CFLAGS_DEBUG= -Wall -fsanitize=address -fdiagnostics-color=always
LIBS	    = -lncurses

APPLICATION_DIRS = $(CODE_DIR)/engine $(CODE_DIR)/utility $(CODE_DIR)/map $(CODE_DIR)/entity $(CODE_DIR)/application $(CODE_DIR)/lib
EDITOR_DIRS = $(CODE_DIR)/engine $(CODE_DIR)/utility $(CODE_DIR)/map $(CODE_DIR)/editor



APPLICATION_SOURCES	= $(shell find $(APPLICATION_DIRS)/ -type f -name '*.cpp')
EDITOR_SOURCES 		= $(shell find $(EDITOR_DIRS)/ -type f -name '*.cpp')

APPLICATION_OBJECTS	= $(APPLICATION_SOURCES:$(SRC_PATTERN)=$(OBJ_PATTERN))
EDITOR_OBJECTS		= $(EDITOR_SOURCES:$(SRC_PATTERN)=$(OBJ_PATTERN))


# ------------------------- commands -------------------------
all: build-debug-full

.PHONY: all

# ------------------------- create directories -------------------------
create-directories:
#	+++ create binary directory +++
	mkdir -p $(BIN_DIR)
# 	+++ create object directory +++
	mkdir -p $(OBJ_DIR)
# 	+++ create out directory +++
	mkdir -p $(OUT_DIR)
#	+++ create data directory +++
	rsync -a --delete $(DATA_DIR) $(OUT_DIR)/data


.PHONY: create-directories


# ------------------------- builds -------------------------
build-release: clean-full build-editor build-application clean

build-application-debug: build-application
	(cd $(OUT_DIR); ./$(APPLICATION_EXECUTABLE))
build-editor-debug: build-editor
	(cd $(OUT_DIR); ./$(EDITOR_EXECUTABLE))

build-application: create-directories $(APPLICATION_SOURCES) $(APPLICATION_EXECUTABLE)
build-editor: create-directories $(EDITOR_SOURCES) $(EDITOR_EXECUTABLE)


# ------------------------- compile and link -------------------------

$(APPLICATION_OBJECTS) $(EDITOR_OBJECTS): $(OBJ_PATTERN) : $(SRC_PATTERN) 
	mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

$(APPLICATION_EXECUTABLE): $(APPLICATION_OBJECTS)
	$(CC) $(CFLAGS) $(LIBS) $(APPLICATION_OBJECTS) -o $(OUT_DIR)/$@
$(EDITOR_EXECUTABLE): $(EDITOR_OBJECTS)
	$(CC) $(CFLAGS) $(LIBS) $(EDITOR_OBJECTS) -o $(OUT_DIR)/$@



# ------------------------- cleans -------------------------
clean: # +++ cleanup +++
	$(RM) -rf $(OBJ_DIR)


clean-full: # +++ cleanup +++
	$(RM) -rf $(BIN_DIR)


.PHONY: clean clean-full