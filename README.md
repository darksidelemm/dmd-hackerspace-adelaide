dmd-hackerspace-adelaide
========================

Driver code for the Freetronics Dot Matrix Display at Hackerspace Adelaide.

This requires the installation and modification of the DMD library, available from:
https://github.com/freetronics/DMD

You will need to move the bDMDScreenRAM declaration in DMD.h out of the private area, so we can write to it.
