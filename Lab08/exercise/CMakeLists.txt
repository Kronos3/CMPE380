add_executable(lab8a lab_a.c)
add_executable(lab8c lab_c.c)
add_executable(lab8d lab_d.c)

set(COMMON_FLAGS -DEN_TIME -DROWS=10000 -DCOLS=100)

add_executable(lab8a2 lab_a.c)
add_executable(lab8c2 lab_c.c)

target_compile_options(lab8a PUBLIC ${COMMON_FLAGS} -DREPEAT_NUM=1000)
target_compile_options(lab8c PUBLIC ${COMMON_FLAGS} -DREPEAT_NUM=1000)
target_compile_options(lab8d PUBLIC ${COMMON_FLAGS} -DREPEAT_NUM=100000000)

target_compile_options(lab8a2 PUBLIC ${COMMON_FLAGS} -DREPEAT_NUM=2)
target_compile_options(lab8c2 PUBLIC ${COMMON_FLAGS} -DREPEAT_NUM=2)
