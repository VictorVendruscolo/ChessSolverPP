#include "pieces/Peao.hpp"
#include "core/Tabuleiro.hpp"

Peao::Peao(Posicao pos, Cor cor) : Peca(pos, cor, TipoPeca::PEAO, 'P') {
}

std::vector<Posicao> Peao::getMovimentosPossiveis(const MTabuleiro& tabuleiro) const {
    std::vector<Posicao> movimentos;

    // Direção depende da cor (brancas sobem, pretas descem)
    int direcao = (cor == Cor::BRANCA) ? 1 : -1;

    // Movimento para frente (1 casa)
    int novaLinha = posicao.linha + direcao;
    int novaColuna = posicao.coluna;

    if (novaLinha >= 0 && novaLinha < tabuleiro.getLinhas()) {
        Posicao frentePosicao = { novaLinha, novaColuna };

        // Não pode mover para buraco
        if (!tabuleiro.isBuraco(frentePosicao)) {
            Peca* pecaFrente = tabuleiro.getPecaEm(frentePosicao);

            // Só pode avançar se a casa estiver vazia
            if (pecaFrente == nullptr) {
                movimentos.push_back(frentePosicao);
            }
        }
    }

    // Capturas nas diagonais
    int diagonais[2][2] = { {direcao, -1}, {direcao, 1} };

    for (int i = 0; i < 2; ++i) {
        int diagLinha = posicao.linha + diagonais[i][0];
        int diagColuna = posicao.coluna + diagonais[i][1];

        if (diagLinha >= 0 && diagLinha < tabuleiro.getLinhas() &&
            diagColuna >= 0 && diagColuna < tabuleiro.getColunas()) {

            Posicao diagPosicao = { diagLinha, diagColuna };

            // Não pode capturar em buraco
            if (tabuleiro.isBuraco(diagPosicao)) {
                continue;
            }

            Peca* pecaDiagonal = tabuleiro.getPecaEm(diagPosicao);

            // Só pode capturar se tiver peça inimiga
            if (pecaDiagonal != nullptr && pecaDiagonal->getCor() != this->cor) {
                movimentos.push_back(diagPosicao);
            }
        }
    }

    return movimentos;
}
