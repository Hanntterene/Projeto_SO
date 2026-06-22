// Simulator.cpp - Executa a simulacao completa
#include "Simulator.h"

// Construtor: guardo as configuracoes escolhidas para usar depois na execucao.
Simulador::Simulador(PoliticaEscalonamento pol_esc, int quantum,
                     int mem_fisica, int tam_pagina,
                     PoliticaSubstituicao pol_mem)
    : politica_escalonamento(pol_esc), quantum(quantum),
      memoria_fisica_mb(mem_fisica), tamanho_pagina_mb(tam_pagina),
      politica_memoria(pol_mem) {}

// Funcao principal: aqui acontece a simulacao de ponta a ponta.
ResultadoSimulacao Simulador::executar(std::vector<Processo> processos) {
    // ETAPA 1 - ESCALONAMENTO
    // Primeiro eu gero a timeline (ordem de execucao dos processos).
    Escalonador escalonador(politica_escalonamento, quantum);
    auto timeline = escalonador.executar(processos);

    // ETAPA 2 - MEMORIA
    // Depois simulo os acessos de memoria em cima da timeline.
    // Neste modelo atual, cada processo acessa uma pagina ligada ao PID.
    GerenciadorMemoria memoria(memoria_fisica_mb, tamanho_pagina_mb, politica_memoria);
    for (auto& entrada : timeline) {
        // Simplificacao didatica: processo -> 1 pagina.
        int id_pagina = entrada.pid;
        // Cada acesso pode (ou nao) gerar page fault.
        memoria.acessarPagina(id_pagina);
    }

    // ETAPA 3 - METRICAS FINAIS
    // Aqui eu somo os tempos para tirar as medias do relatorio.
    double soma_espera    = 0;
    double soma_resposta  = 0;
    
    for (auto& proc : processos) {
        soma_espera   += proc.tempo_espera;
        soma_resposta += proc.tempo_resposta;
    }
    int n = processos.size();

    // ETAPA 4 - EMPACOTAR RESULTADO
    // Retorno tudo que a GUI/terminal precisa mostrar.
    return {
        timeline,
        processos,
        soma_espera   / n,
        soma_resposta / n,
        memoria.totalPageFaults()
    };
}