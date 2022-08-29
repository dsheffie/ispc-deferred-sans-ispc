OBJ = common.o  dynamic_c.o  main.o
CXX = mips-sde-elf-g++
EXE = deferred.mips
OPT = -O3 -flto -fpermissive -DBIG_ENDIAN=1
CXXFLAGS = -g $(OPT)
DEP = $(OBJ:.o=.d)

.PHONY: all clean

all: $(EXE)

$(EXE) : $(OBJ)
	$(CXX) $(CXXFLAGS) $(OBJ) $(LIBS) -o $(EXE) -Tidt.ld
	mips-sde-elf-objdump -dw $(EXE) &> $(EXE).disasm

%.o: %.cpp
	$(CXX) -MMD $(CXXFLAGS)  -mno-branch-likely -c $< 


-include $(DEP)

clean:
	rm -rf $(EXE) $(OBJ) $(DEP)
