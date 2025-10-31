#include "core/Tabuleiro.hpp" 
#include "pieces/Rei.hpp"
#include <iostream>
#include <fstream>
#include <stdexcept>

MTabuleiro::MTabuleiro() {
    grade.resize(8);
    for (int i = 0; i < 8; ++i) {
        grade[i].resize(8, nullptr);
    }
}

void MTabuleiro::carregarDeArquivo(const std::string& caminhoDoArquivo) {
    std::ifstream arquivo(caminhoDoArquivo);
    if (!arquivo.is_open()) {
        throw std::runtime_error("Nao foi possivel abrir o arquivo de teste: " + caminhoDoArquivo);
    }

    for (int i = 0; i < 8; ++i) {
        for (int j = 0; j < 8; ++j) {
            grade[i][j].reset();
        }
    }

    char tipoChar;
    int linha, coluna;

    while (arquivo >> tipoChar >> linha >> coluna) {
        Posicao pos = { linha, coluna };
        Cor cor = isupper(tipoChar) ? Cor::BRANCA : Cor::PRETA;

        if (cor == Cor::PRETA && tolower(tipoChar) != 'k') {
            continue;
        }

        switch (tolower(tipoChar)) {
        case 'k': // Rei
            grade[linha][coluna] = std::make_unique<Rei>(pos, cor);
            break;
        default:
            break;
        }
    }
}

Peca* MTabuleiro::getPecaEm(Posicao pos) const {
    if (pos.linha < 0 || pos.linha >= 8 || pos.coluna < 0 || pos.coluna >= 8) {
        return nullptr;
    }
    return grade[pos.linha][pos.coluna].get();
}

bool MTabuleiro::isPosicaoAtacada(Posicao pos, Cor corDaPecaAtacada) const {
    Cor corAtacante = (corDaPecaAtacada == Cor::BRANCA) ? Cor::PRETA : Cor::BRANCA;

    for (int i = 0; i < 8; ++i) {
        for (int j = 0; j < 8; ++j) {
            Peca* peca = getPecaEm({ i, j });

            if (peca != nullptr && peca->getCor() == corAtacante) {
                std::vector<Posicao> movimentos = peca->getMovimentosPossiveis(*this);
                for (const auto& movimento : movimentos) {
                    if (movimento == pos) {
                        return true;
                    }
                }
            }
        }
    }
    return false;
}

void MTabuleiro::moverPeca(Posicao inicio, Posicao fim) {
    if (getPecaEm(inicio) != nullptr) {
        grade[fim.linha][fim.coluna] = std::move(grade[inicio.linha][inicio.coluna]);
        if (getPecaEm(fim) != nullptr) {
            getPecaEm(fim)->setPosicao(fim);
        }
    }
}

void MTabuleiro::exibir() const {
    std::cout << "  -----------------" << std::endl;
    for (int i = 0; i < 8; ++i) {
        std::cout << i << " | ";
        for (int j = 0; j < 8; ++j) {
            Peca* p = getPecaEm({ i, j });
            if (p == nullptr) {
                std::cout << ". ";
            }
            else {
                char c = ' ';
                switch (p->getTipo()) {
                case TipoPeca::REI: c = 'k'; break;
                default: c = '?'; break;
                }
                if (p->getCor() == Cor::BRANCA) {
                    c = toupper(c);
                }
                std::cout << c << " ";
            }
        }
        std::cout << "|" << std::endl;
    }
    std::cout << "  -----------------" << std::endl;
    std::cout << "    0 1 2 3 4 5 6 7" << std::endl;
}