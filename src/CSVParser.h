// CSVParser.h
#pragma once
#include "Process.h"
#include <vector>
#include <string>

class LeitorCSV {
public:
    static std::vector<Processo> ler(const std::string& caminho_arquivo);
};