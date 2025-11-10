#pragma once

#include "Peca.hpp"
#include <vector>

class Bispo : public Peca {
public:
    Bispo(Posicao pos, Cor cor);

    std::unique_ptr<Peca> clone() const override {
        return std::make_unique<Bispo>(*this);
    }

    std::vector<Posicao> getMovimentosPossiveis(const MTabuleiro& tabuleiro) const override;
};
