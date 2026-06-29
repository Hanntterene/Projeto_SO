#include "Memory.h"
#include <algorithm>

// Construtor: calcula quantos frames (blocos) de memória física temos disponíveis
GerenciadorMemoria::GerenciadorMemoria(int memoria_fisica_mb, int tamanho_pagina_mb,
                                       PoliticaSubstituicao politica)
    : num_frames(std::max(1, memoria_fisica_mb / tamanho_pagina_mb)),
      politica(politica) {}

// Verifica se uma página já está carregada na memória física
bool GerenciadorMemoria::estaNaMemoria(int id_pagina) {
    return std::find(frames.begin(), frames.end(), id_pagina) != frames.end();
}

// Gera nome visual da página (ex: "P1p1", "P2p3")
std::string nomeVisualPagina(int pid, int numero_pagina) {
    return "P" + std::to_string(pid) + "p" + std::to_string(numero_pagina);
}

// Gera lista visual do estado da memória com slots vazios
std::vector<std::string> gerarEstadoVisual(const std::deque<int>& frames, 
                                          int num_frames_total,
                                          const std::unordered_map<int, std::string>& mapa_nomes) {
    std::vector<std::string> visual;
    for (int frame : frames) {
        if (mapa_nomes.count(frame)) {
            visual.push_back(mapa_nomes.at(frame));
        } else {
            visual.push_back(std::to_string(frame));
        }
    }
    // Adiciona slots vazios
    while (static_cast<int>(visual.size()) < num_frames_total) {
        visual.push_back("__");
    }
    return visual;
}

// Função principal: simula acesso a uma página de memória com contexto de processo
bool GerenciadorMemoria::acessarPagina(int pagina_id, int pid, int numero_pagina, int tempo, int uso_futuro) {
    // Constrói nome visual
    std::string pagina_nome = nomeVisualPagina(pid, numero_pagina);
    mapa_nomes_paginas[pagina_id] = pagina_nome;
    
    // Se a página já está em memória
    if (estaNaMemoria(pagina_id)) {
        if (politica == PoliticaSubstituicao::LRU || politica == PoliticaSubstituicao::OTIMO) {
            ordem_lru[pagina_id] = relogio_lru++;
        }
        return false;
    }

    // PAGE FAULT - precisa carregar a página
    ++total_page_faults;
    carregarPagina(pagina_id, pid, numero_pagina, tempo, uso_futuro);
    return true;
}

// Carrega uma página na memória física
void GerenciadorMemoria::carregarPagina(int pagina_id, int pid, int numero_pagina, int tempo, int uso_futuro) {
    std::string pagina_nome = nomeVisualPagina(pid, numero_pagina);
    
    EventoMemoria evento;
    evento.tempo = tempo;
    evento.pagina_id = pagina_id;
    evento.pagina_nome = pagina_nome;
    evento.page_fault = true;
    evento.faults_neste_evento = 1;
    evento.pagina_removida_id = -1;
    evento.pagina_removida_nome = "";
    
    // Se ainda há espaço disponível
    if (static_cast<int>(frames.size()) < num_frames) {
        frames.push_back(pagina_id);
        ordem_lru[pagina_id] = relogio_lru++;
        evento.politica_usada = "";
        evento.descricao = pagina_nome + " carregada";
        evento.estado_memoria_nomes = gerarEstadoVisual(frames, num_frames, mapa_nomes_paginas);
        historico.push_back(evento);
        return;
    }

    // Memória está cheia - precisa remover uma página
    int pagina_removida = -1;
    std::string motivo;
    
    switch (politica) {
        case PoliticaSubstituicao::FIFO:
            pagina_removida = frames.front();
            frames.pop_front();
            frames.push_back(pagina_id);
            ordem_lru[pagina_id] = relogio_lru++;
            evento.politica_usada = "FIFO";
            motivo = "mais antigo";
            break;
            
        case PoliticaSubstituicao::LRU: {
            pagina_removida = frames.front();
            int menor_instante = ordem_lru[pagina_removida];
            for (int p : frames) {
                if (ordem_lru[p] < menor_instante) {
                    menor_instante = ordem_lru[p];
                    pagina_removida = p;
                }
            }
            frames.erase(std::find(frames.begin(), frames.end(), pagina_removida));
            ordem_lru.erase(pagina_removida);
            frames.push_back(pagina_id);
            ordem_lru[pagina_id] = relogio_lru++;
            evento.politica_usada = "LRU";
            motivo = "menos recentemente usada";
            break;
        }
        
        case PoliticaSubstituicao::OTIMO:
            if (uso_futuro == -1) {
                // Fallback para FIFO se não temos informação de futuro
                pagina_removida = frames.front();
                frames.pop_front();
                frames.push_back(pagina_id);
                ordem_lru[pagina_id] = relogio_lru++;
                evento.politica_usada = "ÓTIMO (FIFO fallback)";
                motivo = "sera usada mais tarde";
            } else {
                // Remove a que será usada mais tarde
                pagina_removida = frames.front();
                int maior_distancia = -1;
                for (int p : frames) {
                    int dist = uso_futuro - ordem_lru[p];
                    if (dist > maior_distancia) {
                        maior_distancia = dist;
                        pagina_removida = p;
                    }
                }
                frames.erase(std::find(frames.begin(), frames.end(), pagina_removida));
                ordem_lru.erase(pagina_removida);
                frames.push_back(pagina_id);
                ordem_lru[pagina_id] = relogio_lru++;
                evento.politica_usada = "ÓTIMO";
                motivo = "sera usada mais tarde";
            }
            break;
    }
    
    evento.pagina_removida_id = pagina_removida;
    if (mapa_nomes_paginas.count(pagina_removida)) {
        evento.pagina_removida_nome = mapa_nomes_paginas[pagina_removida];
    } else {
        evento.pagina_removida_nome = std::to_string(pagina_removida);
    }
    
    evento.descricao = pagina_nome + " → Remove " + evento.pagina_removida_nome + " (" + motivo + ")";
    evento.estado_memoria_nomes = gerarEstadoVisual(frames, num_frames, mapa_nomes_paginas);
    historico.push_back(evento);
}


// POLÍTICA 1: FIFO (First In First Out)
void GerenciadorMemoria::substituirFIFO(int id_pagina) {
    if (!frames.empty()) {
        frames.pop_front();
    }
    frames.push_back(id_pagina);
    ordem_lru[id_pagina] = relogio_lru++;
}

// POLÍTICA 2: LRU (Least Recently Used)
void GerenciadorMemoria::substituirLRU(int id_pagina) {
    auto pagina_para_remover = frames.front();
    auto menor_instante = ordem_lru[pagina_para_remover];

    for (int pagina : frames) {
        if (ordem_lru[pagina] < menor_instante) {
            menor_instante = ordem_lru[pagina];
            pagina_para_remover = pagina;
        }
    }

    frames.erase(std::find(frames.begin(), frames.end(), pagina_para_remover));
    ordem_lru.erase(pagina_para_remover);
    frames.push_back(id_pagina);
    ordem_lru[id_pagina] = relogio_lru++;
}

// POLÍTICA 3: ÓTIMO (Optimal)
void GerenciadorMemoria::substituirOtimo(int id_pagina, int uso_futuro) {
    if (uso_futuro == -1 || frames.empty()) {
        substituirFIFO(id_pagina);
        return;
    }

    int pagina_para_remover = frames.front();
    int maior_distancia = -1;

    for (int pagina : frames) {
        int distancia = uso_futuro - ordem_lru[pagina];
        if (distancia > maior_distancia) {
            maior_distancia = distancia;
            pagina_para_remover = pagina;
        }
    }

    frames.erase(std::find(frames.begin(), frames.end(), pagina_para_remover));
    ordem_lru.erase(pagina_para_remover);
    frames.push_back(id_pagina);
    ordem_lru[id_pagina] = relogio_lru++;
}