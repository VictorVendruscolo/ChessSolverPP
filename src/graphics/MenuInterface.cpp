#include "graphics/MenuInterface.hpp"
#include <SFML/Graphics.hpp>
#include <filesystem>
#include <vector>
#include <string>
#include <algorithm>
#include <iostream>

struct Botao {
    sf::FloatRect area;
    std::string label;
    bool hover = false;
};

std::string MenuInterface::show() {
    sf::RenderWindow window(
        sf::VideoMode(sf::Vector2u{ 600u, 450u }),
        "Chess Solver - Menu"
    );

    sf::Font font;
    if (!font.openFromFile("resources/fonts/LilitaOne-Regular.ttf")) {
        std::cerr << "[MenuInterface] ERRO: nao foi possivel carregar LilitaOne-Regular.ttf\n";
        return {};
    }

    // Carrega lista de arquivos .pcin
    std::vector<std::string> files;
    for (auto& entry : std::filesystem::directory_iterator("tests")) {
        if (entry.path().extension() == ".pcin")
            files.push_back(entry.path().filename().string());
    }
    std::sort(files.begin(), files.end());

    int selectedIndex = files.empty() ? -1 : 0;

    auto winSize = window.getSize();
    float winW = static_cast<float>(winSize.x);
    float winH = static_cast<float>(winSize.y);

    float frame = 40.f;
    float boxWidth = 380.f;
    float boxHeight = 60.f;
    float centerX = winW * 0.5f;
    float boxX = centerX - boxWidth * 0.5f;
    float firstY = 150.f;
    float spacing = 80.f;

    std::vector<Botao> botoes{
        { sf::FloatRect(sf::Vector2f{ boxX, firstY + 0.f * spacing }, sf::Vector2f{ boxWidth, boxHeight }), "Comecar teste", false },
        { sf::FloatRect(sf::Vector2f{ boxX, firstY + 1.f * spacing }, sf::Vector2f{ boxWidth, boxHeight }), "Selecionar teste", false },
        { sf::FloatRect(sf::Vector2f{ boxX, firstY + 2.f * spacing }, sf::Vector2f{ boxWidth, boxHeight }), "Sair", false }
    };

    bool selecionandoTeste = false;

    // Scroll da lista de testes
    float scrollOffset = 0.f;
    const float itemH = 32.f;
    const float itemSpacing = 4.f;

    while (window.isOpen()) {
        while (auto event = window.pollEvent()) {
            if (event->getIf<sf::Event::Closed>()) {
                window.close();
                return {};
            }

            if (const auto* key = event->getIf<sf::Event::KeyPressed>()) {
                if (key->scancode == sf::Keyboard::Scancode::Escape) {
                    if (selecionandoTeste) {
                        selecionandoTeste = false;
                    }
                    else {
                        window.close();
                        return {};
                    }
                }
            }

            if (selecionandoTeste) {

                if (const auto* wheel = event->getIf<sf::Event::MouseWheelScrolled>()) {
                    if (wheel->wheel == sf::Mouse::Wheel::Vertical) {

                        scrollOffset += wheel->delta * (itemH + itemSpacing);

                        float panelW = winW - 2.f * (frame + 20.f);
                        float panelH = winH - 2.f * (frame + 40.f);
                        sf::Vector2f panelPos{ frame + 20.f, frame + 40.f };

                        float totalHeight = static_cast<float>(files.size()) * (itemH + itemSpacing);
                        float visibleHeight = panelH - 40.f;

                        float minOffset = std::min(0.f, visibleHeight - totalHeight);
                        float maxOffset = 0.f;
                        if (scrollOffset > maxOffset) scrollOffset = maxOffset;
                        if (scrollOffset < minOffset) scrollOffset = minOffset;
                    }
                }

                if (const auto* mousePressed = event->getIf<sf::Event::MouseButtonPressed>()) {
                    sf::Vector2f ponto{
                        static_cast<float>(mousePressed->position.x),
                        static_cast<float>(mousePressed->position.y)
                    };

                    float panelW = winW - 2.f * (frame + 20.f);
                    float panelH = winH - 2.f * (frame + 40.f);
                    sf::Vector2f panelPos{ frame + 20.f, frame + 40.f };

                    float startY = panelPos.y + 30.f;

                    for (std::size_t i = 0; i < files.size(); ++i) {
                        sf::FloatRect itemRect(
                            sf::Vector2f{ panelPos.x + 10.f,
                                          startY + scrollOffset + (itemH + itemSpacing) * static_cast<float>(i) },
                            sf::Vector2f{ panelW - 20.f, itemH }
                        );
                        if (itemRect.contains(ponto)) {
                            selectedIndex = static_cast<int>(i);
                            selecionandoTeste = false;
                            break;
                        }
                    }

                    sf::FloatRect panelRect(panelPos, sf::Vector2f{ panelW, panelH });
                    if (!panelRect.contains(ponto)) {
                        selecionandoTeste = false;
                    }
                }

                continue;
            }

            if (const auto* mouseMoved = event->getIf<sf::Event::MouseMoved>()) {
                sf::Vector2f ponto{
                    static_cast<float>(mouseMoved->position.x),
                    static_cast<float>(mouseMoved->position.y)
                };
                for (auto& btn : botoes) {
                    btn.hover = btn.area.contains(ponto);
                }
            }

            if (const auto* mousePressed = event->getIf<sf::Event::MouseButtonPressed>()) {
                sf::Vector2f ponto{
                    static_cast<float>(mousePressed->position.x),
                    static_cast<float>(mousePressed->position.y)
                };

                for (std::size_t i = 0; i < botoes.size(); ++i) {
                    if (!botoes[i].area.contains(ponto))
                        continue;

                    if (i == 0) { // Comecar
                        if (selectedIndex >= 0 && selectedIndex < static_cast<int>(files.size())) {
                            std::string result = "tests/" + files[static_cast<std::size_t>(selectedIndex)];
                            window.close();
                            return result;
                        }
                    }
                    else if (i == 1) { // Selecionar teste
                        if (!files.empty())
                            selecionandoTeste = true;
                    }
                    else if (i == 2) { // Sair
                        window.close();
                        return {};
                    }
                }
            }
        }

        window.clear(sf::Color(181, 136, 99));

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
        sf::Text title(font, "ChessSolver - Menu", 28);
        title.setFillColor(sf::Color::White);
        title.setPosition(sf::Vector2f{ frame + 20.f, frame + 10.f });
        window.draw(title);

        // Instruções
        sf::Text hint(font,
            "Clique em 'Selecionar teste' para escolher um arquivo.",
            16);
        hint.setFillColor(sf::Color(230, 230, 230));
        hint.setPosition(sf::Vector2f{ frame + 20.f, frame + 50.f });
        window.draw(hint);

        // Caixas principais
        for (std::size_t i = 0; i < botoes.size(); ++i) {
            const auto& btn = botoes[i];

            sf::RectangleShape rect(btn.area.size);
            rect.setPosition(btn.area.position);

            sf::Color baseColor;
            if (i == 0)      baseColor = sf::Color(40, 120, 40);
            else if (i == 1) baseColor = sf::Color(80, 80, 80);
            else             baseColor = sf::Color(150, 40, 40);

            sf::Color hoverColor(
                std::min<int>(baseColor.r + 40, 255),
                std::min<int>(baseColor.g + 40, 255),
                std::min<int>(baseColor.b + 40, 255),
                255
            );

            rect.setFillColor(btn.hover ? hoverColor : baseColor);
            rect.setOutlineColor(sf::Color::White);
            rect.setOutlineThickness(3.f);
            window.draw(rect);

            std::string textoCaixa;
            if (i == 1) {
                if (selectedIndex >= 0 && selectedIndex < static_cast<int>(files.size()))
                    textoCaixa = "Teste: " + files[static_cast<std::size_t>(selectedIndex)];
                else
                    textoCaixa = "Nenhum teste encontrado";
            }
            else {
                textoCaixa = btn.label;
            }

            sf::Text text(font, textoCaixa, 22);
            text.setFillColor(sf::Color::White);

            sf::FloatRect bounds = text.getLocalBounds();
            float tx = btn.area.position.x + (btn.area.size.x - bounds.size.x) * 0.5f;
            float ty = btn.area.position.y + (btn.area.size.y - bounds.size.y) * 0.5f - 5.f;
            text.setPosition(sf::Vector2f{ tx, ty });

            window.draw(text);
        }

        // Painel de seleção de teste
        if (selecionandoTeste && !files.empty()) {
            float panelW = winW - 2.f * (frame + 20.f);
            float panelH = winH - 2.f * (frame + 40.f);
            sf::Vector2f panelPos{ frame + 20.f, frame + 40.f };

            sf::RectangleShape overlay(sf::Vector2f{ winW, winH });
            overlay.setPosition(sf::Vector2f{ 0.f, 0.f });
            overlay.setFillColor(sf::Color(0, 0, 0, 120));
            window.draw(overlay);

            sf::RectangleShape panel(sf::Vector2f{ panelW, panelH });
            panel.setPosition(panelPos);
            panel.setFillColor(sf::Color(40, 40, 40, 240));
            panel.setOutlineColor(sf::Color::White);
            panel.setOutlineThickness(2.f);
            window.draw(panel);

            sf::Text ttitle(font, "Selecione um teste:", 20);
            ttitle.setFillColor(sf::Color::White);
            ttitle.setPosition(sf::Vector2f{ panelPos.x + 10.f, panelPos.y + 5.f });
            window.draw(ttitle);

            float startY = panelPos.y + 30.f;

            for (std::size_t i = 0; i < files.size(); ++i) {
                sf::Vector2f itemPos{
                    panelPos.x + 10.f,
                    startY + scrollOffset + (itemH + itemSpacing) * static_cast<float>(i)
                };
                sf::Vector2f itemSize{ panelW - 20.f, itemH };

                // Caixa cinza
                sf::RectangleShape itemRect(itemSize);
                itemRect.setPosition(itemPos);
                itemRect.setFillColor(
                    (static_cast<int>(i) == selectedIndex)
                    ? sf::Color(70, 130, 70)
                    : sf::Color(90, 90, 90)
                );
                itemRect.setOutlineColor(sf::Color::White);
                itemRect.setOutlineThickness(1.f);
                window.draw(itemRect);

                std::string label = files[i];
                sf::Text itemText(font, label, 18);
                itemText.setFillColor(sf::Color::White);

                float maxTextWidth = itemSize.x - 20.f;
                sf::FloatRect tb = itemText.getLocalBounds();
                if (tb.size.x > maxTextWidth) {
                    std::string truncated = label;
                    while (!truncated.empty()) {
                        truncated.pop_back();
                        std::string withDots = truncated + "...";
                        sf::Text testText(font, withDots, 18);
                        if (testText.getLocalBounds().size.x <= maxTextWidth) {
                            label = withDots;
                            break;
                        }
                    }
                    itemText.setString(label);
                    tb = itemText.getLocalBounds();
                }

                float ix = itemPos.x + 10.f;
                float iy = itemPos.y + (itemSize.y - tb.size.y) * 0.5f - 4.f;
                itemText.setPosition(sf::Vector2f{ ix, iy });
                window.draw(itemText);
            }
        }

        window.display();
    }

    return {};
}
