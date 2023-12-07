from mininet.topo import Topo
from mininet.net import Mininet
from mininet.node import Controller, OVSSwitch
from mininet.cli import CLI
from mininet.log import setLogLevel, info

class CustomTopo(Topo):
    "Custom topology with 3 routers (switch+host) and 6 hosts."

    def build(self):
        # Add hosts and switches
        hosts = [self.addHost(f'h{i}') for i in range(1, 7)]
        switches = [self.addSwitch(f's{i}') for i in range(1, 4)]

        # Add links between hosts and switches
        for i in range(3):
            self.addLink(hosts[i*2], switches[i])
            self.addLink(hosts[i*2+1], switches[i])

        # Add links between switches
        self.addLink(switches[0], switches[1])
        self.addLink(switches[1], switches[2])
        self.addLink(switches[0], switches[2])

def run():
    "Create and run the network"
    topo = CustomTopo()
    net = Mininet(topo=topo, controller=Controller, switch=OVSSwitch)
    net.start()

    # Assign IP addresses
    for i in range(1, 7):
        net.get(f'h{i}').setIP(f'192.168.{(i-1)//2+1}.{100+(i-1)%2}/24', intf=f'h{i}-eth0')

    CLI(net)
    net.stop()

if _name_ == '_main_':
    setLogLevel('info')
    run()