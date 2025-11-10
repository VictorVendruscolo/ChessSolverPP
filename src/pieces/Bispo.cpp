#include "pieces/Bispo.hpp"
#include "core/Tabuleiro.hpp"

Bispo::Bispo(Posicao pos, Cor cor) : Peca(pos, cor, TipoPeca::BISPO, 'B') {
}

std::vector<Posicao> Bispo::getMovimentosPossiveis(const MTabuleiro& tabuleiro) const {
    std::vector<Posicao> movimentos;

    // 4 direções diagonais
    int direcoes[4][2] = { {-1, -1}, {-1, 1}, {1, -1}, {1, 1} };

    for (int d = 0; d < 4; ++d) {
        int dx = direcoes[d][0];
        int dy = direcoes[d][1];

        int novaLinha = posicao.linha + dx;
        int novaColuna = posicao.coluna + dy;

        while (novaLinha >= 0 && novaLinha < tabuleiro.getLinhas() &&
            novaColuna >= 0 && novaColuna < tabuleiro.getColunas()) {

            Posicao novaPosicao = { novaLinha, novaColuna };

            // Para se encontrar buraco
            if (tabuleiro.isBuraco(novaPosicao)) {
                break;
            }

            Peca* pecaDestino = tabuleiro.getPecaEm(novaPosicao);

            if (pecaDestino == nullptr) {
                movimentos.push_back(novaPosicao);
            }
            else if (pecaDestino->getCor() != this->cor) {
                movimentos.push_back(novaPosicao);
                break;
            }
            else {
                break;
            }

            novaLinha += dx;
            novaColuna += dy;
        }
    }

    return movimentos;
}
