cmake_minimum_required (VERSION 3.14)

project(ADS_DeviceManager_Sample)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable (DevMan_Generic "DevMan_Generic.cpp")
add_executable (DevMan_TC1000 "DevMan_TC1000.cpp")

# TODO: This dependency is in the wrong CMakeLists.txt in the BEckhoff/ADS repo
find_package(Threads)

include(FetchContent)

FetchContent_Declare(
	Beckhoff_ADS
	GIT_REPOSITORY https://github.com/Beckhoff/ADS.git
	GIT_TAG        4e480d105f0a6e20b42f7f4e4593836305317566 
	SOURCE_SUBDIR  AdsLib
)

FetchContent_MakeAvailable(Beckhoff_ADS)

if (WIN32)
    target_include_directories(DevMan_TC1000 PUBLIC "C:\\TwinCAT\\AdsApi\\TcAdsDll\\Include")
    target_link_directories(DevMan_TC1000 PRIVATE "C:\\TwinCAT\\AdsApi\\TcAdsDll\\x64\\lib")
    target_link_libraries(DevMan_TC1000 PUBLIC TcAdsDll.lib)
endif (WIN32)

if (UNIX)
    target_include_directories(DevMan_TC1000 PUBLIC "/usr/local/include")
    target_link_directories(DevMan_TC1000 PRIVATE "/usr/local/lib")
    target_link_libraries(DevMan_TC1000 PUBLIC TcAdsDll)
    target_compile_definitions(DevMan_TC1000 PUBLIC POSIX)
endif (UNIX)

list(APPEND includePath "${PROJECT_BINARY_DIR}" "${beckhoff_ads_SOURCE_DIR}/AdsLib")


target_include_directories(DevMan_Generic PUBLIC
  "$<BUILD_INTERFACE:${includePath}>"
)


macro(print_all_variables)
    message(STATUS "print_all_variables------------------------------------------{")
    get_cmake_property(_variableNames VARIABLES)
    foreach (_variableName ${_variableNames})
        message(STATUS "${_variableName}=${${_variableName}}")
    endforeach()
    message(STATUS "print_all_variables------------------------------------------}")
endmacro()


target_link_libraries(DevMan_Generic PUBLIC ads)


set(CMAKE_EXPORT_COMPILE_COMMANDS ON)


print_all_variables()
