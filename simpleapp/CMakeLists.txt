cmake_minimum_required(VERSION 3.15)
project(simpleapp LANGUAGES C CXX)

include(cef_variables)

set(SIMPLE_SRCS
    cefsimple_linux.cc
    simple_app.cc  simple_app.h
    simple_handler.cc  simple_handler.h
    simple_handler_linux.cc
)

add_executable(simpleapp ${SIMPLE_SRCS})

target_include_directories(simpleapp PRIVATE
    "${CEF_ROOT}/include"
    "${CEF_ROOT}"
    "${CMAKE_CURRENT_SOURCE_DIR}"
)
# ---- Link the CEF libraries --------------------------------
target_link_libraries(simpleapp PRIVATE
    libcef_dll_wrapper
    libcef
    ${CEF_STANDARD_LIBS}
)

set_target_properties(simpleapp PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)

# ---- Copy the CEF runtime after every build ----------------
add_custom_command(TARGET simpleapp POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy_directory
          "${CEF_ROOT}/Release" "$<TARGET_FILE_DIR:simpleapp>"
  COMMAND ${CMAKE_COMMAND} -E copy_directory
          "${CEF_ROOT}/Resources" "$<TARGET_FILE_DIR:simpleapp>"
)
