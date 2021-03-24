cmake_minimum_required (VERSION 2.8)

project(BlackHouse)

#add_executable(main main.c)


include_directories (../)

aux_source_directory(.    SRC_MAIN)
aux_source_directory(../  SRC_BLACKHOUSE)


add_compile_options(-std=c99)

add_executable(main main.c ${SRC_BLACKHOUSE})


