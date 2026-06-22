CXX      = g++
CXXFLAGS = -std=c++17 -Wall -O2

# ===== Configuracao do Qt =====
# Aqui eu deixei o caminho do meu Qt (MinGW). Se mudar de maquina,
# normalmente so precisa ajustar esse valor.
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

# ===== Arquivos do nucleo da simulacao =====
# Esses arquivos sao usados tanto no terminal quanto na GUI.
SRCS_CORE = src/CSVParser.cpp \
            src/Scheduler.cpp \
            src/Memory.cpp \
            src/Simulator.cpp

# ===== Modo terminal (sem Qt) =====
# Compila o executavel de linha de comando.
terminal: src/main.cpp $(SRCS_CORE)
	$(CXX) $(CXXFLAGS) src/main.cpp $(SRCS_CORE) -o Projeto_SO

# Atalho para compilar e executar um CSV de exemplo.
run: terminal
	./Projeto_SO examples/processes.csv

# ===== Execucao da GUI =====
# Roda a interface sem precisar alterar o PATH global do Windows.
run-gui:
	PATH="$(QT_DIR)/bin:$$PATH" ./Projeto_SO_GUI.exe

# ===== Build da interface grafica =====
gui: gui/ui_MainWindow.h gui/moc_MainWindow.cpp
	$(CXX) $(CXXFLAGS) \
		gui/main.cpp \
		gui/MainWindow.cpp \
		gui/moc_MainWindow.cpp \
		$(SRCS_CORE) \
		$(QT_FLAGS) \
		-o Projeto_SO_GUI

# Gera o header da UI a partir do arquivo .ui (feito pelo Qt Designer).
gui/ui_MainWindow.h: gui/MainWindow.ui
	$(QT_DIR)/bin/uic gui/MainWindow.ui -o gui/ui_MainWindow.h

# Gera o moc para sinais/slots da classe MainWindow.
gui/moc_MainWindow.cpp: gui/MainWindow.h
	$(QT_DIR)/bin/moc gui/MainWindow.h -o gui/moc_MainWindow.cpp

# Compila tudo (terminal + GUI).
all: terminal gui

# Limpa os binarios e arquivos gerados automaticamente.
clean:
	rm -f Projeto_SO Projeto_SO_GUI
	rm -f gui/ui_MainWindow.h gui/moc_MainWindow.cpp

.PHONY: terminal run gui all clean