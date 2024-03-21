EXECUTABLE  = maze-collapse

CC          = g++
LDFLAGS     =
CFLAGS      = -fsanitize=address -fdiagnostics-color=always -std=c++20 -Wall

CODE_DIR 	= ./src/code/
DATA_DIR 	= ./src/data/

BIN_DIR     = ./bin/
OBJ_DIR 	= ./bin/obj/
OUT_PREF	?= build
OUT_DIR		= $(BIN_DIR)/$(OUT_PREF)

SOURCES 	= $(wildcard $(CODE_DIR)/*.cpp) $(wildcard $(CODE_DIR)/*/*.cpp)
LIBS	    = -lncurses

OBJ_PATTERN = $(OBJ_DIR)%.o
SRC_PATTERN = $(CODE_DIR)%.cpp
OBJECTS		= $(SOURCES:$(SRC_PATTERN)=$(OBJ_PATTERN))

define cc-command
$(CC) $(CFLAGS) -c $< -o $@
endef

# ------------------------- commands -------------------------
all: build-update


build-debug: create_directories create_executable create_data
	(cd $(OUT_DIR); ./$(EXECUTABLE))


build-debug-full: clean-full create_directories create_executable create_data
	(cd $(OUT_DIR); ./$(EXECUTABLE))


build-prod: clean-full create_directories create_executable create_data clean


.PHONY: all build-debug build-debug-full build-prod

# ------------------------- create directories -------------------------
create_directories:
#	+++ create binary directory +++
	mkdir -p $(BIN_DIR)
# 	+++ create object directory +++
	mkdir -p $(OBJ_DIR)
# 	+++ create out directory +++
	mkdir -p $(OUT_DIR)


.PHONY: create_directories


# ------------------------- create executable -------------------------
create_executable: $(SOURCES) $(EXECUTABLE)


$(OBJECTS): $(OBJ_PATTERN) : $(SRC_PATTERN) # +++ compile +++
	mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@


$(EXECUTABLE): $(OBJECTS) # +++ link +++
	$(CC) $(CFLAGS) $(LIBS) $(OBJECTS) -o $(OUT_DIR)/$(EXECUTABLE)


.PHONY: create_executable $(EXECUTABLE)


# ------------------------- create data -------------------------
create_data:
	rsync -a --delete $(DATA_DIR) $(OUT_DIR)/data


.PHONY: create_data


# ------------------------- extra -------------------------
clean: # +++ cleanup +++
	$(RM) -rf $(OBJ_DIR)


clean-full: # +++ cleanup +++
	$(RM) -rf $(BIN_DIR)


.PHONY: clean clean-full