INCFLAGS  = -I /usr/include/GL -I ./utilities/include -I /usr/include/boost

#LINKFLAGS  = -lGL -lGLUassimp  -lSOIL
LINKFLAGS  = -L ./lib/nix -lfreeimage -lGLEW


CFLAGS    = -g -std=c++1y 
CC        = g++
SRCS      = main.cpp \
			Image.cpp \
	    	Ray.cpp \
	    	RayTracer.cpp \
	    	Scene.cpp \
            SceneParser.cpp \
            Object.cpp \
            Sphere.cpp \
            Triangle.cpp \
            Lights.cpp
            
	    

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
	rm -rf $(OBJS) $(PROG)

