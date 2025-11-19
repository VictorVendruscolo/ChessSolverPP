#pragma once
#include <SFML/Graphics.hpp>
#include <map>
#include <memory>
#include <string>

class GraphicsEngine {
public:
    GraphicsEngine(int size = 80);
    void carregarTexturas();

    // Abre/garante a janela antes da animação (uso no AnimationManager)
    void abrirJanela(int linhas, int colunas);

    // Métodos para exibição
    void desenharTabuleiro(const class MTabuleiro& tab, float frame);
    void desenharPeças(const class MTabuleiro& tab, float frame);

    // Desenha apenas UMA peça em qualquer posição (usado para animação)
    void desenharPeçaSprite(const class Peca& peca, sf::Vector2f pos, float frame);

    // Mostra o frame atual na janela e processa eventos (mantém janela responsiva)
    void displayWindow();
    bool windowIsOpen() const { return window && window->isOpen(); }
    sf::RenderWindow& getWindow() { return *window; }

    // Getters
    int getSquareSize() const { return squareSize; }
    float getFrameSize() const { return squareSize * 0.2f; }

private:
    std::unique_ptr<sf::RenderWindow> window;
    int squareSize;
    sf::Color colorLight, colorDark, colorHole;
    std::map<std::string, sf::Texture> texturasPeças;

    std::string buildTextureName(const class Peca* peca) const;
};
