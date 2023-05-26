MIDIFILE_INCLUDE_DIR = ./external/cragsapp/midifile/include
MIDIFILE_SOURCE_DIR = ./external/cragsapp/midifile/src

INCLUDE_DIR = ./include
SOURCE_DIR = ./src
OBJ_DIR = ./obj

SOURCE_FILES =  $(wildcard $(MIDIFILE_SOURCE_DIR)/*.cpp)
SOURCE_FILES += $(wildcard $(SOURCE_DIR)/*.cpp)
OBJ_FILES = $(addprefix ./obj/,$(notdir $(subst .cpp,.o,$(SOURCE_FILES)))) 

FLAGS =-g -Wall -I$(MIDIFILE_INCLUDE_DIR) -I$(INCLUDE_DIR) 
LD_FLAGS = -g

LIBS =-lavcodec -lavutil -lswscale


ejecutable: $(OBJ_FILES)
	g++ $(LD_FLAGS) -o $@ $^ $(LIBS)

./obj/%.o : ./src/%.cpp
	g++ -c -o $@ $^ $(FLAGS)

./obj/%.o : ./external/cragsapp/midifile/src/%.cpp
	g++ -c -o $@ $^ $(FLAGS)


info:
	$(info $(OBJ_FILES))
clean:
	rm $(OBJ_DIR)/*.o