PLBRAM-ZYBO-Z7
=======================================================================

This Repository provides example for uiomem and FPGA-SoC-Linux.

# Requirement

 * Board: ZYBO-Z7
 * OS: FPGA-SoC-Linux v1.0.1 ([https://github.com/ikwzm/FPGA-SoC-Linux/tree/v1.0.1])
 * uiomem (0.0.4) ([https://github.com/ikwzm/uiomem/tree/develop])

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
shell$ git clone https://github.com/ikwzm/PLBRAM-ZYBO-Z7
Cloning into 'PLBRAM-ZYBO-Z7'...
remote: Enumerating objects: 49, done.
remote: Counting objects: 100% (49/49), done.
remote: Compressing objects: 100% (30/30), done.
remote: Total 49 (delta 21), reused 45 (delta 17), pack-reused 0
Unpacking objects: 100% (49/49), done.
shell$ cd PLBRAM-ZYBO-Z7
shell$ git submodule init
Submodule 'LED_AXI' (https://github.com/ikwzm/LED_AXI) registered for path 'LED_AXI'
Submodule 'uiomem' (https://github.com/ikwzm/uiomem.git) registered for path 'uiomem'
shell$ git submodule update
Cloning into '/home/fpga/examples/PLBRAM-ZYBO-Z7/LED_AXI'...
Cloning into '/home/fpga/examples/PLBRAM-ZYBO-Z7/uiomem'...
Submodule path 'LED_AXI': checked out '94025de5f5f2e1c4480c3a23851b14a34fa87a9d'
Submodule path 'uiomem': checked out 'd28210187505d547fe6e36486f352f688079db50'
```

# Setup

## Build uiomem

```console
shell$ cd uiomem
shell$ make
make -C /lib/modules/5.4.47-armv7-fpga/build ARCH=arm CROSS_COMPILE= M=/home/fpga/examples/PLBRAM-ZYBO-Z7/uiomem obj-m=uiomem.o uiomem.ko
make[1]: Entering directory '/usr/src/linux-headers-5.4.47-armv7-fpga'
  CC [M]  /home/fpga/examples/PLBRAM-ZYBO-Z7/uiomem/uiomem.o
  Building modules, stage 2.
  MODPOST 1 modules
  CC [M]  /home/fpga/examples/PLBRAM-ZYBO-Z7/uiomem/uiomem.mod.o
  LD [M]  /home/fpga/examples/PLBRAM-ZYBO-Z7/uiomem/uiomem.ko
make[1]: Leaving directory '/usr/src/linux-headers-5.4.47-armv7-fpga'
shell$ cd ..
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
<stdin>:16.18-20.20: Warning (unit_address_vs_reg): /fragment@1/__overlay__/uiomem_plbram: node has a reg or ranges property, but no unit name
<stdin>:21.9-24.6: Warning (unit_address_vs_reg): /fragment@1/__overlay__/led4: node has a reg or ranges property, but no unit name
<stdin>:9.13-26.5: Warning (avoid_unnecessary_addr_size): /fragment@1: unnecessary #address-cells/#size-cells without "ranges" or child "reg" property
[  722.212273] fpga_manager fpga0: writing plbram_256k_dbg.bin to Xilinx Zynq FPGA Manager
[  722.290827] OF: overlay: WARNING: memory leak will occur if overlay removed, property: /amba/fpga-region0/firmware-name
[  722.303892] uiomem uiomem0: driver version = 0.0.4
[  722.310286] uiomem uiomem0: major number   = 244
[  722.314909] uiomem uiomem0: minor number   = 0
[  722.321570] uiomem uiomem0: range address  = 0x40000000
[  722.327975] uiomem uiomem0: range size     = 262144
[  722.332854] uiomem 40000000.uiomem_plbram: driver installed.
[  722.341583] uio_pdrv_genirq 43c00000.led4: IRQ index 0 not found
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
mmap write test : sync=1 time=0.006423 sec (0.006420 sec)
mmap read  test : sync=1 time=0.005649 sec (0.005646 sec)
compare = ok
mmap write test : sync=0 time=0.002500 sec (0.000944 sec)
mmap read  test : sync=1 time=0.005673 sec (0.005671 sec)
compare = ok
mmap write test : sync=1 time=0.006579 sec (0.006575 sec)
mmap read  test : sync=0 time=0.002333 sec (0.001299 sec)
compare = ok
mmap write test : sync=0 time=0.002446 sec (0.000936 sec)
mmap read  test : sync=0 time=0.002436 sec (0.001379 sec)
compare = ok
file write test : sync=1 time=0.001312 sec (0.001309 sec)
mmap read  test : sync=0 time=0.002325 sec (0.001300 sec)
compare = ok
file write test : sync=0 time=0.001814 sec (0.001307 sec)
mmap read  test : sync=0 time=0.002405 sec (0.001379 sec)
compare = ok
mmap write test : sync=0 time=0.002454 sec (0.000924 sec)
file read  test : sync=1 time=0.001942 sec (0.001940 sec)
compare = ok
mmap write test : sync=0 time=0.002458 sec (0.000925 sec)
file read  test : sync=0 time=0.002921 sec (0.001908 sec)
compare = ok
```

# Clean up

```console
shell$ sudo rake uninstall
dtbocfg.rb --remove plbram_256k
[ 1725.909727] uiomem 40000000.uiomem_plbram: driver removed.
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

