#include "pieces/Cavalo.hpp"
#include "core/Tabuleiro.hpp"

Cavalo::Cavalo(Posicao pos, Cor cor) : Peca(pos, cor, TipoPeca::CAVALO, 'N') {
}

std::vector<Posicao> Cavalo::getMovimentosPossiveis(const MTabuleiro& tabuleiro) const {
    std::vector<Posicao> movimentos;

    // 8 movimentos em L do cavalo
    int movimentosL[8][2] = {
        {-2, -1}, {-2, 1}, {-1, -2}, {-1, 2},
        {1, -2}, {1, 2}, {2, -1}, {2, 1}
    };

    for (int i = 0; i < 8; ++i) {
        int novaLinha = posicao.linha + movimentosL[i][0];
        int novaColuna = posicao.coluna + movimentosL[i][1];

        // Verifica limites do tabuleiro
        if (novaLinha >= 0 && novaLinha < tabuleiro.getLinhas() &&
            novaColuna >= 0 && novaColuna < tabuleiro.getColunas()) {

            Posicao novaPosicao = { novaLinha, novaColuna };

            // Não pode pousar em buraco
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
