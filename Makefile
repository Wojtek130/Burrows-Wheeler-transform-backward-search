# Compiler and Flags
CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -g

# Source files
SRCS = main.cpp bwt.cpp utils.cpp

# Object files
OBJS = $(SRCS:.cpp=.o)

# Output executable
EXEC = bwt

# Default target (build the executable)
all: $(EXEC)

# Link object files to create the executable
$(EXEC): $(OBJS)
	$(CXX) $(CXXFLAGS) $(OBJS) -o $(EXEC)

# Compile .cpp files into .o files
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean up build artifacts
clean:
	rm -f $(OBJS) $(EXEC)

# Rebuild everything (clean and all)
rebuild: clean all
