# This is the makefile that generates the executable

# Files to compile
FILES_C = main.c red-black-tree.c read_tools.c tree_creation.c search_operations.c  write_read.c

# Exectuable to generate
TARGET = practica5S
CFLAGS = -lpthread


FILES_O = $(subst .c,.o,$(FILES_C))

$(TARGET): $(FILES_O) Makefile
	gcc $(FILES_O) -o $(TARGET) $(CFLAGS)

%.o: %.c Makefile
	gcc -c $<

all: $(TARGET) 

clean:
	rm -f *.o *~ 
