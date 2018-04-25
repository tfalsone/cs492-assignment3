NAME=Assign3Exec
SRC=FileSystem.cpp Block.hh FileBlock.hh GTree.hh LDisk.hh LFile.hh Node.hh
SRCPATH=./

RM=rm -f
CXX      = c++
CXX_FILE = $(wildcard *.cpp)
TARGET   = $(patsubst %.cpp,%,$(CXX_FILE))
CXXFLAGS = -g -fmessage-length=0
INCPATH=include
CPPFLAGS+= -std=c++11 -I $(INCPATH)

all:
	$(CXX) $(CXXFLAGS) $(CXX_FILE) -o $(TARGET)
clean:
	rm -f $(TARGET) $(TARGET).exe
