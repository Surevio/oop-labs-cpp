add_library(f1garage_dll SHARED f1garage_dll.cpp)
target_link_libraries(f1garage_dll PRIVATE Boost::serialization Boost::wserialization)

# dll кладется в lab6/bin, оттуда ее забирает проект на C#
set_target_properties(f1garage_dll PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/../bin
    RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_CURRENT_SOURCE_DIR}/../bin
    RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_CURRENT_SOURCE_DIR}/../bin
)
