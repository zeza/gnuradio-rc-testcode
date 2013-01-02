#!/usr/bin/env python
##################################################
# Gnuradio Python Flow Graph
# Title: Top Block
# Generated: Tue Jun 12 09:37:34 2012
##################################################

from gnuradio import digital
from gnuradio import eng_notation
from gnuradio import gr
from gnuradio.eng_option import eng_option
from gnuradio.gr import firdes
from grc_gnuradio import wxgui as grc_wxgui
from optparse import OptionParser
import wx

class top_block(grc_wxgui.top_block_gui):

	def __init__(self):
		grc_wxgui.top_block_gui.__init__(self, title="Top Block")

		##################################################
		# Variables
		##################################################
		self.samp_rate = samp_rate = 2000000
		self.dec_rate = dec_rate = 4

		##################################################
		# Blocks
		##################################################
		self.gr_unpacked_to_packed_xx_0 = gr.unpacked_to_packed_bb(1, gr.GR_MSB_FIRST)
		self.gr_throttle_1 = gr.throttle(gr.sizeof_gr_complex*1, samp_rate/dec_rate)
		self.gr_freq_xlating_fir_filter_xxx_0 = gr.freq_xlating_fir_filter_ccc(dec_rate, (firdes.low_pass(1,samp_rate,500000,100000)), -535000, samp_rate)
		self.gr_file_source_0 = gr.file_source(gr.sizeof_gr_complex*1, "/shampoo/sdr/capture/nrf-capture/2410500000-2M-1.cap", False)
		self.gr_file_sink_0_0 = gr.file_sink(gr.sizeof_char*1, "/shampoo/sdr/projects/gr-nordic/qam-packed.out")
		self.gr_file_sink_0_0.set_unbuffered(False)
		self.gr_file_sink_0 = gr.file_sink(gr.sizeof_char*1, "/shampoo/sdr/projects/gr-nordic/qam-unpacked.out")
		self.gr_file_sink_0.set_unbuffered(False)
		self.digital_qam_demod_0 = digital.qam.qam_demod(
		  constellation_points=4,
		  differential=False,
		  samples_per_symbol=2,
		  excess_bw=.035,
		  freq_bw=6.28/100.0,
		  timing_bw=6.28/100.0,
		  phase_bw=6.28/100.0,
		  gray_coded=False,
		  verbose=False,
		  log=False,
		  )

		##################################################
		# Connections
		##################################################
		self.connect((self.gr_file_source_0, 0), (self.gr_freq_xlating_fir_filter_xxx_0, 0))
		self.connect((self.gr_freq_xlating_fir_filter_xxx_0, 0), (self.gr_throttle_1, 0))
		self.connect((self.gr_unpacked_to_packed_xx_0, 0), (self.gr_file_sink_0_0, 0))
		self.connect((self.digital_qam_demod_0, 0), (self.gr_file_sink_0, 0))
		self.connect((self.digital_qam_demod_0, 0), (self.gr_unpacked_to_packed_xx_0, 0))
		self.connect((self.gr_throttle_1, 0), (self.digital_qam_demod_0, 0))

	def get_samp_rate(self):
		return self.samp_rate

	def set_samp_rate(self, samp_rate):
		self.samp_rate = samp_rate
		self.gr_freq_xlating_fir_filter_xxx_0.set_taps((firdes.low_pass(1,self.samp_rate,500000,100000)))

	def get_dec_rate(self):
		return self.dec_rate

	def set_dec_rate(self, dec_rate):
		self.dec_rate = dec_rate

if __name__ == '__main__':
	parser = OptionParser(option_class=eng_option, usage="%prog: [options]")
	(options, args) = parser.parse_args()
	tb = top_block()
	tb.Run(True)

