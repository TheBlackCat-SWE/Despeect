# Minimum version required to use Qt5 is CMake 3.1.0
cmake_minimum_required(VERSION 3.1.0 FATAL_ERROR)

project(Despeect)

# Enables C++11 and later
set(CMAKE_CXX_STANDARD 11)

# --- CMake instructions required from Speect ---

# Sets ${spct_DIR} variable to the Speect engine path
set(spct_DIR SpeectLib/install/include/speect/engine)

# Load and run CMake code from the file given
include(${spct_DIR}/SpeectConf.cmake)
include_directories(${SPCT_INCLUDE_DIRS})

# --- CMake instructions required from Speect ---

# --- CMake instructions required from Qt ---

# Instruct CMake to run moc automatically when needed
set(CMAKE_AUTOMOC ON)
# Create code from a list of Qt designer ui files
set(CMAKE_AUTOUIC ON)
# Initialize the AUTORCC property on all the targets
set(CMAKE_AUTORCC ON)
# Find includes in corresponding build directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)
# Find the QtWidgets library
find_package(Qt5Widgets CONFIG REQUIRED)

include_directories(${Qt5Widgets_INCLUDES}) # Useless ???
add_definitions(${Qt5Widgets_DEFINITIONS}) # Useless ???
set(CMAKE_CXX_FLAGS "${Qt5Widgets_EXECUTABLE_COMPILE_FLAGS}") # Useless ???

# --- CMake instructions required from Qt ---

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/source)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/header)
set( HEADERS
    header/DSAdapter.hpp
    header/DSRelation.hpp
    header/DSItem.hpp
    header/DSMainWindow.hpp
    header/DSFlowControlDockWidget.hpp
    header/DSTreeModel.hpp
    header/DSTreeItem.hpp
    header/DSListModel.hpp
    header/DSTextDockWidget.hpp
    header/VNode.hpp
    header/VScene.hpp
    header/VArc.hpp
)
# Qt's Meta-Object-Compiler, required if the class has Q_OBJECT macro
set( MOC_HEADERS
    header/DSMainWindow.hpp
    header/DSFlowControlDockWidget.hpp
    header/DSTreeModel.hpp
    #header/DSTreeItem.hpp Do not inherit from QObject
    header/DSListModel.hpp
    header/DSTextDockWidget.hpp
    header/VNode.hpp
)
set( SOURCES
    source/main.cpp
    source/DSAdapter.cpp
    source/DSRelation.cpp
    source/DSItem.cpp
    source/DSMainWindow.cpp
    source/DSFlowControlDockWidget.cpp
    source/DSTreeModel.cpp
    source/DSTreeItem.cpp
    source/DSListModel.cpp
    source/DSTextDockWidget.cpp
    source/VNode.cpp
    source/VScene.cpp
    source/VArc.cpp
)

add_executable(Despeect ${SOURCES} ${HEADERS} ${MOC_HEADERS})
find_library(libspct libspct.so HINTS ${spct_DIR}/../../../lib)
target_link_libraries( Despeect
    ${QT_LIBRARIES} # Useless ???
    ${Qt5Widgets_LIBRARIES}
    ${libspct}
)
target_include_directories( Despeect PUBLIC
    #${CMAKE_CURRENT_SOURCE_DIR}/source # Useless ???
    #${CMAKE_CURRENT_SOURCE_DIR}/header # Useless ???
    ${spct_DIR}
    #INCLUDE_DIRECTORIES #Useless ??
)
