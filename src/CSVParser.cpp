// CSVParser.cpp - Implementação do leitor de CSV
#include "CSVParser.h"
#include <fstream>      // Para ler arquivo
#include <sstream>      // Para processar strings

// Função que lê um arquivo CSV com lista de processos
// O arquivo deve ter este formato (sem cabeçalho ou com):
// tempo_chegada,tempo_execucao,prioridade,memoria_necessaria
// Ex:
// 0,5,1,128
// 1,3,2,256
// 2,6,1,64
std::vector<Processo> LeitorCSV::ler(const std::string& caminho_arquivo) {
    std::vector<Processo> lista_processos;  // Lista que vamos preencher
    std::ifstream arquivo(caminho_arquivo);  // Abre o arquivo para leitura
    std::string linha;                       // Armazena cada linha lida
    int pid = 1;                             // Contador de ID dos processos

    // Pula o cabeçalho do arquivo (primeira linha)
    std::getline(arquivo, linha);

    // Lê cada linha do arquivo
    while (std::getline(arquivo, linha)) {
        std::stringstream ss(linha);  // Processa a linha como uma stream
        Processo p;                    // Cria novo processo
        char virgula;                  // Variável temporária para ler vírgulas

        // Atribui ID sequencial ao processo
        p.pid = pid++;
        
        // Lê os valores do CSV separados por vírgula
        ss >> p.tempo_chegada    >> virgula      // Lê: tempo_chegada,
           >> p.tempo_execucao   >> virgula      // Lê: tempo_execucao,
           >> p.prioridade       >> virgula      // Lê: prioridade,
           >> p.memoria_necessaria;              // Lê: memoria_necessaria

        // Inicializa o tempo restante com o tempo total de execução
        p.tempo_restante = p.tempo_execucao;
        
        // Adiciona o processo à lista
        lista_processos.push_back(p);
    }
    
    return lista_processos;  // Retorna todos os processos lidos
}