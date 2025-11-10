#pragma once

#include "Peca.hpp"
#include <vector>

class Rainha : public Peca {
public:
    Rainha(Posicao pos, Cor cor);

    std::unique_ptr<Peca> clone() const override {
        return std::make_unique<Rainha>(*this);
    }

    std::vector<Posicao> getMovimentosPossiveis(const MTabuleiro& tabuleiro) const override;
};
