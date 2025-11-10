#pragma once

#include <vector>
#include <memory>
#include <string>
#include "pieces/Peca.hpp"

class MTabuleiro {
private:
    std::vector<std::vector<std::unique_ptr<Peca>>> grade;
    std::vector<std::vector<bool>> buracos;

    int linhas, colunas;

public:
    // Construtor que aceita dimensão dinâmica
    MTabuleiro(int linhas = 8, int colunas = 8);
    ~MTabuleiro() = default;

    void adicionarBuraco(const Posicao& pos);     // Marca casa como buraco (vazia)
    void adicionarPeca(Peca* peca);               // Adiciona peça (gerencia ponteiro)

    // Construtor de cópia personalizado
    MTabuleiro(const MTabuleiro& outro);

    // Operador de atribuição personalizado
    MTabuleiro& operator=(const MTabuleiro& outro);

    void carregarDeArquivo(const std::string& caminhoDoArquivo);

    template<typename PecaTipo>
    void adicionarPeca(Posicao pos, Cor cor) {
        if (pos.linha >= 0 && pos.linha < linhas && pos.coluna >= 0 && pos.coluna < colunas) {
            grade[pos.linha][pos.coluna] = std::make_unique<PecaTipo>(pos, cor);
        }
    }

    int getLinhas() const { return linhas; }
    int getColunas() const { return colunas; }


    Peca* getPecaEm(Posicao pos) const;
    bool isPosicaoAtacada(Posicao pos, Cor corDaPecaAtacada) const;
    bool isBuraco(Posicao pos) const;
    void moverPeca(Posicao inicio, Posicao fim);
    void exibir() const;
};
