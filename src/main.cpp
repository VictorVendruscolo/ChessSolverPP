#include <iostream>
#include <string>
#include "core/Tabuleiro.hpp"
#include "core/Solver.hpp"
#include "core/Parser.hpp"
#include "core/PecaRegistry.hpp"  // <- ADICIONA

// Includes das peças padrão
#include "pieces/Rei.hpp"
#include "pieces/Rainha.hpp"
#include "pieces/Torre.hpp"
#include "pieces/Bispo.hpp"
#include "pieces/Cavalo.hpp"
#include "pieces/Peao.hpp"
#include "pieces/Pulga.hpp"

// ============================================
// ÁREA DE EXTENSÃO - REGISTRAR PEÇAS
// ============================================
void inicializarPecas() {
    REGISTRAR_PECA('K', Rei);
    REGISTRAR_PECA('Q', Rainha);
    REGISTRAR_PECA('R', Torre);
    REGISTRAR_PECA('B', Bispo);
    REGISTRAR_PECA('N', Cavalo);
    REGISTRAR_PECA('P', Peao);
    REGISTRAR_PECA('F', Pulga);

    // Para adicionar novas peças:
    // 1. Crie nomepeça.hpp/cpp herdando de Peca
    // 2. Adicione aqui: REGISTRAR_PECA('A', Arqueiro); (simbulo e nome diferente dos padroes)
    // 3. Recompile
}
// ============================================

int main(int argc, char* argv[]) {
    std::cout << "=== CHESS SOLVER - TRABALHO PP ===" << std::endl;

    // DEBUG - Registra peças
    std::cout << "DEBUG: Registrando pecas..." << std::endl;
    inicializarPecas();
    std::cout << "DEBUG: Pecas registradas!" << std::endl;

    // DEBUG - Testa se F foi registrado
    Peca* teste = PecaRegistry::getInstance().criar('F', { 0, 0 }, Cor::BRANCA);
    if (teste) {
        std::cout << "DEBUG: Pulga (F) foi registrada com sucesso! Simbolo: "
            << teste->getSimbolo() << std::endl;
        delete teste;
    }
    else {
        std::cout << "DEBUG: ERRO! Pulga (F) NAO foi registrada!" << std::endl;
    }





    std::string arquivoEntrada = "tests/mate_em_2.pcin";

    if (argc >= 2) {
        arquivoEntrada = argv[1];
    }

    std::cout << "Arquivo: " << arquivoEntrada << std::endl;
    std::cout << std::endl;

    try {
        int profundidadeMaxima = 0;

        // Carrega tabuleiro E profundidade do arquivo
        MTabuleiro tabuleiro = Parser::carregarArquivo(arquivoEntrada, profundidadeMaxima);

        // Valida se profundidade foi lida corretamente
        if (profundidadeMaxima <= 0) {
            std::cerr << "ERRO: Profundidade invalida no arquivo!" << std::endl;
            return 1;
        }

        std::cout << "Profundidade: " << profundidadeMaxima << " jogadas" << std::endl;
        std::cout << "Carregando posicao inicial..." << std::endl;
        tabuleiro.exibir();
        std::cout << std::endl;

        ChessSolver solver(tabuleiro, profundidadeMaxima);

        std::cout << "Buscando solucao..." << std::endl;
        bool encontrou = solver.resolver();

        if (encontrou) {
            std::cout << "\n=== SOLUCAO ENCONTRADA! ===" << std::endl;

            std::string arquivoSaida = arquivoEntrada;
            size_t pos = arquivoSaida.find(".pcin");
            if (pos != std::string::npos) {
                arquivoSaida.replace(pos, 5, ".pcout");
            }
            else {
                arquivoSaida += ".pcout";
            }

            solver.salvarSolucao(arquivoSaida);
            std::cout << "Salvo em: " << arquivoSaida << std::endl;

            solver.exibirSolucao();

            std::cout << "\nTabuleiro final:" << std::endl;
            solver.getTabuleiro().exibir();
        }
        else {
            std::cout << "\n=== SOLUCAO NAO ENCONTRADA ===" << std::endl;
            std::cout << "Nao ha mate em " << profundidadeMaxima << " jogadas." << std::endl;
        }
    }
    catch (const std::exception& e) {
        std::cerr << "\nERRO: " << e.what() << std::endl;
        return 1;
    }

    std::cout << "\n=== FINALIZADO ===" << std::endl;
    return 0;
}
