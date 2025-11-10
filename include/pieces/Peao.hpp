#pragma once

#include "Peca.hpp"
#include <vector>

class Peao : public Peca {
public:
    Peao(Posicao pos, Cor cor);

    std::unique_ptr<Peca> clone() const override {
        return std::make_unique<Peao>(*this);
    }

    std::vector<Posicao> getMovimentosPossiveis(const MTabuleiro& tabuleiro) const override;
};
