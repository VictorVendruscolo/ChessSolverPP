#include "pieces/Rei.hpp"
#include "core/Tabuleiro.hpp"

Rei::Rei(Posicao pos, Cor cor) : Peca(pos, cor, TipoPeca::REI, 'K') {
}

std::vector<Posicao> Rei::getMovimentosPossiveis(const MTabuleiro& tabuleiro) const {
    std::vector<Posicao> movimentos;

    // 8 direções possíveis
    int direcoes[8][2] = {
        {-1, -1}, {-1, 0}, {-1, 1},
        {0, -1},           {0, 1},
        {1, -1},  {1, 0},  {1, 1}
    };

    for (int d = 0; d < 8; ++d) {
        int novaLinha = posicao.linha + direcoes[d][0];
        int novaColuna = posicao.coluna + direcoes[d][1];

        if (novaLinha >= 0 && novaLinha < tabuleiro.getLinhas() &&
            novaColuna >= 0 && novaColuna < tabuleiro.getColunas()) {

            Posicao novaPosicao = { novaLinha, novaColuna };

            // Não pode mover para buraco
            if (tabuleiro.isBuraco(novaPosicao)) {
                continue;
            }

            Peca* pecaDestino = tabuleiro.getPecaEm(novaPosicao);

            // Pode mover se vazio ou capturar inimigo
            if (pecaDestino == nullptr || pecaDestino->getCor() != this->cor) {
                movimentos.push_back(novaPosicao);
            }
        }
    }

    return movimentos;
}
