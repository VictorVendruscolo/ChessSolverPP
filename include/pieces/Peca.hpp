#pragma once

#include <vector>
#include <memory>

class MTabuleiro;

enum class Cor {
    BRANCA,
    PRETA,
    NENHUMA
};

enum class TipoPeca {
    REI,
    RAINHA,
    TORRE,
    BISPO,
    CAVALO,
    PEAO
};

struct Posicao {
    int linha;
    int coluna;
    bool operator==(const Posicao& other) const;
};

class Peca {
protected:
    Posicao posicao;
    Cor cor;
    TipoPeca tipo;
    char simbolo;
    bool jaMoveu;

public:
    Peca(Posicao pos, Cor cor, TipoPeca tipo, char simbolo);
    virtual ~Peca() = default;

 
    virtual std::unique_ptr<Peca> clone() const = 0;

    virtual std::vector<Posicao> getMovimentosPossiveis(const MTabuleiro& tabuleiro) const = 0;

    Posicao getPosicao() const;
    void setPosicao(Posicao novaPosicao);
    Cor getCor() const;
    TipoPeca getTipo() const;
    char getSimbolo() const;
    bool getJaMoveu() const;
};
