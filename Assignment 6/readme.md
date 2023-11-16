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

# part 1
run: 
```bash
$>sudo python3 topology_part1.py
```
then follow the scripts to be typed in CLI_scripts_part1.md

# part 2 
run: 
```bash
$>sudo python3 topology_part2.py --config {b,c} [--loss LOSS] --cc {vegas,reno,cubic,bbr} 
```
loss parameter is optional for question a,b, and c. As per question LOSS ranges between 0 to 3 (0 when no loss, otherwise 1-3)

to exit from CLI, type:
```bash
mininet>exit
```
## for question a) 
see the pings printed on the screen.

## for question b) 
for e.g. we'trying to run with config b (i.e. just h1 to h4), no loss and congestion control scheme reno
execute:
```bash
$>sudo python3 topology_part2.py --config b --cc reno
```


## for question c)
for e.g. we're trying to run with config c (i.e. simulataneously run h1, h2, and h3 to h4), no loss and congestion control scheme bbr
```bash
$>sudo python3 topology_part2.py --config c --cc bbr
```
## for question d)
for e.g. we're trying to run with config b (i.e. just h1 to h4), 3% loss and congestion control scheme bbr
```bash
$>sudo python3 topology_part2.py --config b --loss 3 --cc bbr
```

#
to view the tcpdump at s1 router through wireshark first execute the topology_part2.py, then execute:
```bash
$>sudo wireshark s1_dump.pcap
```
