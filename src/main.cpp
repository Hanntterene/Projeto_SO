#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include "CSVParser.h"
#include "Simulator.h"

// Esse helper só imprime uma linha de traços para organizar o relatório.
void separador(int largura = 60) {
    std::cout << std::string(largura, '-') << "\n";
}

// Imprime a timeline de execução dos processos.
// Cada bloco mostra quando um processo estava ocupando a CPU.
void imprimirTimeline(const std::vector<EntradaTimeline>& timeline) {
    std::cout << "\nTimeline de execucao:\n";
    separador();

    // Para cada intervalo de tempo em que um processo executou
    for (auto& entrada : timeline) {
        std::cout << "  P" << entrada.pid
                  << "  [" << std::setw(3) << entrada.tempo_inicio
                  << " -> " << std::setw(3) << entrada.tempo_fim << "]  ";

        // Desenha uma barra com # para visualizar quanto tempo o processo rodou
        int duracao = entrada.tempo_fim - entrada.tempo_inicio;
        for (int i = 0; i < duracao * 2; i++) std::cout << "#";
        std::cout << "\n";
    }
}

// Imprime a tabela de resultados e as métricas finais.
// Aqui a ideia é mostrar o que aconteceu com cada processo.
void imprimirResultados(const std::vector<Processo>& processos,
                        const ResultadoSimulacao& resultado) {
    std::cout << "\nResultados por processo:\n";
    separador();
    
    // Cabeçalho da tabela
    std::cout << std::left
              << std::setw(6)  << "PID"
              << std::setw(10) << "Chegada"
              << std::setw(8)  << "Burst"
              << std::setw(10) << "Inicio"
              << std::setw(8)  << "Fim"
              << std::setw(10) << "Espera"
              << std::setw(10) << "Resposta"
              << "\n";
    separador();

    // Imprime cada processo com suas métricas
    for (auto& p : processos) {
        std::cout << std::left
                  << std::setw(6)  << ("P" + std::to_string(p.pid))
                  << std::setw(10) << p.tempo_chegada
                  << std::setw(8)  << p.tempo_execucao
                  << std::setw(10) << p.tempo_inicio
                  << std::setw(8)  << p.tempo_fim
                  << std::setw(10) << p.tempo_espera
                  << std::setw(10) << p.tempo_resposta
                  << "\n";
    }

    separador();
    std::cout << std::fixed << std::setprecision(2);
    
    // Exibe as métricas principais da simulação
    // Tempo de espera = quanto tempo o processo ficou esperando na fila
    // Tempo de resposta = tempo desde que chegou até começar a executar
    // Page faults = quantas vezes precisou trazer página da memória virtual
    std::cout << "\nMedia tempo de espera  : " << resultado.media_tempo_espera   << " u.t.\n";
    std::cout << "Media tempo de resposta: " << resultado.media_tempo_resposta << " u.t.\n";
    std::cout << "Total de page faults   : " << resultado.total_page_faults    << "\n";
}

// Programa principal do simulador em modo terminal.
// Ele carrega o CSV, mostra os processos e executa três combinações de testes.
int main(int argc, char* argv[]) {
    // Define qual arquivo CSV contém os processos a simular
    std::string caminho_csv = "examples/processes.csv";
    if (argc > 1) caminho_csv = argv[1];

    std::cout << "\n=== Simulador de SO - Modo Terminal ===\n";
    std::cout << "Arquivo: " << caminho_csv << "\n";

    // Lê os processos do arquivo CSV (chegada, tempo de execução, prioridade, memória)
    auto processos = LeitorCSV::ler(caminho_csv);
    if (processos.empty()) {
        std::cerr << "Nenhum processo carregado. Verifique o arquivo CSV.\n";
        return 1;
    }

    // Mostra um sumário dos processos que foram carregados
    std::cout << "\n" << processos.size() << " processo(s) carregado(s):\n";
    separador();
    std::cout << std::left
              << std::setw(6)  << "PID"
              << std::setw(10) << "Chegada"
              << std::setw(8)  << "Burst"
              << std::setw(12) << "Prioridade"
              << std::setw(12) << "Memoria(MB)"
              << "\n";
    separador();
    for (auto& p : processos) {
        std::cout << std::left
                  << std::setw(6)  << ("P" + std::to_string(p.pid))
                  << std::setw(10) << p.tempo_chegada
                  << std::setw(8)  << p.tempo_execucao
                  << std::setw(12) << p.prioridade
                  << std::setw(12) << p.memoria_necessaria
                  << "\n";
    }

    // ═══ CONFIGURAÇÕES DA SIMULAÇÃO ═══
    // quantum: tempo máximo que um processo pode rodar continuamente (em unidades de tempo)
    // memoria_fisica: quanto de RAM real o computador simulado tem
    // tamanho_pagina: divide a memória em blocos (páginas) deste tamanho
    int    quantum          = 2;
    int    memoria_fisica   = 256; // MB
    int    tamanho_pagina   = 64;  // MB

    // ═══ TESTE 1: Round Robin + FIFO ═══
    // Round Robin = cada processo pega sua vez de rodar por um tempo limitado (quantum)
    // Se não terminar, volta para o final da fila para tentar depois
    // FIFO = First In First Out = a primeira página carregada é a primeira a ser removida
    std::cout << "\n\n>>> ROUND ROBIN (quantum=" << quantum << ") + FIFO <<<\n";
    {
        auto procs_copia = processos;
        Simulador sim(PoliticaEscalonamento::ROUND_ROBIN, quantum,
                      memoria_fisica, tamanho_pagina,
                      PoliticaSubstituicao::FIFO);
        auto resultado = sim.executar(procs_copia);
        imprimirTimeline(resultado.timeline);
        imprimirResultados(resultado.lista_processos, resultado);
    }

    // ═══ TESTE 2: SJF Preemptivo + LRU ═══
    // SJF = Shortest Job First = sempre executa o processo com menor tempo restante
    // Preemptivo = pode interromper um processo em execução se chegar outro mais curto
    // LRU = Least Recently Used = remove a página que não foi usada há mais tempo
    std::cout << "\n\n>>> SJF PREEMPTIVO + LRU <<<\n";
    {
        auto procs_copia = processos;
        Simulador sim(PoliticaEscalonamento::SJF_PREEMPTIVO, quantum,
                      memoria_fisica, tamanho_pagina,
                      PoliticaSubstituicao::LRU);
        auto resultado = sim.executar(procs_copia);
        imprimirTimeline(resultado.timeline);
        imprimirResultados(resultado.lista_processos, resultado);
    }

    // ═══ TESTE 3: Prioridade Preemptiva + Ótimo ═══
    // Prioridade = executa sempre o processo com maior prioridade (número menor)
    // Preemptivo = interrompe se chegar um processo com prioridade melhor
    // Ótimo = remove a página que será usada mais tarde (teórico, precisa conhecer o futuro)
    std::cout << "\n\n>>> PRIORIDADE PREEMPTIVA + OTIMO <<<\n";
    {
        auto procs_copia = processos;
        Simulador sim(PoliticaEscalonamento::PRIORIDADE_PREEMPTIVA, quantum,
                      memoria_fisica, tamanho_pagina,
                      PoliticaSubstituicao::OTIMO);
        auto resultado = sim.executar(procs_copia);
        imprimirTimeline(resultado.timeline);
        imprimirResultados(resultado.lista_processos, resultado);
    }

    return 0;
}