# Simple Makefile for CS6962 adjacency data structure assignment


SHELL = bash


# ===============================================================

SRCS 	 	= main.C \
		  matrix.C \
		  camera.C \
		  glCanvas.C \
		  Halfedge/mesh.C \
		  Halfedge/edge.C
		  #mesh.C \
		  #edge.C

# ===============================================================

LIBS            = -lm -lGL -lGLU -lglut -L/usr/X11R6/lib
#CC		= g++ -I/usr/X11R6/include -I/usr/local/include
CC		= g++ -I/usr/X11R6/include -I/usr/local/include -IHalfedge -IArray -I./
EXE 	  	= adjacency
OBJS =  $(SRCS:.C=.o)

# ===============================================================
# targets

#.PHONY: all depend clean
.PHONY: all clean

all: depend $(EXE)

depend:
	$(CC) -E -M $(SRCS) > Makefile.depend

clean: 
	#-rm -f *~ *bak *.o  core.* Makefile.depend $(EXE) 

# ===============================================================
# compilation rules

$(EXE): $(OBJS)
	$(CC) -o $@ $(OBJS) $(LIBS)

.C.o:
	$(CC) -g3 $< -c -o $@

# ===============================================================

#-include Makefile.depend

