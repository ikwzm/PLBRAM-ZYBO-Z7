PLBRAM-ZYBO-Z7
=======================================================================

This Repository provides example for uiomem and FPGA-SoC-Linux.

## Requirement

 * Board: ZYBO-Z7
 * OS:
   + ~~FPGA-SoC-Linux v1.0.1 https://github.com/ikwzm/FPGA-SoC-Linux/tree/v1.0.1~~
   + [FPGA-SoC-Debian12](https://github.com/ikwzm/FPGA-SoC-Debian12.git)
   + [FPGA-SoC-Debian13](https://github.com/ikwzm/FPGA-SoC-Debian13.git)
 * uiomem (v1.1.0-beta.1) https://github.com/ikwzm/uiomem/tree/v1.1.0-beta.1

## Setup

### Boot ZYBO-Z7 and login fpga user

fpga'password is "fpga".

```console
debian-fpga login: fpga
Password:
fpga@debian-fpga:~$
```

### Download this repository

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

### Build uiomem and uiomem-test

#### Update submodules

```console
shell$ cd PLBRAM-ZYBO-Z7/
shell$ git submodule init
Submodule 'LED_AXI' (https://github.com/ikwzm/LED_AXI) registered for path 'LED_AXI'
Submodule 'uiomem' (https://github.com/ikwzm/uiomem.git) registered for path 'uiomem'
Submodule 'uiomem-test' (https://github.com/ikwzm/uiomem-test) registered for path 'uiomem-test'
shell$ git submodule update
Cloning into '/home/fpga/examples/PLBRAM-ZYBO-Z7/LED_AXI'...
Cloning into '/home/fpga/examples/PLBRAM-ZYBO-Z7/uiomem'...
Cloning into '/home/fpga/examples/PLBRAM-ZYBO-Z7/uiomem-test'...
Submodule path 'LED_AXI': checked out '94025de5f5f2e1c4480c3a23851b14a34fa87a9d'
Submodule path 'uiomem': checked out 'd9ddd8035832ea77b4cbfec69d3a4aac5893dc14'
Submodule path 'uiomem-test': checked out '9023c48045d853cc6b6202859dd00848f7949dbc'
```

#### Build uiomem kenrel module

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
shell$ cd ..
```

#### Build uiomem test programs

```console
shell$ cd uiomem-test
gcc -O2 -DUSE_UIOMEM_IOCTL -o uiomem-file-test uiomem-file-test.c uiomem.c
gcc -O2 -DUSE_UIOMEM_IOCTL -o uiomem-ioctl-test uiomem-ioctl-test.c
gcc -O2 -DUSE_UIOMEM_IOCTL -o uiomem-throughput-test uiomem-throughput-test.c uiomem.c
shell$ cd ..
```

### Load uiomem

```console
shell$ sudo insmod uiomem/uiomem.ko
```

### Load FPGA and Device Tree

```console
shell$ sudo rake install
gzip -d -f -c plbram_256k_dbg.bin.gz > /lib/firmware/plbram_256k_dbg.bin
./dtbo-config --install plbram_256k --dts plbram_256k_dbg.dts
<stdin>:16.18-20.20: Warning (unit_address_vs_reg): /fragment@1/__overlay__/uiomem_plbram: node has a reg or ranges property, bu[ 1965.062052] fpga_manager fpga0: writing plbram_256k_dbg.bin to Xilinx Zynq FPGA Managert no unit name
<stdin>:21.9-24.6: Warning (unit_address_vs_reg): /fragment@1/__overlay__/led4: node has a reg or ranges property, but no unit name
<stdin>:9.13-26.5: Warning (avoid_unnecessary_addr_size): /fragment@1: unnecessary #address-cells/#size-cells without "ranges", "dma-ranges" or child "reg" property
[ 1965.362018] OF: overlay: WARNING: memory leak will occur if overlay removed, property: /axi/fpga-region0/firmware-name
[ 1965.375150] uiomem uiomem0: driver version = 1.1.0-beta.1
[ 1965.382643] uiomem uiomem0: ioctl version  = 1
[ 1965.388718] uiomem uiomem0: major number   = 244
[ 1965.393368] uiomem uiomem0: minor number   = 0
[ 1965.401207] uiomem uiomem0: range address  = 0x40000000
[ 1965.406539] uiomem uiomem0: range size     = 262144
[ 1965.411431] uiomem uiomem0: cached         = 1
[ 1965.415880] uiomem uiomem0: coherent       = 0
[ 1965.420394] uiomem uiomem0: sync_operation = ARMV7 Native
[ 1965.425802] uiomem uiomem0: shareable      = 0
[ 1965.430353] uiomem 40000000.uiomem_plbram: driver installed.
```

## Run uiomem-file-test

```console
shell$ sudo uiomem-test/uiomem-file-test
device=uiomem0
driver_version=1.1.0-beta.1
sync_operation=ARMV7 Native
ioctl_version=1
size=262144
shareable=0
cached=1
coherent=0
mmap write test : sync=1 time=0.000000 sec (0.006379 sec)
mmap read  test : sync=1 time=0.000000 sec (0.006462 sec)
compare = ok
mmap write test : sync=0 time=0.000000 sec (0.002565 sec)
mmap read  test : sync=1 time=0.000000 sec (0.005663 sec)
compare = ok
mmap write test : sync=1 time=0.000000 sec (0.006673 sec)
mmap read  test : sync=0 time=0.000000 sec (0.002315 sec)
compare = ok
mmap write test : sync=0 time=0.000000 sec (0.002532 sec)
mmap read  test : sync=0 time=0.000000 sec (0.002440 sec)
compare = ok
file write test : sync=1 time=0.000000 sec (0.002008 sec)
mmap read  test : sync=0 time=0.000000 sec (0.002328 sec)
compare = ok
file write test : sync=0 time=0.000000 sec (0.002584 sec)
mmap read  test : sync=0 time=0.000000 sec (0.002330 sec)
compare = ok
mmap write test : sync=0 time=0.000000 sec (0.002515 sec)
file read  test : sync=1 time=0.000000 sec (0.001935 sec)
compare = ok
mmap write test : sync=0 time=0.000000 sec (0.002535 sec)
file read  test : sync=0 time=0.000000 sec (0.002897 sec)
compare = ok
```

## Run uiomem-throughput-test

* sync=0: mmap is cacheable, and cache synchronization is performed before and after each access.
* sync=1: mmap is non-cacheable, and cache synchronization is not performed before or after accesses.

```console
sudo ./uiomem-test/uiomem-throughput-test
device=uiomem0
driver_version=1.1.0-beta.1
sync_operation=ARMV7 Native
ioctl_version=1
size=262144
shareable=0
cached=1
coherent=0
mmap write test : sync=0 throughput=121.5 MBytes/sec
mmap read  test : sync=0 throughput=109.2 MBytes/sec
mmap write test : sync=1 throughput= 27.7 MBytes/sec
mmap read  test : sync=1 throughput= 25.3 MBytes/sec
```

## Clean up

```console
shell$ sudo rake uninstall
dtbocfg.rb --remove plbram_256k
[ 4376.789928] uiomem 40000000.uiomem_plbram: driver removed.
```

## Build Bitstream file

### Requirement

* Vivado 2019.2

### Download this repository

```console
shell$ git clone https://github.com/ikwzm/PLBRAM-ZYBO-Z7
Cloning into 'PLBRAM-ZYBO-Z7'...
remote: Enumerating objects: 30, done.
remote: Counting objects: 100% (30/30), done.
remote: Compressing objects: 100% (22/22), done.
remote: Total 30 (delta 7), reused 30 (delta 7), pack-reused 0
Unpacking objects: 100% (30/30), done.
```

### Create Vivado Project

```console
vivado% cd project
vivado% vivado -mode batch -source create_project.tcl
```

### Build Bitstream file

```console
vivado% cd project
vivado% vivado -mode batch -source implementation.tcl
vivado% ./tools/fpga-bit-to-bin.py -f project.runs/impl_1/design_1_wrapper.bit ../plbram_256k.bin
```

