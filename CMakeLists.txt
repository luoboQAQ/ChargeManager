cmake_minimum_required(VERSION 3.5)

project(ChargeManager LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# QtCreator supports the following variables for Android, which are identical to qmake Android variables.
# Check https://doc.qt.io/qt/deployment-android.html for more information.
# They need to be set before the find_package( ...) calls below.

#if(ANDROID)
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
#    if (ANDROID_ABI STREQUAL "armeabi-v7a")
#        set(ANDROID_EXTRA_LIBS
#            ${CMAKE_CURRENT_SOURCE_DIR}/path/to/libcrypto.so
#            ${CMAKE_CURRENT_SOURCE_DIR}/path/to/libssl.so)
#    endif()
#endif()

find_package(QT NAMES Qt6 Qt5 COMPONENTS Widgets REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Widgets REQUIRED)

#查找当前文件夹中的所有源代码文件，也可以通过Set命令将所有文件设置为一个变量
FILE(GLOB SRC_FILES "./*.cpp")
#同理，查找当前文件夹中所有的头文件
FILE(GLOB HEAD_FILES "./*.h")
#同理，查找当前文件夹中所有的ui文件
FILE(GLOB UI_FILES "./*.ui")

set(PROJECT_SOURCES ${SRC_FILES} ${HEAD_FILES} ${UI_FILES})

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(ChargeManager
        ${PROJECT_SOURCES}
    )
else()
    if(ANDROID)
        add_library(ChargeManager SHARED
            ${PROJECT_SOURCES}
        )
    else()
        add_executable(ChargeManager
            ${PROJECT_SOURCES}
        )
    endif()
endif()

target_link_libraries(ChargeManager PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)
# 添加数据库模块
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Sql REQUIRED)
target_link_libraries(ChargeManager PRIVATE Qt${QT_VERSION_MAJOR}::Sql)