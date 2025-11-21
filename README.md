# DOCUMENTAÇÃO TÉCNICA - ChessSolverPP

**Projeto:** Chess Solver com Interface Gráfica  
**Disciplina:** Paradigmas da Programação - UEMS 2025  

***
Estávamos usando a função "Recompilar tudo" no Visual Studio e executando: "...\ChessSolverPP\bin> .\ChessSolverPP.exe"

Usar as setas (Esquerda e Direita) para ver os movimentos e Enter para finalizar visualização 

As peças pretas não se movem, buracos não podem ser acessados...

## 1. INTRODUÇÃO

O ChessSolverPP é um sistema que resolve posições de xadrez usando algoritmo de backtracking com busca iterativa em profundidade. O projeto foi desenvolvido em C++17 com interface gráfica SFML.

### 1.1 Objetivo Principal

Implementar um sistema **extensível** que permita adicionar novas peças customizadas **sem modificar código além de `main.cpp`**, utilizando padrões de projeto orientados a objetos.

### 1.2 Tecnologias

- **Linguagem:** C++17
- **Interface Gráfica:** SFML 2.6+
- **Build System:** CMake + Visual Studio 2022
- **Controle de Versão:** Git/GitHub

***

## 2. ARQUITETURA DO SISTEMA

### 2.1 Estrutura de Diretórios

```
ChessSolverPP/
├── include/
│   ├── defines.h                    # Macros do sistema
│   ├── core/
│   │   ├── Tabuleiro.hpp           # Gerenciamento do tabuleiro
│   │   ├── Solver.hpp              # Algoritmo de backtracking
│   │   ├── Parser.hpp              # Leitura/escrita arquivos
│   │   └── PecaRegistry.hpp        # Sistema de registro de peças
│   ├── pieces/
│   │   ├── Peca.hpp                # Classe base abstrata
│   │   ├── Rei.hpp, Rainha.hpp, Torre.hpp, Bispo.hpp
│   │   ├── Cavalo.hpp, Peao.hpp
│   │   └── Pulga.hpp               # Exemplo de peça customizada
│   └── graphics/
│       ├── GraphicsEngine.hpp      # Renderização SFML
│       ├── AnimationManager.hpp    # Animação de movimentos
│       └── MenuInterface.hpp       # Menu de seleção
├── src/
│   ├── main.cpp                    # Ponto de entrada
│   ├── core/                       # Implementações lógicas
│   ├── pieces/                     # Implementações das peças
│   └── graphics/                   # Implementações gráficas
├── bin/
│   ├── tests/                      # Arquivos .pcin/.pcout
│   └── resources/                  # Texturas, fontes, sons
└── lib/
    └── SFML/                       # Biblioteca SFML
```

### 2.2 Componentes Principais

**MTabuleiro**  
Gerencia o estado do jogo usando `std::unique_ptr` para gerenciamento automático de memória. Armazena peças em matriz bidimensional e detecta buracos (casas inacessíveis).

**ChessSolver**  
Implementa backtracking com busca iterativa em profundidade (IDS). Valida xeque, xeque-mate e regras especiais do rei.

**Parser**  
Processa arquivos `.pcin` (entrada) e gera arquivos `.pcout` (solução). Converte entre posições matriciais e notação algébrica.

**PecaRegistry**  
Sistema de registro dinâmico usando `std::map` e lambdas. Permite criar peças em tempo de execução sem acoplamento estático.

**GraphicsEngine, AnimationManager, MenuInterface**  
Camada gráfica SFML separada da lógica. Renderiza tabuleiro, anima movimentos e gerencia interface do usuário.

### 2.3 Padrões de Projeto

**Factory Pattern (PecaRegistry)**  
Cria objetos sem especificar classe concreta. Registro via macro `REGISTRAR_PECA`.

**Virtual Clone Pattern**  
Método `clone()` virtual permite cópia polimórfica sem conhecer tipo concreto.

**Strategy Pattern**  
Método `getMovimentosPossiveis()` virtual puro encapsula lógica de movimento específica de cada peça.

***

## 3. HIERARQUIA DE CLASSES

```
Peca (abstrata)
├── Rei
├── Rainha
├── Torre
├── Bispo
├── Cavalo
├── Peao
├── Pulga (customizada)
└── [Nova Peça] ← Adicione aqui
```

**Classe Base `Peca`:**
- `Posicao posicao` - Localização no tabuleiro
- `Cor cor` - BRANCA ou PRETA
- `TipoPeca tipo` - Enumeração do tipo
- `char simbolo` - Identificador único ('K', 'Q', 'R', 'B', 'N', 'P', 'F', ...)
- `virtual std::unique_ptr<Peca> clone() = 0` - Cópia polimórfica
- `virtual std::vector<Posicao> getMovimentosPossiveis(const MTabuleiro&) = 0` - Movimentos válidos

***

## 4. FORMATO DE ARQUIVO (.pcin)

Estrutura de entrada para testes:

```
<linhas> <colunas> <profundidade>
<linha_buraco1> ololuna_buraco1> <linha_buraco2> olunana_buraco2> ...
<peca1>
<peca2>
...
```

**Notação das Peças:**
- Maiúscula = Branca, Minúscula = Preta
- `K/k` = Rei, `Q/q` = Rainha, `R/r` = Torre
- `B/b` = Bispo, `N/n` = Cavalo, `P/p` = Peão
- `F/f` = Pulga (customizada)
- Posição: letra (coluna) + número (linha)

**Exemplo:**
```
3 3 1
1 1
Kb1
qa1
```

Descrição: Tabuleiro 3×3, profundidade 1, buraco em (1,1), Rei branco em b1, Rainha preta em a1.

***

## 5. COMO IMPLEMENTAR NOVA PEÇA (TUTORIAL PRINCIPAL)

### 5.1 Passo 1: Criar Header (`include/pieces/NovaPeca.hpp`)

```cpp
#pragma once
#include "pieces/Peca.hpp"

class NovaPeca : public Peca {
public:
    NovaPeca(Posicao pos, Cor cor);
    
    std::unique_ptr<Peca> clone() const override {
        return std::make_unique<NovaPeca>(*this);
    }
    
    std::vector<Posicao> getMovimentosPossiveis(const MTabuleiro& tabuleiro) const override;
};
```

### 5.2 Passo 2: Criar Implementação (`src/pieces/NovaPeca.cpp`)

```cpp
#include "pieces/NovaPeca.hpp"
#include "core/Tabuleiro.hpp"

NovaPeca::NovaPeca(Posicao pos, Cor cor) 
    : Peca(pos, cor, TipoPeca::CUSTOMIZADA, 'X') {
}

std::vector<Posicao> NovaPeca::getMovimentosPossiveis(const MTabuleiro& tabuleiro) const {
    std::vector<Posicao> movimentos;
    
    // EXEMPLO: Move 2 casas em qualquer direção ortogonal
    int direcoes[4][2] = {{-2,0}, {2,0}, {0,-2}, {0,2}};
    
    for (int d = 0; d < 4; ++d) {
        int novaLinha = posicao.linha + direcoes[d][0];
        int novaColuna = posicao.coluna + direcoes[d][1];
        
        if (novaLinha >= 0 && novaLinha < tabuleiro.getLinhas() &&
            novaColuna >= 0 && novaColuna < tabuleiro.getColunas()) {
            
            Posicao novaPosicao = {novaLinha, novaColuna};
            
            // Não pode mover para buraco
            if (tabuleiro.isBuraco(novaPosicao)) continue;
            
            Peca* pecaDestino = tabuleiro.getPecaEm(novaPosicao);
            
            // Pode mover se vazio ou capturar inimigo
            if (pecaDestino == nullptr || pecaDestino->getCor() != this->cor) {
                movimentos.push_back(novaPosicao);
            }
        }
    }
    
    return movimentos;
}
```

### 5.3 Passo 3: Adicionar Enumeração (se necessário)

**Arquivo:** `include/pieces/Peca.hpp`

```cpp
enum class TipoPeca {
    REI, RAINHA, TORRE, BISPO, CAVALO, PEAO, PULGA,
    CUSTOMIZADA  // ← Adicione esta linha
};
```

### 5.4 Passo 4: Registrar na Main (ÚNICA MODIFICAÇÃO OBRIGATÓRIA)

**Arquivo:** `src/main.cpp`

```cpp
#include "pieces/NovaPeca.hpp"  // ← Adicione include

void inicializarPecas() {
    REGISTRAR_PECA('K', Rei);
    REGISTRAR_PECA('Q', Rainha);
    REGISTRAR_PECA('R', Torre);
    REGISTRAR_PECA('B', Bispo);
    REGISTRAR_PECA('N', Cavalo);
    REGISTRAR_PECA('P', Peao);
    REGISTRAR_PECA('F', Pulga);
    REGISTRAR_PECA('X', NovaPeca);  // ← Adicione registro
}
```

### 5.5 Passo 5: Criar Arquivo de Teste

**Arquivo:** `bin/tests/teste_novapeca.pcin`

```
5 5 2

Xa1
xd4
kb1
```

### 5.6 Passo 6: Adicionar Textura (Opcional)

**Arquivo:** `bin/resources/images/x_white.png` e `x_black.png`

Se não houver textura, o sistema exibirá o símbolo 'X' em texto.

***

## 6. COMO O SISTEMA DE REGISTRO FUNCIONA

### 6.1 Macro `REGISTRAR_PECA`

**Arquivo:** `include/defines.h`

```cpp
#define REGISTRAR_PECA(SIMBOLO, CLASSE) \
    PecaRegistry::getInstance().registrar(SIMBOLO, \
        [](Posicao p, Cor c) -> Peca* { \
            return new CLASSE(p, c); \
        })
```

### 6.2 Classe `PecaRegistry`

**Arquivo:** `include/core/PecaRegistry.hpp`

```cpp
class PecaRegistry {
private:
    std::map<char, std::function<Peca*(Posicao, Cor)>> fabricas;
    
    PecaRegistry() = default;
    
public:
    static PecaRegistry& getInstance() {
        static PecaRegistry instance;
        return instance;
    }
    
    void registrar(char simbolo, std::function<Peca*(Posicao, Cor)> fabrica) {
        fabricas[std::toupper(simbolo)] = fabrica;
    }
    
    Peca* criar(char simbolo, Posicao pos, Cor cor) {
        char simboloUpper = std::toupper(simbolo);
        if (fabricas.count(simboloUpper)) {
            return fabricas[simboloUpper](pos, cor);
        }
        return nullptr;
    }
};
```

### 6.3 Uso no Parser

**Arquivo:** `src/core/Parser.cpp`

```cpp
Peca* peca = PecaRegistry::getInstance().criar(tipoCor, {linhaPos, coluna}, cor);

if (!peca) {
    std::cerr << "ERRO: Tipo de peca nao registrado: " << tipoCor << std::endl;
    continue;
}

tabuleiro.adicionarPeca(peca);
```

**Como Funciona:**
1. Parser lê símbolo do arquivo (ex: 'X', 'x')
2. Chama `PecaRegistry::criar('X', posicao, cor)`
3. Registry busca lambda registrada para 'X'
4. Lambda cria `new NovaPeca(posicao, cor)`
5. Peça é adicionada ao tabuleiro

**Vantagem:** Nenhum código de `Parser.cpp` precisa ser modificado ao adicionar nova peça.

***

## 7. VIRTUAL CLONE PATTERN

### 7.1 Por Que É Necessário

O tabuleiro armazena `std::unique_ptr<Peca>`. Para copiar o tabuleiro (usado no backtracking), precisamos clonar peças polimorficamente sem conhecer tipo concreto.

### 7.2 Implementação

**Classe Base:**
```cpp
class Peca {
public:
    virtual std::unique_ptr<Peca> clone() const = 0;
};
```

**Classe Derivada:**
```cpp
class Rei : public Peca {
public:
    std::unique_ptr<Peca> clone() const override {
        return std::make_unique<Rei>(*this);
    }
};
```

### 7.3 Uso no Tabuleiro

**Arquivo:** `src/core/Tabuleiro.cpp`

```cpp
MTabuleiro::MTabuleiro(const MTabuleiro& outro) {
    // ... inicialização ...
    
    for (int i = 0; i < linhas; ++i) {
        for (int j = 0; j < colunas; ++j) {
            Peca* peca = outro.getPecaEm({i, j});
            if (peca) {
                grade[i][j] = peca->clone();  // ← Clone polimórfico
            }
        }
    }
}
```

***

## 8. INTERFACE GRÁFICA (SFML)

### 8.1 GraphicsEngine

Renderiza tabuleiro, peças e coordenadas. Carrega texturas de `bin/resources/images/`.

**Métodos Principais:**
- `carregarTexturas()` - Carrega sprites das peças
- `desenharTabuleiro()` - Renderiza grade e coordenadas
- `desenharPeca()` - Desenha sprite ou caractere

### 8.2 AnimationManager

Gerencia animação sequencial de movimentos da solução.

**Métodos Principais:**
- `controlarPassosSolucao()` - Loop principal de animação
- `animarMovimento()` - Interpola posição entre origem e destino
- `aplicarMovimento()` - Atualiza estado do tabuleiro

### 8.3 MenuInterface

Exibe lista de arquivos `.pcin` em `bin/tests/` e permite seleção via mouse.

**Método Principal:**
- `show()` - Retorna caminho do arquivo selecionado ou string vazia (sair)

***

## 9. FLUXO DE EXECUÇÃO

### 9.1 Main Loop

```cpp
int main() {
    inicializarPecas();  // Registra todas as peças
    
    while (true) {
        std::string arquivo = MenuInterface::show();
        if (arquivo.empty()) break;
        
        MTabuleiro tabuleiro = Parser::carregarArquivo(arquivo, profundidade);
        ChessSolver solver(tabuleiro, profundidade);
        
        if (solver.resolver()) {
            AnimationManager anim;
            anim.controlarPassosSolucao(solver.getSolucao(), tabuleiro);
        } else {
            mostrarJanelaSemSolucao(arquivo);
        }
    }
    
    return 0;
}
```

### 9.2 Algoritmo de Backtracking

```cpp
bool ChessSolver::backtrack(int profundidade, int limite) {
    if (profundidade >= limite) return false;
    
    // Gera movimentos das brancas
    for (auto& movBranca : gerarTodosMovimentos(Cor::BRANCA)) {
        tabuleiro.moverPeca(movBranca.origem, movBranca.destino);
        solucao.push_back(movBranca);
        
        if (isXequeMate()) return true;  // Solução encontrada
        
        // Gera respostas do rei preto
        for (auto& movRei : movimentosValidosRei()) {
            tabuleiro.moverPeca(posRei, movRei);
            solucao.push_back(movRei);
            
            if (backtrack(profundidade + 1, limite)) return true;
            
            solucao.pop_back();
            tabuleiro = backup;
        }
        
        solucao.pop_back();
        tabuleiro = backup;
    }
    
    return false;
}
```

***

## 10. EXEMPLO PRÁTICO: PEÇA PULGA

### 10.1 Header (`include/pieces/Pulga.hpp`)

```cpp
#pragma once
#include "pieces/Peca.hpp"

class Pulga : public Peca {
public:
    Pulga(Posicao pos, Cor cor);
    
    std::unique_ptr<Peca> clone() const override {
        return std::make_unique<Pulga>(*this);
    }
    
    std::vector<Posicao> getMovimentosPossiveis(const MTabuleiro& tabuleiro) const override;
};
```

### 10.2 Implementação (`src/pieces/Pulga.cpp`)

```cpp
#include "pieces/Pulga.hpp"
#include "core/Tabuleiro.hpp"

Pulga::Pulga(Posicao pos, Cor cor) 
    : Peca(pos, cor, TipoPeca::PULGA, 'F') {
}

std::vector<Posicao> Pulga::getMovimentosPossiveis(const MTabuleiro& tabuleiro) const {
    std::vector<Posicao> movimentos;
    
    // Move em L (como cavalo) mas com alcance 1-2
    int direcoes[8][2] = {
        {-2,-1}, {-2,1}, {-1,-2}, {-1,2},
        {1,-2}, {1,2}, {2,-1}, {2,1}
    };
    
    for (int d = 0; d < 8; ++d) {
        int novaLinha = posicao.linha + direcoes[d][0];
        int novaColuna = posicao.coluna + direcoes[d][1];
        
        if (novaLinha >= 0 && novaLinha < tabuleiro.getLinhas() &&
            novaColuna >= 0 && novaColuna < tabuleiro.getColunas()) {
            
            Posicao novaPosicao = {novaLinha, novaColuna};
            
            if (tabuleiro.isBuraco(novaPosicao)) continue;
            
            Peca* pecaDestino = tabuleiro.getPecaEm(novaPosicao);
            
            if (pecaDestino == nullptr || pecaDestino->getCor() != this->cor) {
                movimentos.push_back(novaPosicao);
            }
        }
    }
    
    return movimentos;
}
```

### 10.3 Registro na Main

```cpp
#include "pieces/Pulga.hpp"

void inicializarPecas() {
    // ... outras peças ...
    REGISTRAR_PECA('F', Pulga);
}
```

### 10.4 Arquivo de Teste (`bin/tests/pulga.pcin`)

```
5 5 2

Fa1
fa5
kb1
```

***

## 11. COMPILAÇÃO E EXECUÇÃO

### 11.1 Visual Studio 2022 (Método Recomendado)

1. Abra Visual Studio 2022
2. **Arquivo** → **Abrir** → **Pasta...** → Selecione `ChessSolverPP`
3. Aguarde CMake configurar automaticamente
4. Abra `src/main.cpp`
5. Selecione **"Documento Atual (main.cpp)"** na lista suspensa
6. Pressione **F5** para compilar e executar

### 11.2 CMake Manual (Alternativo)

```bash
mkdir build
cd build
cmake ..
cmake --build .
./ChessSolverPP
```

### 11.3 PowerShell (Windows)

```powershell
call "C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Auxiliary\Build\vcvars64.bat"
cl src\main.cpp src\core\*.cpp src\pieces\*.cpp src\graphics\*.cpp /std:c++17 /Iinclude /Ilib\SFML\include lib\SFML\lib\*.lib /Fe:bin\main.exe
copy lib\SFML\bin\*.dll bin
bin\main.exe
```

***

## 12. CHECKLIST PARA NOVA PEÇA

### ✅ Arquivos Criados
- [ ] `include/pieces/NovaPeca.hpp`
- [ ] `src/pieces/NovaPeca.cpp`

### ✅ Implementação
- [ ] Construtor com `Peca(pos, cor, TipoPeca::CUSTOMIZADA, 'X')`
- [ ] Método `clone()` retornando `std::make_unique<NovaPeca>(*this)`
- [ ] Método `getMovimentosPossiveis()` com lógica de movimento
- [ ] Validação de limites do tabuleiro
- [ ] Verificação de buracos com `tabuleiro.isBuraco()`
- [ ] Verificação de peças aliadas/inimigas

### ✅ Registro
- [ ] Include em `src/main.cpp`
- [ ] Linha `REGISTRAR_PECA('X', NovaPeca)` em `inicializarPecas()`

### ✅ Teste
- [ ] Arquivo `.pcin` criado em `bin/tests/`
- [ ] Peça aparece corretamente no tabuleiro
- [ ] Movimentos válidos funcionam
- [ ] Solver encontra solução (se existir)

### ✅ Recursos Gráficos (Opcional)
- [ ] `bin/resources/images/x_white.png`
- [ ] `bin/resources/images/x_black.png`

***

## 13. TROUBLESHOOTING

### 13.1 Erro: "Tipo de peca nao registrado"

**Causa:** Peça não registrada em `inicializarPecas()`.  
**Solução:** Adicione `REGISTRAR_PECA('X', NovaPeca)` na função.

### 13.2 Erro de Compilação: "undefined reference to NovaPeca"

**Causa:** Arquivo `.cpp` não incluído no build.  
**Solução:** Adicione em `CMakeLists.txt`:
```cmake
add_executable(ChessSolverPP
    src/main.cpp
    src/pieces/NovaPeca.cpp  # ← Adicione aqui
    # ... outros arquivos ...
)
```

### 13.3 Peça Não Aparece no Tabuleiro Gráfico

**Causa:** Textura não encontrada.  
**Solução:** Sistema renderiza símbolo em texto automaticamente. Para textura, adicione PNG em `bin/resources/images/`.

### 13.4 Movimentos Inválidos

**Causa:** Lógica de `getMovimentosPossiveis()` incorreta.  
**Solução:** Verifique:
- Validação de limites (`>= 0` e `< tabuleiro.getLinhas()`)
- Verificação de buracos (`tabuleiro.isBuraco()`)
- Lógica de captura (não captura peças aliadas)

***

## 14. CONCEITOS DE POO UTILIZADOS

**Herança:** Todas as peças herdam de `Peca` abstrata.

**Polimorfismo:** Métodos virtuais `clone()` e `getMovimentosPossiveis()` implementados por cada subclasse.

**Encapsulamento:** Atributos privados com getters/setters públicos.

**RAII:** `std::unique_ptr` gerencia memória automaticamente, evitando vazamentos.

**Templates:** `std::unique_ptr<Peca>` permite polimorfismo com smart pointers.

**Lambdas:** Funções anônimas no Factory Pattern para criar peças dinamicamente.

***

## 15. CONCLUSÃO

O sistema foi projetado para **extensibilidade máxima** usando padrões de projeto consolidados. A adição de novas peças requer:

1. Criar 2 arquivos (`.hpp` e `.cpp`)
2. Implementar 3 métodos (construtor, `clone()`, `getMovimentosPossiveis()`)
3. Adicionar 1 linha em `main.cpp` (`REGISTRAR_PECA`)

**Nenhuma modificação** em `Parser.cpp`, `Tabuleiro.cpp`, `Solver.cpp`, `GraphicsEngine.cpp` ou outros componentes principais é necessária.

O sistema de registro dinâmico (`PecaRegistry`) desacopla completamente a criação de peças da lógica do jogo, permitindo que o professor teste peças customizadas sem conhecimento profundo da arquitetura interna.

***

## 16. REFERÊNCIAS

**Código-fonte:** [github.com/VictorVendruscolo/ChessSolverPP](https://github.com/VictorVendruscolo/ChessSolverPP)

**Documentação SFML:** [sfml-dev.org/documentation](https://www.sfml-dev.org/documentation/2.6.0/)


