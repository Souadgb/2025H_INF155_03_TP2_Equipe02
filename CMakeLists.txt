cmake_minimum_required(VERSION 3.20)
project(JeuDeplacementVehicule)

set(CMAKE_C_STANDARD 99)

# Liste des fichiers sources
set(SOURCE_FILES
        tuiles.c
        tuiles.h
        qdbmp.c
        qdbmp.h
        exemple_qdbmp.c
)

# Ajouter l'exécutable
add_executable(JeuDeplacementVehicule ${SOURCE_FILES})