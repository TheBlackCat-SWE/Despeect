set(spct_DIR ../SpeectLib/install/include/speect/engine)
include(${spct_DIR}/SpeectConf.cmake)
include_directories(${SPCT_INCLUDE_DIRS})

find_library(despeect libspct.so HINTS ${spct_DIR}/../../../lib)

add_executable(
    unit_tests
    unit_test.cpp
    ../header/DSAdapter.hpp
    ../source/DSAdapter.cpp
    ../header/DSRelation.hpp
    ../source/DSRelation.cpp
    ../header/DSItem.hpp
    ../source/DSItem.cpp
    )

target_link_libraries(
    unit_tests
    ${Qt5Widgets_LIBRARIES}
    gtest_main
    ${despeect}
    )

add_test(
  NAME
    unit
  COMMAND
    ${CMAKE_BINARY_DIR}/${CMAKE_INSTALL_BINDIR}/unit_tests
)
