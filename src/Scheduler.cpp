#include "Scheduler.h"
#include <algorithm>
#include <queue>

Escalonador::Escalonador(PoliticaEscalonamento politica, int quantum)
    : politica(politica), quantum(quantum) {}

std::vector<EntradaTimeline> Escalonador::executar(std::vector<Processo>& processos) {
    switch (politica) {
        case PoliticaEscalonamento::ROUND_ROBIN:           return executarRoundRobin(processos);
        case PoliticaEscalonamento::SJF_PREEMPTIVO:        return executarSJF(processos);
        case PoliticaEscalonamento::PRIORIDADE_PREEMPTIVA: return executarPrioridade(processos);
    }
    return {};
}

// junta entradas consecutivas do mesmo processo na timeline
static std::vector<EntradaTimeline> compactar(const std::vector<EntradaTimeline>& tl) {
    if (tl.empty()) return tl;
    std::vector<EntradaTimeline> resultado = { tl[0] };
    for (size_t i = 1; i < tl.size(); i++) {
        auto& ultimo = resultado.back();
        if (tl[i].pid == ultimo.pid && tl[i].tempo_inicio == ultimo.tempo_fim)
            ultimo.tempo_fim = tl[i].tempo_fim;
        else
            resultado.push_back(tl[i]);
    }
    return resultado;
}

// Round Robin
std::vector<EntradaTimeline> Escalonador::executarRoundRobin(std::vector<Processo>& processos) {
    std::vector<EntradaTimeline> timeline;
    std::queue<int> fila;
    int relogio = 0, chegaram = 0;
    int n = processos.size();

    std::sort(processos.begin(), processos.end(),
              [](const Processo& a, const Processo& b) {
                  return a.tempo_chegada < b.tempo_chegada;
              });

    while (true) {
        while (chegaram < n && processos[chegaram].tempo_chegada <= relogio)
            fila.push(chegaram++);

        if (fila.empty()) {
            if (chegaram >= n) break;
            relogio++;
            continue;
        }

        int idx = fila.front();
        fila.pop();
        Processo& proc = processos[idx];

        if (proc.tempo_inicio == -1) {
            proc.tempo_inicio   = relogio;
            proc.tempo_resposta = relogio - proc.tempo_chegada;
        }

        int fatia = std::min(quantum, proc.tempo_restante);
        timeline.push_back({ proc.pid, relogio, relogio + fatia });
        relogio            += fatia;
        proc.tempo_restante -= fatia;

        while (chegaram < n && processos[chegaram].tempo_chegada <= relogio)
            fila.push(chegaram++);

        if (proc.tempo_restante > 0) {
            fila.push(idx);
        } else {
            proc.tempo_fim    = relogio;
            proc.tempo_espera = proc.tempo_fim - proc.tempo_chegada - proc.tempo_execucao;
        }
    }

    return timeline;
}

// SJF Preemptivo (SRTF)
std::vector<EntradaTimeline> Escalonador::executarSJF(std::vector<Processo>& processos) {
    std::vector<EntradaTimeline> timeline;
    int relogio = 0, finalizados = 0;
    int n = processos.size();

    while (finalizados < n) {
        int escolhido = -1;
        for (int i = 0; i < n; i++) {
            if (processos[i].tempo_chegada <= relogio && processos[i].tempo_restante > 0) {
                if (escolhido == -1 ||
                    processos[i].tempo_restante < processos[escolhido].tempo_restante)
                    escolhido = i;
            }
        }

        if (escolhido == -1) { relogio++; continue; }

        Processo& proc = processos[escolhido];
        if (proc.tempo_inicio == -1) {
            proc.tempo_inicio   = relogio;
            proc.tempo_resposta = relogio - proc.tempo_chegada;
        }

        timeline.push_back({ proc.pid, relogio, relogio + 1 });
        proc.tempo_restante--;
        relogio++;

        if (proc.tempo_restante == 0) {
            proc.tempo_fim    = relogio;
            proc.tempo_espera = proc.tempo_fim - proc.tempo_chegada - proc.tempo_execucao;
            finalizados++;
        }
    }

    return compactar(timeline);
}

// Prioridade Preemptiva
std::vector<EntradaTimeline> Escalonador::executarPrioridade(std::vector<Processo>& processos) {
    std::vector<EntradaTimeline> timeline;
    int relogio = 0, finalizados = 0;
    int n = processos.size();

    while (finalizados < n) {
        int escolhido = -1;
        for (int i = 0; i < n; i++) {
            if (processos[i].tempo_chegada <= relogio && processos[i].tempo_restante > 0) {
                if (escolhido == -1 ||
                    processos[i].prioridade < processos[escolhido].prioridade)
                    escolhido = i;
            }
        }

        if (escolhido == -1) { relogio++; continue; }

        Processo& proc = processos[escolhido];
        if (proc.tempo_inicio == -1) {
            proc.tempo_inicio   = relogio;
            proc.tempo_resposta = relogio - proc.tempo_chegada;
        }

        timeline.push_back({ proc.pid, relogio, relogio + 1 });
        proc.tempo_restante--;
        relogio++;

        if (proc.tempo_restante == 0) {
            proc.tempo_fim    = relogio;
            proc.tempo_espera = proc.tempo_fim - proc.tempo_chegada - proc.tempo_execucao;
            finalizados++;
        }
    }

    return compactar(timeline);
}