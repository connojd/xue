# Xue USB 3.0 Debugger

Xue is a cross-platform driver for the USB 3.0 Debug Capability (DbC). It is a
header-only library so that it may be easily included into various
environments. The goal of Xue is to provide a high-bandwidth, low-level
debugger that can be used with two standard mobile/laptop devices without the
need for legacy UART hardware.

## Hardware Requirements

To use Xue, you need two machines that both have USB 3.0 xHCI host controllers.
The machine you wish to debug is referred to as the target, and the one you view
the debug output from is the host. The target machine must implement the Debug
Capability as described in the xHCI specification. So far the following Intel
(PCI vendor 0x8086) host controllers have been tested to work:

  - Z370 (PCI device 0xA2AF)
  - Z390 (PCI device 0xA36D)
  - Wildcat Point-LP (PCI device 0x9CB1)
  - Sunrise Point-LP (PCI device 0x9D2F)

Other device not listed here will likely work; if you have one not listed
please submit an issue or PR so that support for it can be added. The target
and host machines must be connected with a USB 3.0 A/A debug cable which can be
purchased here:

  https://www.datapro.net/products/usb-3-0-super-speed-a-a-debugging-cable.html

The final requirement is that the cable must be connected directly to a root
Super-Speed port on the target, i.e., it will not work through a hub. The only
host-side requirement is that is must be connected to a Super-Speed port.

## Target-side Usage

Xue is officially supported to run from the following environments:

  - UEFI applications (based on gnuefi)
  - Linux kernel modules
  - Bareflank hypervisor
  - Xen hypervisor

### Limitations

  - Does not run from Windows 10. Based on earlier experiments, Windows
    does not allow multiple drivers to map in a given PCI MMIO region
    simultaneously. This prevents Xue from mapping in the DbC's registers
    because the Windows xHCI driver already owns the mapping. If any one
    knows a workaround for this, please don't hesitate to share.

  - Requires passing 'usbcore.nousb' to the Linux kernel commandline prior to
    booting. Without this option, the Linux xHCI driver will reset the host
    controller, which also resets the DbC. Providing this parameter means you
    will have no USB at all, but depending on your debugging scenario, this may
    be acceptable, e.g., you can ssh into the target machine after boot.
