/********************************************************************************
** Form generated from reading UI file 'MainWindow.ui'
**
** Created by: Qt User Interface Compiler version 6.11.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QFrame>
#include <QtWidgets/QGraphicsView>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QHBoxLayout *layoutPrincipal;
    QWidget *painelEsq;
    QVBoxLayout *layoutEsq;
    QLabel *secaoArquivo;
    QPushButton *btnAbrirCSV;
    QLabel *labelArquivo;
    QLabel *secaoEscalonamento;
    QLabel *labelAlgoritmo;
    QComboBox *comboEscalonamento;
    QLabel *labelQuantum;
    QSpinBox *spinQuantum;
    QLabel *secaoMemoria;
    QLabel *labelMemFisica;
    QSpinBox *spinMemFisica;
    QLabel *labelTamPagina;
    QSpinBox *spinTamPagina;
    QLabel *labelSubstituicao;
    QComboBox *comboSubstituicao;
    QSpacerItem *spacerV;
    QPushButton *btnSimular;
    QWidget *painelDir;
    QVBoxLayout *layoutDir;
    QTabWidget *tabWidget;
    QWidget *abaTimeline;
    QVBoxLayout *vboxLayout;
    QLabel *labelInfoTimeline;
    QGraphicsView *graphicsTimeline;
    QWidget *abaTabela;
    QVBoxLayout *vboxLayout1;
    QTableWidget *tabelaProcessos;
    QWidget *abaMemoria;
    QVBoxLayout *vboxLayout2;
    QTextEdit *textoMemoria;
    QHBoxLayout *layoutMetricas;
    QFrame *cardEspera;
    QVBoxLayout *vboxLayout3;
    QLabel *labelEsperaDesc;
    QLabel *labelEspera;
    QFrame *cardResposta;
    QVBoxLayout *vboxLayout4;
    QLabel *labelRespostaDesc;
    QLabel *labelResposta;
    QFrame *cardFaults;
    QVBoxLayout *vboxLayout5;
    QLabel *labelFaultsDesc;
    QLabel *labelPageFaults;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName("MainWindow");
        MainWindow->resize(960, 620);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName("centralwidget");
        layoutPrincipal = new QHBoxLayout(centralwidget);
        layoutPrincipal->setSpacing(0);
        layoutPrincipal->setObjectName("layoutPrincipal");
        layoutPrincipal->setContentsMargins(0);
        painelEsq = new QWidget(centralwidget);
        painelEsq->setObjectName("painelEsq");
        painelEsq->setMinimumWidth(220);
        painelEsq->setMaximumWidth(220);
        layoutEsq = new QVBoxLayout(painelEsq);
        layoutEsq->setSpacing(8);
        layoutEsq->setObjectName("layoutEsq");
        layoutEsq->setContentsMargins(14);
        layoutEsq->setContentsMargins(0, 0, 0, 0);
        secaoArquivo = new QLabel(painelEsq);
        secaoArquivo->setObjectName("secaoArquivo");

        layoutEsq->addWidget(secaoArquivo);

        btnAbrirCSV = new QPushButton(painelEsq);
        btnAbrirCSV->setObjectName("btnAbrirCSV");
        btnAbrirCSV->setMinimumHeight(52);

        layoutEsq->addWidget(btnAbrirCSV);

        labelArquivo = new QLabel(painelEsq);
        labelArquivo->setObjectName("labelArquivo");
        labelArquivo->setAlignment(Qt::AlignCenter);
        labelArquivo->setWordWrap(true);

        layoutEsq->addWidget(labelArquivo);

        secaoEscalonamento = new QLabel(painelEsq);
        secaoEscalonamento->setObjectName("secaoEscalonamento");

        layoutEsq->addWidget(secaoEscalonamento);

        labelAlgoritmo = new QLabel(painelEsq);
        labelAlgoritmo->setObjectName("labelAlgoritmo");

        layoutEsq->addWidget(labelAlgoritmo);

        comboEscalonamento = new QComboBox(painelEsq);
        comboEscalonamento->addItem(QString());
        comboEscalonamento->addItem(QString());
        comboEscalonamento->addItem(QString());
        comboEscalonamento->setObjectName("comboEscalonamento");

        layoutEsq->addWidget(comboEscalonamento);

        labelQuantum = new QLabel(painelEsq);
        labelQuantum->setObjectName("labelQuantum");

        layoutEsq->addWidget(labelQuantum);

        spinQuantum = new QSpinBox(painelEsq);
        spinQuantum->setObjectName("spinQuantum");
        spinQuantum->setMinimum(1);
        spinQuantum->setMaximum(20);
        spinQuantum->setValue(2);

        layoutEsq->addWidget(spinQuantum);

        secaoMemoria = new QLabel(painelEsq);
        secaoMemoria->setObjectName("secaoMemoria");

        layoutEsq->addWidget(secaoMemoria);

        labelMemFisica = new QLabel(painelEsq);
        labelMemFisica->setObjectName("labelMemFisica");

        layoutEsq->addWidget(labelMemFisica);

        spinMemFisica = new QSpinBox(painelEsq);
        spinMemFisica->setObjectName("spinMemFisica");
        spinMemFisica->setMinimum(64);
        spinMemFisica->setMaximum(4096);
        spinMemFisica->setSingleStep(64);
        spinMemFisica->setValue(256);

        layoutEsq->addWidget(spinMemFisica);

        labelTamPagina = new QLabel(painelEsq);
        labelTamPagina->setObjectName("labelTamPagina");

        layoutEsq->addWidget(labelTamPagina);

        spinTamPagina = new QSpinBox(painelEsq);
        spinTamPagina->setObjectName("spinTamPagina");
        spinTamPagina->setMinimum(4);
        spinTamPagina->setMaximum(256);
        spinTamPagina->setSingleStep(4);
        spinTamPagina->setValue(64);

        layoutEsq->addWidget(spinTamPagina);

        labelSubstituicao = new QLabel(painelEsq);
        labelSubstituicao->setObjectName("labelSubstituicao");

        layoutEsq->addWidget(labelSubstituicao);

        comboSubstituicao = new QComboBox(painelEsq);
        comboSubstituicao->addItem(QString());
        comboSubstituicao->addItem(QString());
        comboSubstituicao->addItem(QString());
        comboSubstituicao->setObjectName("comboSubstituicao");

        layoutEsq->addWidget(comboSubstituicao);

        spacerV = new QSpacerItem(20, 40, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Expanding);

        layoutEsq->addItem(spacerV);

        btnSimular = new QPushButton(painelEsq);
        btnSimular->setObjectName("btnSimular");
        btnSimular->setMinimumHeight(36);
        btnSimular->setEnabled(false);

        layoutEsq->addWidget(btnSimular);


        layoutPrincipal->addWidget(painelEsq);

        painelDir = new QWidget(centralwidget);
        painelDir->setObjectName("painelDir");
        layoutDir = new QVBoxLayout(painelDir);
        layoutDir->setSpacing(12);
        layoutDir->setObjectName("layoutDir");
        layoutDir->setContentsMargins(14);
        layoutDir->setContentsMargins(0, 0, 0, 0);
        tabWidget = new QTabWidget(painelDir);
        tabWidget->setObjectName("tabWidget");
        abaTimeline = new QWidget();
        abaTimeline->setObjectName("abaTimeline");
        vboxLayout = new QVBoxLayout(abaTimeline);
        vboxLayout->setObjectName("vboxLayout");
        vboxLayout->setContentsMargins(8);
        labelInfoTimeline = new QLabel(abaTimeline);
        labelInfoTimeline->setObjectName("labelInfoTimeline");

        vboxLayout->addWidget(labelInfoTimeline);

        graphicsTimeline = new QGraphicsView(abaTimeline);
        graphicsTimeline->setObjectName("graphicsTimeline");
        graphicsTimeline->setMinimumHeight(180);

        vboxLayout->addWidget(graphicsTimeline);

        tabWidget->addTab(abaTimeline, QString());
        abaTabela = new QWidget();
        abaTabela->setObjectName("abaTabela");
        vboxLayout1 = new QVBoxLayout(abaTabela);
        vboxLayout1->setObjectName("vboxLayout1");
        vboxLayout1->setContentsMargins(8);
        tabelaProcessos = new QTableWidget(abaTabela);
        if (tabelaProcessos->columnCount() < 7)
            tabelaProcessos->setColumnCount(7);
        QTableWidgetItem *__qtablewidgetitem = new QTableWidgetItem();
        tabelaProcessos->setHorizontalHeaderItem(0, __qtablewidgetitem);
        QTableWidgetItem *__qtablewidgetitem1 = new QTableWidgetItem();
        tabelaProcessos->setHorizontalHeaderItem(1, __qtablewidgetitem1);
        QTableWidgetItem *__qtablewidgetitem2 = new QTableWidgetItem();
        tabelaProcessos->setHorizontalHeaderItem(2, __qtablewidgetitem2);
        QTableWidgetItem *__qtablewidgetitem3 = new QTableWidgetItem();
        tabelaProcessos->setHorizontalHeaderItem(3, __qtablewidgetitem3);
        QTableWidgetItem *__qtablewidgetitem4 = new QTableWidgetItem();
        tabelaProcessos->setHorizontalHeaderItem(4, __qtablewidgetitem4);
        QTableWidgetItem *__qtablewidgetitem5 = new QTableWidgetItem();
        tabelaProcessos->setHorizontalHeaderItem(5, __qtablewidgetitem5);
        QTableWidgetItem *__qtablewidgetitem6 = new QTableWidgetItem();
        tabelaProcessos->setHorizontalHeaderItem(6, __qtablewidgetitem6);
        tabelaProcessos->setObjectName("tabelaProcessos");
        tabelaProcessos->setColumnCount(7);

        vboxLayout1->addWidget(tabelaProcessos);

        tabWidget->addTab(abaTabela, QString());
        abaMemoria = new QWidget();
        abaMemoria->setObjectName("abaMemoria");
        vboxLayout2 = new QVBoxLayout(abaMemoria);
        vboxLayout2->setObjectName("vboxLayout2");
        vboxLayout2->setContentsMargins(8);
        textoMemoria = new QTextEdit(abaMemoria);
        textoMemoria->setObjectName("textoMemoria");
        textoMemoria->setReadOnly(true);

        vboxLayout2->addWidget(textoMemoria);

        tabWidget->addTab(abaMemoria, QString());

        layoutDir->addWidget(tabWidget);

        layoutMetricas = new QHBoxLayout();
        layoutMetricas->setSpacing(8);
        layoutMetricas->setObjectName("layoutMetricas");
        cardEspera = new QFrame(painelDir);
        cardEspera->setObjectName("cardEspera");
        cardEspera->setFrameShape(QFrame::StyledPanel);
        vboxLayout3 = new QVBoxLayout(cardEspera);
        vboxLayout3->setObjectName("vboxLayout3");
        vboxLayout3->setContentsMargins(10);
        labelEsperaDesc = new QLabel(cardEspera);
        labelEsperaDesc->setObjectName("labelEsperaDesc");

        vboxLayout3->addWidget(labelEsperaDesc);

        labelEspera = new QLabel(cardEspera);
        labelEspera->setObjectName("labelEspera");

        vboxLayout3->addWidget(labelEspera);


        layoutMetricas->addWidget(cardEspera);

        cardResposta = new QFrame(painelDir);
        cardResposta->setObjectName("cardResposta");
        cardResposta->setFrameShape(QFrame::StyledPanel);
        vboxLayout4 = new QVBoxLayout(cardResposta);
        vboxLayout4->setObjectName("vboxLayout4");
        vboxLayout4->setContentsMargins(10);
        labelRespostaDesc = new QLabel(cardResposta);
        labelRespostaDesc->setObjectName("labelRespostaDesc");

        vboxLayout4->addWidget(labelRespostaDesc);

        labelResposta = new QLabel(cardResposta);
        labelResposta->setObjectName("labelResposta");

        vboxLayout4->addWidget(labelResposta);


        layoutMetricas->addWidget(cardResposta);

        cardFaults = new QFrame(painelDir);
        cardFaults->setObjectName("cardFaults");
        cardFaults->setFrameShape(QFrame::StyledPanel);
        vboxLayout5 = new QVBoxLayout(cardFaults);
        vboxLayout5->setObjectName("vboxLayout5");
        vboxLayout5->setContentsMargins(10);
        labelFaultsDesc = new QLabel(cardFaults);
        labelFaultsDesc->setObjectName("labelFaultsDesc");

        vboxLayout5->addWidget(labelFaultsDesc);

        labelPageFaults = new QLabel(cardFaults);
        labelPageFaults->setObjectName("labelPageFaults");

        vboxLayout5->addWidget(labelPageFaults);


        layoutMetricas->addWidget(cardFaults);


        layoutDir->addLayout(layoutMetricas);


        layoutPrincipal->addWidget(painelDir);

        MainWindow->setCentralWidget(centralwidget);

        retranslateUi(MainWindow);

        tabWidget->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "Simulador de SO", nullptr));
        secaoArquivo->setText(QCoreApplication::translate("MainWindow", "ARQUIVO", nullptr));
        btnAbrirCSV->setText(QCoreApplication::translate("MainWindow", "Abrir CSV...", nullptr));
        labelArquivo->setText(QCoreApplication::translate("MainWindow", "Nenhum arquivo carregado", nullptr));
        secaoEscalonamento->setText(QCoreApplication::translate("MainWindow", "ESCALONAMENTO", nullptr));
        labelAlgoritmo->setText(QCoreApplication::translate("MainWindow", "Algoritmo", nullptr));
        comboEscalonamento->setItemText(0, QCoreApplication::translate("MainWindow", "Round Robin", nullptr));
        comboEscalonamento->setItemText(1, QCoreApplication::translate("MainWindow", "SJF Preemptivo", nullptr));
        comboEscalonamento->setItemText(2, QCoreApplication::translate("MainWindow", "Prioridade Preemptiva", nullptr));

        labelQuantum->setText(QCoreApplication::translate("MainWindow", "Quantum (u.t.)", nullptr));
        secaoMemoria->setText(QCoreApplication::translate("MainWindow", "MEMORIA", nullptr));
        labelMemFisica->setText(QCoreApplication::translate("MainWindow", "Memoria fisica (MB)", nullptr));
        labelTamPagina->setText(QCoreApplication::translate("MainWindow", "Tamanho da pagina (MB)", nullptr));
        labelSubstituicao->setText(QCoreApplication::translate("MainWindow", "Substituicao de paginas", nullptr));
        comboSubstituicao->setItemText(0, QCoreApplication::translate("MainWindow", "FIFO", nullptr));
        comboSubstituicao->setItemText(1, QCoreApplication::translate("MainWindow", "LRU", nullptr));
        comboSubstituicao->setItemText(2, QCoreApplication::translate("MainWindow", "Otimo", nullptr));

        btnSimular->setText(QCoreApplication::translate("MainWindow", "Simular", nullptr));
        labelInfoTimeline->setText(QCoreApplication::translate("MainWindow", "Rode a simulacao para ver a timeline.", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(abaTimeline), QCoreApplication::translate("MainWindow", "Timeline", nullptr));
        QTableWidgetItem *___qtablewidgetitem = tabelaProcessos->horizontalHeaderItem(0);
        ___qtablewidgetitem->setText(QCoreApplication::translate("MainWindow", "PID", nullptr));
        QTableWidgetItem *___qtablewidgetitem1 = tabelaProcessos->horizontalHeaderItem(1);
        ___qtablewidgetitem1->setText(QCoreApplication::translate("MainWindow", "Chegada", nullptr));
        QTableWidgetItem *___qtablewidgetitem2 = tabelaProcessos->horizontalHeaderItem(2);
        ___qtablewidgetitem2->setText(QCoreApplication::translate("MainWindow", "Burst", nullptr));
        QTableWidgetItem *___qtablewidgetitem3 = tabelaProcessos->horizontalHeaderItem(3);
        ___qtablewidgetitem3->setText(QCoreApplication::translate("MainWindow", "Inicio", nullptr));
        QTableWidgetItem *___qtablewidgetitem4 = tabelaProcessos->horizontalHeaderItem(4);
        ___qtablewidgetitem4->setText(QCoreApplication::translate("MainWindow", "Fim", nullptr));
        QTableWidgetItem *___qtablewidgetitem5 = tabelaProcessos->horizontalHeaderItem(5);
        ___qtablewidgetitem5->setText(QCoreApplication::translate("MainWindow", "Espera", nullptr));
        QTableWidgetItem *___qtablewidgetitem6 = tabelaProcessos->horizontalHeaderItem(6);
        ___qtablewidgetitem6->setText(QCoreApplication::translate("MainWindow", "Resposta", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(abaTabela), QCoreApplication::translate("MainWindow", "Tabela de processos", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(abaMemoria), QCoreApplication::translate("MainWindow", "Memoria", nullptr));
        labelEsperaDesc->setText(QCoreApplication::translate("MainWindow", "Media de espera", nullptr));
        labelEspera->setText(QCoreApplication::translate("MainWindow", "-- u.t.", nullptr));
        labelRespostaDesc->setText(QCoreApplication::translate("MainWindow", "Media de resposta", nullptr));
        labelResposta->setText(QCoreApplication::translate("MainWindow", "-- u.t.", nullptr));
        labelFaultsDesc->setText(QCoreApplication::translate("MainWindow", "Page faults", nullptr));
        labelPageFaults->setText(QCoreApplication::translate("MainWindow", "--", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
