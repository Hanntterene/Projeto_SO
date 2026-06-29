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
    // Cada processo acessa múltiplas páginas de acordo com sua memória necessária.
    GerenciadorMemoria memoria(memoria_fisica_mb, tamanho_pagina_mb, politica_memoria);
    
    // Primeiro, mapear quantas páginas cada processo precisa
    std::unordered_map<int, int> num_paginas_por_processo;  // pid -> número de páginas
    for (auto& proc : processos) {
        int paginas_necessarias = std::max(1, (proc.memoria_necessaria + tamanho_pagina_mb - 1) / tamanho_pagina_mb);
        num_paginas_por_processo[proc.pid] = paginas_necessarias;
    }
    
    // Simular acessos: cada entrada da timeline representa um acesso
    // O processo acessa suas páginas sequencialmente
    for (size_t i = 0; i < timeline.size(); ++i) {
        auto& entrada = timeline[i];
        int pid = entrada.pid;
        int tempo = entrada.tempo_inicio;
        int num_paginas = num_paginas_por_processo[pid];
        
        // Acessa todas as páginas do processo durante sua execução
        for (int num_pagina = 1; num_pagina <= num_paginas; ++num_pagina) {
            int id_pagina = pid * 100 + num_pagina;  // ID único global
            memoria.acessarPagina(id_pagina, pid, num_pagina, tempo);
        }
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
    ResultadoSimulacao resultado;
    resultado.timeline = timeline;
    resultado.lista_processos = processos;
    resultado.historico_memoria = memoria.obterHistorico();
    resultado.media_tempo_espera = soma_espera / n;
    resultado.media_tempo_resposta = soma_resposta / n;
    resultado.total_page_faults = memoria.totalPageFaults();

    return resultado;
}