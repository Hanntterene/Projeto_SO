CXX      = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -O2

# arquivos fonte (sem gui/ por enquanto)
SRCS = src/main.cpp \
       src/CSVParser.cpp \
       src/Scheduler.cpp \
       src/Memory.cpp \
       src/Simulator.cpp

TARGET = Projeto_SO

all: $(TARGET)

$(TARGET): $(SRCS)
	$(CXX) $(CXXFLAGS) $(SRCS) -o $(TARGET)

run: $(TARGET)
	./$(TARGET) examples/processes.csv

clean:
	rm -f $(TARGET) src/*.o

.PHONY: all run clean