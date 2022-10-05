cmake_minimum_required (VERSION 3.14)

project(ADS_DeviceManager_Sample)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_library(DeviceManager STATIC)
add_subdirectory(Areas)
target_include_directories(DeviceManager PUBLIC
  "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/ADS;${CMAKE_CURRENT_SOURCE_DIR}/Areas>"
)
#add_executable(DevMan_Generic "DevMan_Generic.cpp")
#add_executable(DevMan_TC1000 "DevMan_TC1000.cpp")

#include_directories(
#  "${CMAKE_SOURCE_DIR}/ADS"
#  "${CMAKE_SOURCE_DIR}/Areas"
#)

#add_subdirectory(Areas)
add_subdirectory(Examples)



if (WIN32)
    target_include_directories(DeviceManager PUBLIC "C:\\TwinCAT\\AdsApi\\TcAdsDll\\Include")
    target_link_directories(DeviceManager PRIVATE "C:\\TwinCAT\\AdsApi\\TcAdsDll\\x64\\lib")
    target_link_libraries(DeviceManager PUBLIC TcAdsDll.lib)
endif (WIN32)

if (UNIX) # TwinCAT/BSD

    # TODO: This dependency is in the wrong CMakeLists.txt in the Beckhoff/ADS repo
    find_package(Threads)

    include(FetchContent)

    FetchContent_Declare(
	    Beckhoff_ADS
	    GIT_REPOSITORY https://github.com/Beckhoff/ADS.git
	    GIT_TAG        a2fcc332684a92786a1164f5891d719d299cb2f8 # v14 
	    SOURCE_SUBDIR  AdsLib
    )

    FetchContent_MakeAvailable(Beckhoff_ADS)

    
    target_include_directories(DeviceManager PUBLIC "/usr/local/include")
    target_link_directories(DeviceManager PRIVATE "/usr/local/lib")
    target_compile_definitions(DeviceManager PUBLIC POSIX)

    list(APPEND includePath "${PROJECT_BINARY_DIR}" "${beckhoff_ads_SOURCE_DIR}/AdsLib")
    
    target_include_directories(DeviceManager PUBLIC
      "$<BUILD_INTERFACE:${includePath}>"
    )
    target_link_libraries(DeviceManager PUBLIC ads)
endif (UNIX)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)