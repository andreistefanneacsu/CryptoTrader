cmake_minimum_required(VERSION 3.26)

set(MAIN_PROJECT_NAME "oop")
set(MAIN_EXECUTABLE_NAME "${MAIN_PROJECT_NAME}")

project(${MAIN_PROJECT_NAME})

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include(cmake/Options.cmake)
include(cmake/CompilerFlags.cmake)
include(cmake/CopyHelper.cmake)

add_executable(${MAIN_EXECUTABLE_NAME}
    main.cpp

    include/Monede/Moneda.hpp
    src/Monede/Moneda.cpp
    
    include/Monede/MonedaDeBaza.hpp
    src/Monede/MonedaDeBaza.cpp
    
    include/Monede/MonedaStabila.hpp
    src/Monede/MonedaStabila.cpp
    
    include/Monede/MonedaMeme.hpp
    src/Monede/MonedaMeme.cpp
    
    include/Monede/MonedaFiat.hpp
    src/Monede/MonedaFiat.cpp
    
    include/Monede/Altcoin.hpp
    src/Monede/Altcoin.cpp

    include/Piata/Piata.hpp
    src/Piata/Piata.cpp
    
    include/Piata/AnalizatorPiata.hpp
    src/Piata/AnalizatorPiata.cpp

    include/Utilizator/Utilizator.hpp
    src/Utilizator/Utilizator.cpp
    
    include/Utilizator/Portofel.hpp
    src/Utilizator/Portofel.cpp
    
    include/Utilizator/ManagerUtilizatori.hpp
    src/Utilizator/ManagerUtilizatori.cpp

    include/Tranzactii/Tranzactie.hpp
    src/Tranzactii/Tranzactie.cpp

    include/Utilitare/InterfataTerminal.hpp
    src/Utilitare/InterfataTerminal.cpp
    
    include/Utilitare/ManagerFisiere.hpp
    src/Utilitare/ManagerFisiere.cpp

    include/Utilitare/CititorFisier.hpp
    src/Utilitare/CititorFisier.cpp

    tests/TestRunner.cpp
)


target_include_directories(${MAIN_EXECUTABLE_NAME} PUBLIC 
    include
    .      # pentru fișiere din rădăcină (dacă sunt)
)

set_compiler_flags(RUN_SANITIZERS TRUE TARGET_NAMES ${MAIN_EXECUTABLE_NAME})

install(TARGETS ${MAIN_EXECUTABLE_NAME} DESTINATION ${DESTINATION_DIR})
if(APPLE)
    install(FILES launcher.command DESTINATION ${DESTINATION_DIR})
endif()

add_custom_command(TARGET ${MAIN_EXECUTABLE_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_BINARY_DIR}/date/utilizatori
    COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_BINARY_DIR}/date/piata
    COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_BINARY_DIR}/date/tranzactii
    COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_BINARY_DIR}/date/logs
    COMMENT "Se creeaza structura de directoare pentru date..."
)
