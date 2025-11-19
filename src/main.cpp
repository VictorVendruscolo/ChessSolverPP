#include <iostream>
#include <string>

#include <SFML/Graphics.hpp>              // <- para a janela de mensagem

#include "core/Tabuleiro.hpp"
#include "core/Solver.hpp"
#include "core/Parser.hpp"
#include "core/PecaRegistry.hpp"

#include "graphics/GraphicsEngine.hpp"
#include "graphics/AnimationManager.hpp"
#include "graphics/MenuInterface.hpp"

#include "pieces/Rei.hpp"
#include "pieces/Rainha.hpp"
#include "pieces/Torre.hpp"
#include "pieces/Bispo.hpp"
#include "pieces/Cavalo.hpp"
#include "pieces/Peao.hpp"
#include "pieces/Pulga.hpp"

// ==============================
// Registrar peças
// ==============================
void inicializarPecas() {
    REGISTRAR_PECA('K', Rei);
    REGISTRAR_PECA('Q', Rainha);
    REGISTRAR_PECA('R', Torre);
    REGISTRAR_PECA('B', Bispo);
    REGISTRAR_PECA('N', Cavalo);
    REGISTRAR_PECA('P', Peao);
    REGISTRAR_PECA('F', Pulga);
}
// ==============================

// Janela simples informando que nao ha solucao para o teste
static void mostrarJanelaSemSolucao(const std::string& arquivo) {
    sf::RenderWindow window(
        sf::VideoMode(sf::Vector2u{ 600u, 250u }),
        "Chess Solver - Resultado"
    );

    sf::Font font;
    if (!font.openFromFile("resources/fonts/LilitaOne-Regular.ttf")) {
        // Se der erro na fonte, ainda mostra a janela vazia
    }

    auto winSize = window.getSize();
    float winW = static_cast<float>(winSize.x);
    float winH = static_cast<float>(winSize.y);

    float frame = 40.f;

    float camadas[4] = {
        frame * 0.7f, frame * 0.1f, frame * 0.13f, frame * 0.07f
    };
    sf::Color cores[4] = {
        sf::Color(152, 96, 25),
        sf::Color(211, 161, 72),
        sf::Color(191, 133, 40),
        sf::Color(255, 209, 115)
    };

    while (window.isOpen()) {
        while (auto event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) {
                window.close();
            }
            if (const auto* key = event->getIf<sf::Event::KeyPressed>()) {
                if (key->scancode == sf::Keyboard::Scancode::Escape ||
                    key->scancode == sf::Keyboard::Scancode::Enter) {
                    window.close();
                }
            }
        }

        window.clear(sf::Color(181, 136, 99));

        float offset = 0.f;
        for (int n = 0; n < 4; ++n) {
            sf::RectangleShape camada;
            camada.setFillColor(cores[n]);
            camada.setPosition(sf::Vector2f{ offset, offset });
            camada.setSize(sf::Vector2f{
                winW - 2.f * offset,
                winH - 2.f * offset
                });
            window.draw(camada);
            offset += camadas[n];
        }

        sf::RectangleShape painel(sf::Vector2f{
            winW - 2.f * frame,
            winH - 2.f * frame
            });
        painel.setPosition(sf::Vector2f{ frame, frame });
        painel.setFillColor(sf::Color(60, 40, 25, 220));
        window.draw(painel);

        // Título
        sf::Text title(font, "ChessSolver - Resultado", 26);
        title.setFillColor(sf::Color::White);
        title.setPosition(sf::Vector2f{ frame + 20.f, frame + 10.f });
        window.draw(title);

        // Mensagem principal
        sf::Text msg(font,
            "Nao existe solucao para este teste.",
            22);
        msg.setFillColor(sf::Color(230, 230, 230));
        msg.setPosition(sf::Vector2f{ frame + 20.f, frame + 70.f });
        window.draw(msg);

        // Nome do arquivo (opcional)
        sf::Text fileText(font,
            "Arquivo: " + arquivo,
            18);
        fileText.setFillColor(sf::Color(230, 230, 230));
        fileText.setPosition(sf::Vector2f{ frame + 20.f, frame + 110.f });
        window.draw(fileText);

        // Instrução para fechar
        sf::Text hint(font,
            "Pressione ENTER ou ESC para voltar ao menu.",
            16);
        hint.setFillColor(sf::Color(210, 210, 210));
        hint.setPosition(sf::Vector2f{ frame + 20.f, frame + 150.f });
        window.draw(hint);

        window.display();
    }
}

int main(int /*argc*/, char* /*argv*/[]) {
    std::cout << "=== CHESS SOLVER - TRABALHO PP ===" << std::endl;

    inicializarPecas();

    while (true) {
        // 1) Menu grafico para escolher teste ou sair
        std::string arquivoEntrada = MenuInterface::show();
        if (arquivoEntrada.empty()) {
            std::cout << "Saindo do programa." << std::endl;
            break;
        }

        std::cout << "\nArquivo selecionado: " << arquivoEntrada << std::endl;

        try {
            int profundidadeMaxima = 0;
            MTabuleiro tabuleiro = Parser::carregarArquivo(arquivoEntrada,
                profundidadeMaxima);

            ChessSolver solver(tabuleiro, profundidadeMaxima);
            std::cout << "Buscando solucao..." << std::endl;

            bool encontrou = solver.resolver();
            std::cout << "Resultado do solver: "
                << (encontrou ? "SOLUCAO ENCONTRADA" : "SEM SOLUCAO")
                << std::endl;

            if (encontrou) {
                const auto& solucao = solver.getSolucao();

                GraphicsEngine graphics(80);
                graphics.carregarTexturas();
                AnimationManager anim(graphics);

                anim.controlarPassosSolucao(solucao, tabuleiro);
            }
            else {
                // Mostra janela de "sem solucao" com a mesma identidade visual do menu
                mostrarJanelaSemSolucao(arquivoEntrada);
            }
        }
        catch (const std::exception& e) {
            std::cerr << "\nERRO: " << e.what() << std::endl;
        }

        // loop volta para o menu assim que a janela SFML é fechada
    }

    std::cout << "\n=== FINALIZADO ===" << std::endl;
    return 0;
}
