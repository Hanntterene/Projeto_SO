#pragma once
#include "Process.h"
#include "Scheduler.h"
#include "Memory.h"
#include <vector>

// Struct: guarda todos os resultados da simulação
struct ResultadoSimulacao {
    std::vector<EntradaTimeline> timeline;  // Quando cada processo rodou
    std::vector<Processo> lista_processos;  // Processos com métricas preenchidas
    
    // Métricas de performance
    double media_tempo_espera;              // Tempo médio que processos ficaram na fila
    double media_tempo_resposta;            // Tempo médio desde chegada até primeira execução
    int total_page_faults;                  // Quantas vezes faltou página em memória
};

// Classe: orquestra a simulação inteira
// Coordena o escalonamento de processos + gerenciamento de memória
class Simulador {
public:
    // Construtor: recebe as configurações da simulação
    Simulador(PoliticaEscalonamento politica_escalonamento, int quantum,
              int memoria_fisica_mb, int tamanho_pagina_mb,
              PoliticaSubstituicao politica_memoria);

    // Método principal: executa a simulação e retorna os resultados
    ResultadoSimulacao executar(std::vector<Processo> processos);

private:
    // Configurações da simulação
    PoliticaEscalonamento politica_escalonamento;  // Qual algoritmo de escalonamento usar
    int quantum;                                   // Tempo máximo para Round Robin
    int memoria_fisica_mb;                         // RAM disponível
    int tamanho_pagina_mb;                         // Tamanho de cada página
    PoliticaSubstituicao politica_memoria;         // Qual política de substituição usar
};