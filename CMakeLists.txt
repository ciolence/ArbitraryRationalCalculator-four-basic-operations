cmake_minimum_required(VERSION 3.16)
project(ArbitraryRationalCalculator VERSION 1.0.0 LANGUAGES CXX)

# 设置C++标准
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 自动处理moc文件
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

# 查找Qt6组件
find_package(Qt6 REQUIRED COMPONENTS Core Widgets)

# 包含头文件目录
include_directories(${CMAKE_CURRENT_SOURCE_DIR})

# 添加可执行文件
add_executable(${PROJECT_NAME}
    main.cpp
    CalculatorWidget.h
    CalculatorWidget.cpp
)

# 链接Qt库
target_link_libraries(${PROJECT_NAME}
    Qt6::Core
    Qt6::Widgets
)

# 在macOS上设置应用程序属性
if(APPLE)
    set_target_properties(${PROJECT_NAME} PROPERTIES
        MACOSX_BUNDLE TRUE
        MACOSX_BUNDLE_BUNDLE_NAME "任意大有理数计算器"
        MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION}
    )
endif()
