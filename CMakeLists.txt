cmake_minimum_required(VERSION 3.16)
project(ChessSolverPP)

# Configurar C++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Incluir diretórios
include_directories(${CMAKE_SOURCE_DIR}/include)
include_directories(${CMAKE_SOURCE_DIR}/lib/SFML/include)

# Adicionar diretório de bibliotecas SFML
link_directories(${CMAKE_SOURCE_DIR}/lib/SFML/lib)

# Criar executável
add_executable(ChessSolverPP
    src/main.cpp
    src/pieces/Peca.cpp
    src/pieces/Rei.cpp
    src/pieces/Rainha.cpp
    src/pieces/Torre.cpp
    src/pieces/Bispo.cpp
    src/pieces/Cavalo.cpp
    src/pieces/Peao.cpp
    src/core/Tabuleiro.cpp
    src/core/Parser.cpp          
    src/core/Solver.cpp
    
    src/graphics/GraphicsEngine.cpp
    src/graphics/AnimationManager.cpp
    src/graphics/MenuInterface.cpp
 "src/pieces/Pulga.cpp" "src/graphics/GraphicsEngine.cpp")


# Linkar bibliotecas SFML
target_link_libraries(ChessSolverPP
    debug sfml-graphics-d
    debug sfml-window-d
    debug sfml-system-d
    debug sfml-audio-d
    optimized sfml-graphics
    optimized sfml-window
    optimized sfml-system
    optimized sfml-audio
)

# Copiar DLLs automaticamente (Windows)
if(WIN32)
    add_custom_command(TARGET ChessSolverPP POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_SOURCE_DIR}/lib/SFML/bin" 
        $<TARGET_FILE_DIR:ChessSolverPP>
    )
endif()

# Configurar pasta de saída
set_target_properties(ChessSolverPP PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin
)
