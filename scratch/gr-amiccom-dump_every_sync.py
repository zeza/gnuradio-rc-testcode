#!/usr/bin/python

from gnuradio import gr
from gnuradio import digital
from gnuradio import eng_notation
from gnuradio.eng_option import eng_option
from optparse import OptionParser

import struct
import sys
import string
from math import ceil

#in bits
s_sync=8*4
s_addr=4*8
s_pcf=0
s_payload=256*8
s_crc=2

#in bits
max_sync=4*8
max_addr=4*8
max_pcf=0
max_payload=32*8
max_crc=2*8
max_frame_size=max_sync+max_addr+max_pcf+max_payload+max_crc









class amiccom(gr_top_block):
    def __init__(self):
        gr.top_block.__init__(self)
        input_sample_rate = 500e3
        symbol_rate = 500e3
        output_samples_per_symbol = 2
        output_sample_rate = output_samples_per_symbol * symbol_rate






infile = open(sys.argv[1], 'rt')
#outfile = open(sys.argv[2], 'w')

raw = infile.read();
data = struct.unpack('B1'*len(raw),raw)

def pack_bits(start,length=8):
    byte=0
    for i in range(length):
        byte = byte << 1
        byte = byte | (data[start+i] & 0x01)

    return byte


def decode_frame(start):

    frame = []
    for i in range(ceil(max_frame_size/8)):
        frame.append(pack_bits(start + i*8))

    print "Frame=" + "".join(c[2:4]+" " for c in map(hex,frame))
    print " " 



# look for correlations flagged by gr_correlate_access_code_bb
for i in range(len(data) - max_frame_size ):
    if data[i] & 2:
        decode_frame(i-s_sync)

