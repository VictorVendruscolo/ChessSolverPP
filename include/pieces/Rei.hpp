#pragma once

#include "Peca.hpp"
#include <vector>

class Rei : public Peca {
public:
    Rei(Posicao pos, Cor cor);

    std::unique_ptr<Peca> clone() const override {
        return std::make_unique<Rei>(*this);
    }

    std::vector<Posicao> getMovimentosPossiveis(const MTabuleiro& tabuleiro) const override;
};
