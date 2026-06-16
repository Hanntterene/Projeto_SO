// Simulator.cpp
#include "Simulator.h"

Simulador::Simulador(PoliticaEscalonamento pol_esc, int quantum,
                     int mem_fisica, int tam_pagina,
                     PoliticaSubstituicao pol_mem)
    : politica_escalonamento(pol_esc), quantum(quantum),
      memoria_fisica_mb(mem_fisica), tamanho_pagina_mb(tam_pagina),
      politica_memoria(pol_mem) {}

ResultadoSimulacao Simulador::executar(std::vector<Processo> processos) {
    // 1. escalonamento
    Escalonador escalonador(politica_escalonamento, quantum);
    auto timeline = escalonador.executar(processos);

    // 2. simulação de memória (cada processo acessa páginas ao ser executado)
    GerenciadorMemoria memoria(memoria_fisica_mb, tamanho_pagina_mb, politica_memoria);
    for (auto& entrada : timeline) {
        int id_pagina = entrada.pid; // simplificação: 1 página por processo
        memoria.acessarPagina(id_pagina);
    }

    // 3. cálculo das métricas
    double soma_espera    = 0;
    double soma_resposta  = 0;
    for (auto& proc : processos) {
        soma_espera   += proc.tempo_espera;
        soma_resposta += proc.tempo_resposta;
    }
    int n = processos.size();

    return {
        timeline,
        processos,
        soma_espera   / n,
        soma_resposta / n,
        memoria.totalPageFaults()
    };
}