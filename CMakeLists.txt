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
add_executable(ChessSolverPP src/main.cpp)

# Linkar bibliotecas SFML
target_link_libraries(ChessSolverPP 
    sfml-graphics 
    sfml-window 
    sfml-system
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
