cmake_minimum_required(VERSION 3.15)
project(OpenRM)

set(CMAKE_CXX_STANDARD 20)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_BUILD_TYPE RELEASE)
add_compile_options(-O3 -w)

# 检查 DaHeng 相机库是否存在
find_library(DAHENG_LIB gxiapi)
if(NOT DAHENG_LIB)
    message(STATUS "DaHeng camera support is disabled because gxiapi library is not found.")
    set(DISABLE_DAHENG_CAMERA ON)
endif()

# OpenCV
find_package(OpenCV 4.5.4 REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})

# Eigen
find_package(Eigen3 REQUIRED)
include_directories(${EIGEN3_INCLUDE_DIR})

# Ceres
find_package(Ceres REQUIRED)
include_directories(${CERES_INCLUDE_DIRS})

# OpenVINO
find_package(OpenVINO REQUIRED)
include_directories(${OpenVINO_INCLUDE_DIRS})

# 设置主版本号、次版本号和补丁版本号
set(OPENRM_VERSION_MAJOR 1)
set(OPENRM_VERSION_MINOR 0)
set(OPENRM_VERSION_PATCH 0)

set(OPENRM_VERSION ${OPENRM_VERSION_MAJOR}.${OPENRM_VERSION_MINOR}.${OPENRM_VERSION_PATCH})
add_definitions(-DOPENRM_VERSION="${OPENRM_VERSION}")

include(CMakePackageConfigHelpers)

# 生成配置文件
configure_file(
    ${CMAKE_SOURCE_DIR}/cmake/VersionConfig.h.in
    ${CMAKE_CURRENT_BINARY_DIR}/VersionConfig.h
)

# 添加子目录
add_subdirectory(src/attack)
add_subdirectory(src/kalman)
add_subdirectory(src/pointer)
add_subdirectory(src/solver)
add_subdirectory(src/uniterm)
add_subdirectory(src/utils)
add_subdirectory(src/video)

# 添加目标文件列表
set(
    TARGETS_LIST
        openrm_attack
        openrm_kalman
        openrm_pointer
        openrm_solver
        openrm_delay
        openrm_print
        openrm_serial
        openrm_tf
        openrm_timer
        openrm_uniterm
        openrm_video
)

# 安装目标
install(
    TARGETS
        ${TARGETS_LIST}
    EXPORT
        OpenRMTarget
    RUNTIME DESTINATION
        bin
    LIBRARY DESTINATION
        lib
    ARCHIVE DESTINATION
        lib
)

install(
    FILES
        ${CMAKE_CURRENT_BINARY_DIR}/VersionConfig.h
    DESTINATION
        include/openrm
)

install(
    DIRECTORY
        ${CMAKE_SOURCE_DIR}/include/
    DESTINATION 
        include/openrm
)

install(
    EXPORT
        OpenRMTarget
    NAMESPACE
        openrm::
    DESTINATION
        lib/cmake/openrm
)

# 生成配置文件
configure_package_config_file(
    ${CMAKE_SOURCE_DIR}/cmake/OpenRMConfig.cmake.in.nocuda
    ${CMAKE_CURRENT_BINARY_DIR}/OpenRMConfig.cmake
    INSTALL_DESTINATION
        lib/cmake/openrm
)

install(
    FILES
        ${CMAKE_CURRENT_BINARY_DIR}/OpenRMConfig.cmake
    DESTINATION
        lib/cmake/openrm
)

# 卸载目标
if(NOT TARGET uninstall)
    configure_file(
        "${CMAKE_CURRENT_SOURCE_DIR}/cmake/cmake_uninstall.cmake.in"
        "${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake"
        IMMEDIATE @ONLY
    )

    add_custom_target(uninstall
        COMMAND ${CMAKE_COMMAND} -P ${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake
    )
endif()
