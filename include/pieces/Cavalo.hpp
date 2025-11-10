#pragma once

#include "Peca.hpp"
#include <vector>

class Cavalo : public Peca {
public:
    Cavalo(Posicao pos, Cor cor);

    std::unique_ptr<Peca> clone() const override {
        return std::make_unique<Cavalo>(*this);
    }

    std::vector<Posicao> getMovimentosPossiveis(const MTabuleiro& tabuleiro) const override;
};
