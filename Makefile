# Compiler
CXX = g++
 
# Compiler flags
CXXFLAGS = -Wall -Wextra -std=c++17 `pkg-config --cflags Qt5Widgets`
 
# Linker flags
LDFLAGS = `pkg-config --libs Qt5Widgets`
 
# Source files
SRCS = main.cpp mainwindow.cpp
 
# Header files that need moc
HDRS = mainwindow.h
 
# Moc files
MOCS = $(HDRS:.h=.moc.cpp)
 
# Object files
OBJS = $(SRCS:.cpp=.o) $(MOCS:.cpp=.o)
 
# Executable name
TARGET = my_program
 
# Default target
all: $(TARGET)
 
# Link the executable
$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJS) $(LDFLAGS)
 
# Compile source files into object files
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@
 
# Generate moc files
%.moc.cpp: %.h
	moc $< -o $@
 
# Clean up build files
clean:
	rm -f $(OBJS) $(MOCS) $(TARGET)
 
# Phony targets
.PHONY: all clean
