// Memory.h - Gerenciador de Memória Virtual com Paginação
#pragma once
#include <vector>
#include <deque>
#include <unordered_map>
#include <string>

// Enum: define a política de substituição de páginas
enum class PoliticaSubstituicao { 
    FIFO,      // Primeira a entrar, primeira a sair
    LRU,       // Remove a menos recentemente usada
    OTIMO      // Remove a que será usada mais tarde (teórico)
};

// Struct: registra eventos de memória para visualização
struct EventoMemoria {
    int tempo;                          // Instante do acesso (em unidades de tempo)
    int pagina_id;                      // ID interno da página acessada
    std::string pagina_nome;            // Nome visual (ex: "P1p1", "P2p3")
    bool page_fault;                    // Se houve page fault
    int faults_neste_evento;            // Quantos faults ocorreram neste acesso
    int pagina_removida_id;             // Qual página foi removida (-1 se nenhuma)
    std::string pagina_removida_nome;   // Nome da página removida
    std::string politica_usada;         // Qual política foi aplicada
    std::string descricao;              // Descrição do evento (ex: "P3 precisa p2 → Remove P1p1")
    std::vector<std::string> estado_memoria_nomes;  // Estado da memória com nomes (ex: ["P1p1", "P1p2", "__", "__"])
};

// Classe: simula como o SO gerencia a memória com páginas
class GerenciadorMemoria {
public:
    // Construtor: configura tamanho da memória física, tamanho de cada página, e política
    GerenciadorMemoria(int memoria_fisica_mb, int tamanho_pagina_mb,
                       PoliticaSubstituicao politica);

    // Simula acesso a uma página de memória com informação de processo
    // Retorna true se houve page fault (página não estava em memória física)
    bool acessarPagina(int pagina_id, int pid, int numero_pagina, int tempo, int uso_futuro = -1);
    
    // Retorna o número total de page faults que ocorreram
    int  totalPageFaults() const { return total_page_faults; }
    
    // Retorna o histórico de eventos de memória
    const std::vector<EventoMemoria>& obterHistorico() const { return historico; }
    
    // Retorna o estado atual da memória
    std::vector<int> obterEstadoAtual() const {
        return std::vector<int>(frames.begin(), frames.end());
    }

private:
    int num_frames;                              // Quantos frames (páginas) cabem na memória física
    PoliticaSubstituicao politica;               // Qual política usar
    int total_page_faults = 0;                   // Contador de page faults

    std::deque<int> frames;                      // Páginas atualmente na memória física
    std::unordered_map<int,int> ordem_lru;       // Guarda quando cada página foi último acessada
    int relogio_lru = 0;                         // Contador lógico de tempo para LRU
    std::vector<EventoMemoria> historico;        // Histórico de eventos
    int tempo_simulacao = 0;                     // Contador de tempo da simulação
    std::unordered_map<int, std::string> mapa_nomes_paginas;  // Mapeia ID de página para nome (ex: P1p1)

    // Métodos auxiliares
    bool estaNaMemoria(int id_pagina);           // Verifica se página está carregada
    void carregarPagina(int pagina_id, int pid, int numero_pagina, int tempo, int uso_futuro);  // Carrega nova página
    void substituirFIFO(int id_pagina);          // Remove usando política FIFO
    void substituirLRU(int id_pagina);           // Remove usando política LRU
    void substituirOtimo(int id_pagina, int uso_futuro);  // Remove usando política Ótimo
};