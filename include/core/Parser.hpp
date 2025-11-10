#pragma once

#include "core/Tabuleiro.hpp"
#include <string>
#include <vector>

class Parser {
public:
    static MTabuleiro carregarArquivo(const std::string& nomeArquivo, int& profundidade);
    static std::string posicaoParaNotacao(const Posicao& pos, int numLinhas);
    static void salvarSolucao(const std::string& nomeArquivo, const std::vector<std::string>& movimentos);
};
