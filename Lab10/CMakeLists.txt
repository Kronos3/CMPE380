add_executable(lab10_a prelab/lab_a.c)
add_executable(lab10_b exercise/lab_b.c)

add_executable(lab10 intTest.c eqn2solve.c bisection.c)

target_compile_definitions(lab10 PUBLIC -DQN=34)
