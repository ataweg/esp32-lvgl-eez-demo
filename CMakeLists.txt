cmake_minimum_required(VERSION 3.5)

if (NOT DEFINED PROJECT_NAME)
    include($ENV{IDF_PATH}/tools/cmake/project.cmake)
    project(lvgl-demo)

list( APPEND compile_options
   "-Wno-unused-variable"
   "-Wno-ignored-qualifiers"
   "-Wno-type-limits"
   "-Wno-deprecated-declarations"
)

list( APPEND cxx_compile_options
   "-Wno-missing-field-initializers"
   "-Wno-error=missing-field-initializers"
)

idf_build_set_property( COMPILE_OPTIONS     "${compile_options}"     APPEND )
idf_build_set_property( C_COMPILE_OPTIONS   "${c_compile_options}"   APPEND )
idf_build_set_property( CXX_COMPILE_OPTIONS "${cxx_compile_options}" APPEND )
idf_build_set_property( COMPILE_DEFINITIONS "${compile_definitions}" APPEND )
idf_build_set_property( LINK_OPTIONS        "${link_options}"        APPEND )

else()
    message(FATAL_ERROR "LV PORT ESP32: This must be a project's main CMakeLists.txt.")
endif()