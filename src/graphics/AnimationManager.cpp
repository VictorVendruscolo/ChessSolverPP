#include "graphics/AnimationManager.hpp"
#include "core/Solver.hpp"
#include <SFML/System/Clock.hpp>
#include <stack>
#include <iostream>

AnimationManager::AnimationManager(GraphicsEngine& engine)
    : graphics(engine) {

    // Carrega o som de movimento de peça
    if (!moveBuffer.loadFromFile("resources/sounds/PecaMovendo.wav")) {
        std::cerr << "[Audio] Nao foi possivel carregar resources/sounds/PecaMovendo.wav\n";
    }
    else {
        moveSound = std::make_unique<sf::Sound>(moveBuffer);
        moveSound->setVolume(70.f);
    }
}

// Controle passo a passo usando setas direita/esquerda.
void AnimationManager::controlarPassosSolucao(const std::vector<Movimento>& solucao,
    MTabuleiro& tab) {
    graphics.abrirJanela(tab.getLinhas(), tab.getColunas());
    int passo = 0;
    std::stack<Movimento> desfazerStack;

    while (graphics.windowIsOpen()) {
        graphics.getWindow().clear();
        graphics.desenharTabuleiro(tab, graphics.getFrameSize());
        graphics.desenharPeças(tab, graphics.getFrameSize());
        graphics.getWindow().display();

        bool avancar = false, retroceder = false;

        while (auto event = graphics.getWindow().pollEvent()) {
            // Fechar janela pelo X
            if (event->is<sf::Event::Closed>()) {
                graphics.getWindow().close();
            }
            // Teclas
            if (const auto* key = event->getIf<sf::Event::KeyPressed>()) {
                if (key->scancode == sf::Keyboard::Scancode::Right) {
                    avancar = true;
                }
                else if (key->scancode == sf::Keyboard::Scancode::Left) {
                    retroceder = true;
                }
                else if (key->scancode == sf::Keyboard::Scancode::Escape) {
                    // Sai da animacao e volta para o menu
                    graphics.getWindow().close();
                }
                else if (key->scancode == sf::Keyboard::Scancode::Enter) {
                    // Enter também fecha o tabuleiro e volta para o menu
                    graphics.getWindow().close();
                }
            }
        }

        // Avançar um passo
        if (avancar && passo < static_cast<int>(solucao.size())) {
            const auto& mov = solucao[passo];
            const Peca* pecaMover = tab.getPecaEm(mov.origem);
            if (pecaMover) {
                float frame = graphics.getFrameSize();
                int   sq = graphics.getSquareSize();
                int   linhas = tab.getLinhas();

                // converte linha lógica (0 = baixo) para linha de desenho
                int origemViewRow = linhas - 1 - mov.origem.linha;
                int destinoViewRow = linhas - 1 - mov.destino.linha;

                sf::Vector2f origemPx(
                    mov.origem.coluna * sq + frame,
                    origemViewRow * sq + frame
                );
                sf::Vector2f destinoPx(
                    mov.destino.coluna * sq + frame,
                    destinoViewRow * sq + frame
                );

                int totalFrames = 20;
                for (int t = 0; t <= totalFrames; ++t) {
                    float alfa = static_cast<float>(t) / totalFrames;
                    sf::Vector2f posInter = origemPx + (destinoPx - origemPx) * alfa;
                    graphics.getWindow().clear();
                    graphics.desenharTabuleiro(tab, frame);
                    graphics.desenharPeças(tab, frame);
                    graphics.desenharPeçaSprite(*pecaMover, posInter, frame);
                    graphics.getWindow().display();
                    // Pequeno loop de eventos para manter responsivo
                    while (auto subevent = graphics.getWindow().pollEvent()) {
                        if (subevent->is<sf::Event::Closed>()) {
                            graphics.getWindow().close();
                        }
                    }
                    sf::sleep(sf::milliseconds(8));
                }
            }

            // Aplica o movimento no modelo
            tab.moverPeca(mov.origem, mov.destino);
            desfazerStack.push(mov);

            // Toca o som de movimento (se carregou corretamente)
            if (moveSound) {
                moveSound->play();
            }

            passo++;
        }

        // Retroceder um passo (também toca som, se quiser)
        if (retroceder && passo > 0 && !desfazerStack.empty()) {
            const auto mov = desfazerStack.top();
            desfazerStack.pop();
            tab.moverPeca(mov.destino, mov.origem); // volta jogada

            if (moveSound) {
                moveSound->play();
            }

            passo--;
        }

        sf::sleep(sf::milliseconds(20));
    }
}
