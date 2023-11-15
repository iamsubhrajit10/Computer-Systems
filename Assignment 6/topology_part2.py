#!/usr/bin/env python

"""
topo2.py: Network for question 2
"""


from mininet.topo import Topo
from mininet.net import Mininet
from mininet.node import Node
from mininet.log import setLogLevel, info
from mininet.cli import CLI

class NetworkTopo( Topo ):
    "Topology for question 2"

    # pylint: disable=arguments-differ
    def build( self, **_opts ):

        s1, s2 = [ self.addSwitch( s ) for s in ( 's1', 's2' ) ]

        h1 = self.addHost( 'h1', ip ='192.168.1.1/24')
        h2 = self.addHost( 'h2', ip ='192.168.1.2/24')
        h3 = self.addHost( 'h3', ip ='192.168.1.3/24')
        h4 = self.addHost( 'h4', ip ='192.168.1.4/24')
        
        self.addLink(h1,s1)
        self.addLink(h2,s1)
        self.addLink(h3,s2)
        self.addLink(h4,s2)
        self.addLink(s1,s2)
        
def run():
    "Test the topology"
    topo = NetworkTopo()
    net = Mininet( topo=topo,
                   waitConnected=True )  # controller is used by switches
    
    net.start()
    # info( '*** Routing Table on Router:\n' )
    # info( net[ 'ra' ].cmd( 'route' ) )
    CLI( net )
    net.stop()


if __name__ == '__main__':
    setLogLevel( 'info' )
    run()
