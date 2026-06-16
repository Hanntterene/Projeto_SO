// Memory.h - Gerenciador de Memória Virtual com Paginação
#pragma once
#include <vector>
#include <deque>
#include <unordered_map>

// Enum: define a política de substituição de páginas
enum class PoliticaSubstituicao { 
    FIFO,      // Primeira a entrar, primeira a sair
    LRU,       // Remove a menos recentemente usada
    OTIMO      // Remove a que será usada mais tarde (teórico)
};

// Classe: simula como o SO gerencia a memória com páginas
class GerenciadorMemoria {
public:
    // Construtor: configura tamanho da memória física, tamanho de cada página, e política
    GerenciadorMemoria(int memoria_fisica_mb, int tamanho_pagina_mb,
                       PoliticaSubstituicao politica);

    // Simula acesso a uma página de memória
    // Retorna true se houve page fault (página não estava em memória física)
    bool acessarPagina(int id_pagina, int uso_futuro = -1);
    
    // Retorna o número total de page faults que ocorreram
    int  totalPageFaults() const { return total_page_faults; }

private:
    int num_frames;                              // Quantos frames (páginas) cabem na memória física
    PoliticaSubstituicao politica;               // Qual política usar
    int total_page_faults = 0;                   // Contador de page faults

    std::deque<int> frames;                      // Páginas atualmente na memória física
    std::unordered_map<int,int> ordem_lru;       // Guarda quando cada página foi último acessada
    int relogio_lru = 0;                         // Contador lógico de tempo para LRU

    // Métodos auxiliares
    bool estaNaMemoria(int id_pagina);           // Verifica se página está carregada
    void carregarPagina(int id_pagina, int uso_futuro);  // Carrega nova página
    void substituirFIFO(int id_pagina);          // Remove usando política FIFO
    void substituirLRU(int id_pagina);           // Remove usando política LRU
    void substituirOtimo(int id_pagina, int uso_futuro);  // Remove usando política Ótimo
};