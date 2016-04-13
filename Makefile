# make clean, release, debug | PREFIX, CC optional
# be careful about PREFIX since clean and build rm -rf it
# use as make PREFIX=/mydirectory/utf8rewind !! 

CC := gcc
LIBNAME := utf8rewind
PREFIX := ./utf8rewind

DIRS := ../source ../source/internal
SRC := $(foreach dir,$(DIRS),$(wildcard $(dir)/*.c))
INCLUDES := $(foreach dir,$(DIRS),$(addprefix -I,$(dir)))
INCLUDES += -I../include/utf8rewind

# common CFLAGS for everything
CFLAGS := -std=c99 -Wall -Werror -Wno-unused -fdiagnostics-color=always -c 
# debug equivavlents, override manually or set to same as release flags
DCFLAGS := $(CFLAGS)
DINCLUDES := $(INCLUDES)

.PHONY : all debug release clean

.DEFAULT_GOAL := all

all: release

# debug
debug: DCFLAGS += -g
debug: DEBUG := TRUE
debug: LIBNAME := $(LIBNAME)d
debug: proglib

# release
release: CFLAGS += -O3 -fomit-frame-pointer
# optimizations: -msse4.2 -mavx(2) -ftree-vectorize -ffast-math OR -fassociative-math
release: DEBUG := FALSE
release: proglib

clean:
	@rm -rf $(PREFIX)

proglib:
	@rm -rf $(PREFIX)
	@mkdir -p $(PREFIX)

	@echo "\033[33mUTF8REWIND:\033[32m$(MODULES)" "\033[0m" "\033[0m"
	@echo "\033[33m==================================================\033[0m"

	@echo "Platform:\033[34m" neutral"\033[0m"
	@echo "Prefix:" $(PREFIX)
	@echo "CC:" $(CC)

	$(if $(filter FALSE,$(DEBUG)), \
		@echo "Common CFLAGS:" $(CFLAGS);, \
		@echo "Common debug CFLAGS:" $(DCFLAGS); \
	)

	@echo "Includes:" $(INCLUDES)

	@echo
	@$(foreach cfile,$(SRC), \
		echo 'compiling \033[33m $(cfile) \033[0m'; \
		if [[ "$(DEBUG)" == "FALSE" ]]; then \
		echo $(CC) $(CFLAGS) $(INCLUDES) $(cfile) -o $(PREFIX)/$(notdir $(subst .c,.o,$(cfile))); \
		$(CC) $(CFLAGS) $(INCLUDES) $(cfile) -o $(PREFIX)/$(notdir $(subst .c,.o,$(cfile))); \
		else \
		echo $(CC) $(DCFLAGS) $(INCLUDES) $(cfile) -o $(PREFIX)/$(notdir $(subst .c,.o,$(cfile))); \
		$(CC) $(DCFLAGS) $(INCLUDES) $(cfile) -o $(PREFIX)/$(notdir $(subst .c,.o,$(cfile))); \
		fi; \
	)
	@echo

	@echo "Linking:" $(addprefix $(PREFIX)/,$(notdir $(subst .c,.o,$(SRC))));
	@ar rcs $(PREFIX)/lib$(LIBNAME).a $(addprefix $(PREFIX)/,$(notdir $(subst .c,.o,$(SRC))));

	@if [[ "$(DEBUG)" == "FALSE" ]]; then \
		echo; \
		echo "Cleaning $(PREFIX)/*.o"; \
		rm -f $(PREFIX)/*.o; \
	fi;

	@echo
	@echo "Copying include to $(PREFIX)"
	@cp -R ../include $(PREFIX)

	@echo "\033[33m==================================================\033[0m"
	@echo "\033[32mDone - $(PREFIX)/lib$(LIBNAME).a\033[0m"
