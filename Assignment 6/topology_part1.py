from mininet.topo import Topo
from mininet.net import Mininet
from mininet.node import Controller, OVSSwitch
from mininet.cli import CLI
from mininet.log import setLogLevel, info

class CustomTopo(Topo):
    "Custom topology with 3 routers (switch+host) and 6 hosts."

    def build(self):
        # Add hosts and switches
        hosts = [self.addHost('h{}'.format(i)) for i in range(1, 7)]
        switches = [self.addSwitch('s{}'.format(i)) for i in range(1, 4)]

        # Add links between hosts and switches
        for i in range(3):
            self.addLink(hosts[i*2], switches[i])
            self.addLink(hosts[i*2+1], switches[i])
	    

        # Add links between switches
        self.addLink(switches[0], switches[1])
        self.addLink(switches[1], switches[2])
        #self.addLink(switches[0], switches[2])

def run():
    "Create and run the network"
    topo = CustomTopo()
    net = Mininet(topo=topo, controller=Controller, switch=OVSSwitch)
    net.start()
    h1, h2, h6, r1, r2, r3 = net.get('h1', 'h2','h6', 's1', 's2', 's3')

    r1.cmd('tcpdump -i s1-eth1 -w r1.pcap &')  # replace 's1-eth1' with the name of the interface you want to capture on
    # Assign IP addresses
    for i in range(1, 7):
        net.get('h{}'.format(i)).setIP('192.168.{}.{}'.format((i-1)//2+1, 100+(i-1)%2), intf='h{}-eth0'.format(i))

    print("Ping h1 to h2: ")

    print(h1.cmd('ping -c 4 %s' % h2.IP()))
	



    # Measure the latency with ping
    print('Ping latency:')
    print(h1.cmd('ping -c 4 %s' % h6.IP()))
    

    # Measure the latency with iperf
    print('Iperf latency:')
    h6.cmd('iperf -s &')  # start iperf server on h6
    iperf_print=h1.cmd('iperf -c %s -t 10' % h6.IP())  # start iperf client on h1
    print(iperf_print)
   
    print("Changing routes...")

    h1.cmd('ip route add default via %s' % r1.IP())
    r1.cmd('ip route add default via %s' % r2.IP())
    r2.cmd('ip route add default via %s' % r3.IP())
    r3.cmd('ip route add default via %s' % h6.IP())
    print('Ping latency:')
    print(h1.cmd('ping -c 4 %s' % h6.IP()))
    

    # Measure the latency with iperf
    print('Iperf latency:')

    iperf_print=h1.cmd('iperf -c %s -t 10' % h6.IP())  # start iperf client on h1
    print(iperf_print)
    CLI(net)
    r1.cmd('pkill -SIGINT tcpdump')
    net.stop()


if __name__ == '__main__':
    setLogLevel('info')
    run()
