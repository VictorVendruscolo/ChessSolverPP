#include "pieces/Peca.hpp" 
#include "core/Tabuleiro.hpp" 

Peca::Peca(Posicao pos, Cor cor, TipoPeca tipo)
    : posicao(pos), cor(cor), tipo(tipo), jaMoveu(false) {
}

bool Posicao::operator==(const Posicao& other) const {
    return linha == other.linha && coluna == other.coluna;
}

Posicao Peca::getPosicao() const {
    return this->posicao;
}

void Peca::setPosicao(Posicao novaPosicao) {
    this->posicao = novaPosicao;
    this->jaMoveu = true;
}

Cor Peca::getCor() const {
    return this->cor;
}

TipoPeca Peca::getTipo() const {
    return this->tipo;
}

bool Peca::getJaMoveu() const {
    return this->jaMoveu;
}

