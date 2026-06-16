#pragma once
#include <string>

enum class EstadoProcesso { PRONTO, EXECUTANDO, ESPERANDO, FINALIZADO };

struct Processo {
    int pid;
    int tempo_chegada;
    int tempo_execucao;     // burst total
    int tempo_restante;     // usado nos algoritmos preemptivos
    int prioridade;
    int memoria_necessaria; // em MB

    // preenchidos durante a simulação
    int tempo_inicio     = -1;
    int tempo_fim        = -1;
    int tempo_espera     = 0;
    int tempo_resposta   = -1;

    EstadoProcesso estado = EstadoProcesso::PRONTO;
};