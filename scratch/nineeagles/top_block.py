#!/usr/bin/env python
##################################################
# Gnuradio Python Flow Graph
# Title: Top Block
# Generated: Tue Jun 12 21:35:24 2012
##################################################

from gnuradio import digital
from gnuradio import eng_notation
from gnuradio import gr
from gnuradio import window
from gnuradio.eng_option import eng_option
from gnuradio.gr import firdes
from gnuradio.wxgui import fftsink2
from grc_gnuradio import wxgui as grc_wxgui
from optparse import OptionParser
import wx

class top_block(grc_wxgui.top_block_gui):

	def __init__(self):
		grc_wxgui.top_block_gui.__init__(self, title="Top Block")

		##################################################
		# Variables
		##################################################
		self.samp_rate = samp_rate = 25000000
		self.dec_rate = dec_rate = 25

		##################################################
		# Blocks
		##################################################
		self.wxgui_fftsink2_0 = fftsink2.fft_sink_c(
			self.GetWin(),
			baseband_freq=0,
			y_per_div=10,
			y_divs=10,
			ref_level=0,
			ref_scale=2.0,
			sample_rate=samp_rate/dec_rate,
			fft_size=1024,
			fft_rate=15,
			average=False,
			avg_alpha=None,
			title="FFT Plot",
			peak_hold=False,
		)
		self.Add(self.wxgui_fftsink2_0.win)
		self.gr_unpacked_to_packed_xx_0 = gr.unpacked_to_packed_bb(1, gr.GR_MSB_FIRST)
		self.gr_throttle_0 = gr.throttle(gr.sizeof_gr_complex*1, samp_rate)
		self.gr_freq_xlating_fir_filter_xxx_0 = gr.freq_xlating_fir_filter_ccc(dec_rate, (firdes.low_pass(1,samp_rate,1000000,100000)), -1000000, samp_rate)
		self.gr_file_source_0 = gr.file_source(gr.sizeof_gr_complex*1, "/shampoo/sdr/capture/j9Pro-capture/2400000000-25M-1.cap", False)
		self.gr_file_sink_0_0 = gr.file_sink(gr.sizeof_char*1, "/shampoo/sdr/projects/gr-nineeagles/gmsk-packed.out")
		self.gr_file_sink_0_0.set_unbuffered(False)
		self.gr_file_sink_0 = gr.file_sink(gr.sizeof_char*1, "/shampoo/sdr/projects/gr-nineeagles/gmsk-unpacked.out")
		self.gr_file_sink_0.set_unbuffered(False)
		self.digital_gmsk_demod_0 = digital.gmsk_demod(
			samples_per_symbol=2,
			gain_mu=0.175,
			mu=0.5,
			omega_relative_limit=0.005,
			freq_error=0.0,
			verbose=False,
			log=False,
		)

		##################################################
		# Connections
		##################################################
		self.connect((self.gr_file_source_0, 0), (self.gr_throttle_0, 0))
		self.connect((self.gr_unpacked_to_packed_xx_0, 0), (self.gr_file_sink_0_0, 0))
		self.connect((self.digital_gmsk_demod_0, 0), (self.gr_file_sink_0, 0))
		self.connect((self.digital_gmsk_demod_0, 0), (self.gr_unpacked_to_packed_xx_0, 0))
		self.connect((self.gr_freq_xlating_fir_filter_xxx_0, 0), (self.wxgui_fftsink2_0, 0))
		self.connect((self.gr_throttle_0, 0), (self.gr_freq_xlating_fir_filter_xxx_0, 0))
		self.connect((self.gr_freq_xlating_fir_filter_xxx_0, 0), (self.digital_gmsk_demod_0, 0))

	def get_samp_rate(self):
		return self.samp_rate

	def set_samp_rate(self, samp_rate):
		self.samp_rate = samp_rate
		self.gr_freq_xlating_fir_filter_xxx_0.set_taps((firdes.low_pass(1,self.samp_rate,1000000,100000)))
		self.wxgui_fftsink2_0.set_sample_rate(self.samp_rate/self.dec_rate)

	def get_dec_rate(self):
		return self.dec_rate

	def set_dec_rate(self, dec_rate):
		self.dec_rate = dec_rate
		self.wxgui_fftsink2_0.set_sample_rate(self.samp_rate/self.dec_rate)

if __name__ == '__main__':
	parser = OptionParser(option_class=eng_option, usage="%prog: [options]")
	(options, args) = parser.parse_args()
	tb = top_block()
	tb.Run(True)

