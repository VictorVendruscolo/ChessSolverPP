#pragma once

#include "Peca.hpp"
#include <vector>

class Torre : public Peca {
public:
    Torre(Posicao pos, Cor cor);

    std::unique_ptr<Peca> clone() const override {
        return std::make_unique<Torre>(*this);
    }

    std::vector<Posicao> getMovimentosPossiveis(const MTabuleiro& tabuleiro) const override;
};
