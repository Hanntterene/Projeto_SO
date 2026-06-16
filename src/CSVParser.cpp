// CSVParser.cpp
#include "CSVParser.h"
#include <fstream>
#include <sstream>

std::vector<Processo> LeitorCSV::ler(const std::string& caminho_arquivo) {
    std::vector<Processo> lista_processos;
    std::ifstream arquivo(caminho_arquivo);
    std::string linha;
    int pid = 1;

    std::getline(arquivo, linha); // pula o cabeçalho

    while (std::getline(arquivo, linha)) {
        std::stringstream ss(linha);
        Processo p;
        char virgula;

        p.pid = pid++;
        ss >> p.tempo_chegada    >> virgula
           >> p.tempo_execucao   >> virgula
           >> p.prioridade       >> virgula
           >> p.memoria_necessaria;

        p.tempo_restante = p.tempo_execucao; // começa com o burst completo
        lista_processos.push_back(p);
    }
    return lista_processos;
}