PLBRAM-ZYBO-Z7
=======================================================================

This Repository provides example for uiomem and FPGA-SoC-Linux.

# Requirement

 * Board: ZYBO-Z7
 * OS:
   + ~~FPGA-SoC-Linux v1.0.1 https://github.com/ikwzm/FPGA-SoC-Linux/tree/v1.0.1~~
   + [FPGA-SoC-Debian12](https://github.com/ikwzm/FPGA-SoC-Debian12.git)
   + [FPGA-SoC-Debian13](https://github.com/ikwzm/FPGA-SoC-Debian13.git)
 * uiomem (v1.0.0-alpha.6) https://github.com/ikwzm/uiomem/tree/v1.0.0-alpha.6

# Boot ZYBO-Z7 and login fpga user

fpga'password is "fpga".

```console
debian-fpga login: fpga
Password:
fpga@debian-fpga:~$
```

# Download this repository

## Download this repository

```console
shell$ git clone --branch develop https://github.com/ikwzm/PLBRAM-ZYBO-Z7
Cloning into 'PLBRAM-ZYBO-Z7'...
remote: Enumerating objects: 75, done.
remote: Counting objects: 100% (75/75), done.
remote: Compressing objects: 100% (50/50), done.
remote: Total 75 (delta 35), reused 63 (delta 23), pack-reused 0 (from 0)
Receiving objects: 100% (75/75), 321.99 KiB | 1.02 MiB/s, done.
Resolving deltas: 100% (35/35), done.
```

```console
shell$ cd PLBRAM-ZYBO-Z7/
shell$ git submodule init
Submodule 'LED_AXI' (https://github.com/ikwzm/LED_AXI) registered for path 'LED_AXI'
Submodule 'uiomem' (https://github.com/ikwzm/uiomem.git) registered for path 'uiomem'
shell$ git submodule update
Cloning into '/home/fpga/examples/PLBRAM-ZYBO-Z7/LED_AXI'...
Cloning into '/home/fpga/examples/PLBRAM-ZYBO-Z7/uiomem'...
Submodule path 'LED_AXI': checked out '94025de5f5f2e1c4480c3a23851b14a34fa87a9d'
Submodule path 'uiomem': checked out 'b78225881b8009f78f7700678df8c9b72c1f2ef1'
```

# Setup

## Build uiomem

```console
shell$ cd uiomem
shell$ make
make -C /lib/modules/6.12.55-armv7-fpga/build ARCH=arm CROSS_COMPILE= M=/home/fpga/examples/PLBRAM-ZYBO-Z7/uiomem CONFIG_UIOMEM=m modules
make[1]: Entering directory '/usr/src/linux-headers-6.12.55-armv7-fpga'
warning: the compiler differs from the one used to build the kernel
  The kernel was built by: arm-linux-gnueabihf-gcc (Ubuntu 13.3.0-6ubuntu2~24.04) 13.3.0
  You are using:           gcc (Debian 14.2.0-19) 14.2.0
  CC [M]  /home/fpga/examples/PLBRAM-ZYBO-Z7/uiomem/uiomem.o
  MODPOST /home/fpga/examples/PLBRAM-ZYBO-Z7/uiomem/Module.symvers
  CC [M]  /home/fpga/examples/PLBRAM-ZYBO-Z7/uiomem/uiomem.mod.o
  CC [M]  /home/fpga/examples/PLBRAM-ZYBO-Z7/uiomem/.module-common.o
  LD [M]  /home/fpga/examples/PLBRAM-ZYBO-Z7/uiomem/uiomem.ko
make[1]: Leaving directory '/usr/src/linux-headers-6.12.55-armv7-fpga'
```

## Load uiomem

```console
shell$ sudo insmod uiomem/uiomem.ko
```

## Load FPGA and Device Tree

```console
shell$ sudo rake install
cp plbram_256k_dbg.bin /lib/firmware/plbram_256k_dbg.bin
dtbocfg.rb --install plbram_256k --dts plbram_256k_dbg.dts
<stdin>:16.18-20.20: Warning (unit_address_vs_reg): /fragment@1/[ 4275.116448] fpga_manager fpga0: writing plbram_256k_dbg.bin to Xilinx Zynq FPGA Manager
__overlay__/uiomem_plbram: node has a reg or ranges property, but no unit name
<stdin>:21.9-24.6: Warning (unit_address_vs_reg): /fragment@1/__overlay__/led4: node has a reg or ranges property, but no unit name
<stdin>:9.13-26.5: Warning (avoid_unnecessary_addr_size): /fragment@1: unnecessary #address-cells/#size-cells without "ranges", "dma-ranges" or child "reg" property
[ 4275.422192] OF: overlay: WARNING: memory leak will occur if overlay removed, property: /axi/fpga-region0/firmware-name
[ 4275.435235] uiomem uiomem0: driver version = 1.0.0-alpha.6
[ 4275.440833] uiomem uiomem0: major number   = 242
[ 4275.445462] uiomem uiomem0: minor number   = 0
[ 4275.449965] uiomem uiomem0: range address  = 0x40000000
[ 4275.455209] uiomem uiomem0: range size     = 262144
[ 4275.462456] uiomem 40000000.uiomem_plbram: driver installed.
```

## Build plbram_test

```console
shell$ rake plbram_test
gcc  -o plbram_test plbram_test.c
```

# Run plbram_test

```console
shell$ ./plbram_test
size=262144
mmap write test : sync=1 time=6.560504 msec (6.556286 msec)
mmap read  test : sync=1 time=5.679575 msec (5.675327 msec)
compare = ok
mmap write test : sync=0 time=2.592458 msec (1.054030 msec)
mmap read  test : sync=1 time=5.682437 msec (5.677997 msec)
compare = ok
mmap write test : sync=1 time=6.729284 msec (6.724484 msec)
mmap read  test : sync=0 time=2.327539 msec (1.299808 msec)
compare = ok
mmap write test : sync=0 time=2.552798 msec (1.026339 msec)
mmap read  test : sync=0 time=2.553763 msec (1.300330 msec)
compare = ok
file write test : sync=1 time=1.984116 msec (1.980120 msec)
mmap read  test : sync=0 time=2.337637 msec (1.299970 msec)
compare = ok
file write test : sync=0 time=2.502085 msec (1.987950 msec)
mmap read  test : sync=0 time=2.346403 msec (1.299460 msec)
compare = ok
mmap write test : sync=0 time=2.528551 msec (1.019151 msec)
file read  test : sync=1 time=1.941360 msec (1.937820 msec)
compare = ok
mmap write test : sync=0 time=2.555677 msec (1.035021 msec)
file read  test : sync=0 time=2.895531 msec (1.893168 msec)
compare = ok
```

# Clean up

```console
shell$ sudo rake uninstall
dtbocfg.rb --remove plbram_256k
[ 4376.789928] uiomem 40000000.uiomem_plbram: driver removed.
```

# Build Bitstream file

## Requirement

* Vivado 2019.2

## Download this repository

```console
shell$ git clone https://github.com/ikwzm/PLBRAM-ZYBO-Z7
Cloning into 'PLBRAM-ZYBO-Z7'...
remote: Enumerating objects: 30, done.
remote: Counting objects: 100% (30/30), done.
remote: Compressing objects: 100% (22/22), done.
remote: Total 30 (delta 7), reused 30 (delta 7), pack-reused 0
Unpacking objects: 100% (30/30), done.
```

## Create Vivado Project

```console
vivado% cd project
vivado% vivado -mode batch -source create_project.tcl
```

## Build Bitstream file

```console
vivado% cd project
vivado% vivado -mode batch -source implementation.tcl
vivado% ./tools/fpga-bit-to-bin.py -f project.runs/impl_1/design_1_wrapper.bit ../plbram_256k.bin
```

