cmake_minimum_required(VERSION 2.8.6)
project(dictation-helper)

# find dependencies
find_package(Qt4 REQUIRED COMPONENTS QtCore QtGui)
set(CMAKE_AUTOMOC TRUE)
include(${QT_USE_FILE})

# source files
set(dictation_helper_SOURCES
    main.cpp
    mainwindow.cpp
    myqmpwidget.cpp
    mediaplayerwidget.cpp
    )

# ui files
set(dictation_helper_FORMS
    mainwindow.ui
    mediaplayerwidget.ui
    )

# ts files
set(dictation_helper_TRANSLATIONS
)

# qmpwidget (embedded mplayer)
set(dictation_helper_SOURCES
    ${dictation_helper_SOURCES} qmpwidget/qmpwidget.cpp)

# .ui -> .cxx
qt4_wrap_ui(UI_SRCS ${dictation_helper_FORMS})

include_directories(${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR})

add_executable(dictation_helper ${dictation_helper_SOURCES} ${UI_SRCS})
target_link_libraries(dictation_helper ${QT_LIBRARIES})
