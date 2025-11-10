#pragma once

#include "Peca.hpp"
#include <vector>

class MTabuleiro;

class Pulga : public Peca {
public:
    Pulga(Posicao pos, Cor cor);

    std::unique_ptr<Peca> clone() const override {
        return std::make_unique<Pulga>(*this);
    }

    std::vector<Posicao> getMovimentosPossiveis(const MTabuleiro& tabuleiro) const override;

private:
    bool temPecasAdjacentes(const Posicao& pos, const MTabuleiro& tabuleiro) const;
};