#wireless.tcl

# ======================================================================
# Define options
# ======================================================================
set val(chan) Channel/WirelessChannel ;# channel type
set val(prop) Propagation/TwoRayGround ;# radio-propagation model
set val(netif) Phy/WirelessPhy ;# network interface type
set val(mac) Mac/MncPrj ;#Mac/Simple ;# MAC type //TBM
set val(ifq) Queue/DropTail/PriQueue ;# interface queue type //TBM
set val(ll) LL ;# link layer type
set val(ant) Antenna/OmniAntenna ;# antenna model
set val(ifqlen) 50 ;# max packet in ifq //TBM
set val(nn) 51 ;# number of mobilenodes (100 source nodes + 1 sink node)
set val(rp) DumbAgent ;# routing protocol
set val(x) 25 ;# X dimension of topography
set val(y) 25 ;# Y dimension of topography 
set val(stop) 100 ;# time of simulation end
if {($argc == 0)} {
    #do something here
	puts "no arguments passed"
	puts "set to default : repeatnum = 1"
	set argv 1
}
set val(repeatTimes) $argv ;#duplicate copies of packets

# ======================================================================
# Main Program
# ======================================================================
#
# Initialize Global Variables
#
set chan_ [new $val(chan)]
set ns [new Simulator]
set tracefd [open trace_file.tr w]
set namtrace [open namfile.nam w] 
$ns trace-all $tracefd
#$ns namtrace-all-wireless $namtrace $val(x) $val(y)

# set up topography object
set topo [new Topography]

$topo load_flatgrid $val(x) $val(y)

create-god $val(nn)
#add code to bind repeat times with protocol
$val(mac) set repeatnum $val(repeatTimes)
# ======================================================================
#node configuration 
# ======================================================================
 $ns node-config -adhocRouting $val(rp) \
		-llType $val(ll) \
		-macType $val(mac) \
		-ifqType $val(ifq) \
		-ifqLen $val(ifqlen) \
		-antType $val(ant) \
		-propType $val(prop) \
		-phyType $val(netif) \
		-topoInstance $topo \
		-agentTrace ON \
		-routerTrace ON \
		-macTrace ON \
		-movementTrace ON \
		-channel $chan_

for {set i 0} {$i < $val(nn) } { incr i } {
set n($i) [$ns node] 
}
#setup random number generator
set rng [new RNG]
$rng seed 1

#random variable for node positioning
set rand1 [new RandomVariable/Uniform]
$rand1 use-rng $rng
$rand1 set min_ -25.0
$rand1 set max_ 25.0
for {set i 0} {$i < $val(nn)} {incr i} {
$n($i) random-motion 0
$n($i) set X_ [expr 25 + [$rand1 value]]
$n($i) set Y_ [expr 25 + [$rand1 value]]
}

for {set i 0} {$i < $val(nn)} {incr i} {
	$ns initial_node_pos $n($i) 1
}

#sink node
set sink [new Agent/LossMonitor]
$ns attach-agent $n(0) $sink
# traffic for all nodes 
for {set i 1} {$i < $val(nn)} {incr i} {
	set protocol($i) [new Agent/UDP]
	$ns attach-agent $n($i) $protocol($i)
	$ns connect $protocol($i) $sink
	set traffic($i) [new Application/Traffic/CBR]
	$traffic($i) set packetSize_ 128
	$traffic($i) set interval_ 0.02
	$traffic($i) attach-agent $protocol($i)
}

for {set i 1} {$i < $val(nn)} {incr i} {
	set init [expr 0 + {0.02/$val(nn)}]
	$ns at [expr {$i*$init}] "$traffic($i) start"
}

# Telling nodes when the simulation ends
for {set i 0} {$i < $val(nn) } { incr i } {
$ns at $val(stop) "$n($i) reset";
}

# ======================================================================
#update stats
# ======================================================================
set stats [open stats.dat w]
proc update_stats {} {
global sink
global stats
set ns [new Simulator]
set NOW [$ns now]
set bytes [$sink set bytes_]
#reset bytes value
$sink set bytes_ 0
#reschedule the procedure
set time 1 
$ns at [expr $NOW + $time] "update_stats"
}

# ending nam and the simulation
#$ns at $val(stop) "$ns nam-end-wireless $val(stop)"
$ns at $val(stop) "stop"

#finishing procedure
proc stop {} {
#global ns tracefd namtrace
global ns tracefd
$ns flush-trace

close $tracefd
#close $namtrace
#exec nam namfile.nam &
}

#update status
$ns at 1 "update_stats"
$ns at $val(stop) "puts \"end simulation\" ; $ns halt"
#tell ns to run
$ns run
