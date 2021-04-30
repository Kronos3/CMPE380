add_executable(lab12_d exercise/lab_d.c)
add_executable(lab12_e exercise/lab_e.c)
add_executable(lab12_f exercise/lab_f.c)

add_executable(lab12 hw12.c)
target_link_libraries(lab12 gsl gslcblas)
target_compile_options(lab12 PUBLIC -O0)
