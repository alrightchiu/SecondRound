# first target: all, this targe will compile all the file

SRC_DIR = src

CC = g++
EXE_NAME = exe

DEPS += $(SRC_DIR)/hash.h 


OBJ += $(SRC_DIR)/main.o 
OBJ += $(SRC_DIR)/hash.o 

CFLAGS += -Wall


# -c: generate object files
# -o $@: put putout of the ciompilation in the file named on the left side of the ":"
# $<: the first item in the dependencies list 
%.o: %.cpp $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS) 

# $@: the file name of the target
# $^: the name of all the prereqruisites(duplicate prerequisites are omitted)
$(EXE_NAME): $(OBJ)
	$(CC) -o $@ $^ $(LDFLAGS)

clean:
	rm -rf $(SRC_DIR)/*.o *.o $(EXE_NAME)   # rm: remove command 
	clear                    # clear console
