cmake_minimum_required(VERSION 3.12)

project(Siv3D_Web
    VERSION 0.8.0
    LANGUAGES C CXX
)

if (NOT CMAKE_CONFIGURATION_TYPES AND
    NOT CMAKE_NO_BUILD_TYPE AND
    NOT CMAKE_BUILD_TYPE AND
    CMAKE_SOURCE_DIR STREQUAL CMAKE_CURRENT_SOURCE_DIR
)
    message(STATUS "[!] Setting build type to 'Release' as none was specified.")
    set(CMAKE_BUILD_TYPE Release)
endif()

include(GNUInstallDirs)

set(SIV3D_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../Siv3D")

set(SIV3D_C_FLAGS
    -msimd128
)
set(SIV3D_CXX_FLAGS
    ${SIV3D_C_FLAGS}
    -fexceptions
)
set(SIV3D_PRIVATE_C_CXX_FLAGS
    -Wall
    -Wextra
    -Wno-deprecated-declarations
    -Wno-unused-parameter
    -Wno-unused-template
    -fcolor-diagnostics
)
set(SIV3D_PREPROCESSOR_DEFINITIONS
    _XM_NO_INTRINSICS_
    $<$<CONFIG:Debug>:_DEBUG>
)
set(SIV3D_PRIVATE_PREPROCESSOR_DEFINITIONS
    SIV3D_LIBRARY_BUILD
    # ZSTD_MULTITHREAD
    ONIG_STATIC
    MUPARSER_STATIC
    SK_RELEASE
    SK_ENABLE_OPTIMIZE_SIZE
    SK_DISABLE_TRACING
    SK_DISABLE_EFFECT_DESERIALIZATION
    LUNASVG_BUILD_STATIC
    OSC_HOST_LITTLE_ENDIAN
    R8B_PFFFT_DOUBLE
    TOML11_COMPILE_SOURCES
    _UNICODE
)
set(SIV3D_EXE_LINKER_FLAGS
    -sUSE_GLFW=3
    -sFULL_ES3
    -sMIN_WEBGL_VERSION=2
    -sMAX_WEBGL_VERSION=2
    -sALLOW_MEMORY_GROWTH=1
)

file(GLOB_RECURSE SIV3D_INTERNAL_SOURCES CONFIGURE_DEPENDS
    "${SIV3D_DIR}/src/Siv3D/*.cpp"
    "${SIV3D_DIR}/src/Siv3D-Platform/Web/Siv3D/*.cpp"
    "${SIV3D_DIR}/src/Siv3D-Platform/macOS_Linux/Siv3D/*.cpp"
)
file(GLOB_RECURSE SIV3D_THIRDPARTY_SOURCES CONFIGURE_DEPENDS
    "${SIV3D_DIR}/src/ThirdParty/*.c"
    "${SIV3D_DIR}/src/ThirdParty/*.cpp"
    "${SIV3D_DIR}/src/ThirdParty/*.cc"
)
set(SIV3D_THIRDPARTY_LIBRARIES
    $<BUILD_INTERFACE:
    >
    $<INSTALL_INTERFACE:
    >
)
file(GLOB SIV3D_JSLIBS CONFIGURE_DEPENDS
    "${SIV3D_DIR}/lib/Web/jslib/*.js"
)

add_library(Siv3D STATIC
    ${SIV3D_INTERNAL_SOURCES}
    ${SIV3D_THIRDPARTY_SOURCES}
)
add_library(Siv3D::Siv3D ALIAS Siv3D)
target_compile_features(Siv3D PUBLIC c_std_17 cxx_std_23)
set_target_properties(Siv3D PROPERTIES
    C_EXTENSIONS OFF
    CXX_EXTENSIONS OFF
    CXX_STANDARD_REQUIRED ON
    DEBUG_POSTFIX "d"
)
target_compile_options(Siv3D
    PUBLIC
        $<$<COMPILE_LANGUAGE:C>:${SIV3D_C_FLAGS}>
        $<$<COMPILE_LANGUAGE:CXX>:${SIV3D_CXX_FLAGS}>
    PRIVATE
        ${SIV3D_PRIVATE_C_CXX_FLAGS}
)
target_compile_definitions(Siv3D
    PUBLIC  ${SIV3D_PREPROCESSOR_DEFINITIONS}
    PRIVATE ${SIV3D_PRIVATE_PREPROCESSOR_DEFINITIONS}
)
target_include_directories(Siv3D
    PUBLIC
        "$<BUILD_INTERFACE:${SIV3D_DIR}/include>"
        "$<BUILD_INTERFACE:${SIV3D_DIR}/include/ThirdParty>"
        "$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/Siv3D>"
        "$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/Siv3D/ThirdParty>"
    PRIVATE
        "${SIV3D_DIR}/src"
        "${SIV3D_DIR}/src/Siv3D-Platform/Web"
        "${SIV3D_DIR}/src/Siv3D-Platform/macOS_Linux"
        "${SIV3D_DIR}/src/ThirdParty"
        "${SIV3D_DIR}/src/ThirdParty/skia"
        "${SIV3D_DIR}/src/ThirdParty-prebuilt"
        "${SIV3D_DIR}/src/ThirdParty-prebuilt/freetype"
        "${CMAKE_CURRENT_SOURCE_DIR}/../Dependencies/boost_1_88_0"
)
target_link_options(Siv3D PUBLIC
    ${SIV3D_EXE_LINKER_FLAGS}
    "$<BUILD_INTERFACE:--js-library=${CMAKE_CURRENT_BINARY_DIR}/Siv3D.js>"
    "$<INSTALL_INTERFACE:--js-library=${CMAKE_INSTALL_LIBDIR}/Siv3D/Siv3D.js>"
)
target_link_libraries(Siv3D INTERFACE
    ${SIV3D_THIRDPARTY_LIBRARIES}
)

add_custom_command(
    OUTPUT Siv3D.js
    COMMAND cat ${SIV3D_JSLIBS} > Siv3D.js
    DEPENDS ${SIV3D_JSLIBS}
)
add_custom_target(Siv3DJSLib DEPENDS Siv3D.js)
add_dependencies(Siv3D Siv3DJSLib)

install(
    TARGETS Siv3D
    EXPORT Siv3DExport
    RUNTIME DESTINATION "${CMAKE_INSTALL_BINDIR}"
    LIBRARY DESTINATION "${CMAKE_INSTALL_LIBDIR}"
    ARCHIVE DESTINATION "${CMAKE_INSTALL_LIBDIR}"
)
install(
    EXPORT Siv3DExport
    FILE Siv3DConfig.cmake
    NAMESPACE Siv3D::
    EXPORT_LINK_INTERFACE_LIBRARIES
    DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/Siv3D"
)
install(
    FILES "${SIV3D_DIR}/include/Siv3D.hpp"
    DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}/Siv3D"
)
install(
    DIRECTORY "${SIV3D_DIR}/include/Siv3D" "${SIV3D_DIR}/include/ThirdParty"
    DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}/Siv3D"
)
install(
    DIRECTORY "${SIV3D_DIR}/lib/Web/wasm/"
    DESTINATION "${CMAKE_INSTALL_LIBDIR}/Siv3D"
)
install(
    FILES "${CMAKE_CURRENT_BINARY_DIR}/Siv3D.js"
    DESTINATION "${CMAKE_INSTALL_LIBDIR}/Siv3D"
)

file(GLOB SIV3D_TEST_SOURCES CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/../Test/*.cpp"
)
add_executable(Siv3DTest
    ${SIV3D_TEST_SOURCES}
)
target_include_directories(Siv3DTest PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/../Test
)
target_link_libraries(Siv3DTest PRIVATE Siv3D)
target_link_options(Siv3DTest PRIVATE
    --emrun
    -g2
    -fexceptions
    -sASYNCIFY=1
    "--preload-file=${CMAKE_CURRENT_SOURCE_DIR}/../Test/data@/data"
    "--preload-file=${CMAKE_CURRENT_SOURCE_DIR}/App/engine@/engine"
    "--preload-file=${CMAKE_CURRENT_SOURCE_DIR}/App/example@/example"
)
set_target_properties(Siv3DTest PROPERTIES
    SUFFIX ".html"
)

set(SIV3D_ASYNCIFY_IMPORTS
    invoke_v
    invoke_vi
)

function(target_add_listed_link_options target option_name)
    set(option_list ${ARGN})
    list(TRANSFORM option_list REPLACE ".+" "'\\0'")
    string(REPLACE ";" "," option_list "${option_list}")
    target_link_options(${target} PRIVATE -s${option_name}=[${option_list}])
endfunction()

function(target_asyncify_imports target)
    target_add_listed_link_options(${target} "ASYNCIFY_IMPORTS" ${ARGN})
endfunction()

target_asyncify_imports(Siv3DTest ${SIV3D_ASYNCIFY_IMPORTS})

if(BUILD_TESTING)
    enable_testing()
    add_test(
        NAME Test
        COMMAND emrun Siv3DTest.html
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    )
endif()
