#include "MainWindow.h"
#include "ui_MainWindow.h"
#include <QFileDialog>
#include <QMessageBox>
#include <QGraphicsRectItem>
#include <QGraphicsTextItem>

// cores para cada processo na timeline
static const QList<QColor> CORES = {
    QColor("#C0D8F0"), QColor("#C0E8D4"),
    QColor("#FAE0C0"), QColor("#F5CEC0"), QColor("#E0CBFA")
};

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // conecta os botões aos slots
    connect(ui->btnAbrirCSV, &QPushButton::clicked,
            this, &MainWindow::ao_clicar_abrir_csv);
    connect(ui->btnSimular,  &QPushButton::clicked,
            this, &MainWindow::ao_clicar_simular);
}

MainWindow::~MainWindow() { delete ui; }

void MainWindow::ao_clicar_abrir_csv() {
    QString caminho = QFileDialog::getOpenFileName(
        this, "Abrir arquivo de processos", "", "CSV (*.csv)");
    if (caminho.isEmpty()) return;

    processos_carregados = LeitorCSV::ler(caminho.toStdString());
    ui->labelArquivo->setText(
        QString("%1 processo(s) carregado(s)").arg(processos_carregados.size()));
}

void MainWindow::ao_clicar_simular() {
    if (processos_carregados.empty()) {
        QMessageBox::warning(this, "Aviso", "Carregue um arquivo CSV primeiro!");
        return;
    }

    // lê as configurações da interface
    int quantum        = ui->spinQuantum->value();
    int mem_fisica     = ui->spinMemFisica->value();
    int tam_pagina     = ui->spinTamPagina->value();

    PoliticaEscalonamento pol_esc;
    switch (ui->comboEscalonamento->currentIndex()) {
        case 0: pol_esc = PoliticaEscalonamento::ROUND_ROBIN;           break;
        case 1: pol_esc = PoliticaEscalonamento::SJF_PREEMPTIVO;        break;
        default:pol_esc = PoliticaEscalonamento::PRIORIDADE_PREEMPTIVA; break;
    }

    PoliticaSubstituicao pol_mem;
    switch (ui->comboSubstituicao->currentIndex()) {
        case 0: pol_mem = PoliticaSubstituicao::FIFO;  break;
        case 1: pol_mem = PoliticaSubstituicao::LRU;   break;
        default:pol_mem = PoliticaSubstituicao::OTIMO; break;
    }

    // roda a simulação
    Simulador sim(pol_esc, quantum, mem_fisica, tam_pagina, pol_mem);
    auto resultado = sim.executar(processos_carregados);

    // calcula o tempo total para escalar a timeline
    int tempo_total = 0;
    for (auto& e : resultado.timeline)
        tempo_total = std::max(tempo_total, e.tempo_fim);

    exibirTimeline(resultado.timeline, tempo_total);
    exibirMetricas(resultado);
}

void MainWindow::exibirTimeline(const std::vector<EntradaTimeline>& timeline,
                                int tempo_total)
{
    QGraphicsScene* cena = new QGraphicsScene(this);
    ui->graphicsTimeline->setScene(cena);

    const int ALTURA_LINHA = 28;
    const int LARGURA_MAX  = ui->graphicsTimeline->width() - 60;
    const double ESCALA    = (double)LARGURA_MAX / tempo_total;
    const int OFFSET_Y     = 10;
    const int OFFSET_X     = 30; // espaço para o label do PID

    // descobre quais PIDs existem para indexar as linhas
    std::map<int, int> linha_do_pid;
    int linha_atual = 0;
    for (auto& e : timeline) {
        if (linha_do_pid.find(e.pid) == linha_do_pid.end())
            linha_do_pid[e.pid] = linha_atual++;
    }

    for (auto& entrada : timeline) {
        int linha = linha_do_pid[entrada.pid];
        double x  = OFFSET_X + entrada.tempo_inicio * ESCALA;
        double y  = OFFSET_Y + linha * (ALTURA_LINHA + 4);
        double w  = (entrada.tempo_fim - entrada.tempo_inicio) * ESCALA - 2;

        QColor cor = CORES[(entrada.pid - 1) % CORES.size()];

        // bloco colorido
        QGraphicsRectItem* bloco = cena->addRect(x, y, w, ALTURA_LINHA);
        bloco->setBrush(cor);
        bloco->setPen(QPen(cor.darker(130), 0.5));

        // label do PID na primeira ocorrência da linha
        if (entrada.tempo_inicio == 0 ||
            entrada.tempo_inicio == timeline[0].tempo_inicio)
        {
            cena->addText(QString("P%1").arg(entrada.pid))
                ->setPos(0, y + 4);
        }
    }

    // adiciona label do PID para cada linha
    for (auto& [pid, linha] : linha_do_pid) {
        QGraphicsTextItem* t = cena->addText(QString("P%1").arg(pid));
        t->setPos(0, OFFSET_Y + linha * (ALTURA_LINHA + 4) + 4);
        t->setDefaultTextColor(QColor("#666"));
    }
}

void MainWindow::exibirMetricas(const ResultadoSimulacao& resultado) {
    ui->labelEspera->setText(
        QString::number(resultado.media_tempo_espera, 'f', 1) + " u.t.");
    ui->labelResposta->setText(
        QString::number(resultado.media_tempo_resposta, 'f', 1) + " u.t.");
    ui->labelPageFaults->setText(
        QString::number(resultado.total_page_faults));
}