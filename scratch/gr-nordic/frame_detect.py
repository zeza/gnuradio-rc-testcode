#!/usr/bin/python

from gnuradio import digital
from gnuradio import eng_notation
from gnuradio import gr
from gnuradio.eng_option import eng_option
from optparse import OptionParser

import struct
import sys
import string


s_addr=5
s_control=0
s_payload=32
s_crc=0



infile = open(sys.argv[1], 'rt')
#outfile = open(sys.argv[2], 'w')

raw = infile.read();
data = struct.unpack('B1'*len(raw),raw)


def decode_packet(data):
    #raw_packet = data[loc:loc+48*8]
    text = ''
    for x in range(48):
        text += hex(pack_byte(data[x+(x*8):x+8+(x*8)]))[2:4]
    return text
        

def pack_byte(data):
    byte = 0
    for x in range(len(data)):
        byte = byte<<1
        byte = byte | ord(data[x])
    return byte


for loc in range(len(data)):
    #if data[loc:loc+9] == '\x00\x01\x00\x01\x00\x01\x00\01\00':
    if data[loc:loc+16] == '\x01\x00\x01\x00\x01\x00\x01\x00\01\01\01\01\01\01\01\00':
        #print loc
        print str(loc) + '\t'+ str(decode_packet(data[loc:loc+(48*8)]))
        


