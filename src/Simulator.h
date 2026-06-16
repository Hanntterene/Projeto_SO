#pragma once
#include "Process.h"
#include "Scheduler.h"
#include "Memory.h"
#include <vector>

struct ResultadoSimulacao {
    std::vector<EntradaTimeline> timeline;
    std::vector<Processo>        lista_processos; // processos com metricas preenchidas
    double media_tempo_espera;
    double media_tempo_resposta;
    int    total_page_faults;
};

class Simulador {
public:
    Simulador(PoliticaEscalonamento politica_escalonamento, int quantum,
              int memoria_fisica_mb, int tamanho_pagina_mb,
              PoliticaSubstituicao politica_memoria);

    ResultadoSimulacao executar(std::vector<Processo> processos);

private:
    PoliticaEscalonamento politica_escalonamento;
    int quantum;
    int memoria_fisica_mb;
    int tamanho_pagina_mb;
    PoliticaSubstituicao politica_memoria;
};