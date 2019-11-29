# This is the makefile that generates the executable

# Files to compile
FILES_C = red-black-tree.c create-tree.c read-write-tree.c consult-tree.c main.c

# Exectuable to generate
TARGET = main

# Compilation options
CFLAGS = -Wall -Werror -g

# Linker options 
LFLAGS = -lm

# There is no need to change the instructions below this
# line. Change if you really know what you are doing.

FILES_O = $(subst .c,.o,$(FILES_C))

$(TARGET): $(FILES_O) Makefile
	gcc $(FILES_O) -o $(TARGET) $(LFLAGS) 

%.o: %.c Makefile
	gcc $(CFLAGS) -c $<

all: $(TARGET) 

clean:
	/bin/rm $(FILES_O) $(TARGET) 
