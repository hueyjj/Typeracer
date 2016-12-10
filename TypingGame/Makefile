COMPILE	= g++
FLAGS 	= -std=c++11
LINK_FLAGS = #-mwindows #no console pop up
DEBUG	= -g
DIR 	= $(dir $(lastword $(MAKEFILE_LIST)))
#HEADER_FILES = $(wildcard $(DIR)Header/*.h)
CPP_FILES = $(filter-out $(DIR)CPP/Test.cpp, $(wildcard $(DIR)CPP/*.cpp))
OBJ_FILES = $(filter-out $(DIR)CPP/Test.o, $(addprefix $(DIR),$(notdir $(CPP_FILES:.cpp=.o))))

SOURCES = $(CPP_FILES)  #Test.cpp TextProcessor.cpp
BUILDDIR = $(OBJ_FILES) #Test.o TextProcessor.o # $(CPP_FILES:.cpp=.o)
OBJECTS = $(patsubst $(DIR)/%.cpp,$(BUILDDIR)/%.o, $(DIR)*.o)
#HEADERS = $(HEADER_FILES)  #Window.h TextProcessor.h
EXEBIN  = Typeracer

all: $(EXEBIN)

$(EXEBIN) : $(OBJECTS) 
	$(COMPILE) $(LINK_FLAGS) -std=c++11 -o $(EXEBIN) $(OBJECTS)
	rm $(DIR)*.o

$(OBJECTS) : $(SOURCES) 
	$(COMPILE) -c $(FLAGS) $(SOURCES)

clean :
	@rm -f $(EXEBIN)
	@echo Removed $(EXEBIN).exe

check:
	valgrind --leak-check=full $(EXEBIN) 

debug:
	$(COMPILE) $(DEBUG) $(FLAGS) $(SOURCES) -o $(EXEBIN)
