#include "core/Parser.hpp"
#include "core/PecaRegistry.hpp"  // <- ADICIONA
#include <fstream>
#include <sstream>
#include <iostream>

MTabuleiro Parser::carregarArquivo(const std::string& nomeArquivo, int& profundidade) {
    std::ifstream arquivo(nomeArquivo);
    if (!arquivo.is_open()) {
        throw std::runtime_error("Erro ao abrir arquivo: " + nomeArquivo);
    }

    int linhas, colunas;
    arquivo >> linhas >> colunas >> profundidade;

    std::cout << "Tabuleiro: " << linhas << "x" << colunas << std::endl;
    std::cout << "Profundidade: " << profundidade << " jogadas" << std::endl;

    MTabuleiro tabuleiro(linhas, colunas);

    std::string linhaBuracos;
    std::getline(arquivo, linhaBuracos);
    std::getline(arquivo, linhaBuracos);

    if (!linhaBuracos.empty()) {
        std::istringstream iss(linhaBuracos);
        int i, j;
        while (iss >> i >> j) {
            tabuleiro.adicionarBuraco({ i, j });
            std::cout << "Buraco em (" << i << "," << j << ")" << std::endl;
        }
    }

    bool primeiroRei = true;
    std::string linha;
    while (std::getline(arquivo, linha)) {
        if (linha.empty()) continue;

        std::istringstream iss(linha);
        std::string notacao;
        iss >> notacao;

        char tipoCor;
        std::string posicaoStr;

        if (notacao.length() == 2) {
            tipoCor = 'P';
            posicaoStr = notacao;
        }
        else if (notacao.length() == 3) {
            tipoCor = notacao[0];
            posicaoStr = notacao.substr(1);
        }
        else {
            std::cerr << "ERRO: Notacao invalida: " << notacao << std::endl;
            continue;
        }

        char colunaChar = posicaoStr[0];
        char linhaChar = posicaoStr[1];
        int coluna = colunaChar - 'a';
        int linhaPos = linhaChar - '1';

        Cor cor = std::isupper(tipoCor) ? Cor::BRANCA : Cor::PRETA;

        // USA REGISTRY!
        Peca* peca = PecaRegistry::getInstance().criar(tipoCor, { linhaPos, coluna }, cor);

        // DEBUG
        std::cout << "DEBUG: Tentando criar peca com simbolo '" << tipoCor << "'" << std::endl;

        if (!peca) {
            std::cerr << "ERRO: Tipo de peca nao registrado: " << tipoCor << std::endl;
            continue;
        }

        // DEBUG
        std::cout << "DEBUG: Peca criada com sucesso! Simbolo real: " << peca->getSimbolo() << std::endl;


        if (primeiroRei) {
            if (peca->getTipo() != TipoPeca::REI || cor != Cor::PRETA) {
                delete peca;
                throw std::runtime_error("Primeira peca deve ser rei preto!");
            }
            primeiroRei = false;
        }

        tabuleiro.adicionarPeca(peca);
    }

    arquivo.close();
    return tabuleiro;
}

std::string Parser::posicaoParaNotacao(const Posicao& pos, int numLinhas) {
    char coluna = 'a' + pos.coluna;
    char linha = '1' + pos.linha;
    return std::string(1, coluna) + linha;
}

void Parser::salvarSolucao(const std::string& nomeArquivo, const std::vector<std::string>& movimentos) {
    std::ofstream arquivo(nomeArquivo);

    if (!arquivo.is_open()) {
        std::cerr << "Erro ao criar arquivo de saida!" << std::endl;
        return;
    }

    for (const auto& mov : movimentos) {
        arquivo << mov << std::endl;
    }

    arquivo.close();
}
