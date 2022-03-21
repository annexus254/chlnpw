CC					=	cc
CXX					=	c++
CXXFLAGS			=	-O3 -Wall -Wpedantic -std=c++17
CCFLAGS				=	-O3 -Wall -Wpedantic -std=c17

LIB_HEADERS			=	$(wildcard include/*.h)
LIB_SOURCES_C		=	$(wildcard src/*.c)
LIB_SOURCES_CXX		=	$(wildcard src/*.cc)
LIB_OBJECTS_C_temp	=	$(LIB_SOURCES_C:.c=.o)
LIB_OBJECTS_C		=	$(subst src,objects,$(LIB_OBJECTS_C_temp) )
LIB_OBJECTS_CXX_temp=	$(LIB_SOURCES_CXX:.cc=.o)
LIB_OBJECTS_CXX		=	$(subst src,objects,$(LIB_OBJECTS_CXX_temp) )

MAIN_SOURCE			=	chlnpw.cc
MAIN_OBJECT			=	$(MAIN_SOURCE:.cc=.o)
MAIN_PROG			=	bin/chlnpw

.PHONY: all clean

all:	$(MAIN_PROG)

$(MAIN_PROG): $(MAIN_OBJECT) $(LIB_OBJECTS_C) $(LIB_OBJECTS_CXX)
	$(CXX) $(MAIN_OBJECT) $(LIB_OBJECTS_C) $(LIB_OBJECTS_CXX) -o $(MAIN_PROG)

$(LIB_OBJECTS_C):
	$(CC) $(CCFLAGS) -c $(LIB_SOURCES_C)
	mv $(subst objects/,,$(LIB_OBJECTS_C)) objects

$(LIB_OBJECTS_CXX):
	$(CXX) $(CXXFLAGS) -c $(LIB_SOURCES_CXX)
	mv $(subst objects/,,$(LIB_OBJECTS_CXX)) objects

clean:
	rm -rvf $(LIB_OBJECTS_C) $(LIB_OBJECTS_CXX) $(MAIN_OBJECT) $(MAIN_PROG)