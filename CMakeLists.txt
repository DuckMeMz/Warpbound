cmake_minimum_required(VERSION 3.10)

project(Warpbound)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

#Multithread compile for MSVC
if (MSVC)
    add_compile_options(/MP)
endif()

#Create some QoL vars
set(SRC_DIR "${CMAKE_SOURCE_DIR}/Hornet")
set(INCLUDE_DIR "${SRC_DIR}/include")
set(SDL2_LIB_DIR "${CMAKE_SOURCE_DIR}/Hornet/lib/x64")

file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS
    "${SRC_DIR}/*.cpp"
    "${SRC_DIR}/*.h"
)


add_executable(Warpbound ${SOURCES})

#Include sdl's headers
target_include_directories(Warpbound PUBLIC
    ${INCLUDE_DIR}
)

#link the dir
link_directories(${SDL2_LIB_DIR})

#Link sdl's libs
target_link_libraries(Warpbound PRIVATE
    "${SDL2_LIB_DIR}/SDL2.lib"
    "${SDL2_LIB_DIR}/SDL2main.lib"
    "${SDL2_LIB_DIR}/SDL2_image.lib"
    "${SDL2_LIB_DIR}/SDL2_mixer.lib"
    "${SDL2_LIB_DIR}/SDL2_ttf.lib"
    "${SDL2_LIB_DIR}/SDL2test.lib"
)

#Program folder for Hornet
set(PROGRAM_FOLDER "${CMAKE_SOURCE_DIR}/Hornet/Program")

#Copy program folder & SDL dlls into the root of the exe
foreach(folder IN ITEMS assets docs fonts)
    add_custom_command(TARGET Warpbound POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${PROGRAM_FOLDER}/${folder}"
        "$<TARGET_FILE_DIR:Warpbound>/${folder}"
    )
endforeach()

#Copy the dlls out of the program folder
# Copy DLLs (if you want to copy all DLLs from Program folder root)
file(GLOB SDL_DLLS "${PROGRAM_FOLDER}/*.dll")
foreach(dll ${SDL_DLLS})
    add_custom_command(TARGET Warpbound POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${dll}"
        $<TARGET_FILE_DIR:Warpbound>
    )
endforeach()