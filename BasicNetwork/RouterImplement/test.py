from mininet.net import Mininet
from mininet.cli import CLI
from mininet.link import Link

if '__main__' == __name__:

    net = Mininet()

    # Add Hosts
    h1 = net.addHost('h1')
    h2 = net.addHost('h2')

    #Add L2 Switch s5
    s5 = net.addHost('s5')

    # link names are auto generated in the order eth0 eth1 eth2..
    # as they are added on the device
    Link(h1, s5) # h1-(h1-eth0)------------(s5-eth0)-s5
    
    # If you want to explicitely insert the link between two devices such
    # that you can confiugre your own parameters on the link, you can
    # use below cmd for link insertion:
    # link_h1s5 = net.addLink( h1, s5 , intfName1 = 'h1-s5', intfName2 = 's5-h1')
    # The link inserted shal be using above cmd shall be :  
    # h1-(h1-s5)------------(s5-h1)-s5

    Link(h2, s5) # h2-(h2-eth0)------------(s5-eth1)-s5

    net.build()

    # Remove default IP addresses from host's interfaces 
    h1.cmd("ifconfig h1-eth0 0")
    h2.cmd("ifconfig h2-eth0 0")

    # Remove default IP addresses from Switch's Interfaces.
    # Switch Interfaces do not contain any IP addresses anyway
    s5.cmd("ifconfig s5-eth0 0")
    s5.cmd("ifconfig s5-eth1 0")

    # Create a vlan 10 on Switch s5
    # Note to use brctl, you must have bridge-utils pkg installed.
    # apt-get install bridge-utils
    s5.cmd("brctl addbr vlan10")
    # Bring up the Vlan interfaces on L2 switch up
    s5.cmd("ifconfig vlan10 up")

    # Add s5-eth0 to vlan 10 on L2 siwtch in Access mode
    s5.cmd("brctl addif vlan10 s5-eth0")
    # Add s5-eth1 to vlan 10 on L2 siwtch in Access mode
    s5.cmd("brctl addif vlan10 s5-eth1")


    # Assign IP Address to Hosts as ususal
    h1.cmd("ifconfig h1-eth0 10.0.10.1 netmask 255.255.255.0")
    h2.cmd("ifconfig h2-eth0 10.0.10.2 netmask 255.255.255.0")

    h1.cmd("ip route add default via 10.0.10.254 dev h1-eth0")
    h2.cmd("ip route add default via 10.0.10.254 dev h2-eth0")
    
    # Start Mininet Cli prompt
    CLI(net)

    net.stop()