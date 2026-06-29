#include "Scheduler.h"
#include <algorithm>
#include <queue>

// Construtor: inicializa o escalonador com a política escolhida e o quantum.
Escalonador::Escalonador(PoliticaEscalonamento politica, int quantum)
    : politica(politica), quantum(quantum) {}

// Esse método escolhe qual algoritmo será usado a partir da política.
std::vector<EntradaTimeline> Escalonador::executar(std::vector<Processo>& processos) {
    switch (politica) {
        // Round Robin: cada processo tem um tempo máximo para rodar
        case PoliticaEscalonamento::ROUND_ROBIN:           return executarRoundRobin(processos);
        // SJF: executa sempre o que tem menos tempo faltando (pode ser interrompido)
        case PoliticaEscalonamento::SJF_PREEMPTIVO:        return executarSJF(processos);
        // Prioridade: executa o de maior prioridade (número menor = mais importante)
        case PoliticaEscalonamento::PRIORIDADE_PREEMPTIVA: return executarPrioridade(processos);
    }
    return {};
}

// Junta blocos consecutivos do mesmo processo na timeline.
// Torna a saída mais fácil de ler no terminal.
static std::vector<EntradaTimeline> compactar(const std::vector<EntradaTimeline>& tl) {
    if (tl.empty()) return tl;
    std::vector<EntradaTimeline> resultado = { tl[0] };
    for (size_t i = 1; i < tl.size(); i++) {
        auto& ultimo = resultado.back();
        // Se o próximo é o mesmo processo e começa exatamente quando o anterior terminou
        if (tl[i].pid == ultimo.pid && tl[i].tempo_inicio == ultimo.tempo_fim)
            // Estende o final do bloco anterior
            ultimo.tempo_fim = tl[i].tempo_fim;
        else
            // Se não, é um novo bloco, adiciona na lista
            resultado.push_back(tl[i]);
    }
    return resultado;
}

// ALGORITMO 1: ROUND ROBIN
// Cada processo pega um pedaço de CPU por vez. Se não terminar, volta para o fim da fila.
std::vector<EntradaTimeline> Escalonador::executarRoundRobin(std::vector<Processo>& processos) {
    std::vector<EntradaTimeline> timeline;  // Guarda quando cada processo executou
    std::queue<int> fila;                   // Fila de espera dos processos
    int relogio = 0, chegaram = 0;          // Contador de tempo e quantos chegaram
    int n = processos.size();

    // Ordena os processos por tempo de chegada (primeiro vem, primeiro serve)
    std::sort(processos.begin(), processos.end(),
              [](const Processo& a, const Processo& b) {
                  return a.tempo_chegada < b.tempo_chegada;
              });

    // Loop principal da simulação
    while (true) {
        // Adiciona à fila todos os processos que já chegaram (tempo_chegada <= relogio)
        while (chegaram < n && processos[chegaram].tempo_chegada <= relogio)
            fila.push(chegaram++);

        // Se fila vazia e todos chegaram, termina
        if (fila.empty()) {
            if (chegaram >= n) break;
            relogio++;  // Se fila vazia mas faltam processos chegar, avança o tempo
            continue;
        }

        // Pega o primeiro processo da fila
        int idx = fila.front();
        fila.pop();
        Processo& proc = processos[idx];

        // Se é a primeira vez que este processo executa, registra o tempo de início
        if (proc.tempo_inicio == -1) {
            proc.tempo_inicio   = relogio;
            proc.tempo_resposta = relogio - proc.tempo_chegada;  // Desde que chegou até agora
        }

        // Calcula quantas unidades de tempo este processo vai rodar
        // O mínimo entre: quantum (tempo máximo) e tempo_restante (quanto falta)
        int fatia = std::min(quantum, proc.tempo_restante);
        
        // Registra na timeline que este processo rodou
        timeline.push_back({ proc.pid, relogio, relogio + fatia });
        
        // Atualiza o tempo e reduz o tempo restante do processo
        relogio            += fatia;
        proc.tempo_restante -= fatia;

        // Adiciona à fila novos processos que chegaram durante a execução
        while (chegaram < n && processos[chegaram].tempo_chegada <= relogio)
            fila.push(chegaram++);

        // Se o processo ainda não terminou, volta para o final da fila
        if (proc.tempo_restante > 0) {
            fila.push(idx);
        } else {
            // Processo terminou
            proc.tempo_fim    = relogio;
            proc.tempo_espera = proc.tempo_fim - proc.tempo_chegada - proc.tempo_execucao;
        }
    }

    return timeline;
}

// ALGORITMO 2: SJF PREEMPTIVO
// Sempre escolhe o processo com menos tempo restante para executar.
std::vector<EntradaTimeline> Escalonador::executarSJF(std::vector<Processo>& processos) {
    std::vector<EntradaTimeline> timeline;
    int relogio = 0, finalizados = 0;  // Contador de tempo e processos terminados
    int n = processos.size();

    // Continua enquanto houver processos não finalizados
    while (finalizados < n) {
        // Procura o processo com menor tempo restante entre os que já chegaram
        int escolhido = -1;
        for (int i = 0; i < n; i++) {
            // Verifica se o processo já chegou e ainda não terminou
            if (processos[i].tempo_chegada <= relogio && processos[i].tempo_restante > 0) {
                // Se é o primeiro ou tem menos tempo que o anterior
                if (escolhido == -1 ||
                    processos[i].tempo_restante < processos[escolhido].tempo_restante)
                    escolhido = i;
            }
        }

        // Se não achou nenhum processo pronto, avança o tempo
        if (escolhido == -1) { relogio++; continue; }

        Processo& proc = processos[escolhido];
        
        // Se é a primeira execução, registra início e resposta
        if (proc.tempo_inicio == -1) {
            proc.tempo_inicio   = relogio;
            proc.tempo_resposta = relogio - proc.tempo_chegada;
        }

        // Executa por apenas 1 unidade de tempo (para permitir preemption)
        timeline.push_back({ proc.pid, relogio, relogio + 1 });
        proc.tempo_restante--;
        relogio++;

        // Se terminou
        if (proc.tempo_restante == 0) {
            proc.tempo_fim    = relogio;
            proc.tempo_espera = proc.tempo_fim - proc.tempo_chegada - proc.tempo_execucao;
            finalizados++;
        }
    }

    // Compacta entradas consecutivas do mesmo processo
    return compactar(timeline);
}

// ALGORITMO 3: PRIORIDADE PREEMPTIVA
// Sempre executa o processo de maior prioridade disponível.
std::vector<EntradaTimeline> Escalonador::executarPrioridade(std::vector<Processo>& processos) {
    std::vector<EntradaTimeline> timeline;
    int relogio = 0, finalizados = 0;  // Contador de tempo e processos terminados
    int n = processos.size();

    // Continua enquanto houver processos não finalizados
    while (finalizados < n) {
        // Procura o processo com melhor (menor) prioridade entre os que já chegaram
        int escolhido = -1;
        for (int i = 0; i < n; i++) {
            // Verifica se o processo já chegou e ainda não terminou
            if (processos[i].tempo_chegada <= relogio && processos[i].tempo_restante > 0) {
                // Se é o primeiro ou tem prioridade melhor (número menor)
                if (escolhido == -1 ||
                    processos[i].prioridade < processos[escolhido].prioridade)
                    escolhido = i;
            }
        }

        // Se não achou nenhum processo pronto, avança o tempo
        if (escolhido == -1) { relogio++; continue; }

        Processo& proc = processos[escolhido];
        
        // Se é a primeira execução, registra início e resposta
        if (proc.tempo_inicio == -1) {
            proc.tempo_inicio   = relogio;
            proc.tempo_resposta = relogio - proc.tempo_chegada;
        }

        // Executa por apenas 1 unidade de tempo (para permitir preemption)
        timeline.push_back({ proc.pid, relogio, relogio + 1 });
        proc.tempo_restante--;
        relogio++;

        // Se terminou
        if (proc.tempo_restante == 0) {
            proc.tempo_fim    = relogio;
            proc.tempo_espera = proc.tempo_fim - proc.tempo_chegada - proc.tempo_execucao;
            finalizados++;
        }
    }

    // Compacta entradas consecutivas do mesmo processo
    return compactar(timeline);
}