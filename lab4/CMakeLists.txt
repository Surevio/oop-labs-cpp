add_executable(lab4 WIN32
    src/main.cpp
    src/MainWindow.cpp src/MainWindow.h
    src/EditDialog.cpp src/EditDialog.h
    src/AddEditDialog.cpp src/AddEditDialog.h
    src/Container.cpp src/Container.h
    src/BaseItem.cpp src/BaseItem.h
    src/DerivedItem.cpp src/DerivedItem.h
)
target_include_directories(lab4 PRIVATE src)
target_link_libraries(lab4 PRIVATE Qt6::Widgets Boost::serialization)
