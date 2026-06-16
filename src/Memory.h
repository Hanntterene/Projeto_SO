// Memory.h
#pragma once
#include <vector>
#include <deque>
#include <unordered_map>

enum class PoliticaSubstituicao { FIFO, LRU, OTIMO };

class GerenciadorMemoria {
public:
    GerenciadorMemoria(int memoria_fisica_mb, int tamanho_pagina_mb,
                       PoliticaSubstituicao politica);

    // retorna true se houve page fault
    bool acessarPagina(int id_pagina, int uso_futuro = -1);
    int  totalPageFaults() const { return total_page_faults; }

private:
    int num_frames;                          // quantos frames cabem na memória física
    PoliticaSubstituicao politica;
    int total_page_faults = 0;

    std::deque<int> frames;                  // páginas atualmente na memória
    std::unordered_map<int,int> ordem_lru;  // página → instante do último acesso
    int relogio_lru = 0;

    bool estaNaMemoria(int id_pagina);
    void carregarPagina(int id_pagina, int uso_futuro);
    void substituirFIFO(int id_pagina);
    void substituirLRU(int id_pagina);
    void substituirOtimo(int id_pagina, int uso_futuro);
};