add_executable(lab13_a exercise/lab_a.c)
add_executable(lab13_b exercise/lab_b.c)
add_executable(lab13_c exercise/lab_c.c)
add_executable(lab13_d exercise/lab_d.c)

target_link_libraries(lab13_a pthread)
target_link_libraries(lab13_b pthread)
target_link_libraries(lab13_c pthread)
target_link_libraries(lab13_d pthread)


add_executable(lab13 hw13.c)
target_link_libraries(lab13 pthread)
