EXE = ssm
CC=gcc

SRC_DIR = src
OBJ_DIR = obj

SRC = $(wildcard $(SRC_DIR)/*.c)
OBJ = $(SRC:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)

CPPFLAGS += -Iinclude -I$(SRC_DIR) -I src/librequest-0.4/api -I src/libparseur
CFLAGS += -Wall -g
LDFLAGS += -Llib
LDLIBS += -lm -L src/libparser -lparser -Lsrc/librequest-0.4 -lrequest -lmagic

.PHONY: all clean

all: $(EXE)

$(EXE): $(OBJ)
	$(CC) $^ $(LDLIBS) -o $@

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

clean:
	$(RM) $(OBJ)
	$(RM) $(EXE)

test: $(EXE)
	export LD_LIBRARY_PATH=. && ./$(EXE) 

