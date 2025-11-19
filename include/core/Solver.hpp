#pragma once

#include "core/Tabuleiro.hpp"
#include <string>
#include <vector>

struct Movimento {
    Posicao origem;
    Posicao destino;
    TipoPeca tipoPeca;
    char simbolo;

    std::string paraNotacao(int numLinhas) const;
};

class ChessSolver {
private:
    MTabuleiro tabuleiro;
    int profundidadeMaxima;
    std::vector<Movimento> solucao;
    int iteracoes;
    static const int MAX_ITERACOES = 1000000;

    bool backtrack(int profundidade, int limiteAtual);

    // REMOVE const dessas 3 funções
    bool isXequeMate();              // <- SEM const
    bool isXeque(Cor corDoRei);      // <- SEM const  
    Posicao encontrarRei(Cor cor) const;  // <- MANTÉM const (não modifica)

    std::vector<Movimento> gerarTodosMovimentos(Cor cor);

public:
    ChessSolver(const MTabuleiro& tab, int profundidade);
    const std::vector<Movimento>& getSolucao() const { return solucao; }
    bool resolver();
    void salvarSolucao(const std::string& arquivo) const;
    void exibirSolucao() const;
    const MTabuleiro& getTabuleiro() const { return tabuleiro; }
};
