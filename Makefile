CXX      = g++
CXXFLAGS = -std=c++17 -Wall -O2

QT_DIR   = C:/Qt/6.11.1/mingw_64

QT_INC   = -I$(QT_DIR)/include \
            -I$(QT_DIR)/include/QtCore \
            -I$(QT_DIR)/include/QtGui \
            -I$(QT_DIR)/include/QtWidgets

QT_LIBS  = -L$(QT_DIR)/lib \
            -lQt6Widgets \
            -lQt6Gui \
            -lQt6Core

QT_FLAGS = $(QT_INC) $(QT_LIBS)

SRCS_CORE = src/CSVParser.cpp \
            src/Scheduler.cpp \
            src/Memory.cpp \
            src/Simulator.cpp

# versão terminal (sem Qt)
terminal: src/main.cpp $(SRCS_CORE)
	$(CXX) $(CXXFLAGS) src/main.cpp $(SRCS_CORE) -o Projeto_SO

run: terminal
	./Projeto_SO examples/processes.csv

# roda a interface sem depender do PATH global do Qt
run-gui:
	PATH="$(QT_DIR)/bin:$$PATH" ./Projeto_SO_GUI.exe

# versão com interface Qt
gui: gui/ui_MainWindow.h gui/moc_MainWindow.cpp
	$(CXX) $(CXXFLAGS) \
		gui/main.cpp \
		gui/MainWindow.cpp \
		gui/moc_MainWindow.cpp \
		$(SRCS_CORE) \
		$(QT_FLAGS) \
		-o Projeto_SO_GUI

gui/ui_MainWindow.h: gui/MainWindow.ui
	$(QT_DIR)/bin/uic gui/MainWindow.ui -o gui/ui_MainWindow.h

gui/moc_MainWindow.cpp: gui/MainWindow.h
	$(QT_DIR)/bin/moc gui/MainWindow.h -o gui/moc_MainWindow.cpp

all: terminal gui

clean:
	rm -f Projeto_SO Projeto_SO_GUI
	rm -f gui/ui_MainWindow.h gui/moc_MainWindow.cpp

.PHONY: terminal run gui all clean