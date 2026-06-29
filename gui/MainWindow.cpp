#include "MainWindow.h"
#include "ui_MainWindow.h"
#include "../src/CSVParser.h"
#include <QFileDialog>
#include <QFileInfo>
#include <QMessageBox>
#include <QGraphicsRectItem>
#include <QGraphicsTextItem>
#include <QGraphicsLineItem>
#include <QHeaderView>
#include <QTableWidgetItem>
#include <algorithm>
#include <set>

// cores idênticas ao mockup HTML, uma por processo
static const QList<QPair<QColor,QColor>> CORES = {
    { QColor("#C0D8F0"), QColor("#185FA5") },  // P1 azul
    { QColor("#C0E8D4"), QColor("#0F6E56") },  // P2 verde
    { QColor("#FAE0C0"), QColor("#854F0B") },  // P3 âmbar
    { QColor("#F5CEC0"), QColor("#993C1D") },  // P4 coral
    { QColor("#E0CBFA"), QColor("#534AB7") },  // P5 roxo
};

// ── construtor ───────────────────────────────────────────────────────────────
MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    aplicarEstilo();

    // conecta botões
    connect(ui->btnAbrirCSV,  &QPushButton::clicked,
            this, &MainWindow::ao_clicar_abrir_csv);
    connect(ui->btnSimular,   &QPushButton::clicked,
            this, &MainWindow::ao_clicar_simular);
    connect(ui->comboEscalonamento, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &MainWindow::ao_mudar_algoritmo);

    // começa com quantum visível (Round Robin é o padrão)
    ao_mudar_algoritmo(0);
}

MainWindow::~MainWindow() { delete ui; }

// ── estilo visual idêntico ao mockup ─────────────────────────────────────────
void MainWindow::aplicarEstilo() {
    setStyleSheet(R"(
        QMainWindow, QWidget {
            background-color: #ffffff;
            font-family: 'Segoe UI', Arial, sans-serif;
            font-size: 13px;
            color: #3d3d3a;
        }

        /* painel esquerdo com borda direita */
        #painelEsq {
            background-color: #f8f8f6;
            border-right: 1px solid #e8e7e2;
        }

        /* labels de seção em maiúsculo e cor cinza */
        #secaoArquivo, #secaoEscalonamento, #secaoMemoria {
            font-size: 11px;
            font-weight: 600;
            color: #888780;
            letter-spacing: 0.06em;
        }

        /* labels menores */
        QLabel {
            font-size: 12px;
            color: #5f5e5a;
        }

        /* botão abrir CSV - estilo pontilhado igual ao mockup */
        #btnAbrirCSV {
            background-color: #f1efe8;
            border: 1px dashed #b4b2a9;
            border-radius: 8px;
            color: #888780;
            font-size: 12px;
            padding: 8px;
            text-align: center;
        }
        #btnAbrirCSV:hover {
            border-color: #185FA5;
            color: #185FA5;
            background-color: #e6f1fb;
        }

        /* label do arquivo carregado */
        #labelArquivo {
            font-size: 11px;
            color: #888780;
            text-align: center;
        }

        /* combos e spinboxes */
        QComboBox, QSpinBox {
            background-color: #ffffff;
            border: 0.5px solid #d3d1c7;
            border-radius: 6px;
            padding: 4px 8px;
            font-size: 12px;
            min-height: 28px;
        }
        QComboBox:focus, QSpinBox:focus {
            border-color: #185FA5;
        }
        QComboBox::drop-down {
            border: none;
            width: 20px;
        }

        /* botão simular - azul igual ao mockup */
        #btnSimular {
            background-color: #185FA5;
            color: #ffffff;
            border: none;
            border-radius: 8px;
            font-size: 13px;
            font-weight: 600;
            padding: 8px;
            min-height: 36px;
        }
        #btnSimular:hover { background-color: #0C447C; }
        #btnSimular:disabled {
            background-color: #b4b2a9;
            color: #f1efe8;
        }

        /* abas */
        QTabWidget::pane {
            border: 0.5px solid #e8e7e2;
            border-radius: 8px;
            background: #ffffff;
        }
        QTabBar::tab {
            font-size: 12px;
            color: #888780;
            padding: 6px 14px;
            background: transparent;
            border: none;
            border-bottom: 2px solid transparent;
        }
        QTabBar::tab:selected {
            color: #185FA5;
            border-bottom: 2px solid #185FA5;
            font-weight: 600;
        }
        QTabBar::tab:hover { color: #3d3d3a; }

        /* área da timeline */
        #graphicsTimeline {
            background-color: #f8f8f6;
            border: 0.5px solid #e8e7e2;
            border-radius: 8px;
        }

        /* tabela de processos */
        QTableWidget {
            border: 0.5px solid #e8e7e2;
            border-radius: 8px;
            gridline-color: #f1efe8;
            font-size: 12px;
        }
        QHeaderView::section {
            background-color: #f8f8f6;
            border: none;
            border-bottom: 1px solid #e8e7e2;
            padding: 6px 8px;
            font-size: 11px;
            font-weight: 600;
            color: #888780;
        }
        QTableWidget::item { padding: 4px 8px; }
        QTableWidget::item:alternate { background-color: #fafaf8; }

        /* cards de métricas */
        #cardEspera, #cardResposta, #cardFaults {
            background-color: #f8f8f6;
            border: 0.5px solid #e8e7e2;
            border-radius: 8px;
        }
        #labelEsperaDesc, #labelRespostaDesc, #labelFaultsDesc {
            font-size: 11px;
            color: #888780;
        }
        #labelEspera, #labelResposta, #labelPageFaults {
            font-size: 22px;
            font-weight: 600;
            color: #3d3d3a;
        }

        /* área de texto da memória */
        QTextEdit {
            font-family: 'Consolas', 'Courier New', monospace;
            font-size: 12px;
            border: 0.5px solid #e8e7e2;
            border-radius: 8px;
            background-color: #f8f8f6;
        }
    )");
}

// ── slot: esconde/mostra quantum conforme o algoritmo ─────────────────────────
void MainWindow::ao_mudar_algoritmo(int indice) {
    bool ehRR = (indice == 0);
    ui->spinQuantum->setVisible(ehRR);
    ui->labelQuantum->setVisible(ehRR);
}

// ── slot: abre o CSV ──────────────────────────────────────────────────────────
void MainWindow::ao_clicar_abrir_csv() {
    QString caminho = QFileDialog::getOpenFileName(
        this, "Abrir lista de processos", "", "Arquivos CSV (*.csv)");

    if (caminho.isEmpty()) return;

    processos_carregados = LeitorCSV::ler(caminho.toStdString());

    if (processos_carregados.empty()) {
        QMessageBox::warning(this, "Erro", "Nenhum processo encontrado no arquivo.");
        return;
    }

    // extrai só o nome do arquivo (sem o caminho completo)
    QString nomeArquivo = QFileInfo(caminho).fileName();
    ui->labelArquivo->setText(
        QString("✓  %1  (%2 processos)").arg(nomeArquivo).arg(processos_carregados.size())
    );
    ui->labelArquivo->setStyleSheet("color: #0F6E56; font-weight: 600;");
    ui->btnSimular->setEnabled(true);
}

// ── slot: roda a simulação ────────────────────────────────────────────────────
void MainWindow::ao_clicar_simular() {
    if (processos_carregados.empty()) return;

    PoliticaEscalonamento pol_esc;
    switch (ui->comboEscalonamento->currentIndex()) {
        case 0:  pol_esc = PoliticaEscalonamento::ROUND_ROBIN;           break;
        case 1:  pol_esc = PoliticaEscalonamento::SJF_PREEMPTIVO;        break;
        default: pol_esc = PoliticaEscalonamento::PRIORIDADE_PREEMPTIVA; break;
    }

    PoliticaSubstituicao pol_mem;
    switch (ui->comboSubstituicao->currentIndex()) {
        case 0:  pol_mem = PoliticaSubstituicao::FIFO;  break;
        case 1:  pol_mem = PoliticaSubstituicao::LRU;   break;
        default: pol_mem = PoliticaSubstituicao::OTIMO; break;
    }

    int quantum    = ui->spinQuantum->value();
    int mem_fisica = ui->spinMemFisica->value();
    int tam_pagina = ui->spinTamPagina->value();

    Simulador sim(pol_esc, quantum, mem_fisica, tam_pagina, pol_mem);
    ResultadoSimulacao resultado = sim.executar(processos_carregados);

    int tempo_total = 0;
    for (auto& e : resultado.timeline)
        tempo_total = std::max(tempo_total, e.tempo_fim);

    // atualiza nome da aba com o algoritmo e parâmetros usados
    QString nomeAlgo = ui->comboEscalonamento->currentText();
    if (pol_esc == PoliticaEscalonamento::ROUND_ROBIN)
        nomeAlgo += QString(" (q=%1)").arg(quantum);
    ui->labelInfoTimeline->setText(
        nomeAlgo + " · " + ui->comboSubstituicao->currentText()
    );

    desenharTimeline(resultado.timeline, tempo_total);
    preencherTabela(resultado.lista_processos);
    exibirMemoria(resultado);
    exibirMetricas(resultado);

    // vai para a aba de timeline automaticamente
    ui->tabWidget->setCurrentIndex(0);
}

// ── desenha a timeline no QGraphicsView ──────────────────────────────────────
void MainWindow::desenharTimeline(const std::vector<EntradaTimeline>& timeline,
                                   int tempo_total)
{
    QGraphicsScene* cena = new QGraphicsScene(this);
    ui->graphicsTimeline->setScene(cena);
    ui->graphicsTimeline->setRenderHint(QPainter::Antialiasing);

    const int    ALTURA   = 24;
    const int    ESPACO   = 6;
    const int    OFF_X    = 32;  // margem para label PID
    const int    OFF_Y    = 8;
    const double W_DISP   = ui->graphicsTimeline->width() - OFF_X - 16;
    const double ESCALA   = tempo_total > 0 ? W_DISP / tempo_total : 1.0;

    // descobre a linha de cada PID na ordem de aparição
    std::map<int,int> linha_pid;
    int prox = 0;
    for (auto& e : timeline)
        if (linha_pid.find(e.pid) == linha_pid.end())
            linha_pid[e.pid] = prox++;

    // blocos coloridos
    for (auto& e : timeline) {
        int    li  = linha_pid[e.pid];
        double x   = OFF_X + e.tempo_inicio * ESCALA;
        double y   = OFF_Y + li * (ALTURA + ESPACO);
        double w   = (e.tempo_fim - e.tempo_inicio) * ESCALA - 1.0;

        auto [cor_fundo, cor_texto] = CORES[(e.pid - 1) % CORES.size()];

        QGraphicsRectItem* bloco = cena->addRect(x, y, w, ALTURA);
        bloco->setBrush(cor_fundo);
        bloco->setPen(QPen(cor_texto.lighter(160), 0.5));
        bloco->setToolTip(QString("P%1: %2 → %3").arg(e.pid).arg(e.tempo_inicio).arg(e.tempo_fim));

        // texto dentro do bloco se tiver espaço
        if (w > 28) {
            QString txt = QString("%1→%2").arg(e.tempo_inicio).arg(e.tempo_fim);
            QGraphicsTextItem* t = cena->addText(txt);
            t->setDefaultTextColor(cor_texto);
            t->setFont(QFont("Segoe UI", 8));
            // centraliza verticalmente dentro do bloco
            t->setPos(x + 3, y + (ALTURA - 14) / 2.0);
        }
    }

    // labels PID na margem esquerda
    for (auto& [pid, li] : linha_pid) {
        double y = OFF_Y + li * (ALTURA + ESPACO) + (ALTURA / 2.0) - 7;
        QGraphicsTextItem* label = cena->addText(QString("P%1").arg(pid));
        label->setDefaultTextColor(QColor("#888780"));
        label->setFont(QFont("Segoe UI", 9, QFont::Bold));
        label->setPos(0, y);
    }

    // eixo de tempo no rodapé
    int n     = linha_pid.size();
    double yE = OFF_Y + n * (ALTURA + ESPACO) + 4;

    // linha do eixo
    cena->addLine(OFF_X, yE, OFF_X + W_DISP, yE,
                  QPen(QColor("#d3d1c7"), 0.5));

    // ticks e números
    int passo = std::max(1, tempo_total / 12);
    for (int t = 0; t <= tempo_total; t += passo) {
        double x = OFF_X + t * ESCALA;
        cena->addLine(x, yE, x, yE + 4, QPen(QColor("#b4b2a9"), 0.5));
        QGraphicsTextItem* num = cena->addText(QString::number(t));
        num->setDefaultTextColor(QColor("#888780"));
        num->setFont(QFont("Segoe UI", 8));
        num->setPos(x - 4, yE + 5);
    }
    // garante que o último tick aparece
    double xFim = OFF_X + tempo_total * ESCALA;
    cena->addLine(xFim, yE, xFim, yE + 4, QPen(QColor("#b4b2a9"), 0.5));
    QGraphicsTextItem* numFim = cena->addText(QString::number(tempo_total));
    numFim->setDefaultTextColor(QColor("#888780"));
    numFim->setFont(QFont("Segoe UI", 8));
    numFim->setPos(xFim - 4, yE + 5);

    ui->graphicsTimeline->setSceneRect(cena->itemsBoundingRect().adjusted(-4,-4,4,12));
}

// ── preenche a tabela de processos ───────────────────────────────────────────
void MainWindow::preencherTabela(const std::vector<Processo>& processos) {
    QTableWidget* tb = ui->tabelaProcessos;
    tb->setRowCount(processos.size());
    tb->setAlternatingRowColors(true);
    tb->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    tb->verticalHeader()->setVisible(false);
    tb->setSelectionBehavior(QAbstractItemView::SelectRows);
    tb->setEditTriggers(QAbstractItemView::NoEditTriggers);

    for (int i = 0; i < (int)processos.size(); i++) {
        const Processo& p = processos[i];
        auto cell = [&](int col, QString txt, bool negrito = false) {
            QTableWidgetItem* item = new QTableWidgetItem(txt);
            item->setTextAlignment(Qt::AlignCenter);
            if (negrito) {
                QFont f = item->font();
                f.setBold(true);
                item->setFont(f);
            }
            // colore o PID com a cor do processo
            if (col == 0) {
                auto [cor_fundo, cor_txt] = CORES[i % CORES.size()];
                item->setBackground(cor_fundo);
                item->setForeground(cor_txt);
            }
            tb->setItem(i, col, item);
        };

        cell(0, QString("P%1").arg(p.pid), true);
        cell(1, QString::number(p.tempo_chegada));
        cell(2, QString::number(p.tempo_execucao));
        cell(3, QString::number(p.tempo_inicio));
        cell(4, QString::number(p.tempo_fim));
        cell(5, QString::number(p.tempo_espera));
        cell(6, QString::number(p.tempo_resposta));
    }
}

// ── exibe log de memória na aba de memória ────────────────────────────────────
void MainWindow::exibirMemoria(const ResultadoSimulacao& resultado) {
    QString texto;
    const int frames_total = std::max(1, ui->spinMemFisica->value() / ui->spinTamPagina->value());

    auto nomeProcesso = [](const std::string& pagina_nome) -> std::string {
        const size_t pos = pagina_nome.find('p');
        return pos == std::string::npos ? pagina_nome : pagina_nome.substr(0, pos);
    };

    auto nomePagina = [](const std::string& pagina_nome) -> std::string {
        const size_t pos = pagina_nome.find('p');
        return pos == std::string::npos ? "" : pagina_nome.substr(pos + 1);
    };

    auto formatarEstadoMemoria = [&](const std::vector<std::string>& estado) {
        QStringList partes;
        for (const std::string& slot : estado) {
            QString valor = slot == "__" ? "_" : QString::fromStdString(slot);
            partes << valor;
        }
        return QString("[%1]").arg(partes.join(", "));
    };

    if (resultado.historico_memoria.empty()) {
        texto = "Nenhum evento de memória registrado.\n";
        ui->textoMemoria->setPlainText(texto);
        ui->tabWidget->setTabText(2, QString("Memória  [%1 faults]").arg(resultado.total_page_faults));
        return;
    }

    std::vector<std::vector<EventoMemoria>> grupos;
    for (const EventoMemoria& evento : resultado.historico_memoria) {
        if (grupos.empty() || grupos.back().back().tempo != evento.tempo ||
            nomeProcesso(grupos.back().back().pagina_nome) != nomeProcesso(evento.pagina_nome)) {
            grupos.push_back({evento});
        } else {
            grupos.back().push_back(evento);
        }
    }

    std::set<std::string> processos_vistos;
    for (const std::vector<EventoMemoria>& grupo : grupos) {
        const EventoMemoria& ultimo = grupo.back();
        const std::string processo = nomeProcesso(ultimo.pagina_nome);
        const bool processo_novo = processos_vistos.insert(processo).second;

        QString linha;
        if (processo_novo) {
            linha = QString("t=%1s: %2 chega")
                .arg(ultimo.tempo)
                .arg(QString::fromStdString(processo));
        } else {
            linha = QString("t=%1s: %2 precisa p%3")
                .arg(ultimo.tempo)
                .arg(QString::fromStdString(processo))
                .arg(QString::fromStdString(nomePagina(ultimo.pagina_nome)));
        }

        const int paginas_ocupadas = std::count_if(
            ultimo.estado_memoria_nomes.begin(),
            ultimo.estado_memoria_nomes.end(),
            [](const std::string& slot) {
                return slot != "_" && slot != "__";
            });

        linha += QString(" → Mem: %1 (%2/%3 páginas)")
            .arg(formatarEstadoMemoria(ultimo.estado_memoria_nomes))
            .arg(paginas_ocupadas)
            .arg(frames_total);

        if (ultimo.descricao.find("Remove") != std::string::npos) {
            std::string parte = ultimo.descricao;
            const size_t pos = parte.find(" → ");
            if (pos != std::string::npos) {
                parte = parte.substr(pos + 3);
            }
            linha += QString("\n    %1 precisa p%2 → %3")
                .arg(QString::fromStdString(processo))
                .arg(QString::fromStdString(nomePagina(ultimo.pagina_nome)))
                .arg(QString::fromStdString(parte));
        }

        linha += QString("\n+%1 page fault%2")
            .arg(static_cast<int>(grupo.size()))
            .arg(grupo.size() == 1 ? "" : "s");

        texto += linha + "\n\n";
    }

    texto += QString("PAGE FAULTS totais: %1").arg(resultado.total_page_faults);

    ui->textoMemoria->setPlainText(texto);

    // atualiza o título da aba com o número de faults
    ui->tabWidget->setTabText(2,
        QString("Memória  [%1 faults]").arg(resultado.total_page_faults));
}

// ── atualiza os cards de métricas ─────────────────────────────────────────────
void MainWindow::exibirMetricas(const ResultadoSimulacao& resultado) {
    ui->labelEspera->setText(
        QString::number(resultado.media_tempo_espera, 'f', 1) + " u.t.");
    ui->labelResposta->setText(
        QString::number(resultado.media_tempo_resposta, 'f', 1) + " u.t.");
    ui->labelPageFaults->setText(
        QString::number(resultado.total_page_faults));
}
