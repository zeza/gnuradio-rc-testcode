#!/usr/bin/python

from gnuradio import digital
from gnuradio import eng_notation
from gnuradio import gr
from gnuradio.eng_option import eng_option
from optparse import OptionParser

import struct
import sys
import string
from math import ceil

#in bits
s_sync=8*6
s_addr=4*8
s_pcf=0
s_payload=32*8
s_crc=2

s_frame_size=s_sync+s_addr+s_pcf+s_payload+s_crc


infile = open(sys.argv[1], 'rt')
#outfile = open(sys.argv[2], 'w')

raw = infile.read();
data = struct.unpack('B1'*len(raw),raw)

def pack_bits(start,length=4):
    byte=0
    for i in range(length):
        byte = byte << 1
        byte = byte | (data[start+i] & 0x01)

    return byte


def decode_frame(start):

    frame = []
    for i in range(ceil(s_frame_size/4)):
        frame.append(pack_bits(start + i*4))

    #print "Frame=" + "".join(c[2:4]+" " for c in map(hex,frame))
    print "Frame=" + "".join(c[2:4] for c in map(hex,frame))
    #print " " 
    
    #print map(hex,frame)



# look for correlations flagged by gr_correlate_access_code_bb
for i in range(len(data) - s_frame_size ):
    if data[i] & 2:
        decode_frame(i-s_sync)

