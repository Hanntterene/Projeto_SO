#pragma once
#include <string>

// Enum: define os estados possíveis de um processo durante sua execução
enum class EstadoProcesso { 
    PRONTO,        // Processo está na fila, esperando sua vez
    EXECUTANDO,    // Processo está rodando na CPU
    ESPERANDO,     // Processo está aguardando recursos (disco, I/O, etc)
    FINALIZADO     // Processo completou e terminou
};

// Struct: representa um processo do sistema operacional
struct Processo {
    // Informações básicas (vem do arquivo CSV)
    int pid;                    // Identificador único do processo
    int tempo_chegada;          // Que horas o processo chegou no sistema
    int tempo_execucao;         // Quanto tempo de CPU o processo precisa (burst total)
    int tempo_restante;         // Usado nos algoritmos preemptivos: quanto tempo ainda falta
    int prioridade;             // Prioridade do processo (número menor = mais importante)
    int memoria_necessaria;     // Quanto de memória (em MB) o processo precisa

    // Informações preenchidas DURANTE a simulação
    int tempo_inicio     = -1;  // Em que hora a simulação começou a executar o processo
    int tempo_fim        = -1;  // Em que hora o processo terminou
    int tempo_espera     = 0;   // Quanto tempo em total o processo ficou esperando na fila
    int tempo_resposta   = -1;  // Tempo entre chegada e primeira execução

    EstadoProcesso estado = EstadoProcesso::PRONTO;  // Estado atual do processo
};