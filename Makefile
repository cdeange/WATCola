CXX = u++
CXXFLAGS = -g -Wall -Wno-unused-label -MMD
MAKEFILE_NAME = ${firstword ${MAKEFILE_LIST}}

OBJECTS = 	main.o \
			bank.o \
			config.o \
			nameserver.o \
			parent.o \
			plant.o \
			printer.o \
			student.o \
			truck.o \
			vendingmachine.o \
			WATCard.o \
			WATCardOffice.o

EXEC = soda

DEPENDS = ${OBJECTS:.o=.d}

#############################################################

.PHONY : all clean

all : ${EXEC}

${EXEC} : ${OBJECTS}
	${CXX} ${CXXFLAGS} $^ -o $@

#############################################################

${OBJECTS} : ${MAKEFILE_NAME}

-include ${DEPENDS}				# include *.d files containing program dependences

clean :						# remove files that can be regenerated
	rm -f *.d *.o ${EXEC}
