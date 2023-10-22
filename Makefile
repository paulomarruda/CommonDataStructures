.PHONY: clean lib examples
CC := gcc
OPTS := -fPIC -g -O3
INCLUDES := ./include/
# generate files that include make rules for header files
DEPFLAGS := -MP -MD
CFLAGS := -Wall -Wextra -pedantic $(foreach T, $(INLCUDES), -I $(T)) $(OPTS) $(DEPFLAGS)
SRC := $(wildcard ./src/*.c)
SRC_O := $(patsubst %.c, %.o, $(SRC))
EXMP := $(wildcard ./examples/*.c)
TESTS := $(wildcard ./tests/*.c)
BIN := bin
LIBS := -L ./$(BIN)/
CFLAGS += $(LIBS)
AR := ar
ARFLAGS := rcs
all: lib examples
%/:
	mkdir -p ./$@
%.o:%.c
	@ $(CC) $(CFLAGS) -c $< -o $(bin)$@
lib: $(SRC_O) | $(BIN)/
	@ echo "== Compiling shared library libCDS =="
	@ $(CC) $(CFLAGS) -shared $(SRC_O) -o ./$(BIN)/libCDS.so
	@ echo "== Compiling the static library libCDS =="
	@ $(AR) $(ARFLAGS) ./$(BIN)/libCDS-static.a $(SRC_O)
examples: $(EXMP) lib | $(BIN)/
	@ echo "== Compiling the examples =="
	@ $(foreach T, $(EXMP), \
	  $(CC) $(CFLAGS) $(T) -lCDS-static -o $(patsubst %.c, %.out, $(T));)
clean:
	@ echo "== Deleting all .o, .d and .o files =="
	@ find . -type f -name '*.o' -delete
	@ find . -type f -name '*.out' -delete
	@ find . -type f -name '*.d' -delete
	@ echo "== Deleting bin/ folder =="
	@ rm -rf bin/
