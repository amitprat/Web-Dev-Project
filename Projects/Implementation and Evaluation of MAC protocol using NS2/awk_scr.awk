# ======================================================================
# awk_scr.awk
# Count sent/recieved and generated packets and
# calculate ratio X = (recieved/generated)
# ======================================================================
BEGIN {
seqno = -1; 
receivedPackets = 0;
sentPackets= 0;
}
{
#packet delivery ratio
if($4 == "AGT" && $1 == "s" && seqno < $6) {
seqno = $6;
} else if(($4 == "MAC") && ($1 == "r") && ($7 == "cbr")) {
receivedPackets++;
} else if(($4 == "MAC") && ($1 == "s") && ($7 == "cbr")) {
sentPackets++;
}
}
END { 
print "===========================================================\n";
print "GeneratedPackets = " seqno+1;
print "ReceivedPackets = " receivedPackets;
print "SentPackets = " sentPackets;
print "Packet Delivery Ratio (X = Recv/Generated) = " (receivedPackets/ ((seqno+1))) "%";
print "============================================================\n";
}
