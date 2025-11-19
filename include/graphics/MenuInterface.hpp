#pragma once
#include <string>

class MenuInterface {
public:
    // Abre o menu e retorna o caminho do arquivo selecionado (ou vazio em cancelar/fechar)
    static std::string show();
};
