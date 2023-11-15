# CLI Scripts Part 1
run these commands one by one on the CLI of mininet

# Test the network

run on CLI: 
```bash
mininet>pingall
```

# Changing routes
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


# Modification of Routing tables:
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

# Dumping all the routing table
```bash
mininet>ra route
mininet>rb route
mininet>rc route
```


# Showing wireshark for route setup
exit the mininet CLI 
```bash
mininet>exit
```
after exiting the mininet , use the tcpdump file to run wireshark.

run: 
```bash
$>sudo wireshark ra_dump.pcap
```
On the wireshark, you can see various packets.
notably the ARP requests, TCP transmissions,etc.


