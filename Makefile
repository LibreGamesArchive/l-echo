CXXFLAGS = -I./ -I../include -DTIXML_USE_STL -g3 -Wall
TINYXML_USE_STL := YES

.SUFFIXES: .cpp .OBJ

all: echo_gfx.o echo_loader.o echo_math.o echo_ns.o echo_stage.o echo_sys_linux.o escgrid.o grid.o hole.o isect_grid.o main.o stair.o static_grid.o t_grid.o tinyxml/tinystr.o tinyxml/tinyxml.o tinyxml/tinyxmlerror.o tinyxml/tinyxmlparser.o
	gcc tinyxml/*.o *.o -DTIXML_USE_STL -lGL -lGLU  /usr/lib/libglut.so.3.8.0 -lpthread -g3 -Wall -o l-echo

.cpp.OBJ:
	i586-mingw32msvc-g++ $(CXXFLAGS) -c -o $@ $<

echo_gfx.OBJ: echo_gfx.cpp
echo_loader.OBJ: echo_loader.cpp
echo_math.OBJ: echo_math.cpp
echo_ns.OBJ: echo_ns.cpp
echo_stage.OBJ: echo_stage.cpp
echo_sys_w32.OBJ: echo_sys_w32.cpp
escgrid.OBJ: escgrid.cpp
grid.OBJ: grid.cpp
hole.OBJ: hole.cpp
isect_grid.OBJ: isect_grid.cpp
main.OBJ: main.cpp
stair.OBJ: stair.cpp
static_grid.OBJ: static_grid.cpp
t_grid.OBJ: t_grid.cpp
tinyxml/tinystr.OBJ: tinyxml/tinystr.cpp
tinyxml/tinyxml.OBJ: tinyxml/tinyxml.cpp
tinyxml/tinyxmlerror.OBJ: tinyxml/tinyxmlerror.cpp
tinyxml/tinyxmlparser.OBJ: tinyxml/tinyxmlparser.cpp

w32: echo_gfx.OBJ echo_loader.OBJ echo_math.OBJ echo_ns.OBJ echo_stage.OBJ echo_sys_w32.OBJ escgrid.OBJ grid.OBJ hole.OBJ isect_grid.OBJ main.OBJ stair.OBJ static_grid.OBJ t_grid.OBJ tinyxml/tinystr.OBJ tinyxml/tinyxml.OBJ tinyxml/tinyxmlerror.OBJ tinyxml/tinyxmlparser.OBJ
	i586-mingw32msvc-g++ *.OBJ tinyxml/*.OBJ glut32.lib -lGL -lGLU -g3 -Wall -o l-echo.exe

clean:
	rm *.o *.OBJ l-echo.exe l-echo || echo

clean-all: clean
	rm tinyxml/*.o || echo

run: all
	./l-echo perspective_movement.xml

dbg: all
	gdb ./l-echo

