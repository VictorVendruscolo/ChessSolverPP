#include "pieces/Peca.hpp"

Peca::Peca(Posicao pos, Cor cor, TipoPeca tipo, char simbolo)
    : posicao(pos), cor(cor), tipo(tipo), simbolo(simbolo), jaMoveu(false) {
}

bool Posicao::operator==(const Posicao& other) const {
    return this->linha == other.linha && this->coluna == other.coluna;
}

Posicao Peca::getPosicao() const {
    return posicao;
}

void Peca::setPosicao(Posicao novaPosicao) {
    posicao = novaPosicao;
    jaMoveu = true;
}

Cor Peca::getCor() const {
    return cor;
}

TipoPeca Peca::getTipo() const {
    return tipo;
}

char Peca::getSimbolo() const {
    return simbolo;
}

bool Peca::getJaMoveu() const {
    return jaMoveu;
}
