add_executable(lab11_a prelab/lab_a.c)
target_link_libraries(lab11_a gsl gslcblas)

add_executable(lab11_b
        exercise/lab_b.c
        DynamicArrays.c)
target_link_libraries(lab11_b gsl gslcblas)
target_compile_definitions(lab11_b PUBLIC -DHW8)
target_include_directories(lab11_b PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

add_executable(lab11
        hw11.c
        DynamicArrays.c DynamicArrays.h)

target_link_libraries(lab11 gsl gslcblas)
target_compile_definitions(lab11 PUBLIC -DHW8)

add_executable(lab11_correction
        correction.c)