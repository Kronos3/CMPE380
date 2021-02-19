add_executable(prelab3
        prelab/lab_a.c
        )

add_subdirectory(exercise)

add_executable(lab3
        DynamicArrays.c
        DynamicArrays.h
        simpleTest.c
        )

add_executable(TestDarray
        DynamicArrays.c
        DynamicArrays.h
        TestDarray.c
        )