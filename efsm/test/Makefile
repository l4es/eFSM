#
#
#
INCLUDE = -I. -I../include -I../../safe_base/include

SRC =	demo.c \
        demo_session_fsm.c \
        demo_event_handlers.c

OBJ = $(SRC:.c=.o)

LIB = ../lib/fsm.a 

IMAGE =	d_fsm  


CCC = gcc  
DEBUG = -g
CFLAGS =  -Wall -c $(DEBUG)
LFLAGS = -Wall $(DEBUG)


$(IMAGE): 
	$(CCC) $(INCLUDE) $(LFLAGS) $(SRC) $(LIB)  -o $(IMAGE)

clean:
	rm -f $(OBJ) $(IMAGE)  

# DO NOT DELETE 


