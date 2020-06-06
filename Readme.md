PLBRAM-ZYBO-Z7
=======================================================================

This Repository provides example for u-dma-buf and FPGA-SoC-Linux.

# Requirement

 * Board: ZYBO-Z7
 * OS: FPGA-SoC-Linux v1.0.1 ([https://github.com/ikwzm/FPGA-SoC-Linux/tree/v1.0.1])
 * u-dma-buf v2.2.0-rc3 ([https://github.com/ikwzm/udmabuf/tree/v2.2.0-rc3])

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

