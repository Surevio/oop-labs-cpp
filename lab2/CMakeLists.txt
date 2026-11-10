add_executable(lab2
    main.cpp
    RikhelgofStudent.cpp RikhelgofStudent.h
    RikhelgofStarosta.cpp RikhelgofStarosta.h
    RikhelgofGroup.cpp RikhelgofGroup.h
)
target_link_libraries(lab2 PRIVATE Boost::serialization)
