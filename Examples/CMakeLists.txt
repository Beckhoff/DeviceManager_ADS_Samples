cmake_minimum_required (VERSION 3.14)
set(CMAKE_VERBOSE_MAKEFILE ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSION OFF)


if (WIN32)
    add_compile_definitions(USE_TWINCAT_ROUTER)
    link_libraries(TcAdsDll.lib)
    link_directories(BEFORE "C:\\TwinCAT\\AdsApi\\TcAdsDll\\x64\\lib")
elseif(UNIX)

    execute_process(
        COMMAND sh -c "kenv loader_brand"
        OUTPUT_VARIABLE manufacturer
        )

    if(${manufacturer} MATCHES beckhoff)
        # Use local ADS route on TwinCAT/BSD
        add_compile_definitions(USE_TWINCAT_ROUTER)
        link_libraries(TcAdsDll)
        link_directories("/usr/local/lib")
    else()
        # Use generic ADS on any other Unix-like system
        link_libraries(ads)
    endif()
endif()

link_libraries(DeviceManager)

# Configuration Area
#NIC
add_executable(ChangeIP "change_ip.cpp")
# TwinCAT
add_executable(DeleteAdsRoute "delete_route.cpp")
add_executable(GetTcVersion "get_tc_version.cpp")
# CPU
add_executable(GetCpuInfo "get_cpu_info.cpp")
# File System Object
add_executable(DeleteFile "delete_file.cpp")
add_executable(Dir "dir.cpp")
add_executable(Mkdir "mkdir.cpp")
add_executable(ReadFile "read_file.cpp")
add_executable(WriteFile "write_file.cpp")
add_executable(CopyFile "copy_file.cpp")
# Mainboard
add_executable(GetMainboardInfo "get_mainboard_info.cpp")
# Miscellaneous
add_executable(Reboot "reboot.cpp")

# General Area
add_executable(GetDeviceName "get_device_name.cpp")

# Device Area
add_executable(GetSerialNumber "get_serial_number.cpp")