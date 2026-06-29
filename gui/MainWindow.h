#pragma once
#include <QMainWindow>
#include <QGraphicsScene>
#include <vector>
#include "../src/Simulator.h"

namespace Ui { class MainWindow; }

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget* parent = nullptr);
    ~MainWindow();

private slots:
    void ao_clicar_abrir_csv();   // botão "Abrir CSV"
    void ao_clicar_simular();     // botão "Simular"
    void ao_mudar_algoritmo(int indice);

private:
    Ui::MainWindow* ui;
    std::vector<Processo> processos_carregados;

    void aplicarEstilo();
    void desenharTimeline(const std::vector<EntradaTimeline>& timeline, int tempo_total);
    void preencherTabela(const std::vector<Processo>& processos);
    void exibirMemoria(const ResultadoSimulacao& resultado);
    void exibirMetricas(const ResultadoSimulacao& resultado);
};