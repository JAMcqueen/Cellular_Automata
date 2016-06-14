CXX = g++
CXXFLAGS = -std=c++0x
CXXFLAGS += -Wall
CXXFLAGS += -pedantic-errors
CXXFLAGS += -g

OBJS = Organism.o Ant.o DoodleBug.o OrganismGrid.o Final.o

SRCS = Organism.cpp Ant.cpp DoodleBug.cpp OrganismGrid.cpp Final.cpp

HEADERS = Organism.hpp Ant.hpp DoodleBug.hpp OrganismGrid.hpp

all: Final

Final: ${OBJS} ${HEADERS}
	${CXX} ${OBJS} -lncurses -o Final

${OBJS}: ${SRCS}
	${CXX} ${CXXFLAGS} -c $(@:.o=.cpp)
