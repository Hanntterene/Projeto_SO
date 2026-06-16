#include "Memory.h"

#include <algorithm>

GerenciadorMemoria::GerenciadorMemoria(int memoria_fisica_mb, int tamanho_pagina_mb,
                                       PoliticaSubstituicao politica)
    : num_frames(std::max(1, memoria_fisica_mb / tamanho_pagina_mb)),
      politica(politica) {}

bool GerenciadorMemoria::estaNaMemoria(int id_pagina) {
    return std::find(frames.begin(), frames.end(), id_pagina) != frames.end();
}

bool GerenciadorMemoria::acessarPagina(int id_pagina, int uso_futuro) {
    if (estaNaMemoria(id_pagina)) {
        if (politica == PoliticaSubstituicao::LRU || politica == PoliticaSubstituicao::OTIMO) {
            ordem_lru[id_pagina] = relogio_lru++;
        }
        return false;
    }

    ++total_page_faults;
    carregarPagina(id_pagina, uso_futuro);
    return true;
}

void GerenciadorMemoria::carregarPagina(int id_pagina, int uso_futuro) {
    if (static_cast<int>(frames.size()) < num_frames) {
        frames.push_back(id_pagina);
        ordem_lru[id_pagina] = relogio_lru++;
        return;
    }

    switch (politica) {
        case PoliticaSubstituicao::FIFO:
            substituirFIFO(id_pagina);
            break;
        case PoliticaSubstituicao::LRU:
            substituirLRU(id_pagina);
            break;
        case PoliticaSubstituicao::OTIMO:
            substituirOtimo(id_pagina, uso_futuro);
            break;
    }
}

void GerenciadorMemoria::substituirFIFO(int id_pagina) {
    if (!frames.empty()) {
        frames.pop_front();
    }

    frames.push_back(id_pagina);
    ordem_lru[id_pagina] = relogio_lru++;
}

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