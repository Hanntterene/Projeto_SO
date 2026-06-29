// CSVParser.cpp - implementação do leitor de arquivo CSV
#include "CSVParser.h"
#include <fstream>      // Para abrir e ler o arquivo
#include <sstream>      // Para separar cada linha por vírgula

// Essa função lê o arquivo de processos e monta um vetor com todos eles.
// O CSV deve ter cabeçalho e depois cada linha com:
// chegada,burst,prioridade,memoria
// Exemplo:
// 0,5,1,128
// 1,3,2,256
// 2,6,1,64
std::vector<Processo> LeitorCSV::ler(const std::string& caminho_arquivo) {
    std::vector<Processo> lista_processos;  // Vai guardar todos os processos do CSV
    std::ifstream arquivo(caminho_arquivo);  // Abre o arquivo para leitura
    std::string linha;                       // Linha atual que estamos lendo
    int pid = 1;                             // Número do processo gerado automaticamente

    // Pula o cabeçalho, ele não tem dados de processo.
    std::getline(arquivo, linha);

    // Lê o arquivo linha a linha até acabar.
    while (std::getline(arquivo, linha)) {
        std::stringstream ss(linha);  // Cria uma stream para separar os campos
        Processo p;                    // Novo processo que vamos preencher
        char virgula;                  // Leio as vírgulas entre os valores

        // Cada processo ganha um PID sequencial.
        p.pid = pid++;
        
        // Aqui a linha do CSV é convertida em valores do processo.
        ss >> p.tempo_chegada    >> virgula  // tempo de chegada,
           >> p.tempo_execucao   >> virgula  // tempo de execução,
           >> p.prioridade       >> virgula  // prioridade,
           >> p.memoria_necessaria;          // memória necessária

        // O tempo restante começa igual ao tempo total necessário.
        p.tempo_restante = p.tempo_execucao;
        
        // Salva o processo na lista final.
        lista_processos.push_back(p);
    }
    
    return lista_processos;  // Retorna tudo que foi lido do CSV.
}