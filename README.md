# ChessSolverPP 🏰♟️

**Solver de Xadrez Inteligente com Interface Gráfica**

Projeto desenvolvido em C++17 com SFML para a disciplina **Paradigmas da Programação - UFSC 2025**.

![Status](https://img.shields.io/badge/Status-Em%20Desenvolvimento-yellow)
![C++](https://img.shields.io/badge/C++-17-blue)
![SFML](https://img.shields.io/badge/SFML-2.6+-green)
![Platform](https://img.shields.io/badge/Platform-Windows-lightgrey)

---

## 🎯 Sobre o Projeto

O **ChessSolverPP** é um solver inteligente que resolve posições de xadrez usando **algoritmo de backtracking** com interface gráfica desenvolvida em **SFML**.

### Características Principais
- 🧠 **Algoritmo de Backtracking** para resolver posições
- 🎮 **Interface Gráfica** interativa com SFML
- 🏗️ **Arquitetura POO** com herança e polimorfismo
- 📁 **Separação de Responsabilidades** (lógica vs apresentação)
- 🔄 **Sistema de Build Moderno** com CMake
- 📋 **Leitura/Escrita** de arquivos .pcin/.pcout

### Tecnologias
- **Linguagem:** C++17
- **Interface:** SFML 2.6+
- **Build System:** CMake + Ninja
- **IDE:** Visual Studio 2022
- **Controle de Versão:** Git/GitHub

---

## 🚀 Início Rápido

### Pré-requisitos
- Windows 10/11 (64 bits)
- Visual Studio 2022 Community+
- Git instalado


### Como Executar
1. Abra **Visual Studio 2022**
2. **Arquivo** → **Abrir** → **Pasta...** → Selecione `ChessSolverPP`
3. Aguarde o CMake configurar automaticamente
4. Abra `src/main.cpp`
5. Selecione **"Documento Atual (main.cpp)"** na lista suspensa
6. Pressione **F5** para compilar e executar

---

## 📁 Estrutura do Projeto

ChessSolverPP/
├── 📂 src/ # Código fonte principal
│ ├── 📄 main.cpp # Ponto de entrada
│ ├── 📂 core/ # Lógica principal
│ ├── 📂 pieces/ # Classes das peças
│ ├── 📂 graphics/ # Interface SFML
│ └── 📂 io/ # Leitura/escrita arquivos
├── 📂 include/ # Headers (.hpp)
│ ├── 📄 Tabuleiro.hpp # Classe do tabuleiro
│ ├── 📄 Peca.hpp # Classe base das peças
│ ├── 📄 Solver.hpp # Algoritmo de backtracking
│ └── 📄 Interface.hpp # Interface gráfica
├── 📂 lib/ # Bibliotecas externas
│ └── 📂 SFML/ # SFML (gráficos)
├── 📂 bin/ # Executáveis (git ignore)
├── 📂 resources/ # Assets (imagens, fontes)
├── 📂 tests/ # Casos de teste
├── 📂 docs/ # Documentação
├── 📄 CMakeLists.txt # Configuração do build
├── 📄 CMakeSettings.json # Config Visual Studio
├── 📄 .gitignore # Arquivos ignorados
└── 📄 README.md # Este arquivo

### Workflow Git

Sempre sincronizar antes de trabalhar
git pull origin main

Fazer mudanças e commitar
git add .
git commit -m "Implementar classe Tabuleiro"
git push origin main


### Padrões de Commit
- ✅ `"Adicionar classe Rei com movimento básico"`
- ✅ `"Implementar algoritmo de backtracking"`
- ✅ `"Corrigir bug na validação de movimentos"`
- ❌ `"fix stuff"` ou `"changes"`

### Arquivos que NÃO devem ser commitados
- Pasta `out/` ou `build/`
- Arquivos `.exe` ou `.dll`
- Pasta `.vs/`
- Arquivos temporários (`.obj`, `.tmp`)

---

## 🔧 Desenvolvimento

### Adicionando Nova Classe
1. Criar `include/MinhaClasse.hpp`
2. Criar `src/core/MinhaClasse.cpp`
3. Adicionar no `CMakeLists.txt`:
add_executable(ChessSolverPP
src/main.cpp
src/core/MinhaClasse.cpp # ← Adicionar aqui


### Estrutura Planejada
Peca (abstrata) → Rei, Rainha, Torre, Bispo, Cavalo, Peao
Tabuleiro → Gerencia posições e movimentos
Solver → Algoritmo de backtracking
Interface → Visualização com SFML

### F5 não funciona
1. Certifique-se de ter `src/main.cpp` aberto
2. Verifique se "Documento Atual" está selecionado
3. Se não funcionar, use build manual (ver docs/)

### Compilação Manual (Emergência)
call "C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Auxiliary\Build\vcvars64.bat"
cl src\main.cpp /Iinclude /Ilib\SFML\include lib\SFML\lib\sfml-*.lib /std:c++17 /Fe:bin\main.exe
copy lib\SFML\bin*.dll bin
bin\main.exe


## 📚 Documentação

- 📄 **[docs/](docs/)** - Documentação completa
- 🔗 **[SFML Docs](https://www.sfml-dev.org/documentation/2.6.0/)** - Referência SFML
- 🐛 **[Issues](https://github.com/VictorVendruscolo/ChessSolverPP/issues)** - Bugs e melhorias
