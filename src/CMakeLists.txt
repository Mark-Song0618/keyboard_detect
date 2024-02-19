# config Qt
set(CMAKE_PREFIX_PATH "D:\\qtForVs")
find_package(Qt6 COMPONENTS Widgets REQUIRED)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

file(GLOB src *.cpp)
file(GLOB headers *.h)
file(GLOB res ../resource/*.qrc)
qt_add_resources(QRC_SOURCES ${res})	#QRC_SOURCES is the generated binary file
add_executable(KB_HIT ${src} ${headers} ${QRC_SOURCES})
target_link_libraries(KB_HIT Qt6::Widgets)