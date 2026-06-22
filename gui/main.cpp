#include <QApplication>
#include "MainWindow.h"

// Ponto de entrada da versao grafica.
// Aqui eu so inicializo o QApplication, crio a janela principal
// e deixo o loop de eventos do Qt rodando.
int main(int argc, char** argv) {
    QApplication app(argc, argv);

    // Janela principal da aplicacao.
    MainWindow w;
    w.show();

    // Mantem a interface viva ate o usuario fechar.
    return app.exec();
}
