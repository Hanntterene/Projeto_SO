#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include "CSVParser.h"
#include "Simulator.h"

// ── imprime linha separadora ─────────────────────────────────────────────────
void separador(int largura = 60) {
    std::cout << std::string(largura, '-') << "\n";
}

// ── imprime a timeline de execucao ───────────────────────────────────────────
void imprimirTimeline(const std::vector<EntradaTimeline>& timeline) {
    std::cout << "\nTimeline de execucao:\n";
    separador();

    for (auto& entrada : timeline) {
        std::cout << "  P" << entrada.pid
                  << "  [" << std::setw(3) << entrada.tempo_inicio
                  << " -> " << std::setw(3) << entrada.tempo_fim << "]  ";

        // barra visual proporcional
        int duracao = entrada.tempo_fim - entrada.tempo_inicio;
        for (int i = 0; i < duracao * 2; i++) std::cout << "#";
        std::cout << "\n";
    }
}

// ── imprime a tabela de resultados por processo ──────────────────────────────
void imprimirResultados(const std::vector<Processo>& processos,
                        const ResultadoSimulacao& resultado) {
    std::cout << "\nResultados por processo:\n";
    separador();
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
    std::cout << "\nMedia tempo de espera  : " << resultado.media_tempo_espera   << " u.t.\n";
    std::cout << "Media tempo de resposta: " << resultado.media_tempo_resposta << " u.t.\n";
    std::cout << "Total de page faults   : " << resultado.total_page_faults    << "\n";
}

// ── main ─────────────────────────────────────────────────────────────────────
int main(int argc, char* argv[]) {
    std::string caminho_csv = "examples/processes.csv";
    if (argc > 1) caminho_csv = argv[1];

    std::cout << "\n=== Simulador de SO - Modo Terminal ===\n";
    std::cout << "Arquivo: " << caminho_csv << "\n";

    // le os processos do CSV
    auto processos = LeitorCSV::ler(caminho_csv);
    if (processos.empty()) {
        std::cerr << "Nenhum processo carregado. Verifique o arquivo CSV.\n";
        return 1;
    }

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

    // configuracoes da simulacao
    int    quantum          = 2;
    int    memoria_fisica   = 256; // MB
    int    tamanho_pagina   = 64;  // MB

    // ── teste 1: Round Robin ────────────────────────────────────────────────
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

    // ── teste 2: SJF Preemptivo ─────────────────────────────────────────────
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

    // ── teste 3: Prioridade Preemptiva ──────────────────────────────────────
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