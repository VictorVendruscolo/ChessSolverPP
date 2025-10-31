#include <iostream>
// CORREÇÃO:
// Inclui o arquivo que está na subpasta "core" e tem o nome "_Tabuleiro.hpp"
#include "core/Tabuleiro.hpp" 

// CORREÇÃO:
// Inclui o arquivo que está na subpasta "core/pecas" e tem o nome "_Rei.hpp"
#include "pieces/Rei.hpp"

int main() {
    // 1. Criação do objeto do tabuleiro
    MTabuleiro meuJogo;

    // 2. Configuração do cenário de teste (sem usar arquivo)
    // Vamos adicionar um Rei branco, um Rei preto e uma Torre branca.
    // (Para a Torre funcionar, você precisaria ter a classe Torre criada)

    std::cout << "Configurando tabuleiro para teste..." << std::endl;
    meuJogo.adicionarPeca<Rei>({ 0, 4 }, Cor::BRANCA); // Rei branco em e1
    meuJogo.adicionarPeca<Rei>({ 7, 4 }, Cor::PRETA);  // Rei preto em e8
    // meuJogo.adicionarPeca<Torre>({0, 0}, Cor::BRANCA); // Exemplo: Torre branca em a1

    // 3. Exibição do tabuleiro inicial
    std::cout << "Estado inicial do tabuleiro:" << std::endl;
    meuJogo.exibir();
    std::cout << std::endl;

    // 4. Realização de um teste lógico
    // Vamos testar a função getMovimentosPossiveis do Rei branco.
    Posicao posReiBranco = { 0, 4 };
    Peca* reiBranco = meuJogo.getPecaEm(posReiBranco);

    if (reiBranco != nullptr) {
        std::cout << "Testando os movimentos possiveis do Rei branco em (0, 4):" << std::endl;

        // A mágica do polimorfismo acontece aqui!
        std::vector<Posicao> movimentos = reiBranco->getMovimentosPossiveis(meuJogo);

        if (movimentos.empty()) {
            std::cout << "O Rei branco nao pode se mover." << std::endl;
        }
        else {
            std::cout << "Movimentos validos encontrados: " << movimentos.size() << std::endl;
            for (const auto& pos : movimentos) {
                std::cout << "- Pode mover para (" << pos.linha << ", " << pos.coluna << ")" << std::endl;
            }
        }
    }
    else {
        std::cout << "Erro: Nao foi encontrado um Rei branco na posicao (0, 4)." << std::endl;
    }

    return 0;
}
