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

set(SIV3D_C_CXX_FLAGS
    -msimd128
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
    -fexceptions
    --use-port=harfbuzz
    --use-port=libpng
    --use-port=zlib
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
set(SIV3D_ONIGURUMA_DIR "${SIV3D_DIR}/src/ThirdParty/Oniguruma")
set(SIV3D_SKIA_DIR "${SIV3D_DIR}/src/ThirdParty/skia")
file(GLOB_RECURSE SIV3D_THIRDPARTY_SOURCES CONFIGURE_DEPENDS
    "${SIV3D_DIR}/src/ThirdParty/absl/*.cc"
    "${SIV3D_DIR}/src/ThirdParty/bc7enc_rdo/*.cpp"
    "${SIV3D_DIR}/src/ThirdParty/double-conversion/*.cc"
    "${SIV3D_DIR}/src/ThirdParty/easyexif/*.cpp"
    "${SIV3D_DIR}/src/ThirdParty/fmt/*.cc"
    "${SIV3D_DIR}/src/ThirdParty/lunasvg/*.cpp"
    "${SIV3D_DIR}/src/ThirdParty/miniaudio/*.c"
    "${SIV3D_DIR}/src/ThirdParty/msdfgen/*.cpp"
    "${SIV3D_DIR}/src/ThirdParty/muparser/*.cpp"
    "${SIV3D_ONIGURUMA_DIR}/ascii.c"
    "${SIV3D_ONIGURUMA_DIR}/onig_init.c"
    "${SIV3D_ONIGURUMA_DIR}/reg*.c"
    "${SIV3D_ONIGURUMA_DIR}/st.c"
    "${SIV3D_ONIGURUMA_DIR}/unicode.c"
    "${SIV3D_ONIGURUMA_DIR}/unicode_fold[1-3]_key.c"
    "${SIV3D_ONIGURUMA_DIR}/unicode_unfold_key.c"
    "${SIV3D_ONIGURUMA_DIR}/utf32_le.c"
    # "${SIV3D_DIR}/src/ThirdParty/oscpack/ip/IpEndpointName.cpp"
    # "${SIV3D_DIR}/src/ThirdParty/oscpack/ip/posix/*.cpp"
    # "${SIV3D_DIR}/src/ThirdParty/oscpack/osc/*.cpp"
    "${SIV3D_DIR}/src/ThirdParty/PFFFT/*.c"
    "${SIV3D_DIR}/src/ThirdParty/plutovg/*.c"
    # "${SIV3D_DIR}/src/ThirdParty/serial_cpp/serial.cc"
    # "${SIV3D_DIR}/src/ThirdParty/serial_cpp/impl/unix.cc"
    "${SIV3D_DIR}/src/ThirdParty/simdutf/*.cpp"
    "${SIV3D_SKIA_DIR}/modules/*.cc"
    "${SIV3D_SKIA_DIR}/src/base/*.cpp"
    "${SIV3D_SKIA_DIR}/src/codec/*.cpp"
    "${SIV3D_SKIA_DIR}/src/core/*.cpp"
    "${SIV3D_SKIA_DIR}/src/effects/colorfilters/SkBlendModeColorFilter.cpp"
    "${SIV3D_SKIA_DIR}/src/effects/colorfilters/SkColorFilterBase.cpp"
    "${SIV3D_SKIA_DIR}/src/effects/colorfilters/SkComposeColorFilter.cpp"
    "${SIV3D_SKIA_DIR}/src/effects/colorfilters/SkRuntimeColorFilter.cpp"
    "${SIV3D_SKIA_DIR}/src/effects/colorfilters/SkWorkingFormatColorFilter.cpp"
    "${SIV3D_SKIA_DIR}/src/effects/imagefilters/SkBlendImageFilter.cpp"
    "${SIV3D_SKIA_DIR}/src/effects/imagefilters/SkBlurImageFilter.cpp"
    "${SIV3D_SKIA_DIR}/src/effects/imagefilters/SkCropImageFilter.cpp"
    "${SIV3D_SKIA_DIR}/src/effects/SkBlenders.cpp"
    "${SIV3D_SKIA_DIR}/src/image/SkImage.cpp"
    "${SIV3D_SKIA_DIR}/src/image/SkImage_Base.cpp"
    "${SIV3D_SKIA_DIR}/src/image/SkImage_Lazy.cpp"
    "${SIV3D_SKIA_DIR}/src/image/SkImage_Raster.cpp"
    "${SIV3D_SKIA_DIR}/src/image/SkImage_RasterFactories.cpp"
    "${SIV3D_SKIA_DIR}/src/image/SkRescaleAndReadPixels.cpp"
    "${SIV3D_SKIA_DIR}/src/image/SkSurface.cpp"
    "${SIV3D_SKIA_DIR}/src/image/SkSurface_Base.cpp"
    "${SIV3D_SKIA_DIR}/src/image/SkSurface_Raster.cpp"
    "${SIV3D_SKIA_DIR}/src/opts/*.cpp"
    "${SIV3D_SKIA_DIR}/src/pathops/*.cpp"
    "${SIV3D_SKIA_DIR}/src/ports/*.cpp"
    "${SIV3D_SKIA_DIR}/src/sfnt/*.cpp"
    "${SIV3D_SKIA_DIR}/src/shaders/*.cpp"
    "${SIV3D_SKIA_DIR}/src/sksl/*.cpp"
    "${SIV3D_SKIA_DIR}/src/text/*.cpp"
    "${SIV3D_SKIA_DIR}/src/utils/SkCharToGlyphCache.cpp"
    "${SIV3D_SKIA_DIR}/src/utils/SkCustomTypeface.cpp"
    "${SIV3D_SKIA_DIR}/src/utils/SkDashPath.cpp"
    "${SIV3D_SKIA_DIR}/src/utils/SkJSON.cpp"
    "${SIV3D_SKIA_DIR}/src/utils/SkJSONWriter.cpp"
    "${SIV3D_SKIA_DIR}/src/utils/SkMatrix22.cpp"
    "${SIV3D_SKIA_DIR}/src/utils/SkParse.cpp"
    "${SIV3D_SKIA_DIR}/src/utils/SkPatchUtils.cpp"
    "${SIV3D_SKIA_DIR}/src/utils/SkShadowUtils.cpp"
    "${SIV3D_SKIA_DIR}/src/utils/SkTextUtils.cpp"
    "${SIV3D_DIR}/src/ThirdParty/toml11/*.cpp"
    "${SIV3D_DIR}/src/ThirdParty/wuffs/*.cpp"
    "${SIV3D_DIR}/src/ThirdParty/zstd/*.c"
)
list(REMOVE_ITEM SIV3D_THIRDPARTY_SOURCES
    "${SIV3D_SKIA_DIR}/src/base/SkBase64.cpp"
    "${SIV3D_SKIA_DIR}/src/base/SkTime.cpp"
    "${SIV3D_SKIA_DIR}/src/core/SkAnnotation.cpp"
    "${SIV3D_SKIA_DIR}/src/core/SkATrace.cpp"
    "${SIV3D_SKIA_DIR}/src/core/SkClipStackDevice.cpp"
    "${SIV3D_SKIA_DIR}/src/core/SkDataTable.cpp"
    "${SIV3D_SKIA_DIR}/src/core/SkDistanceFieldGen.cpp"
    "${SIV3D_SKIA_DIR}/src/core/SkDocument.cpp"
    "${SIV3D_SKIA_DIR}/src/core/SkFontStream.cpp"
    "${SIV3D_SKIA_DIR}/src/core/SkMD5.cpp"
    "${SIV3D_SKIA_DIR}/src/core/SkOverdrawCanvas.cpp"
    "${SIV3D_SKIA_DIR}/src/core/SkSLTypeShared.cpp"
    "${SIV3D_SKIA_DIR}/src/core/SkTypeface_remote.cpp"
    "${SIV3D_SKIA_DIR}/src/core/SkYUVMath.cpp"
    "${SIV3D_SKIA_DIR}/src/ports/SkDebug_win.cpp"
    "${SIV3D_SKIA_DIR}/src/ports/SkOSFile_win.cpp"
    "${SIV3D_SKIA_DIR}/src/shaders/SkCoordClampShader.cpp"
    "${SIV3D_SKIA_DIR}/src/shaders/SkGainmapShader.cpp"
    "${SIV3D_SKIA_DIR}/src/shaders/SkPerlinNoiseShaderImpl.cpp"
)
set_source_files_properties(
    "${SIV3D_SKIA_DIR}/src/core/SkWriteBuffer.cpp" PROPERTIES
    COMPILE_DEFINITIONS SK_DISABLE_LEGACY_PNG_WRITEBUFFER
)
set_source_files_properties(
    "${SIV3D_ONIGURUMA_DIR}/unicode.c" PROPERTIES
    COMPILE_OPTIONS -Wno-missing-field-initializers
)
set(SIV3D_THIRDPARTY_LIBRARIES
    $<BUILD_INTERFACE:
        "${SIV3D_DIR}/lib/Web/wasm/libjpeg-turbo/libturbojpeg.a"
        "${SIV3D_DIR}/lib/Web/wasm/opencv/libopencv_core.a"
        "${SIV3D_DIR}/lib/Web/wasm/opencv/libopencv_imgproc.a"
    >
    $<INSTALL_INTERFACE:
        "${CMAKE_INSTALL_LIBDIR}/Siv3D/libjpeg-turbo/libturbojpeg.a"
        "${CMAKE_INSTALL_LIBDIR}/Siv3D/opencv/libopencv_core.a"
        "${CMAKE_INSTALL_LIBDIR}/Siv3D/opencv/libopencv_imgproc.a"
    >
)
file(GLOB_RECURSE SIV3D_JSLIBS CONFIGURE_DEPENDS
    "${SIV3D_DIR}/src/Siv3D-Platform/Web/Siv3D/*.js"
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
    PUBLIC  ${SIV3D_C_CXX_FLAGS}
    PRIVATE ${SIV3D_PRIVATE_C_CXX_FLAGS}
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
target_link_options(Siv3D INTERFACE
    ${SIV3D_EXE_LINKER_FLAGS}
    "$<BUILD_INTERFACE:--js-library=${CMAKE_CURRENT_BINARY_DIR}/libSiv3D.js>"
    "$<INSTALL_INTERFACE:--js-library=${CMAKE_INSTALL_LIBDIR}/Siv3D/libSiv3D.js>"
)
target_link_libraries(Siv3D INTERFACE
    ${SIV3D_THIRDPARTY_LIBRARIES}
)

add_custom_command(
    OUTPUT libSiv3D.js
    COMMAND cat ${SIV3D_JSLIBS} > libSiv3D.js
    DEPENDS ${SIV3D_JSLIBS}
)
add_custom_target(Siv3DJSLib DEPENDS libSiv3D.js)
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
    FILES "${CMAKE_CURRENT_BINARY_DIR}/libSiv3D.js"
    DESTINATION "${CMAKE_INSTALL_LIBDIR}/Siv3D"
)

file(GLOB SIV3D_TEST_SOURCES CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/../Test/*.cpp"
)
add_executable(Siv3DTest
    # ${SIV3D_TEST_SOURCES}
    "${CMAKE_CURRENT_SOURCE_DIR}/App/Main.cpp"
)
target_include_directories(Siv3DTest PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/../Test
)
target_link_libraries(Siv3DTest PRIVATE Siv3D)
target_link_options(Siv3DTest PRIVATE
    --emrun
    -g2
    -sASYNCIFY=1
    # "--preload-file=${CMAKE_CURRENT_SOURCE_DIR}/../Test/data@/data"
    "--preload-file=${CMAKE_CURRENT_SOURCE_DIR}/App/resources@/resources"
    "--preload-file=${CMAKE_CURRENT_SOURCE_DIR}/App/example@/example"
    -Wl,--no-gc-sections
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
