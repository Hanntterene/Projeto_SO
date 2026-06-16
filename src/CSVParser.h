// CSVParser.h - Leitor de arquivos CSV
#pragma once
#include "Process.h"
#include <vector>
#include <string>

// Classe responsável por ler o arquivo CSV com lista de processos
class LeitorCSV {
public:
    // Lê um arquivo CSV e retorna um vetor com todos os processos
    // Formato esperado do CSV:
    // tempo_chegada, tempo_execucao, prioridade, memoria_necessaria
    static std::vector<Processo> ler(const std::string& caminho_arquivo);
};