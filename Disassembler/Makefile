bold := $(shell tput bold)
NC := $(shell tput sgr0)
red := $(shell tput setaf 1)
green := $(shell tput setaf 2)
yellow := $(shell tput setaf 3)
CC:=g++

HEADERS_DIR:= ./headers/
SRC_DIR:= ./src/
LIBS:= 
OBJECTS_DIR:= Objs/

OUTPUT:= rv32dis.out
OUTPUT_DIR:= ./Build/


SOURCES:= $(shell find $(SRC_DIR) -name  '*.cpp')
HEADERS_DIRS:=$(sort $(dir $(shell find $(HEADERS_DIR) -name '*.h')))

INC= $(addprefix -I, $(HEADERS_DIRS))
CCFLAGS= $(INC) -std=c++17 -g
OBJECTS:= $(patsubst %.cpp, %.o, $(subst ./src/, ./Objs/, $(SOURCES)))

VPATH_SRC:=$(sort $(dir $(VPATH_SRC)))

CREATE_DIRS:= $(OUTPUT_DIR) $(sort $(dir $(OBJECTS)))

vpath %.h $(HEADERS_DIR)
vpath %.cpp $(VPATH_SRC)

all: | prep
	@printf '$(red)Compiling and Linking the created objects\n$(NC)'
	@$(MAKE) -s $(OUTPUT_DIR)$(OUTPUT)
	@printf '$(green)Created binary file $(OUTPUT_DIR)$(OUTPUT)\n\n$(NC)'

$(OUTPUT_DIR)$(OUTPUT): $(OBJECTS)
	@$(CC) $(CCFLAGS) $(OBJECTS) -o $(OUTPUT_DIR)$(OUTPUT)


prep:
	@mkdir -p $(CREATE_DIRS)

$(OBJECTS): $(OBJECTS_DIR)%.o : $(SRC_DIR)%.cpp	
	@printf '$(bold)========== Compiling $(red)$(notdir $^)$(NC)$(bold) ======\n$(NC)'
	@$(CC) -c $(CCFLAGS) $< -o $@
	@printf '$(green)========== Compiled Successfully ======\n$(NC)'

.PHONY: clean

clean:
	@$(RM) $(OBJECTS)
	@$(RM) $(OUTPUT_DIR)$(OUTPUT)
	@printf 'Successfully Cleaned\n'
	

print:
	@for obj in $(CREATE_DIRS); do\
		echo $$obj;\
	done