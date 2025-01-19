CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -g

SRCS = main.cpp bwt.cpp utils.cpp

OBJS = $(SRCS:.cpp=.o)

EXEC = bwt

all: $(EXEC)

$(EXEC): $(OBJS)
	$(CXX) $(CXXFLAGS) $(OBJS) -o $(EXEC)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(EXEC)

rebuild: clean all
