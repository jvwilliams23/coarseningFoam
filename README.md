# coarseningFoam

## Overview
An OpenFOAM extension for coarse-graining fluid fields to get data for 'a priori' LES. Such data can be used for training machine learning models (since LES and DNS fields are known at the same time).

**Changes**

This fork has been ported to OpenFOAM-6 (original version was OpenFOAM-2.2.x by Ali Ozel). 
As such, I have not updated all functionality. I have only added an application for the LES filtering mentioned above.
I have also added OpenMP to the convolution.C file (only in coarseningVec). 
It improves performance somewhat when large filter widths are used, but main bottleneck is still I/O.

## Usage
### Installation

The filtering libraries are compiled by
```bash
cd libraries
./Allwmake
```

The filtering application then be compiled with
```bash
cd ../applications/utilities/aprioriWALEfiltering/
wmake
```

### Running
One may use OpenMP accelerated filtering by:
```bash
OMP_NUM_THREADS=4 aprioriWALEfiltering
```

