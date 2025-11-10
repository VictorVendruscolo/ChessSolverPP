#include "pieces/Pulga.hpp"
#include "core/Tabuleiro.hpp"

Pulga::Pulga(Posicao pos, Cor cor)
    : Peca(pos, cor, TipoPeca::CAVALO, 'F') {  // F de "Flea" (Pulga em inglês)
    //                      ^^^^^^^     ^
    //                      |           Símbolo: F (Flea/Pulga)
    //                      Reutiliza tipo CAVALO (não afeta lógica)
}

bool Pulga::temPecasAdjacentes(const Posicao& pos, const MTabuleiro& tabuleiro) const {
    // Verifica todas as 8 casas ao redor
    int direcoes[8][2] = {
        {-1, -1}, {-1, 0}, {-1, 1},
        {0, -1},           {0, 1},
        {1, -1},  {1, 0},  {1, 1}
    };

    for (int d = 0; d < 8; ++d) {
        int linhaAdj = pos.linha + direcoes[d][0];
        int colunaAdj = pos.coluna + direcoes[d][1];

        // Verifica se está dentro do tabuleiro
        if (linhaAdj >= 0 && linhaAdj < tabuleiro.getLinhas() &&
            colunaAdj >= 0 && colunaAdj < tabuleiro.getColunas()) {

            Posicao posAdj = { linhaAdj, colunaAdj };

            // Se tem peça adjacente (ignorando buracos), retorna true
            Peca* pecaAdj = tabuleiro.getPecaEm(posAdj);
            if (pecaAdj != nullptr) {
                return true;  // Tem peça adjacente!
            }
        }
    }

    return false;  // Nenhuma peça adjacente
}

std::vector<Posicao> Pulga::getMovimentosPossiveis(const MTabuleiro& tabuleiro) const {
    std::vector<Posicao> movimentos;

    // Itera por TODAS as posições do tabuleiro
    for (int i = 0; i < tabuleiro.getLinhas(); ++i) {
        for (int j = 0; j < tabuleiro.getColunas(); ++j) {
            // Não pode pular para a própria posição
            if (i == posicao.linha && j == posicao.coluna) {
                continue;
            }

            Posicao destino = { i, j };

            // Não pode pular para buraco
            if (tabuleiro.isBuraco(destino)) {
                continue;
            }

            // Verifica se destino tem peças adjacentes
            if (temPecasAdjacentes(destino, tabuleiro)) {
                continue;  // Destino tem peças ao redor, não pode!
            }

            // Verifica o que tem no destino
            Peca* pecaDestino = tabuleiro.getPecaEm(destino);

            if (pecaDestino == nullptr) {
                // Casa vazia e isolada - pode mover!
                movimentos.push_back(destino);
            }
            else if (pecaDestino->getCor() != this->cor) {
                // Peça inimiga ISOLADA - pode capturar!
                movimentos.push_back(destino);
            }
            // Se for peça aliada, não adiciona
        }
    }

    return movimentos;
}
