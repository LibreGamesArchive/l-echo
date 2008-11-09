CXXFLAGS = -I./ -DSTRICT_MEM  -DDEBUG -DTIXML_USE_STL -DUSE_PUGIXML -g3 -Wall
#ifeq( $() )
#endif
pugiXML_USE_STL := YES

DEVELOPER_SDK_DIR := /opt/mac/SDKs

CFLAGS := -isysroot /opt/mac/SDKs/MacOSX10.5.sdk
CPPFILES  := $(wildcard *.cpp) $(wildcard pugixml/*.cpp)

OFILES    := $(CPPFILES:.cpp=.o)
OBJFILES  := $(CPPFILES:.cpp=.OBJ)
#DOPFILES  := $(CPPFILES:.cpp=.DOP)
#DOIFILES  := $(CPPFILES:.cpp=.DOI)
DOFILES   := $(CPPFILES:.cpp=.DO)

VERSION   := 0.4.0
REVISION  := 73

PKGCOMMON := -echo-$(VERSION)_r$(REVISION)
PKGPREFIX := ../l$(PKGCOMMON)-
NPKG 	:= ../n$(PKGCOMMON).zip

DESC      := L-Echo $(VERSION)
UPLOAD := python googlecode_upload.py -p 'l-echo' -s 


all: $(OFILES)
	gcc pugixml/*.o *.o -DTIXML_USE_STL -lGL -lGLU -lglut -lpthread -g3 -Wall -o l-echo

valgrind:
	valgrind --tool=memcheck --leak-check=yes --show-reachable=yes --num-callers=20 --track-fds=yes --suppressions=fglrx.supp --sim-hints=lax-ioctls ./l-echo A1.xml.real 2> summary.txt
	sed 's|.*/usr/lib.*|LOL|'  summary.txt > summary3.txt

#lab: CXXFLAGS += -DLAB
lab: $(OFILES)
	gcc pugixml/*.o *.o -DTIXML_USE_STL /usr/lib/libGLU.so.1 /usr/lib/libGL.so.1.2 libglut.so.3 -lpthread -g3 -Wall -o l-echo

%.OBJ: %.cpp
	i586-mingw32msvc-g++ $(CXXFLAGS) -c -o $@ $<

w32: $(OBJFILES)
	i586-mingw32msvc-g++ pugixml/*.OBJ *.OBJ glut32.lib -lGL -lGLU -g3 -Wall -o l-echo.exe

#%.DO: CXXFLAGS += -isysroot /opt/mac/SDKs/MacOSX10.5.sdk -I/opt/mac/SDKs/MacOSX10.5.sdk/usr/include  
%.DO: %.cpp
	powerpc-apple-darwin8-g++ -arch i386 -arch ppc $(CXXFLAGS) -c -o $@ $<

mac: $(DOFILES)
	powerpc-apple-darwin8-g++ -arch i386 -arch ppc pugixml/*.DO *.DO -framework OpenGL -framework GLUT -g3 -Wall -o l-echo-mac

#%.DOP: %.cpp #DOP stands for "Darwin Object (PowerPC)"
#	powerpc-apple-darwin8-g++ $(CXXFLAGS) -c -o $@ $<

#%.DOI: %.cpp #DOP stands for "Darwin Object (Intel)"
#	i686-apple-darwin8-g++ $(CXXFLAGS) -c -o $@ $<

#macppc: $(DOPFILES)
#	powerpc-apple-darwin8-g++ *.DOP pugixml/*.DOP -framework OpenGL -framework GLUT -g3 -Wall -o l-echo-macppc

#macintel: $(DOIFILES)
#	i686-apple-darwin8-g++ *.DOI pugixml/*.DOI -framework OpenGL -framework GLUT -g3 -Wall -o l-echo-macintel

clean:
	rm *.o *.OBJ l-echo.exe l-echo l-echo-mac *.DO *~ || echo

clean-all: clean
	rm pugixml/*.o pugixml/*.OBJ pugixml/*.DO || echo
	rm -rf n-echo || echo

run: all
	./l-echo perspective_movement.xml

dbg: all
	gdb ./l-echo

package: all w32
	zip -r $(PKGPREFIX)lin32.zip l-echo *.xml *.xml.real L_ECHO_README
	zip -r $(PKGPREFIX)w32.zip l-echo.exe *.xml *.xml.real L_ECHO_README

package-mac: mac
#macintel macppc
#	powerpc-apple-darwin8-lipo -create l-echo-macppc l-echo-macintel -output l-echo-mac
	dd if=/dev/zero of=$(PKGPREFIX)osx.dmg bs=1M count=5
	mkfs.hfsplus -v '$(DESC)' $(PKGPREFIX)osx.dmg
	sudo mkdir -p /mnt/dmg
	sudo mount -t hfsplus -o loop $(PKGPREFIX)osx.dmg /mnt/dmg
	sudo cp -t /mnt/dmg l-echo-mac *.xml *.xml.real L_ECHO_README
	sudo umount /mnt/dmg
	
setup-nds:
	cp -r n-echo_template n-echo
	cp -t n-echo A*.xml.real
	cp -t n-echo/source *.cpp pugixml/*.cpp
	cp -t n-echo/include *.h pugixml/*.hpp

nds:
	make -C n-echo
	zip -r $(NPKG) n-echo/n-echo.nds n-echo/n-echo.ds.gba n-echo/apps n-echo/N_ECHO_README
	
upload: package package-mac nds
	$(UPLOAD) 'Linux (32) binary + xml stages - $(VERSION) (revision $(REVISION))' $(PKGPREFIX)lin32.zip 
	$(UPLOAD)  'Windows (32) binary + xml stages - $(VERSION) (revision $(REVISION))' $(PKGPREFIX)w32.zip
	$(UPLOAD)  'Mac OS X (universal, for 10.4+) binary + xml stages - $(VERSION) (revision $(REVISION))' $(PKGPREFIX)osx.dmg
	$(UPLOAD) 'N-Echo .nds, .ds.gba + xml stages - $(VERSION) (revision $(REVISION))' $(NPKG)
	
upload-nds: nds
	$(UPLOAD) 'N-Echo .nds, .ds.gba + xml stages - $(VERSION) (revision $(REVISION))' $(NPKG)

count:
	wc -l *.cpp *.h

