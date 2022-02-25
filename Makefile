CC = g++
OPT = -O3
#OPT = -g
WARN = -Wall
CFLAGS = $(OPT) $(WARN) $(INC) $(LIB)

# Other directories
BUILD = ./build/
LIBS = ./libs/
SRC = ./src/
TEST = ./test/

# List all your .cc files here (source files, excluding header files)
SIM_SRC = ./src/main_sim.cpp ./libs/ArgumentWrapper/ArgumentWrapper.cpp ./libs/utils/utils.cpp ./libs/Cache/Cache.cpp 

# List corresponding compiled object files here (.o files)
SIM_OBJ = ./src/main_sim.o ./libs/ArgumentWrapper/ArgumentWrapper.o ./libs/utils/utils.o ./libs/Cache/Cache.o
 
#################################

# default rule

all: sim_cache
	@echo "my work is done here..."


# rule for making sim_cache

sim_cache: $(SIM_OBJ)
	$(CC) -o sim_cache $(CFLAGS) $(SIM_OBJ) -lm
	@if [ ! -d $(BUILD) ]; then mkdir $(BUILD); fi
	@mv $(SRC)*.o ./build/
	@mv $(LIBS)*/*.o ./build/
	@echo "-----------DONE WITH SIM_CACHE-----------"


# generic rule for converting any .cc file to any .o file
 
.cc.o:
	$(CC) $(CFLAGS)  -c $*.cc


# type "make clean" to remove all .o files plus the sim_cache binary

clean:
	rm -f $(BUILD)*.o sim_cache
	rm -f $(SRC)*.o sim_cache
	rm -f $(TEST)/*.o sim_cache
	rm -f $(LIBS)*/*.o
	rm -rf ./build


# type "make clobber" to remove all .o files (leaves sim_cache binary)

clobber:
	rm -f ./build/*.o


