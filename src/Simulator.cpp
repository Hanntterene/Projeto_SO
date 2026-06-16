// Simulator.cpp - Executa a simulação completa
#include "Simulator.h"

// Construtor: armazena todas as configurações para a simulação
Simulador::Simulador(PoliticaEscalonamento pol_esc, int quantum,
                     int mem_fisica, int tam_pagina,
                     PoliticaSubstituicao pol_mem)
    : politica_escalonamento(pol_esc), quantum(quantum),
      memoria_fisica_mb(mem_fisica), tamanho_pagina_mb(tam_pagina),
      politica_memoria(pol_mem) {}

// Função principal: executa toda a simulação
ResultadoSimulacao Simulador::executar(std::vector<Processo> processos) {
    // ETAPA 1: ESCALONAMENTO
    // Decide em qual ordem os processos executam usando o algoritmo escolhido
    Escalonador escalonador(politica_escalonamento, quantum);
    auto timeline = escalonador.executar(processos);

    // ETAPA 2: SIMULAÇÃO DE MEMÓRIA
    // Simula como o sistema acessa as páginas de memória
    // Cada vez que um processo executa, acessa sua página de memória
    GerenciadorMemoria memoria(memoria_fisica_mb, tamanho_pagina_mb, politica_memoria);
    for (auto& entrada : timeline) {
        // Simplificação: cada processo = 1 página
        int id_pagina = entrada.pid;
        // Simula acesso à página (pode gerar page fault)
        memoria.acessarPagina(id_pagina);
    }

    // ETAPA 3: CÁLCULO DAS MÉTRICAS
    // Calcula as estatisticas importantes da simulação
    double soma_espera    = 0;   // Soma de todos os tempos de espera
    double soma_resposta  = 0;   // Soma de todos os tempos de resposta
    
    for (auto& proc : processos) {
        soma_espera   += proc.tempo_espera;     // Acumula tempos de espera
        soma_resposta += proc.tempo_resposta;   // Acumula tempos de resposta
    }
    int n = processos.size();  // Total de processos

    // Retorna todos os resultados da simulação
    return {
        timeline,                              // Quando cada processo executou
        processos,                             // Processos com métricas calculadas
        soma_espera   / n,                     // Média de espera
        soma_resposta / n,                     // Média de resposta
        memoria.totalPageFaults()              // Total de page faults
    };
}