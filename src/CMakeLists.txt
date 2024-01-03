add_executable(
    sort
    main.cpp
    file_operator.cpp
    file_output.cpp
    file_input.cpp
    shared_data.cpp
    auxiliary_function.cpp
)

target_include_directories(sort PRIVATE ../include)