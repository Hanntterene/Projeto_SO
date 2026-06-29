#include "Memory.h"
#include <algorithm>

// Construtor: calcula quantos frames (blocos) de memória física temos disponíveis
// num_frames = memória total / tamanho de cada página
// Exemplo: 256 MB / 64 MB por página = 4 frames
/*
Exemplo: 
3 Processos + Memória Física 1024 MB (4 páginas de 256 MB)
P1: 0s → 512 MB (2 páginas)
P2: 1s → 256 MB (1 página)  
P3: 3s → 768 MB (3 páginas)
*/

GerenciadorMemoria::GerenciadorMemoria(int memoria_fisica_mb, int tamanho_pagina_mb,
                                       PoliticaSubstituicao politica)
    : num_frames(std::max(1, memoria_fisica_mb / tamanho_pagina_mb)),
      politica(politica) {}

// Verifica se uma página já está carregada na memória física
bool GerenciadorMemoria::estaNaMemoria(int id_pagina) {
    // Procura a página na lista de frames
    return std::find(frames.begin(), frames.end(), id_pagina) != frames.end();
}

// Função principal: simula acesso a uma página de memória
// Retorna true se houve PAGE FAULT (página não estava em memória)
bool GerenciadorMemoria::acessarPagina(int id_pagina, int uso_futuro) {
    // Se a página já está em memória física
    if (estaNaMemoria(id_pagina)) {
        // Para LRU e Ótimo, precisa atualizar quando foi acessada
        if (politica == PoliticaSubstituicao::LRU || politica == PoliticaSubstituicao::OTIMO) {
            ordem_lru[id_pagina] = relogio_lru++;  // Registra o tempo deste acesso
        }
        return false;  // Não houve falta
    }

    // Página não estava em memória (PAGE FAULT!)
    ++total_page_faults;
    carregarPagina(id_pagina, uso_futuro);
    return true;
}

// Carrega uma página na memória física
// Se houver espaço vago, simplesmente adiciona
// Se estiver cheio, remove uma página existente (segundo a política)
void GerenciadorMemoria::carregarPagina(int id_pagina, int uso_futuro) {
    // Se ainda há espaço disponível
    if (static_cast<int>(frames.size()) < num_frames) {
        frames.push_back(id_pagina);  // Apenas adiciona
        ordem_lru[id_pagina] = relogio_lru++;  // Registra tempo de carregamento
        return;
    }

    // Se chegou aqui, a memória está cheia
    // Precisa remover uma página existente usando a política escolhida
    switch (politica) {
        case PoliticaSubstituicao::FIFO:     // Primeira a entrar, primeira a sair
            substituirFIFO(id_pagina);
            break;
        case PoliticaSubstituicao::LRU:      // Remove a menos recentemente usada
            substituirLRU(id_pagina);
            break;
        case PoliticaSubstituicao::OTIMO:    // Remove a que será usada mais tarde
            substituirOtimo(id_pagina, uso_futuro);
            break;
    }
}

// POLÍTICA 1: FIFO (First In First Out)
// 
// Como funciona:
// A primeira página que foi carregada é a primeira a ser removida.
// É como uma fila: quem entra primeiro, sai primeiro.
//
/*
t=0s: P1 chega → Mem: [P1p1, P1p2]            (2/4 páginas)
t=1s: P2 chega → Mem: [P1p1, P1p2, P2p1]      (3/4 páginas)
t=3s: P3 chega → Mem: [P1p1, P1p2, P2p1, P3p1] (4/4 CHEIO)

t=4s: P3 precisa p2 → Remove P1p1 (mais antigo)
     Mem: [P1p2, P2p1, P3p1, P3p2]
     
t=5s: P3 precisa p3 → Remove P1p2
     Mem: [P2p1, P3p1, P3p2, P3p3]
     
PAGE FAULTS: 5
*/
void GerenciadorMemoria::substituirFIFO(int id_pagina) {
    // Remove a página mais antiga (primeiro da fila)
    if (!frames.empty()) {
        frames.pop_front();
    }

    // Adiciona a nova página (entra no final da fila)
    frames.push_back(id_pagina);
    ordem_lru[id_pagina] = relogio_lru++;  // Registra tempo de entrada
}

// POLÍTICA 2: LRU (Least Recently Used)
// 
// Como funciona:
// Remove a página que foi ACESSADA HÁ MAIS TEMPO.
// Baseia-se na ideia: se não usou recentemente, provavelmente não usará em breve.
/*
t=0s: P1 chega → Mem: [P1p1, P1p2]
t=1s: P2 chega → Mem: [P1p1, P1p2, P2p1]
t=3s: P3 chega → Mem: [P1p1, P1p2, P2p1, P3p1] (4/4 CHEIO)

t=4s: P3 precisa p2 → P1p1 é a menos usada recentemente
     Remove P1p1 → Mem: [P1p2, P2p1, P3p1, P3p2]
     
t=5s: P3 precisa p3 → P1p2 é a menos usada
     Remove P1p2 → Mem: [P2p1, P3p1, P3p2, P3p3]
     
PAGE FAULTS: 5
*/

void GerenciadorMemoria::substituirLRU(int id_pagina) {
    // Encontra qual página foi acessada há mais tempo
    auto pagina_para_remover = frames.front();
    auto menor_instante = ordem_lru[pagina_para_remover];

    // Procura entre todas as páginas em memória
    for (int pagina : frames) {
        // Se achou uma que foi acessada mais cedo
        if (ordem_lru[pagina] < menor_instante) {
            menor_instante = ordem_lru[pagina];
            pagina_para_remover = pagina;
        }
    }

    // Remove a página menos recentemente usada
    frames.erase(std::find(frames.begin(), frames.end(), pagina_para_remover));
    ordem_lru.erase(pagina_para_remover);

    // Adiciona a nova página
    frames.push_back(id_pagina);
    ordem_lru[id_pagina] = relogio_lru++;  // Registra tempo do novo acesso
}

// POLÍTICA 3: ÓTIMO (Optimal)
// 
// Como funciona:
// Remove a página que será ACESSADA MAIS TARDE NO FUTURO.

/*
t=0s: P1 chega → Mem: [P1p1, P1p2]
t=1s: P2 chega → Mem: [P1p1, P1p2, P2p1]
t=3s: P3 chega → Mem: [P1p1, P1p2, P2p1, P3p1] (4/4 CHEIO)

t=4s: P3 precisa p2 → P1p1 nunca mais será usado (P1 terminou)
     Remove P1p1 → Mem: [P1p2, P2p1, P3p1, P3p2]
     
t=5s: P3 precisa p3 → P1p2 nunca mais será usado
     Remove P1p2 → Mem: [P2p1, P3p1, P3p2, P3p3]
     
PAGE FAULTS: 5  */

//
void GerenciadorMemoria::substituirOtimo(int id_pagina, int uso_futuro) {
    if (uso_futuro == -1 || frames.empty()) {
        substituirFIFO(id_pagina);
        return;
    }

    // Encontra qual página será usada mais tarde
    int pagina_para_remover = frames.front();
    int maior_distancia = -1;

    // Procura entre todas as páginas em memória
    for (int pagina : frames) {
        // Calcula quando será usado (uso_futuro - último acesso)
        int distancia = uso_futuro - ordem_lru[pagina];
        // Se vai ser usado mais tarde que as outras
        if (distancia > maior_distancia) {
            maior_distancia = distancia;
            pagina_para_remover = pagina;
        }
    }

    // Remove a página que será usada mais tarde
    frames.erase(std::find(frames.begin(), frames.end(), pagina_para_remover));
    ordem_lru.erase(pagina_para_remover);

    // Adiciona a nova página
    frames.push_back(id_pagina);
    ordem_lru[id_pagina] = relogio_lru++;  // Registra tempo do novo acesso
}