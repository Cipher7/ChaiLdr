# ChaiLdr - AV Evasive Payload Loader

A simple shellcode loader built with the concepts of Malware development I have learnt till now.

## Features 

- Indirect syscalls with SysWhispers3 - jumper_randomized
- QueueUserAPC Injection
- HTTP/S shellcode staging
- Execution delay using API Hammering
- IAT Camouflage
- API Hashing

## Testing with Havoc and Latest Windows Defender

![windows-defender](./images/win10.png)

![havoc-shell](./images/shell.png)

---

> **NOTE :**\
> 
> **CRT Library Removal :** I tried a lot to get the payload working with CRT Library removed and make it independent with custom intrinsic functions for - memcpy, memset, rand, srand etc, but ended up with a lot of crashes and after hours of debugging couldn't get it working, i'll incorporate it into a dev branch soon enough. I have used minicrt, MiniCRT etc.  
>
> **Shellcode Encryption :** The shellcode is fetched from a remote server, with provided support for SSL. As of now I haven't incorporated any shellcode encryption as Havoc already implements it in it's shellcode.
>
> **Why not HellGate? :** HellGate incorporated only direct syscalls, HellsHall uses indirect syscalls, but I'm still learning that, will definitely build my own custom implementation of it in the future
>
> **EDR Evasion ? :** This is a simple shellcode payload loader, it can bypass a lot of antivirus softwares and also some EDRs but the techniques it incorporates aren't the best, so as I keep learning i'll make better loaders! 

