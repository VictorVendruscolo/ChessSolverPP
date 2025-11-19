#pragma once
#include <vector>

#include <SFML/Audio.hpp>
#include "core/Solver.hpp"
#include "core/Tabuleiro.hpp"
#include "graphics/GraphicsEngine.hpp"

class AnimationManager {
private:
    GraphicsEngine& graphics;

    // Som de movimento de peça
    sf::SoundBuffer moveBuffer;
    std::unique_ptr<sf::Sound> moveSound;

public:
    AnimationManager(GraphicsEngine& engine);
    // Controle interativo passo a passo
    void controlarPassosSolucao(const std::vector<Movimento>& solucao, MTabuleiro& tab);
};
