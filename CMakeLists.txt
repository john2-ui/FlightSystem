cmake_minimum_required(VERSION 3.16)

project(FlightSystem VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 找 Qt6
find_package(Qt6 REQUIRED COMPONENTS Widgets Core Gui)

# 包含源文件
set(SOURCES
    main.cpp
)

# 生成可执行文件
qt_add_executable(FlightSystem
    ${SOURCES}
)

# 链接 Qt 库
target_link_libraries(FlightSystem PRIVATE Qt6::Widgets Qt6::Core Qt6::Gui)
