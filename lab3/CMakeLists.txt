add_executable(lab3 WIN32
    src/main.cpp
    src/MainWindow.cpp src/MainWindow.h
    src/Container.cpp src/Container.h
    src/BaseItem.cpp src/BaseItem.h
    src/DerivedItem.cpp src/DerivedItem.h
)
target_include_directories(lab3 PRIVATE src)
target_link_libraries(lab3 PRIVATE Qt6::Widgets Boost::serialization)
