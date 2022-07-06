# DeviceManager ADS Samples

This example shows how to get and set system information made available by the [Beckhoff Device Manager](https://infosys.beckhoff.com/content/1033/devicemanager/262925835.html) using [ADS](https://infosys.beckhoff.com/content/1033/tcinfosys3/11291871243.html). While this example uses C++, the general procedure can be adapted to any ADS implementation. 

The project defines two build targets:
 
#### - DevMan_Generic
  - Works on systems without TC1000 ADS Router
  - Utilize the [Beckhoff/ADS](https://github.com/Beckhoff/ADS) library
  - Tested under Windows, Linux and TwinCAT/BSD
#### - DevMan_TC1000
  - Only works on systems that run an TC1000 ADS Router
  - Utilize the libraries shipped with the TC1000 ADS Router installtion
  - Tested on Windows and TwinCAT/BSD


## Build & Run
In order to build and run the examples, do the following:

1.	Clone the repository and open a command line in root of the repository
2.	Run `cmake -B build`
3.	Adapt the AmsNetId of the target device in [DevMan_Generic.cpp](https://github.com/Beckhoff/DeviceManager_ADS_Example/blob/main/DevMan_Generic.cpp) or [DevMan_TC1000.cpp](https://github.com/Beckhoff/DeviceManager_ADS_Example/blob/main/DevMan_TC1000.cpp)
4.	Build a specicifc target based on your configuration
    - `cmake –-build build –-target DevMan_Generic`
    - `cmake –-build build –-target DevMan_TC1000`
5. Run the example
    - `./build/DevMan_Generic`
    - `./build/DevMan_TC1000`
