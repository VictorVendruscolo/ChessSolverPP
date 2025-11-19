#include "graphics/GraphicsEngine.hpp"
#include "core/Tabuleiro.hpp"
#include <iostream>

GraphicsEngine::GraphicsEngine(int size)
    : squareSize(size),
    colorLight(240, 217, 181),
    colorDark(181, 136, 99),
    colorHole(80, 80, 80)
{
}

void GraphicsEngine::carregarTexturas() {
    const char pecas[] = { 'K','Q','B','N','R','P' };
    const std::string bases[2] = { "branca", "preta" };
    for (const std::string& cor : bases) {
        for (char p : pecas) {
            std::string nome = "resources/images/pieces/texture_";
            nome += p;
            nome += "_" + cor + ".png";
            sf::Texture textura;
            std::cout << "Tentando carregar: " << nome << std::endl;
            if (textura.loadFromFile(nome))
                texturasPeças[nome] = std::move(textura);
        }
    }
}

void GraphicsEngine::abrirJanela(int linhas, int colunas) {
    float frame = getFrameSize();
    sf::Vector2u tamanho_janela(
        static_cast<unsigned int>(colunas * squareSize + 2 * frame),
        static_cast<unsigned int>(linhas * squareSize + 2 * frame)
    );
    if (!window || !window->isOpen()) {
        window = std::make_unique<sf::RenderWindow>(
            sf::VideoMode(tamanho_janela),
            "Chess Solver"
        );
    }
}

std::string GraphicsEngine::buildTextureName(const Peca* peca) const {
    char letra = peca->getSimbolo();
    std::string cor = (peca->getCor() == Cor::BRANCA) ? "branca" : "preta";
    return "resources/images/pieces/texture_" + std::string(1, letra) + "_" + cor + ".png";
}

void GraphicsEngine::desenharTabuleiro(const MTabuleiro& tab, float frame) {
    int linhas = tab.getLinhas();
    int colunas = tab.getColunas();

    float camadas[4] = {
        frame * 0.7f, frame * 0.1f, frame * 0.13f, frame * 0.07f
    };
    sf::Color cores[4] = {
        sf::Color(152, 96, 25),
        sf::Color(211, 161, 72),
        sf::Color(191, 133, 40),
        sf::Color(255, 209, 115)
    };

    float offset = 0.f;
    for (int n = 0; n < 4; ++n) {
        sf::RectangleShape camada;
        camada.setFillColor(cores[n]);
        camada.setPosition(sf::Vector2f{ offset, offset });
        camada.setSize(sf::Vector2f{
            colunas * squareSize + 2.f * (frame - offset),
            linhas * squareSize + 2.f * (frame - offset)
            });
        window->draw(camada);
        offset += camadas[n];
    }

    // linha 0 do tabuleiro = rank 1 (parte de baixo)
    for (int i = 0; i < linhas; ++i) {
        int viewRow = linhas - 1 - i; // converte linha lógica para linha de desenho (topo)
        for (int j = 0; j < colunas; ++j) {
            sf::RectangleShape casa(
                sf::Vector2f{ static_cast<float>(squareSize),
                              static_cast<float>(squareSize) });

            casa.setPosition(sf::Vector2f{
                j * squareSize + frame,
                viewRow * squareSize + frame
                });

            Posicao pos{ i, j }; // coordenada interna (linha 0 = baixo)
            if (tab.isBuraco(pos))
                casa.setFillColor(colorHole);
            else
                casa.setFillColor(((i + j) % 2 == 0) ? colorLight : colorDark);

            window->draw(casa);
        }
    }
}

void GraphicsEngine::desenharPeças(const MTabuleiro& tab, float frame) {
    int linhas = tab.getLinhas();
    int colunas = tab.getColunas();

    for (int i = 0; i < linhas; ++i) {
        int viewRow = linhas - 1 - i; // mesma conversão que no tabuleiro
        for (int j = 0; j < colunas; ++j) {
            Posicao pos{ i, j };
            const Peca* peca = tab.getPecaEm(pos);
            if (!peca) continue;

            std::string arq = buildTextureName(peca);
            auto it = texturasPeças.find(arq);
            if (it == texturasPeças.end()) continue;

            sf::Sprite sprite(it->second);
            sprite.setPosition(sf::Vector2f{
                static_cast<float>(j * squareSize + frame),
                static_cast<float>(viewRow * squareSize + frame)
                });

            sf::Vector2u sizeTextura = it->second.getSize();
            float scaleX = static_cast<float>(squareSize) / sizeTextura.x;
            float scaleY = static_cast<float>(squareSize) / sizeTextura.y;
            sprite.setScale(sf::Vector2f{ scaleX, scaleY });

            window->draw(sprite);
        }
    }
}

void GraphicsEngine::desenharPeçaSprite(const Peca& peca, sf::Vector2f pos, float frame) {
    std::string arq = buildTextureName(&peca);
    auto it = texturasPeças.find(arq);
    if (it != texturasPeças.end()) {
        sf::Sprite sprite(it->second);
        sf::Vector2u sizeTextura = it->second.getSize();
        float scaleX = static_cast<float>(squareSize) / sizeTextura.x;
        float scaleY = static_cast<float>(squareSize) / sizeTextura.y;
        sprite.setScale(sf::Vector2f(scaleX, scaleY));
        sprite.setPosition(pos);
        window->draw(sprite);
    }
}

void GraphicsEngine::displayWindow() {
    if (window) window->display();
    // Processa eventos para manter a janela responsiva - SFML 3.0
    while (window) {
        auto event = window->pollEvent();
        if (!event) break;
        if (event->is<sf::Event::Closed>()) {
            window->close();
        }
    }
}
