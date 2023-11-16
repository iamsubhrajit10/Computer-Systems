# CLI Scripts Part 1
run these commands one by one on the CLI of mininet.
Also note incase you abruptly close mininet with ctrl+z,
please run the following command before running the mininet code:
```bash
$>sudo mn -c
```

# Test the network (question a)

run on CLI: 
```bash
mininet>pingall
mininet>dump
```

# Observation of route setup (question b)
start from begining i.e. open the python script by running 
```bash
$>sudo python3 topology_part1.py
```
observing the routes for packets sent from 
```bash
mininet>h1 ping -c 4 h3 
mininet>h1 ping -c 4 h6
```
# Dumping all the routing table (for question d in context of question b)
```bash
mininet>ra route
mininet>rb route
mininet>rc route
```
# Showing wireshark for route setup (question b)
exit the mininet CLI 
```bash
mininet>exit
```
after exiting the mininet , use the tcpdump file to run wireshark.

run on three terminals: 
terminal 1:
```bash
$>sudo wireshark ra_dump.pcap
```
terminal 2:
```bash
$>sudo wireshark rb_dump.pcap
```
terminal 3:
```bash
$>sudo wireshark rc_dump.pcap
```
On the wireshark, you can see various packets.
notably the ICMP packets to trace the routes from h1 to h3 and h1 to h6.

# Routes (question c)
again run the code with
```bash
$>sudo python3 topology_part1.py
```
(make sure you've installed 'traceroute')

first trace the route of packet sent from h1 to h6
run: 
```bash
mininet>h1 traceroute h6
```
It will show the IPs which takes the path h1->ra->rc->h6

Now check ping and iperf for h1 and h6
```bash
mininet>h1 ping h6
mininet>iperf h1 h6
```


# Modification of Routing tables: (question c)
to change the default route of h1->ra->rc->h6 to h1->ra->rb->rc->h6
```bash
mininet>ra ip route del 192.168.3.0/24
mininet>ra ip route add 192.168.3.0/24 via 10.0.0.2 dev ra-eth1
```

Now check the route:
```bash
mininet>h1 traceroute h6
```
It will show the IPs which takes the path h1->ra->rb->rc->h6

Now compare the ping and iperf data with the new route:
```bash
mininet>h1 ping h6
mininet>iperf h1 h6
```

# Dumping all the routing table (for question d in context of question c)
```bash
mininet>ra route
mininet>rb route
mininet>rc route
```

# Done part 1!
```bash
mininet>exit
```





