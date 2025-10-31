#pragma once

#include <vector>
#include <memory>
#include <string>
#include "pieces/Peca.hpp" 

// A classe agora se chama MTabuleiro
class MTabuleiro {
private:
    std::vector<std::vector<std::unique_ptr<Peca>>> grade;

public:
    // O construtor e o destrutor são atualizados para o novo nome
    MTabuleiro();
    ~MTabuleiro() = default;

    void carregarDeArquivo(const std::string& caminhoDoArquivo);
    //Funçao temporaria para testar o codigo por txt
    template<typename PecaTipo>
    void adicionarPeca(Posicao pos, Cor cor) {
        // Verifica se a posição é válida
        if (pos.linha >= 0 && pos.linha < 8 && pos.coluna >= 0 && pos.coluna < 8) {
            grade[pos.linha][pos.coluna] = std::make_unique<PecaTipo>(pos, cor);
        }
    }
    Peca* getPecaEm(Posicao pos) const;
    bool isPosicaoAtacada(Posicao pos, Cor corDaPecaAtacada) const;
    void moverPeca(Posicao inicio, Posicao fim);
    void exibir() const;
};