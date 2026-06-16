// Scheduler.h - Algoritmos de Escalonamento de Processos
#pragma once
#include "Process.h"
#include <vector>

// Enum: define qual algoritmo de escalonamento usar
enum class PoliticaEscalonamento {
    ROUND_ROBIN,              // Cada processo tem um tempo máximo para rodar
    SJF_PREEMPTIVO,           // Executa sempre o com menos tempo faltando
    PRIORIDADE_PREEMPTIVA     // Executa sempre o de maior prioridade
};

// Struct: registra quando um processo executou na timeline
struct EntradaTimeline {
    int pid;            // Qual processo
    int tempo_inicio;   // De que horas
    int tempo_fim;      // Até que horas
};

// Classe: responsavel por escalonar (escolher qual processo executa) 
class Escalonador {
public:
    // Construtor: recebe a política de escalonamento e o quantum (para Round Robin)
    Escalonador(PoliticaEscalonamento politica, int quantum = 2);
    
    // Método principal: executa a simulação e retorna a timeline de execução
    std::vector<EntradaTimeline> executar(std::vector<Processo>& processos);

private:
    PoliticaEscalonamento politica;  // Qual algoritmo usar
    int quantum;                     // Tempo máximo para Round Robin

    // Implementação de cada algoritmo
    std::vector<EntradaTimeline> executarRoundRobin(std::vector<Processo>& processos);
    std::vector<EntradaTimeline> executarSJF(std::vector<Processo>& processos);
    std::vector<EntradaTimeline> executarPrioridade(std::vector<Processo>& processos);
};