# README

The PIO driver is coming from the pico-examples and is Copyright (c) 2020 Raspberry Pi (Trading) Ltd.
It is covered by the license included in this folder.

The source code has been modified a little bit to use it from Arm-2D framework.

You can modify the Arm-2D headers :

`RTE_Components.h`		
`arm_2d_cfg.h`	
`arm_2d_disp_adapter_0.h`

You can override some implementations with new ones using hardware acceleration by adding some function to `rp2040_hw_acceleration.c`

