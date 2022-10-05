cmake_minimum_required (VERSION 3.14)
set(CMAKE_VERBOSE_MAKEFILE ON)

if (WIN32)
    add_compile_definitions(USE_TWINCAT_ROUTER)
    link_libraries(TcAdsDll.lib)
    link_directories(BEFORE "C:\\TwinCAT\\AdsApi\\TcAdsDll\\x64\\lib")
endif (WIN32)

if (UNIX)
    link_libraries(ads)
endif (UNIX)

link_libraries(DeviceManager)

# Configuration Area
#NIC
add_executable(ChangeIP "change_ip.cpp")
# TwinCAT
add_executable(DeleteAdsRoute "delete_route.cpp")
# CPU
add_executable(GetCpuInfo "get_cpu_info.cpp")
# File System Object
add_executable(DeleteFile "delete_file.cpp")
add_executable(Dir "dir.cpp")
add_executable(Mkdir "mkdir.cpp")
add_executable(ReadFile "read_file.cpp")
add_executable(WriteFile "write_file.cpp")
add_executable(CopyFile "copy_file.cpp")
# Miscellaneous
add_executable(Reboot "reboot.cpp")


# General Area
add_executable(GetDeviceName "get_device_name.cpp")

# Device Area
add_executable(GetSerialNumber "get_serial_number.cpp")