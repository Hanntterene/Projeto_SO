// Scheduler.h
#pragma once
#include "Process.h"
#include <vector>

enum class PoliticaEscalonamento {
    ROUND_ROBIN,
    SJF_PREEMPTIVO,
    PRIORIDADE_PREEMPTIVA
};

struct EntradaTimeline {
    int pid;
    int tempo_inicio;
    int tempo_fim;
};

class Escalonador {
public:
    Escalonador(PoliticaEscalonamento politica, int quantum = 2);
    std::vector<EntradaTimeline> executar(std::vector<Processo>& processos);

private:
    PoliticaEscalonamento politica;
    int quantum;

    std::vector<EntradaTimeline> executarRoundRobin(std::vector<Processo>& processos);
    std::vector<EntradaTimeline> executarSJF(std::vector<Processo>& processos);
    std::vector<EntradaTimeline> executarPrioridade(std::vector<Processo>& processos);
};