#include "pieces/Rei.hpp"
#include "core/Tabuleiro.hpp" // Necessário para interagir com o tabuleiro e validar movimentos

Rei::Rei(Posicao pos, Cor cor)
    : Peca(pos, cor, TipoPeca::REI) {
    // O corpo do construtor pode ser vazio.
}

std::vector<Posicao> Rei::getMovimentosPossiveis(const MTabuleiro& tabuleiro) const {
    std::vector<Posicao> movimentos;

    for (int i = -1; i <= 1; ++i) {
        for (int j = -1; j <= 1; ++j) {
            // Ignora o movimento para a própria casa (0,0)
            if (i == 0 && j == 0) {
                continue;
            }

            Posicao alvo = { this->posicao.linha + i, this->posicao.coluna + j };

            // --- Validação do Movimento ---

            // 1. O alvo está dentro do tabuleiro 8x8? [cite: 5]
            if (alvo.linha >= 0 && alvo.linha < 8 && alvo.coluna >= 0 && alvo.coluna < 8) {

                // 2. O alvo não está ocupado por uma peça da mesma cor?
                Peca* pecaNoAlvo = tabuleiro.getPecaEm(alvo);
                if (pecaNoAlvo != nullptr && pecaNoAlvo->getCor() == this->cor) {
                    continue; // Pula este movimento, pois é uma peça aliada.
                }

                // 3. O movimento não resulta em suicídio? 
                // A posição alvo não pode estar sob ataque do oponente.
                // (Assumimos que você criará uma função no Tabuleiro para verificar isso)
                if (tabuleiro.isPosicaoAtacada(alvo, this->cor)) {
                    continue; // Pula este movimento, é suicídio!
                }

                // Se passou por todas as validações, o movimento é válido.
                movimentos.push_back(alvo);
            }
        }
    }

    return movimentos;
}