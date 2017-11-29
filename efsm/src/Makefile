
SRC =	fsm.c

OBJ = $(SRC:.c=.o)

OUT = ../lib/fsm.a

INCLUDES = -I. \
           -I../include/ \
           -I../../safe_base/include

CCFLAGS = -g
CCC = gcc
LDFLAGS = -g
.SUFFIXES: .c

.c.o:
	$(CCC) $(INCLUDES) $(CCFLAGS) -c $< -o $@

$(OUT): $(OBJ)
	ar rcs $(OUT) $(OBJ)

clean:
	rm -f $(OBJ)  

# DO NOT DELETE


