CC	= cc
CXX	= cc
CXXFLAGS	= -W -O1
LDLIBS	= -ltranslation -lbe -lmedia

TARGET	= BShisen

# for compiling
OBJS	= main.o board.o Shisen.o GameWindow.o ID_Win.o \
	BitmapControl.o Tiles.o Hall.o \
	Prefs.o

.SUFFIXES: .cpp

.cpp.o:
	$(CXX) -c $(CXXFLAGS) $<


$(TARGET): $(OBJS) 
	$(CC) $(LDFLAGS) $(OBJS) $(LDLIBS) -o $@
	strip $@
	xres -o $@ $(TARGET)_x86.rsrc 
	mimeset $(TARGET)


clean:
	$(RM) *.o 

clobber: clean
	$(RM) -f $(TARGET)
               
main.o:		main.cpp board.h
Shisen.o:	Shisen.cpp Shisen.h 
board.o:	board.cpp board.h
GameWindow.o:	GameWindow.cpp GameWindow.h board.h array2d.cpp \
		BitmapControl.h Prefs.h
ID_Win.o:	ID_Win.cpp ID_Win.h
BitmapControl.o:	BitmapControl.cpp BitmapControl.h
Tiles.o:	Tiles.cpp Tiles.h
Hall.o:		Hall.cpp Hall.h
Prefs.o:	Prefs.cpp Prefs.h
