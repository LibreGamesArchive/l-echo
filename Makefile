CXXFLAGS = -I./ -I../include -DSTRICT_MEM -DDEBUG -DTIXML_USE_STL -g3 -Wall
TINYXML_USE_STL := YES

CPPFILES  := $(wildcard *.cpp) $(wildcard tinyxml/*.cpp) $(wildcard ext/*.cpp)

LINFILES  := $(wildcard lin/*.cpp)
OFILES    := $(CPPFILES:.cpp=.o) $(LINFILES:.cpp=.o)

WINFILES  := $(wildcard win/*.cpp)
OBJFILES  := $(CPPFILES:.cpp=.OBJ) $(WINFILES:.cpp=.OBJ)

PKGPREFIX := ../l-echo-0.2.2_r38-

all: $(OFILES)
	gcc tinyxml/*.o *.o lin/*.o -DTIXML_USE_STL -lGL -lGLU  /usr/lib/libglut.so.3.8.0 -lpthread -g3 -Wall -o l-echo

%.OBJ: %.cpp
	i586-mingw32msvc-g++ $(CXXFLAGS) -c -o $@ $<

w32: $(OBJFILES)
	i586-mingw32msvc-g++ *.OBJ win/*.OBJ tinyxml/*.OBJ glut32.lib -lGL -lGLU -g3 -Wall -o l-echo.exe

clean:
	rm *.o *.OBJ l-echo.exe l-echo lin/*.o win/*.OBJ *~ || echo

clean-all: clean
	rm tinyxml/*.o tinyxml/*.OBJ || echo

run: all
	./l-echo perspective_movement.xml

dbg: all
	gdb ./l-echo

package: all w32
	zip -r $(PKGPREFIX)lin32.zip l-echo *.xml L_ECHO_README
	zip -r $(PKGPREFIX)w32.zip l-echo.exe *.xml L_ECHO_README

count:
	wc -l *.cpp *.h lin/*.cpp win/*.cpp

