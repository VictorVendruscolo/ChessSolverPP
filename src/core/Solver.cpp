#include "core/Solver.hpp"
#include "core/Parser.hpp"
#include <iostream>

std::string Movimento::paraNotacao(int numLinhas) const {
    std::string notacao = "";

    // USA O SÍMBOLO, NÃO O TIPO!
    if (simbolo != 'P') {  // Peão não tem letra
        notacao += simbolo;
    }

    notacao += Parser::posicaoParaNotacao(origem, numLinhas);
    notacao += "-";
    notacao += Parser::posicaoParaNotacao(destino, numLinhas);

    return notacao;
}

ChessSolver::ChessSolver(const MTabuleiro& tab, int profundidade)
    : tabuleiro(tab), profundidadeMaxima(profundidade), iteracoes(0) {
}

bool ChessSolver::resolver() {
    solucao.clear();

    for (int prof = 0; prof <= profundidadeMaxima; ++prof) {
        std::cout << "Tentando profundidade " << prof << "..." << std::endl;
        iteracoes = 0;
        solucao.clear();

        if (backtrack(0, prof)) {
            std::cout << "Solucao encontrada em " << prof << " jogadas!" << std::endl;
            return true;
        }
    }

    return false;
}

bool ChessSolver::backtrack(int profundidade, int limiteAtual) {
    iteracoes++;

    if (iteracoes > MAX_ITERACOES) {
        std::cout << "\n!!! SOLUCAO MUITO COMPLEXA !!!" << std::endl;
        std::cout << "Limite de " << MAX_ITERACOES << " iteracoes atingido." << std::endl;
        return false;
    }

    if (profundidade >= limiteAtual) {
        return false;
    }

    std::vector<Movimento> movimentosBrancas = gerarTodosMovimentos(Cor::BRANCA);

    std::cout << "DEBUG [Prof " << profundidade << "]: " << movimentosBrancas.size()
        << " movimentos das brancas" << std::endl;

    for (const auto& movBranca : movimentosBrancas) {
        MTabuleiro backup = tabuleiro;
        tabuleiro.moverPeca(movBranca.origem, movBranca.destino);
        solucao.push_back(movBranca);

        std::cout << "DEBUG: Testando " << movBranca.paraNotacao(tabuleiro.getLinhas()) << std::endl;

        bool emXeque = isXeque(Cor::PRETA);
        std::cout << "  -> Rei em xeque? " << (emXeque ? "SIM" : "NAO") << std::endl;

        if (isXequeMate()) {
            std::cout << "  -> XEQUE-MATE DETECTADO!" << std::endl;
            return true;
        }

        try {
            Posicao posReiPreto = encontrarRei(Cor::PRETA);
            Peca* reiPreto = tabuleiro.getPecaEm(posReiPreto);

            if (reiPreto) {
                std::vector<Posicao> movimentosRei = reiPreto->getMovimentosPossiveis(tabuleiro);
                std::cout << "  -> Rei tem " << movimentosRei.size() << " movimentos possiveis" << std::endl;

                std::vector<Posicao> movimentosValidos;
                for (const auto& destRei : movimentosRei) {
                    MTabuleiro tempBackup = tabuleiro;
                    tabuleiro.moverPeca(posReiPreto, destRei);

                    // Verifica se fica em xeque
                    bool ficaEmXeque = isXeque(Cor::PRETA);

                    // Verifica se fica adjacente ao rei inimigo
                    bool adjacenteReiInimigo = false;
                    try {
                        Posicao posReiInimigo = encontrarRei(Cor::BRANCA);
                        int distLinha = abs(destRei.linha - posReiInimigo.linha);
                        int distColuna = abs(destRei.coluna - posReiInimigo.coluna);

                        if (distLinha <= 1 && distColuna <= 1) {
                            adjacenteReiInimigo = true;
                        }
                    }
                    catch (...) {
                        // Rei branco não existe
                    }

                    if (!ficaEmXeque && !adjacenteReiInimigo) {
                        movimentosValidos.push_back(destRei);
                    }

                    tabuleiro = tempBackup;
                }

                std::cout << "  -> Rei tem " << movimentosValidos.size() << " movimentos VALIDOS" << std::endl;

                if (emXeque && movimentosValidos.empty()) {
                    std::cout << "  -> XEQUE-MATE! (rei sem saida)" << std::endl;
                    return true;
                }

                for (const auto& destRei : movimentosValidos) {
                    MTabuleiro backup2 = tabuleiro;
                    tabuleiro.moverPeca(posReiPreto, destRei);
                    solucao.push_back({ posReiPreto, destRei, TipoPeca::REI, 'K'});

                    if (backtrack(profundidade + 1, limiteAtual)) {
                        return true;
                    }

                    solucao.pop_back();
                    tabuleiro = backup2;
                }
            }
        }
        catch (const std::exception& e) {
            // Silencioso
        }

        tabuleiro = backup;
        solucao.pop_back();
    }

    return false;
}

bool ChessSolver::isXequeMate() {
    try {
        if (!isXeque(Cor::PRETA)) {
            return false;
        }

        Posicao posRei = encontrarRei(Cor::PRETA);
        Peca* rei = tabuleiro.getPecaEm(posRei);

        if (!rei) {
            return false;
        }

        std::vector<Posicao> movimentos = rei->getMovimentosPossiveis(tabuleiro);

        for (const auto& dest : movimentos) {
            MTabuleiro tempBackup = tabuleiro;
            tabuleiro.moverPeca(posRei, dest);

            bool ficaEmXeque = isXeque(Cor::PRETA);

            // Verifica adjacência com rei inimigo
            bool adjacenteReiInimigo = false;
            try {
                Posicao posReiInimigo = encontrarRei(Cor::BRANCA);
                int distLinha = abs(dest.linha - posReiInimigo.linha);
                int distColuna = abs(dest.coluna - posReiInimigo.coluna);

                if (distLinha <= 1 && distColuna <= 1) {
                    adjacenteReiInimigo = true;
                }
            }
            catch (...) {}

            tabuleiro = tempBackup;

            if (!ficaEmXeque && !adjacenteReiInimigo) {
                return false;
            }
        }

        return true;
    }
    catch (...) {
        return false;
    }
}

bool ChessSolver::isXeque(Cor corDoRei) {
    try {
        Posicao posRei = encontrarRei(corDoRei);
        return tabuleiro.isPosicaoAtacada(posRei, corDoRei);
    }
    catch (...) {
        return false;
    }
}

Posicao ChessSolver::encontrarRei(Cor cor) const {
    for (int i = 0; i < tabuleiro.getLinhas(); ++i) {
        for (int j = 0; j < tabuleiro.getColunas(); ++j) {
            Peca* peca = tabuleiro.getPecaEm({ i, j });
            if (peca && peca->getTipo() == TipoPeca::REI && peca->getCor() == cor) {
                return { i, j };
            }
        }
    }
    throw std::runtime_error("Rei nao encontrado!");
}

std::vector<Movimento> ChessSolver::gerarTodosMovimentos(Cor cor) {
    std::vector<Movimento> movimentos;

    Posicao posReiAdversario;
    bool temReiAdversario = true;
    try {
        Cor corAdversaria = (cor == Cor::BRANCA) ? Cor::PRETA : Cor::BRANCA;
        posReiAdversario = encontrarRei(corAdversaria);
    }
    catch (...) {
        temReiAdversario = false;
    }

    for (int i = 0; i < tabuleiro.getLinhas(); ++i) {
        for (int j = 0; j < tabuleiro.getColunas(); ++j) {
            Peca* peca = tabuleiro.getPecaEm({ i, j });
            if (peca && peca->getCor() == cor) {
                auto destinos = peca->getMovimentosPossiveis(tabuleiro);

                for (const auto& dest : destinos) {
                    // Filtra movimentos do rei que ficam adjacentes ao rei adversário
                    if (peca->getTipo() == TipoPeca::REI && temReiAdversario) {
                        int distLinha = abs(dest.linha - posReiAdversario.linha);
                        int distColuna = abs(dest.coluna - posReiAdversario.coluna);

                        if (distLinha <= 1 && distColuna <= 1) {
                            continue;
                        }
                    }

                    movimentos.push_back({ {i, j}, dest, peca->getTipo(), peca->getSimbolo()});
                }
            }
        }
    }
    return movimentos;
}

void ChessSolver::salvarSolucao(const std::string& arquivo) const {
    std::vector<std::string> notacoes;
    int numLinhas = tabuleiro.getLinhas();

    for (const auto& mov : solucao) {
        notacoes.push_back(mov.paraNotacao(numLinhas));
    }

    Parser::salvarSolucao(arquivo, notacoes);
}

void ChessSolver::exibirSolucao() const {
    std::cout << "\nSolucao:" << std::endl;
    int numLinhas = tabuleiro.getLinhas();

    for (size_t i = 0; i < solucao.size(); ++i) {
        std::cout << (i + 1) << ". " << solucao[i].paraNotacao(numLinhas) << std::endl;
    }
}
