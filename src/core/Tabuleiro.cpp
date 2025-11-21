#include "core/Tabuleiro.hpp"
#include "pieces/Rei.hpp"
#include "pieces/Rainha.hpp"
#include "pieces/Torre.hpp"
#include "pieces/Bispo.hpp"
#include "pieces/Cavalo.hpp"
#include "pieces/Peao.hpp"
#include <iostream>
#include <fstream>
#include <stdexcept>

MTabuleiro::MTabuleiro(int linhas, int colunas) : linhas(linhas), colunas(colunas) {
    grade.resize(linhas);
    buracos.resize(linhas);
    for (int i = 0; i < linhas; ++i) {
        grade[i].resize(colunas);
        buracos[i].resize(colunas, false);
    }
}

void MTabuleiro::adicionarBuraco(const Posicao& pos) {
    if (pos.linha >= 0 && pos.linha < linhas &&
        pos.coluna >= 0 && pos.coluna < colunas) {
        grade[pos.linha][pos.coluna].reset(); // remove peça caso existisse
        buracos[pos.linha][pos.coluna] = true; // marca buraco
    }
}


void MTabuleiro::adicionarPeca(Peca* peca) {
    if (!peca) return;
    Posicao pos = peca->getPosicao();
    if (pos.linha >= 0 && pos.linha < linhas &&
        pos.coluna >= 0 && pos.coluna < colunas) {
        grade[pos.linha][pos.coluna].reset(peca); // Responsável pelo gerenciamento da peça
    }
    else {
        throw std::runtime_error("Posicao fora dos limites ao adicionar peça");
    }
}

Peca* MTabuleiro::getPecaEm(Posicao pos) const {
    if (pos.linha < 0 || pos.linha >= linhas || pos.coluna < 0 || pos.coluna >= colunas)
        return nullptr;
    if (buracos[pos.linha][pos.coluna])
        return nullptr; // casa buraco não contém peça
    return grade[pos.linha][pos.coluna].get();
}


bool MTabuleiro::isPosicaoAtacada(Posicao pos, Cor corDaPecaAtacada) const {
    Cor corAtacante = (corDaPecaAtacada == Cor::BRANCA) ? Cor::PRETA : Cor::BRANCA;

    for (int i = 0; i < linhas; ++i) {
        for (int j = 0; j < colunas; ++j) {
            Peca* peca = getPecaEm({ i, j });
            if (peca != nullptr && peca->getCor() == corAtacante) {

                // Tratamento especial para REI (não chama getMovimentosPossiveis para evitar recursão)
                if (peca->getTipo() == TipoPeca::REI) {
                    // Verifica se está a 1 casa de distância (ataque do rei)
                    int distLinha = abs(peca->getPosicao().linha - pos.linha);
                    int distColuna = abs(peca->getPosicao().coluna - pos.coluna);

                    if (distLinha <= 1 && distColuna <= 1 && (distLinha + distColuna) > 0) {
                        return true;  // Está no alcance do rei
                    }
                }
                else {
                    // Para outras peças, usa getMovimentosPossiveis normalmente
                    std::vector<Posicao> movimentos = peca->getMovimentosPossiveis(*this);
                    for (const auto& movimento : movimentos) {
                        if (movimento == pos) {
                            return true;
                        }
                    }
                }
            }
        }
    }
    return false;
}


bool MTabuleiro::isBuraco(Posicao pos) const {
    if (pos.linha < 0 || pos.linha >= linhas ||
        pos.coluna < 0 || pos.coluna >= colunas) {
        return false;
    }
    return buracos[pos.linha][pos.coluna];
}


void MTabuleiro::moverPeca(Posicao inicio, Posicao fim) {
    
    if (inicio.linha < 0 || inicio.linha >= linhas || inicio.coluna < 0 || inicio.coluna >= colunas) return;
    if (fim.linha < 0 || fim.linha >= linhas || fim.coluna < 0 || fim.coluna >= colunas) return;

    
    if (getPecaEm(inicio) != nullptr) {
        grade[fim.linha][fim.coluna] = std::move(grade[inicio.linha][inicio.coluna]);
        Peca* p = getPecaEm(fim);
        if (p != nullptr) {
            p->setPosicao(fim);
        }
    }
}

void MTabuleiro::exibir() const {
    // Linha superior com letras das colunas
    std::cout << "   ";
    for (int j = 0; j < colunas; ++j) {
        std::cout << " " << (char)('a' + j);
    }
    std::cout << std::endl;

    std::cout << "  +";
    for (int j = 0; j < colunas; ++j) {
        std::cout << "--";
    }
    std::cout << "+" << std::endl;

    // Exibe de cima para baixo
    for (int i = linhas - 1; i >= 0; --i) {
        std::cout << (i + 1) << " |";

        for (int j = 0; j < colunas; ++j) {
            if (buracos[i][j]) {
                std::cout << " #";
            }
            else {
                Peca* p = getPecaEm({ i, j });
                if (p == nullptr) {
                    std::cout << " .";
                }
                else {
                    // USA getSimbolo() EM VEZ DE getTipo()!
                    char c = p->getSimbolo();

                    if (p->getCor() == Cor::PRETA) {
                        c = tolower(c);
                    }
                    std::cout << " " << c;
                }
            }
        }
        std::cout << " |" << std::endl;
    }

    std::cout << "  +";
    for (int j = 0; j < colunas; ++j) {
        std::cout << "--";
    }
    std::cout << "+" << std::endl;

    // Linha inferior com letras das colunas
    std::cout << "   ";
    for (int j = 0; j < colunas; ++j) {
        std::cout << " " << (char)('a' + j);
    }
    std::cout << std::endl;
}




MTabuleiro::MTabuleiro(const MTabuleiro& outro) : linhas(outro.linhas), colunas(outro.colunas) {
    grade.resize(linhas);
    buracos.resize(linhas);

    for (int i = 0; i < linhas; ++i) {
        grade[i].resize(colunas);
        buracos[i] = outro.buracos[i];

        for (int j = 0; j < colunas; ++j) {
            Peca* peca = outro.getPecaEm({ i, j });
            if (peca) {
                grade[i][j] = peca->clone();  // Polimorfismo vibes
            }
        }
    }
}


MTabuleiro& MTabuleiro::operator=(const MTabuleiro& outro) {
    if (this != &outro) {
        linhas = outro.linhas;
        colunas = outro.colunas;
        grade.clear();
        buracos.clear();

        grade.resize(linhas);
        buracos.resize(linhas);

        for (int i = 0; i < linhas; ++i) {
            grade[i].resize(colunas);
            buracos[i] = outro.buracos[i];

            for (int j = 0; j < colunas; ++j) {
                Peca* peca = outro.getPecaEm({ i, j });
                if (peca) {
                    grade[i][j] = peca->clone();  // Polimorfismo vibes pt.2
                }
            }
        }
    }
    return *this;
}



