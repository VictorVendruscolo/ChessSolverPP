#pragma once

#include "pieces/Peca.hpp" // Inclui a definição da classe base

class Rei : public Peca {
public:

    Rei(Posicao pos, Cor cor);

    std::vector<Posicao> getMovimentosPossiveis(const MTabuleiro& tabuleiro) const override;
};