## Intro

This directory contains a bash script to create virtual envrironment for Litex python packages at build stage and clone the virtaul environment to install directory.

## Dependencies:

Following OS dependencies are needed at developer machine:

* Python 3.6
* Pip
* Pipenv

The script creates virtaul environment using pipenv.

## List of Packages:

Script creates the virtaul environment and install following packages inside it:

* Litex
* Migen
* pythondata-cpu-vexriscv
* liteeth.git
* litedram
* litepcie
* litesata
* litesdcard
* liteiclink
* litescope
* litejesd204b
* litex-boards
* litespi
* pythondata-software-picolibc
* pythondata-software-compiler_rt
* valentyusb
* pythondata-misc-tapcfg
* pythondata-cpu-vexriscv-smp

These packages are installed at build stage. The end user will receive all these packages bundle inside the Raptor. He does not need to install them or create virtual environment.   


