#/bin/bash

# Exit on failure
#set -e


{
    #cd asgard-bin/kernel
    cd "$(dirname "$0")"

    echo $TARGET_BIN &&

    # Get current installed asgard kernel version (based on git commit)
    CUR_INST_KERN_VER=$(echo $(uname -r) | cut -d"-" -f3) &&

    # Exit if Asgard Kernel Version is already installed
    if ls *${CUR_INST_KERN_VER}* 1> /dev/null 2>&1; then
        echo "Asgard Kernel Version ${CUR_INST_KERN_VER} is already installed."
        exit 0
    fi

    # Prepare GRUB
    # CPU Isol + Huge Tables
    echo "Setting up boot parameters..." &&
    sudo sed -i "s|GRUB_CMDLINE_LINUX=.*|GRUB_CMDLINE_LINUX='intel_pstate=disable poll=idle intel_idle.max_cstate=0 processor.max_cstate=0 nohz_full=50-55 isolcpus=nohz,50-55 rcu_nocbs=50-55 mce=off cpuidle.off=1 powersave=off nonmi_ipi nosoftlockup nowatchdog acpi_irq_nobalance loglevel=7 default_hugepagesz=1G hugepagesz=1G hugepages=8'|g" /etc/default/grub &&

    # Append hugetable entry if it does not already exist
    sudo grep -qxF 'nodev /mnt/huge_1GB hugetlbfs pagesize=1GB 0 0' /etc/fstab || sudo echo 'nodev /mnt/huge_1GB hugetlbfs pagesize=1GB 0 0' >> /etc/fstab

    echo "Removing old Asgard Kernels..." &&
    sudo DEBIAN_FRONTEND=noninteractive dpkg --configure -a &&
    sudo DEBIAN_FRONTEND=noninteractive apt-get purge 'linux-image-5.9-asgard-.*' -y || echo "..." &&
    dpkg -l | grep asgard | awk '{print $2}' | xargs -n1 sudo apt purge -y || echo "..." &&

    echo "Install new Asgard Kernel..." &&
    sudo DEBIAN_FRONTEND=noninteractive dpkg --configure -a &&
    sudo DEBIAN_FRONTEND=noninteractive dpkg -i linux-*.deb &&

    echo "Updating Grub..." &&
    sudo update-grub &&

    echo "update links and cache for new shared libraries..." &&
    sudo ldconfig &&

    echo "clean up.." &&
    sudo rm -f /home/dsp/asgard-bin/*.deb &&

    echo "rebooting system now..."&&
    sudo reboot now

} || {
    echo "cleaning up after failure.."
    sudo rm -f /home/dsp/asgard-bin/*.deb
    exit 1
}

exit 0