cmake_minimum_required(VERSION 3.22)
project(cef_of_app LANGUAGES C CXX)

message(STATUS "CMAKE_SYSTEM_PROCESSOR = ${CMAKE_SYSTEM_PROCESSOR}")
message(STATUS "OF_PLATFORM_DIR = ${OF_PLATFORM_DIR}")

# -------------------------------
# openFrameworks Setup
# -------------------------------
set(OF_ROOT "${CMAKE_SOURCE_DIR}/third_party/openFrameworks")
if(CMAKE_SYSTEM_PROCESSOR STREQUAL "aarch64")
    set(OF_PLATFORM_DIR "linuxaarch64")
elseif(CMAKE_SYSTEM_PROCESSOR STREQUAL "x86_64")
    set(OF_PLATFORM_DIR "linux64")
else()
    message(FATAL_ERROR "Unsupported architecture: ${CMAKE_SYSTEM_PROCESSOR}")
endif()
set(OF_LIB "${OF_ROOT}/libs/openFrameworksCompiled/lib/${OF_PLATFORM_DIR}/libopenFrameworks.a")

include(ProcessorCount)
ProcessorCount(NPROC)
if(NOT NPROC)
    set(NPROC 2)
endif()

include(ExternalProject)
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
    INTERFACE_INCLUDE_DIRECTORIES "${OF_ROOT}/libs;${OF_ROOT}/addons"
)

# -------------------------------
# CEF Setup
# -------------------------------
set(CEF_ROOT "${CMAKE_SOURCE_DIR}/third_party/cef_binary")
list(APPEND CMAKE_MODULE_PATH "${CEF_ROOT}/cmake")
find_package(CEF REQUIRED)

add_subdirectory(${CEF_LIBCEF_DLL_WRAPPER_PATH} libcef_dll_wrapper)

add_library(libcef SHARED IMPORTED GLOBAL)
set_target_properties(libcef PROPERTIES
    IMPORTED_LOCATION "${CEF_ROOT}/Release/libcef.so"
)

file(COPY "${CEF_ROOT}/Resources/icudtl.dat" DESTINATION "${CEF_ROOT}/Release")
file(COPY "${CEF_ROOT}/Resources/locales"     DESTINATION "${CEF_ROOT}/Release")

# -------------------------------
# External Dependencies
# -------------------------------
find_package(Freetype REQUIRED)
find_package(OpenGL   REQUIRED)
find_package(GLEW     REQUIRED)
find_package(OpenAL   REQUIRED)
find_package(GLUT     REQUIRED)
find_package(Threads  REQUIRED)
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
pkg_check_modules(X11       REQUIRED x11)

# -------------------------------
# Application Target
# -------------------------------
file(GLOB APP_SOURCES CONFIGURE_DEPENDS
    "${CMAKE_SOURCE_DIR}/simpleapp/*.cpp"
    "${CMAKE_SOURCE_DIR}/simpleapp/*.cc"
    "${CMAKE_SOURCE_DIR}/simpleapp/*.h"
)

add_executable(simpleapp ${APP_SOURCES})
set_target_properties(simpleapp PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
    BUILD_RPATH "$ORIGIN"
)

target_include_directories(simpleapp PRIVATE
    ${CEF_ROOT}/include
    ${CEF_ROOT}
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

target_link_libraries(simpleapp PRIVATE
    libcef_dll_wrapper
    libcef
    of::openframeworks
    udev

    ${OF_ROOT}/libs/tess2/lib/${OF_PLATFORM_DIR}/libtess2.a
    freeimage
    nlohmann_json::nlohmann_json
    Freetype::Freetype
    OpenGL::GL
    GLEW::GLEW
    OpenAL::OpenAL
    GLUT::GLUT
    Threads::Threads

    ${X11_LIBRARIES}
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

)

target_include_directories(simpleapp PRIVATE
    ${X11_INCLUDE_DIRS}
    ${FONTCONFIG_INCLUDE_DIRS}
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
)

target_compile_features(simpleapp PRIVATE cxx_std_17)

add_custom_command(TARGET simpleapp POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy_directory "${CEF_ROOT}/Release"   "$<TARGET_FILE_DIR:simpleapp>"
  COMMAND ${CMAKE_COMMAND} -E copy_directory "${CEF_ROOT}/Resources" "$<TARGET_FILE_DIR:simpleapp>"
  COMMAND ${CMAKE_COMMAND} -E make_directory "$<TARGET_FILE_DIR:simpleapp>/data"
  COMMAND ${CMAKE_COMMAND} -E copy "${CMAKE_SOURCE_DIR}/media/video.mp4" "$<TARGET_FILE_DIR:simpleapp>/data/video.mp4"
  COMMAND ${CMAKE_COMMAND} -E copy "${CMAKE_SOURCE_DIR}/simpleapp/animation/*" "$<TARGET_FILE_DIR:simpleapp>/data/"
)
