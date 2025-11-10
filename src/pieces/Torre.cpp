#include "pieces/Torre.hpp"
#include "core/Tabuleiro.hpp"

Torre::Torre(Posicao pos, Cor cor) : Peca(pos, cor, TipoPeca::TORRE, 'R') {
}

std::vector<Posicao> Torre::getMovimentosPossiveis(const MTabuleiro& tabuleiro) const {
    std::vector<Posicao> movimentos;

    // Direções: cima, baixo, esquerda, direita
    int direcoes[4][2] = { {-1, 0}, {1, 0}, {0, -1}, {0, 1} };

    for (int d = 0; d < 4; ++d) {
        int dx = direcoes[d][0];
        int dy = direcoes[d][1];

        int novaLinha = posicao.linha + dx;
        int novaColuna = posicao.coluna + dy;

        // Continua na direção até sair do tabuleiro ou encontrar peça
        while (novaLinha >= 0 && novaLinha < tabuleiro.getLinhas() &&
            novaColuna >= 0 && novaColuna < tabuleiro.getColunas()) {

            Peca* pecaDestino = tabuleiro.getPecaEm({ novaLinha, novaColuna });

            // ADICIONE ESTA LINHA: Declara novaPosicao
            Posicao novaPosicao = { novaLinha, novaColuna };

            // Verifica se é buraco
            if (tabuleiro.isBuraco(novaPosicao)) {
                break;  // Para de avançar nesta direção
            }

            if (pecaDestino == nullptr) {
                // Casa vazia - pode mover
                movimentos.push_back({ novaLinha, novaColuna });
            }
            else if (pecaDestino->getCor() != this->cor) {
                // Peça inimiga - pode capturar e PARA
                movimentos.push_back({ novaLinha, novaColuna });
                break;  // IMPORTANTE!
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


