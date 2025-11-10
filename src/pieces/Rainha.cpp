#include "pieces/Rainha.hpp"
#include "core/Tabuleiro.hpp"

Rainha::Rainha(Posicao pos, Cor cor) : Peca(pos, cor, TipoPeca::RAINHA, 'Q') {
}

std::vector<Posicao> Rainha::getMovimentosPossiveis(const MTabuleiro& tabuleiro) const {
    std::vector<Posicao> movimentos;

    // 8 direções: 4 da torre + 4 do bispo
    int direcoes[8][2] = {
        {-1, 0}, {1, 0}, {0, -1}, {0, 1},      // Torre: cima, baixo, esquerda, direita
        {-1, -1}, {-1, 1}, {1, -1}, {1, 1}     // Bispo: diagonais
    };

    for (int d = 0; d < 8; ++d) {
        int dx = direcoes[d][0];
        int dy = direcoes[d][1];

        int novaLinha = posicao.linha + dx;
        int novaColuna = posicao.coluna + dy;

        // Continua na direção até sair do tabuleiro, encontrar peça ou buraco
        while (novaLinha >= 0 && novaLinha < tabuleiro.getLinhas() &&
            novaColuna >= 0 && novaColuna < tabuleiro.getColunas()) {

            Posicao novaPosicao = { novaLinha, novaColuna };

            // CRÍTICO: Para se encontrar buraco
            if (tabuleiro.isBuraco(novaPosicao)) {
                break;
            }

            Peca* pecaDestino = tabuleiro.getPecaEm(novaPosicao);

            if (pecaDestino == nullptr) {
                // Casa vazia - pode mover
                movimentos.push_back(novaPosicao);
            }
            else if (pecaDestino->getCor() != this->cor) {
                // Peça inimiga - pode capturar e PARA
                movimentos.push_back(novaPosicao);
                break;
            }
            else {
                // Peça aliada - PARA sem incluir
                break;
            }

            novaLinha += dx;
            novaColuna += dy;
        }
    }

    return movimentos;
}