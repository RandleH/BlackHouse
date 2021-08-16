cmake_minimum_required (VERSION 2.8)

Project(BlackHouse)


aux_source_directory( ./        dir_main )
aux_source_directory( ./core    dir_blk  )

include_directories( ./core )
include_directories( ./ )

set( CMAKE_CXX_STANDARD     99 )

link_libraries( "-lm" )

add_executable( main ${dir_blk};${dir_main} )


