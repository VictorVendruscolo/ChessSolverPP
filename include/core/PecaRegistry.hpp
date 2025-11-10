#pragma once

#include <map>
#include <functional>
#include "pieces/Peca.hpp"

class PecaRegistry {
private:
    using CreatorFunc = std::function<Peca* (Posicao, Cor)>;
    std::map<char, CreatorFunc> creators;

    PecaRegistry() = default;

public:
    static PecaRegistry& getInstance() {
        static PecaRegistry instance;
        return instance;
    }

    void registrar(char simbolo, CreatorFunc creator) {
        creators[std::toupper(simbolo)] = creator;
    }

    Peca* criar(char simbolo, Posicao pos, Cor cor) const {
        char upper = std::toupper(simbolo);
        auto it = creators.find(upper);
        if (it != creators.end()) {
            return it->second(pos, cor);
        }
        return nullptr;
    }
};

#define REGISTRAR_PECA(SIMBOLO, CLASSE) \
    PecaRegistry::getInstance().registrar(SIMBOLO, \
        [](Posicao pos, Cor cor) -> Peca* { return new CLASSE(pos, cor); })
