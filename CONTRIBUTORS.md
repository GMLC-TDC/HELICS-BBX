# Contributors
This file describes the contributors to the HELICS BBX executable and the software used as part of this project It is part of the GMLC-TDC project and used in the HELICS library.  HELICS is a joint project between PNNL, LLNL, and NREL, with contributions from many other national Labs
If you would like to contribute to the Utilities or HELICS project see [CONTRIBUTING](CONTRIBUTING.md)
## Individual contributors
### Pacific Northwest National Lab

### Lawrence Livermore National Lab
 - Ryan Mast*
 - Philip Top*

### National Renewable Energy Lab


### Argonne National Lab

## Used Libraries or Code
### [HELICS](https://github.com/GMLC-TDC/HELICS)  
Most of the original code for this library was pulled from use inside HELICS.  It was pulled out as the containers are not core to HELICS and it was useful as a standalone library so it could have better testing and separation of concerns.  HELICS is released with a BSD-3-Clause license.

### [googleTest](https://github.com/google/googletest)  
  The tests are written to use google test and mock frameworks and is included as a submodule.  Googletest is released with a BSD-3-clause licensed
  
### [JsonCpp](https://github.com/open-source-parsers/jsoncpp)
  JsonCpp is used for parsing json files. It is included as a submodule from a slightly modified [fork](https://github.com/phlptp/jsoncpp.git) to add support for object libraries and to clean up some warning issues. JsonCpp is licensed under public domain or MIT in case public domain is not recognized [LICENSE](https://github.com/open-source-parsers/jsoncpp/blob/master/LICENSE).

### [CLI11](https://github.com/CLIUtils/CLI11)
CLI11 is a command line parser and was used as a replacement for boost::program_options. Some modifications used in HELICS were committed to the CLI11 library and others are in the process of doing so. The modified single header library is included in the HELICS source code. The project was created by Henry Schreiner. CLI11 is available under a [3-Clause BSD](https://github.com/CLIUtils/CLI11/blob/master/LICENSE) license.


### cmake scripts
Several cmake scripts came from other sources and were either used or modified for use in HELICS.
 - Lars Bilke [CodeCoverage.cmake](https://github.com/bilke/cmake-modules/blob/master/CodeCoverage.cmake)
