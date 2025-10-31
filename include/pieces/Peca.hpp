#pragma once

#include <vector>

// Forward declaration para evitar inclusões circulares com Tabuleiro.hpp
class MTabuleiro;

// Enumerações e Estruturas que fazem parte da interface pública da Peça.
enum class Cor {
    BRANCA,
    PRETA,
    NENHUMA
};

enum class TipoPeca {
    REI,
    DAMA,
    TORRE,
    BISPO,
    CAVALO,
    PEAO,
    NENHUMA
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
    bool jaMoveu;

public:

    Peca(Posicao pos, Cor cor, TipoPeca tipo);

    virtual ~Peca() = default;

    virtual std::vector<Posicao> getMovimentosPossiveis(const MTabuleiro& tabuleiro) const = 0;

    Posicao getPosicao() const;
    void setPosicao(Posicao novaPosicao);
    Cor getCor() const;
    TipoPeca getTipo() const;
    bool getJaMoveu() const;
};
