# allow easily using a different compiler by setting CC
CC ?= gcc
LIBNAME := libutf8rewind

ifeq ($(OS), Windows_NT)
SHLIB_EXT = dll
else ifeq ($(shell uname), Darwin)
SHLIB_EXT = dylib
else
SHLIB_EXT = so
endif

OBJS = source/internal/casemapping.o \
       source/internal/codepoint.o \
       source/internal/composition.o \
       source/internal/database.o \
       source/internal/decomposition.o \
       source/internal/seeking.o \
       source/internal/streaming.o \
       source/unicodedatabase.o \
       source/utf8rewind.o

.PHONY : all objects clean

.DEFAULT_GOAL := all

all: $(LIBNAME).a $(LIBNAME).$(SHLIB_EXT)

# magic makefile variables, see
# https://www.gnu.org/software/make/manual/html_node/Automatic-Variables.html
%.o : %.c
	$(CC) -Iinclude/utf8rewind -c $< -o $@

objects: $(OBJS)

# static library
$(LIBNAME).a: $(OBJS)
	ar rcs $(LIBNAME).a $(OBJS)

# shared library
$(LIBNAME).$(SHLIB_EXT): $(OBJS)
	$(CC) -shared -o $(LIBNAME).$(SHLIB_EXT) $(OBJS)

clean:
	rm -f $(OBJS) $(LIBNAME).a $(LIBNAME).$(SHLIB_EXT)
