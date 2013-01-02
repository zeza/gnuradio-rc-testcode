#!/usr/bin/env python
from gnuradio import gr, gru, eng_notation , uhd, digital
from gnuradio.gr import firdes
from gnuradio.eng_option import eng_option
from optparse import OptionParser
from gruel import pmt
from gnuradio.extras import block_gateway
import numpy
import time, sys

import flysky




    
class main(gr.top_block):

    def __init__(self):
        gr.top_block.__init__(self)

        self.samp_rate = samp_rate = 1000000
        self.dump_freq = dump_freq = 2400490000

        self._u = uhd.usrp_source(
                device_addr="%default",
                io_type=uhd.io_type.COMPLEX_FLOAT32,
                num_channels=1)
                
        self._u.set_gain(0, 0)
        self._u.set_samp_rate(self.samp_rate)       


        treq = uhd.tune_request(self.dump_freq, 0)
        tr = self._u.set_center_freq(treq)

        if tr == None:
            sys.stderr.write('Failed to set center frequency\n')
            raise SystemExit, 1


        self.filter1 = gr.interp_fir_filter_ccf(1, firdes.low_pass(1, samp_rate, 500000, 10000, firdes.WIN_HAMMING, 6.76))
        self.squelch = gr.pwr_squelch_cc(-100, 0.001, 0, True)
        self.demod = gr.quadrature_demod_cf(1)
        
        self.sync = digital.clock_recovery_mm_ff(2, 0.0076562, 0.5, 0.175, 0.005)
        self.slicer = digital.binary_slicer_fb()
        self.detect_seq = digital.correlate_access_code_bb("01010101010101010101010101010101", 1)
        self.dump = flysky.dumpsync()

        self.connect(self._u,self.filter1,self.squelch,self.demod,self.sync,self.slicer,self.detect_seq, self.dump)



if __name__ == '__main__':
    main = main()
    
    main.run()

