#pragma once
#include <QMainWindow>
#include <QGraphicsScene>
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

private:
    Ui::MainWindow* ui;
    std::vector<Processo> processos_carregados;

    void exibirTimeline(const std::vector<EntradaTimeline>& timeline, int tempo_total);
    void exibirMetricas(const ResultadoSimulacao& resultado);
};