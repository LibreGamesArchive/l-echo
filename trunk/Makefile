CXXFLAGS = -I./ -DSTRICT_MEM  -DDEBUG -DTIXML_USE_STL -DUSE_IK -DUSE_PUGIXML -g3 -Wall
LINUX_LDFLAGS = -lalut -lopenal -lGL -lGLU -lglut -lpthread
WINDOWS_LDFLAGS = -lalut -lopenal glut32.lib -lGL -lGLU
MACOSX_LDFLAGS =  -framework OpenGL -framework GLUT -framework OpenAL 
#-arch ppc libmadppc.a  -arch i386 libmadi386.a  


pugiXML_USE_STL := YES
CPPFILES  := $(wildcard *.cpp) $(wildcard pugixml/*.cpp)

OFILES    := $(CPPFILES:.cpp=.o)
OBJFILES  := $(CPPFILES:.cpp=.OBJ)
DOFILES   := $(CPPFILES:.cpp=.DO)
#.DOA - Darwin Object ARM (iPhone, iPod Touch)
DOAFILES   := $(CPPFILES:.cpp=.DOA)

VERSION   := 0.4.3
REVISION  := 108

PKGCOMMON := -echo-$(VERSION)_r$(REVISION)
PKGPREFIX := ../l$(PKGCOMMON)-
NPKG 	:= ../n$(PKGCOMMON).zip

DESC      := L-Echo $(VERSION)
UPLOAD := python googlecode_upload.py -p 'l-echo' -s 


all: $(OFILES)
	gcc pugixml/*.o *.o $(LINUX_LDFLAGS) -g3 -Wall -o l-echo

source-tarball:
	zip -r $(PKGPREFIX)src.zip *.cpp *.h pugixml/ .svn/ gen/ *.xml* L_ECHO_README Makefile n-echo_template/
#zip -r *.cpp *.h pugixml/ *.xml*

valgrind:
	valgrind --tool=memcheck --leak-check=yes --show-reachable=yes --num-callers=20 --track-fds=yes --suppressions=fglrx.supp --sim-hints=lax-ioctls ./l-echo A1.xml.real 2> summary.txt
	sed 's|.*/usr/lib.*|LOL|'  summary.txt > summary3.txt

#lab: CXXFLAGS += -DLAB
lab: $(OFILES)
	gcc pugixml/*.o *.o -DTIXML_USE_STL /usr/lib/libGLU.so.1 /usr/lib/libGL.so.1.2 libglut.so.3 -lpthread -g3 -Wall -o l-echo

%.OBJ: %.cpp
	i586-mingw32msvc-g++ $(CXXFLAGS) -c -o $@ $<

w32: $(OBJFILES)
	i586-mingw32msvc-g++ pugixml/*.OBJ *.OBJ $(WINDOWS_LDFLAGS) -g3 -Wall -o l-echo.exe

echo_mp3.DO:
	powerpc-apple-darwin8-g++ $(CXXFLAGS) -I /opt/mac/SDKs/MacOSX10.4u.sdk/System/Library/Frameworks/OpenAL.framework/Headers/ -c echo_mp3.cpp -o macbuild/echo_mp3-macppc.DO
	i686-apple-darwin8-g++ $(CXXFLAGS) -I /opt/mac/SDKs/MacOSX10.4u.sdk/System/Library/Frameworks/OpenAL.framework/Headers/ -c echo_mp3.cpp -o macbuild/echo_mp3-maci386.DO
	powerpc-apple-darwin8-lipo -create -arch ppc macbuild/echo_mp3-macppc.DO -arch i386 macbuild/echo_mp3-maci386.DO -output echo_mp3-mac.DO

#%.DO: CXXFLAGS += -isysroot /opt/mac/SDKs/MacOSX10.5.sdk -I/opt/mac/SDKs/MacOSX10.5.sdk/usr/include  
%.DO: %.cpp
	powerpc-apple-darwin8-g++ -arch i386 -arch ppc $(CXXFLAGS) -c -o $@ $<

mac: $(DOFILES)
	powerpc-apple-darwin8-g++ $(MACOSX_LDFLAGS) pugixml/*.DO *.DO -g3 -Wall -o l-echo-mac

clean:
	rm *.o *.OBJ l-echo.exe l-echo l-echo-mac *.DO *.DOA macbuild/* *~ || echo

clean-all: clean
	rm pugixml/*.o pugixml/*.OBJ pugixml/*.DO pugixml/*.DOA || echo
	rm -rf n-echo || echo

run: all
	./l-echo perspective_movement.xml

dbg: all
	gdb ./l-echo

package: all w32
	zip -r $(PKGPREFIX)lin32.zip l-echo *.xml *.xml.real L_ECHO_README
	zip -r $(PKGPREFIX)w32.zip l-echo.exe *.xml *.xml.real glut32.dll L_ECHO_README

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
	
upload: source-tarball package package-mac nds
	$(UPLOAD) 'Source Code - $(VERSION) (revision $(REVISION))' -l Featured,Type-Source  $(PKGPREFIX)src.zip 
	$(UPLOAD) 'Linux (32) binary + xml stages - $(VERSION) (revision $(REVISION))' -l Featured,Type-Executable,OpSys-Linux $(PKGPREFIX)lin32.zip 
	$(UPLOAD) 'Windows (32) binary + xml stages - $(VERSION) (revision $(REVISION))' -l Featured,Type-Executable,OpSys-Windows $(PKGPREFIX)w32.zip
	$(UPLOAD) 'Mac OS X (universal, for 10.4+) binary + xml stages - $(VERSION) (revision $(REVISION))' -l Featured,Type-Executable,OpSys-OSX $(PKGPREFIX)osx.dmg
	$(UPLOAD) 'N-Echo .nds, .ds.gba + xml stages - $(VERSION) (revision $(REVISION))' -l Featured,Type-Executable $(NPKG)
	
upload-nds: nds
	$(UPLOAD) 'N-Echo .nds, .ds.gba + xml stages - $(VERSION) (revision $(REVISION))' $(NPKG)

count:
	wc -l *.cpp *.h

