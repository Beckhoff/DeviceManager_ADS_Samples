# DeviceManager ADS

This repository shows how to get and set system information made available by the [Beckhoff Device Manager](https://infosys.beckhoff.com/content/1033/devicemanager/262925835.html) using [ADS](https://infosys.beckhoff.com/content/1033/tcinfosys3/11291871243.html). While this repository uses C++, the general procedure can be adapted to any ADS implementation. 

The examples in this repository are continuously developed. If the functionality you want is not already included, feel free to create an [Issue](https://github.com/Beckhoff/DeviceManager_ADS_Samples/issues/new/choose) describing your use case. 

## Notes on TwinCAT/BSD

The following packages are required to build and run this example:
  - os-generic-userland-devtools
  - cmake
  - git

## Examples

The following examples are currently available in the [Examples](/Examples) folder:

- [General Area](https://infosys.beckhoff.com/content/1031/devicemanager/262981387.html?id=4293180299832159029)
  - [GetDeviceName](/Examples/get_device_name.cpp)
- [Device Area](https://infosys.beckhoff.com/content/1031/devicemanager/263037707.html?id=2194017811770186156)
  - [GetSerialNumber](/Examples/get_serial_number.cpp)
- [Configuration Area](https://infosys.beckhoff.com/content/1031/devicemanager/262982923.html?id=3581134456374512489)
  - [NIC](https://infosys.beckhoff.com/content/1031/devicemanager/263013131.html?id=5976289690375372601)
    - [ChangeIP](/Examples/change_ip.cpp)
  - [TwinCAT](https://infosys.beckhoff.com/content/1031/devicemanager/263030539.html?id=1967927695808387382)
    - [DeleteAdsRoute](/Examples/delete_route.cpp)
    - [GetTcVersion](/Examples/get_tc_version.cpp)
  - [CPU](https://infosys.beckhoff.com/content/1031/devicemanager/262984459.html?id=5232713418016897865)
    - [GetCpuInfo](/Examples/get_cpu_info.cpp)
  - [File System Object](https://infosys.beckhoff.com/content/1031/devicemanager/263000843.html?id=5965980679203448020)
    - [DeleteFile](/Examples/delete_file.cpp)
    - [Dir](/Examples/dir.cpp)
    - [Mkdir](/Examples/mkdir.cpp)
    - [ReadFile](/Examples/read_file.cpp)
    - [WriteFile](/Examples/write_file.cpp)
    - [CopyFile](/Examples/copy_file.cpp)
  - [Mainboard](https://infosys.beckhoff.com/content/1031/devicemanager/263006475.html?id=3759807160000515243)
    - [GetMainboardInfo](/Examples/get_mainboard_info.cpp)
  - [Miscellaneous](https://infosys.beckhoff.com/content/1031/devicemanager/263010571.html?id=6080561606085130)
    - [Reboot](/Examples/reboot.cpp)

Follow these instructions in order to build and run examples:

1.	Clone the repository and open a command line in root of the repository
2.	Run `cmake -B build`
3.	Adapt the AmsNetId of the target device the examples code
4.	Build a specicifc example, e.g.:
    - `cmake –-build build –-target GetDeviceName`
    - `cmake –-build build –-target GetCpuInfo`
5. Run the example
    - `./build/Examples/GetDeviceName`
    - `./build/Examples/GetCpuInfo`
