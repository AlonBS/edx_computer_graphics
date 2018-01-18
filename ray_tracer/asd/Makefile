#INCFLAGS  = -I /usr/include/GL -I ./utilities/include
#INCFLAGS += -I ~/programming/Computer_Graphics_Exs/Computer_Graphics/Ex0_MIT/mit-vecmath

#INCFLAGS += -I /mit/6.837/public/include/vecmath

#LINKFLAGS  = -lGL -lGLU -lassimp  -lSOIL
LINKFLAGS  = -L ./lib/nix -lfreeimage -lGLEW


CFLAGS    = -g -std=c++1y
CC        = g++
SRCS      = Camera.h \
            RenderInfo.h \
            Scene.h \
            RayTrace.h \
            Ray.h \
            Image.h \
            main.cpp \
            SceneParser.cpp \
	    

OBJS      = $(SRCS:.cpp=.o)
PROG      = ray_tracer

all: $(SRCS) $(PROG)

$(PROG): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $@ $(INCFLAGS) $(LINKFLAGS)

.cpp.o:
	$(CC) $(CFLAGS) $< -c -o $@ $(INCFLAGS)

depend:
	makedepend $(INCFLAGS) -Y $(SRCS)

clean:
	rm $(OBJS) $(PROG)

