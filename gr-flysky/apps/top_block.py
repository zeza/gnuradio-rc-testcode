#!/usr/bin/env python
##################################################
# Gnuradio Python Flow Graph
# Title: Top Block
# Generated: Mon Jul  2 23:18:20 2012
##################################################

from gnuradio import digital
from gnuradio import eng_notation
from gnuradio import gr
from gnuradio import uhd
from gnuradio.eng_option import eng_option
from gnuradio.gr import firdes
from grc_gnuradio import wxgui as grc_wxgui
from optparse import OptionParser
import flysky
import wx

class top_block(grc_wxgui.top_block_gui):

	def __init__(self):
		grc_wxgui.top_block_gui.__init__(self, title="Top Block")

		##################################################
		# Variables
		##################################################
		self.samp_rate = samp_rate = 1000000
		self.dec_rate = dec_rate = 2

		##################################################
		# Blocks
		##################################################
		self.uhd_usrp_source_0 = uhd.usrp_source(
			device_addr="",
			stream_args=uhd.stream_args(
				cpu_format="fc32",
				channels=range(1),
			),
		)
		self.uhd_usrp_source_0.set_samp_rate(1000000)
		self.uhd_usrp_source_0.set_center_freq(2400490000, 0)
		self.uhd_usrp_source_0.set_gain(0, 0)
		self.low_pass_filter_0_0 = gr.interp_fir_filter_ccf(1, firdes.low_pass(
			1, samp_rate, 500000, 10000, firdes.WIN_HAMMING, 6.76))
		self.gr_quadrature_demod_cf_0 = gr.quadrature_demod_cf(1)
		self.gr_pwr_squelch_xx_0 = gr.pwr_squelch_cc(-100, 0.001, 0, True)
		self.flysky_dumpsync_0 = flysky.dumpsync()
		self.digital_correlate_access_code_bb_0_1 = digital.correlate_access_code_bb("010101010101010101010101010101010101010001110101", 1)
		self.digital_clock_recovery_mm_xx_0 = digital.clock_recovery_mm_ff(2, 0.0076562, 0.5, 0.175, 0.005)
		self.digital_binary_slicer_fb_0 = digital.binary_slicer_fb()

		##################################################
		# Connections
		##################################################
		self.connect((self.digital_clock_recovery_mm_xx_0, 0), (self.digital_binary_slicer_fb_0, 0))
		self.connect((self.digital_binary_slicer_fb_0, 0), (self.digital_correlate_access_code_bb_0_1, 0))
		self.connect((self.digital_correlate_access_code_bb_0_1, 0), (self.flysky_dumpsync_0, 0))
		self.connect((self.low_pass_filter_0_0, 0), (self.gr_pwr_squelch_xx_0, 0))
		self.connect((self.uhd_usrp_source_0, 0), (self.low_pass_filter_0_0, 0))
		self.connect((self.gr_pwr_squelch_xx_0, 0), (self.gr_quadrature_demod_cf_0, 0))
		self.connect((self.gr_quadrature_demod_cf_0, 0), (self.digital_clock_recovery_mm_xx_0, 0))

	def get_samp_rate(self):
		return self.samp_rate

	def set_samp_rate(self, samp_rate):
		self.samp_rate = samp_rate
		self.low_pass_filter_0_0.set_taps(firdes.low_pass(1, self.samp_rate, 500000, 10000, firdes.WIN_HAMMING, 6.76))

	def get_dec_rate(self):
		return self.dec_rate

	def set_dec_rate(self, dec_rate):
		self.dec_rate = dec_rate

if __name__ == '__main__':
	parser = OptionParser(option_class=eng_option, usage="%prog: [options]")
	(options, args) = parser.parse_args()
	tb = top_block()
	tb.Run(True)

