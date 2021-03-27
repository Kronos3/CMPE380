add_subdirectory(exercise)

add_executable(hw7
        hw7.c
        ClassErrors.h
        eqn2solve.c
        rootfinding.c
        rootfinding.h)

target_link_libraries(hw7 m)

target_compile_options(hw7 PUBLIC -O1 -DEN_TIME -DTIM_ITER=5)
