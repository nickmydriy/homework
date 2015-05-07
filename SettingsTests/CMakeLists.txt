project(ttestt)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

add_executable(${PROJECT_NAME} UnitTests.cpp)

add_library(settings SHARED ${SOURCES} Settings.cpp)

target_link_libraries(settings)
