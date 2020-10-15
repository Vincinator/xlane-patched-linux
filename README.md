# ASGARD-Kernel Repo
Each {stable,dev}/<version> branch contains a Linux kernel source bundled with sassy. 
The ASGARD-Kernel is composed of the Linux kernel source and the ASGARD sources from the [ASGARD Repo](https://github.com/Distributed-Systems-Programming-Group/sassy). ASGARD is included as git subtree at ```net/sassy```.
The Linux kernel sources in this repository also include an optimized kernel configuration. Furthermore, the kernel sources here can be modified for debugging and testing purposes. But the real (sassy) deal is provided by the [ASGARD Repo](https://github.com/Distributed-Systems-Programming-Group/sassy).

The stable branches contain the latest sassy milestone features. stable branches are tested, the stable sassy kernels can be deployed without the host system exploding. But anything can happen with the dev sassy kernel versions - for example: compile error, does not boot, feature broken, or explosions. 



