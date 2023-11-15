#!/usr/bin/env python

"""
topo1.py: Network for question 1 with Linux IP routers

Converts a Node into a router using IP forwarding
already built into Linux.

The topology creates 3 routers and 4 IP subnets:

    - 192.168.1.0/24 (ra-h1-h2, IP: 192.168.1.{1, 100, 101})
    - 192.168.2.0/24 (rb-h3-h4, IP: 192.168.2.{1, 100, 101})
    - 192.168.3.0/24 (rc-h5-h6, IP: 192.168.3.{1, 100, 101})
    - 10.0.0.0/8 (ra-rb-rc, IP: 10.0.0.{1, 2, 3})

The example relies on default routing entries that are
automatically created for each router interface, as well
as 'defaultRoute' parameters for the host interfaces.

Additional routes may be added to the router or hosts by
executing 'ip route' or 'route' commands on the router or hosts.
"""


from mininet.topo import Topo
from mininet.net import Mininet
from mininet.node import Node
from mininet.log import setLogLevel, info
from mininet.cli import CLI


class LinuxRouter( Node ):
    "A Node with IP forwarding enabled."

    # pylint: disable=arguments-differ
    def config( self, **params ):
        super( LinuxRouter, self).config( **params )
        # Enable forwarding on the router
        self.cmd( 'sysctl net.ipv4.ip_forward=1' )

    def terminate( self ):
        self.cmd( 'sysctl net.ipv4.ip_forward=0' )
        super( LinuxRouter, self ).terminate()


class NetworkTopo( Topo ):
    "Topology for question 1"

    # pylint: disable=arguments-differ
    def build( self, **_opts ):
        
        ra_IP = '192.168.1.1/24'
        rb_IP = '192.168.2.1/24'
        rc_IP = '192.168.3.1/24'
        host_route_1 = f'via {ra_IP[:-3]}'
        host_route_2 = f'via {rb_IP[:-3]}'
        host_route_3 = f'via {rc_IP[:-3]}'

        ra = self.addNode( 'ra', cls=LinuxRouter, ip=ra_IP )
        rb = self.addNode( 'rb', cls=LinuxRouter, ip=rb_IP )
        rc = self.addNode( 'rc', cls=LinuxRouter, ip=rc_IP )

        s1, s2, s3, s4 = [ self.addSwitch( s ) for s in ( 's1', 's2', 's3', 's4' ) ]
        
        self.addLink( s1, ra, intfName2='ra-eth0',
                      params2={ 'ip' : ra_IP } )
        self.addLink( s2, rb, intfName2='rb-eth0',
                      params2={ 'ip' : rb_IP } )
        self.addLink( s3, rc, intfName2='rc-eth0',
                      params2={ 'ip' : rc_IP } )
        
        self.addLink( s4, ra, intfName2='ra-eth1',
                      params2={ 'ip' : '10.0.0.1/8' } )
        self.addLink( s4, rb, intfName2='rb-eth1',
                      params2={ 'ip' : '10.0.0.2/8' } )
        self.addLink( s4, rc, intfName2='rc-eth1',
                      params2={ 'ip' : '10.0.0.3/8' } )
        
        h1 = self.addHost( 'h1', ip= '192.168.1.100/24',
                           defaultRoute=host_route_1 )
        h2 = self.addHost( 'h2', ip='192.168.1.101/24',
                           defaultRoute=host_route_1 )
        h3 = self.addHost( 'h3', ip='192.168.2.100/24',
                           defaultRoute=host_route_2 )
        h4 = self.addHost( 'h4', ip='192.168.2.101/24',
                           defaultRoute=host_route_2 )
        h5 = self.addHost( 'h5', ip='192.168.3.100/24',
                           defaultRoute=host_route_3 )
        h6 = self.addHost( 'h6', ip='192.168.3.101/24',
                           defaultRoute=host_route_3 )
        
        self.addLink(h1,s1)
        self.addLink(h2,s1)
        self.addLink(h3,s2)
        self.addLink(h4,s2)
        self.addLink(h5,s3)
        self.addLink(h6,s3)
        
def run():
    "Test the topology"
    topo = NetworkTopo()
    net = Mininet( topo=topo,
                   waitConnected=True )  # controller is used by switches
    
    # Add routing for reaching networks that aren't directly connected
    info(net['ra'].cmd("ip route add 192.168.2.0/24 via 10.0.0.2 dev ra-eth1"))
    info(net['ra'].cmd("ip route add 192.168.3.0/24 via 10.0.0.3 dev ra-eth1"))
    info(net['rb'].cmd("ip route add 192.168.1.0/24 via 10.0.0.1 dev rb-eth1"))
    info(net['rb'].cmd("ip route add 192.168.3.0/24 via 10.0.0.3 dev rb-eth1"))
    info(net['rc'].cmd("ip route add 192.168.1.0/24 via 10.0.0.1 dev rc-eth1"))
    info(net['rc'].cmd("ip route add 192.168.2.0/24 via 10.0.0.2 dev rc-eth1"))
    
    net.start()
    # info( '*** Routing Table on Router:\n' )
    # info( net[ 'ra' ].cmd( 'route' ) )
    CLI( net )
    net.stop()


if __name__ == '__main__':
    setLogLevel( 'info' )
    run()
