# Packages required
mininet VM, SSH Client with X11
# Installation guide
[Guide link](http://mininet.org/download/#option-1-mininet-vm-installation-easy-recommended) 

If you're running mininet ubuntu server through VM,  
you would need a ssh client and X11 on your client machine to use graphical programs like wireshark.
# Setup VM and SSH into it
Virtual Machine Settings -> Network -> Attached to: Host-only Adapter

In VM:
```bash
$ ip addr
```
Write down the ip address of the ethX interface. It should look like `192.168.X.Y`

On client:
```bash
$ ssh -Y mininet@192.168.X.Y
```

# Run
```bash
$ sudo python <filename.py>
```
It will execute: adding controllers, adding switches, adding hosts etc.

```bash
mininet>
```
Now you may try several options listed by typing help
