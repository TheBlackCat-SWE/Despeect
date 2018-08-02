set(spct_DIR ../SpeectLib/install/include/speect/engine)
include(${spct_DIR}/SpeectConf.cmake)
include_directories(${SPCT_INCLUDE_DIRS})

find_library(despeect libspct.so HINTS ${spct_DIR}/../../../lib)

add_executable(
    voice_load
    voice_load.cpp
    ../header/DSAdapter.hpp
    ../source/DSAdapter.cpp
    )

target_link_libraries(
    voice_load
    ${Qt5Widgets_LIBRARIES}
    gtest_main
    ${despeect}
    )

add_test(
  NAME
    voice_load
  COMMAND
    voice_load
)

add_executable(
    relation_name
    relation_name.cpp
    ../header/DSAdapter.hpp
    ../source/DSAdapter.cpp
    ../header/DSRelation.hpp
    ../source/DSRelation.cpp
    ../header/DSItem.hpp
    ../source/DSItem.cpp
    )

target_link_libraries(
    relation_name
    ${Qt5Widgets_LIBRARIES}
    gtest_main
    ${despeect}
    )

add_test(
  NAME
    relation_name
  COMMAND
    relation_name
)

add_executable(
    text_load
    text_load.cpp
    ../header/DSAdapter.hpp
    ../source/DSAdapter.cpp
    )

target_link_libraries(
    text_load
    ${Qt5Widgets_LIBRARIES}
    gtest_main
    ${despeect}
    )

add_test(
  NAME
    text_load
  COMMAND
    text_load
)
