# EFInvaders
EFInvaders is a Space Invaders UEFI application inspired by [GRUB Invaders by erikyyy](http://www.erikyyy.de/invaders/). That can easily be added to your [EDK2](https://github.com/tianocore/edk2) project as a lovely easter egg.

![EFInvaders](images/grubinvaders.png = x720) 

## UEFI Compliance
EFInvaders utilizes only two UEFI protocols EFI\_SIMPLE\_TEXT\_OUTPUT\_PROTOCOL to display the ascii art video frames, and EFI\_SIMPLE\_TEXT\_INPUT\_EX\_PROTOCOL.

## Building
To compile EFInvaders, add it to your EDK2 project by adding the EFInvaders.inf to your projects dsc file.
```
[Components]
  ../EFInvaders/EFInvaders.inf
```
EFInvaders will then compile with your EDK2 project as an EFI application that can be copied to your EFI partition or embedded in your project image.

## Running
 
 In your EFI shell simply navigate to where you saved the application and invoke it as a command.
```
fs0:
cd /cool/stuff
EFInvaders.efi
```
 
## Limitations
While the game is fully functional, the keyboard inputs are limited to the keyboard firmware repeat rate. Newer EDK2 versions use a reasonably fast repeat rate closer to what a user would expect from an operating system keyboard driver however older versions may have slower repeat rates.
