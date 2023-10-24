.PHONY: clean lib examples doxygen
CC := gcc
OPTS := -fPIC -g -O3
INCLUDES := ./include/
# generate files that include make rules for header files
DEPFLAGS := -MP -MD
CFLAGS := -Wall -Wextra -Werror -pedantic $(foreach T, $(INLCUDES), -I $(T)) $(OPTS) $(DEPFLAGS)
SRC := $(wildcard ./src/*.c)
SRC_O := $(patsubst %.c, %.o, $(SRC))
EXMP := $(wildcard ./examples/*.c)
TESTS := $(wildcard ./tests/*.c)
BIN := bin
LIBS := -L ./$(BIN)/
AR := ar
ARFLAGS := rcs
all: lib examples
%/:
	@ echo "* Preparing bin/ folder"
	@ mkdir -p ./$@
	@echo "	Done :)"
%.o:%.c
	@ $(CC) $(CFLAGS) -c $< -o $(bin)$@
lib: $(SRC_O) | $(BIN)/
	@ echo "* Compiling shared library libCDS"
	@ $(CC) $(CFLAGS) -shared $(SRC_O) -o ./$(BIN)/libCDS.so
	@ echo "	Done :)"
	@ echo "* Compiling the static library libCDS"
	@ $(AR) $(ARFLAGS) ./$(BIN)/libCDS-static.a $(SRC_O)
	@ echo "	Done :)"
examples: $(EXMP) lib | $(BIN)/
	@ echo "* Compiling the examples =="
	@ $(foreach T, $(EXMP), \
	  $(CC) $(CFLAGS) $(T) $(LIBS) -lCDS-static -o $(patsubst %.c, %.out, $(T));)
	@ echo "	Done :)"
doxygen: ./docs/

clean:
	@ echo "* Deleting all .o, .d and .o files"
	@ find . -type f -name '*.o' -delete
	@ find . -type f -name '*.out' -delete
	@ find . -type f -name '*.d' -delete
	@ echo "	Done :)"
	@ echo "* Deleting bin/ folder"
	@ rm -rf bin/
	@ echo "	Done :)"
