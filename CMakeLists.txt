cmake_minimum_required(VERSION 3.22)
project(cef_of_player LANGUAGES CXX)

# --------------------------------------------------------------
# Paths
# --------------------------------------------------------------
set(OF_ROOT ${CMAKE_SOURCE_DIR}/third_party/openFrameworks)
set(OF_LIB  ${OF_ROOT}/libs/openFrameworksCompiled/lib/linux64/libopenFrameworks.a)

include(ProcessorCount)
ProcessorCount(NPROC)
if(NOT NPROC)
    set(NPROC 2)
endif()

include(ExternalProject)

# --------------------------------------------------------------
# Build openFrameworks core (uses the libs we unpacked manually)
# --------------------------------------------------------------
ExternalProject_Add(openframeworks_build
    SOURCE_DIR        ${OF_ROOT}
    BUILD_IN_SOURCE   1
    CONFIGURE_COMMAND ""
    BUILD_COMMAND     ${OF_ROOT}/scripts/linux/compileOF.sh -j${NPROC}
    INSTALL_COMMAND   ""
    BUILD_BYPRODUCTS  ${OF_LIB}
)

add_library(of::openframeworks STATIC IMPORTED GLOBAL)
add_dependencies(of::openframeworks openframeworks_build)
set_target_properties(of::openframeworks PROPERTIES
    IMPORTED_LOCATION             ${OF_LIB}
    INTERFACE_INCLUDE_DIRECTORIES "${OF_ROOT}/libs;${OF_ROOT}/addons")

# --------------------------------------------------------------
# Your application target
# --------------------------------------------------------------
file(GLOB APP_SOURCES CONFIGURE_DEPENDS src/*.cpp src/*.h)
add_executable(cef_of ${APP_SOURCES})
set_target_properties(cef_of PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# Include openFrameworks headers
target_include_directories(cef_of PRIVATE
    ${OF_ROOT}/libs/openFrameworks
    ${OF_ROOT}/libs/openFrameworks/app
    ${OF_ROOT}/libs/openFrameworks/communication
    ${OF_ROOT}/libs/openFrameworks/graphics
    ${OF_ROOT}/libs/openFrameworks/math
    ${OF_ROOT}/libs/openFrameworks/sound
    ${OF_ROOT}/libs/openFrameworks/types
    ${OF_ROOT}/libs/openFrameworks/utils
    ${OF_ROOT}/libs/openFrameworks/video
    ${OF_ROOT}/libs/openFrameworks/events
    ${OF_ROOT}/libs/openFrameworks/3d
    ${OF_ROOT}/libs/openFrameworks/gl
    ${OF_ROOT}/libs
    ${OF_ROOT}/addons
    ${OF_ROOT}/libs/tess2/include
    ${OF_ROOT}/libs/glm/include
    ${OF_ROOT}/libs/utf8/include
    ${OF_ROOT}/libs/pugixml/include
    ${OF_ROOT}/libs/fmtlib/include
    ${OF_ROOT}/libs/FreeImage/include
    ${OF_ROOT}/libs/freetype/include
    ${OF_ROOT}/libs/freetype/include/freetype2
    ${OF_ROOT}/libs/boost/include
    ${OF_ROOT}/libs/openssl/include
    ${OF_ROOT}/libs/poco/include
    ${OF_ROOT}/libs/rtAudio/include
    ${OF_ROOT}/libs/uriparser/include
)

# --------------------------------------------------------------
# External dependencies
# --------------------------------------------------------------
find_package(Freetype REQUIRED)
find_package(OpenGL REQUIRED)
find_package(GLEW   REQUIRED)
find_package(OpenAL REQUIRED)
find_package(X11    REQUIRED)
find_package(Threads REQUIRED)
find_package(GLUT REQUIRED)
find_package(nlohmann_json REQUIRED)

find_package(PkgConfig REQUIRED)
pkg_check_modules(FONTCONFIG REQUIRED fontconfig)
pkg_check_modules(GLFW      REQUIRED glfw3)
pkg_check_modules(GLIB      REQUIRED glib-2.0 gobject-2.0)
pkg_check_modules(GSTREAMER REQUIRED gstreamer-1.0 gstreamer-base-1.0 gstreamer-app-1.0 gstreamer-video-1.0)
pkg_check_modules(PULSE     REQUIRED libpulse)
pkg_check_modules(ALSA      REQUIRED alsa)
pkg_check_modules(MPG123    REQUIRED libmpg123)
pkg_check_modules(SNDFILE   REQUIRED sndfile)
pkg_check_modules(CURL      REQUIRED libcurl)
pkg_check_modules(URIPARSER REQUIRED liburiparser)
pkg_check_modules(KISSFFT   REQUIRED kissfft-float)

# --------------------------------------------------------------
# Linking everything together
# --------------------------------------------------------------
target_link_libraries(cef_of PRIVATE
    of::openframeworks
    ${OF_ROOT}/libs/tess2/lib/linux64/libtess2.a

    OpenGL::GL
    GLEW::GLEW
    OpenAL::OpenAL
    GLUT::GLUT
    freeimage
    nlohmann_json::nlohmann_json
    Freetype::Freetype
    ${FONTCONFIG_LIBRARIES}
    ${GLFW_LIBRARIES}
    ${GLIB_LIBRARIES}
    ${GSTREAMER_LIBRARIES}
    ${PULSE_LIBRARIES}
    ${ALSA_LIBRARIES}
    ${MPG123_LIBRARIES}
    ${SNDFILE_LIBRARIES}
    ${CURL_LIBRARIES}
    ${URIPARSER_LIBRARIES}
    ${KISSFFT_LIBRARIES}
    ${X11_LIBRARIES}
    Threads::Threads
)

target_include_directories(cef_of PRIVATE
    ${GLFW_INCLUDE_DIRS}
    ${GLIB_INCLUDE_DIRS}
    ${GSTREAMER_INCLUDE_DIRS}
    ${PULSE_INCLUDE_DIRS}
    ${ALSA_INCLUDE_DIRS}
    ${MPG123_INCLUDE_DIRS}
    ${SNDFILE_INCLUDE_DIRS}
    ${CURL_INCLUDE_DIRS}
    ${URIPARSER_INCLUDE_DIRS}
    ${KISSFFT_INCLUDE_DIRS}
    ${X11_INCLUDE_DIRS}
    ${FONTCONFIG_INCLUDE_DIRS}
)

target_compile_features(cef_of PRIVATE cxx_std_17)
