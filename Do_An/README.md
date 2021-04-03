<h1>Basic of Frequency and Bandwidth</h1>
Definition of Bandwidth
Bandwidth is completely related to frequency but definitely, bandwidth is not frequency. However, bandwidth is the range of frequencies. Bandwidth is defined as the difference in the upper and lower frequency components present in a signal. Thus, it specifies the amount of data being transmitted per second.
Basically, we know that a signal which is transmitted from an end to another consists of various frequency components. Then among the various frequencies how can we specify the range occupied by the complete signal?

As bandwidth provides an idea about the range of the transmitted signal. So, the range of the signal is determined by calculating the difference between the lowest and highest frequency component present in the signal.

Suppose a signal which is to be transmitted has different frequencies such as 6000MHz, 4200MHz, 1500 MHz, and 820 MHZ. So, here the signal bandwidth will be specified by the difference of the lowest and highest frequency component.

Bandwidth = 6000 MHz – 820 MHz = 5180 MHz

<h1>LoRa parameters</h1>
The parameters used by LoRa are SF, coding rate (CR), and bandwidth (BW):
- SF determines the number of chips per symbol, which is inversely proportional to the modulation rate of the chirp. LoRa can be configured for spreading factors between 5 and 12, although only 6 through 12 are accessible in the sub-gigahertz band, and 6 often requires a TXCO to be stable
- CR is the ratio of non-redundant data to all data within the transmit and receive frames.  LoRa coding rate essentially controls the amount of Forward Error Correction that is added to the payload data. As such, a higher coding rate will not increase range, but will make a link more reliable if interference is present. As one would expect, it also comes at the cost of decreasing the data rate. LoRa can be configured for four different coding rates.
- BW can take one of three values: 125, 250, or 500 kHz
Because they affect LoRa packet modulation and demodulation, these parameters must agree between transmitter and receiver for successful communication.If the BW of LoRa is constant, the chirp rate differs according to the SF. For each SF, the orthogonality of the chirp prevents interference with any other; when two or more transmitters use the same channel to simultaneously transmit with different SFs, the packets do not interfere with each other
To check the presence of LoRa signals, channel activity detection (CAD) is performed. The CAD mode efficiently detects the LoRa preamble signal [7,8]. A receiver in CAD mode only detects a preamble with a matching SF
For more detail, please refer to following:

<h1>How to calculate these parameters properly</h1>
When setting up a LoRa transceiver, you will not only need to know what frequency it should transmit on, and what the output power should be, but also three other parameters, that will drastically change how the transceiver operates. These parameters are:

Bandwidth
Spreading Factor
Coding Rate
LoRa is a very flexible modulation scheme, that can provide relatively fast data transfers up to 253 kbit/s. Conversely, the parameters can be configured in a way that will result in a very low data rate, all the way down to a mere 11 bits per second. This will in turn result in a large processing gain for the receiver, and therefore much longer range of the transmission.

In this post I will focus on how the parameters affect data rate, and not go into the specific processing gain calculations for various LoRa configurations. As a rule of thumb, you can expect the range to increase as the data rate is lowered.

LoRa modulation can be configured to use variety of different radio frequency bandwidths. Most commonly, a modulation scheme will occupy a more or less set bandwidth of radio spectrum, which is a result of the modulation technique used, and the rate at which information is transferred.

Since LoRa is a chirp spread spectrum modulation scheme, it intentionally spreads out the signal over a wider bandwidth than would strictly be necessary to transmit the information. The technique offers better noise immunity in the frequency domain, and is especially useful when transmitting with low output power.

LoRa can be configured to use bandwidths in pre-determined steps from 7.8 KHz to 500 KHz in the sub-gigahertz bands, and from 250 KHz to 1.6 MHz in the 2.4 GHz band. Generally speaking, choosing a narrower bandwidth will result in a slower transfer rate, but improved range. Choosing a wider bandwidth will result in an increased data rate, but a shorter range.

The chirp spread spectrum scheme employed by LoRa spreads out each bit of payload data over multiple symbols. This can improve noise immunity in the time-domain, and also incurs a processing gain for the receiver. The LoRa spreading factor is the parameter that controls how spread out in time each data bit is. If a higher spreading factor is selected, each payload data bit will be spread out over more symbols. LoRa can be configured for spreading factors between 5 and 12, although only 6 through 12 are accessible in the sub-gigahertz band, and 6 often requires a TXCO to be stable.

Where the bandwidth and spreading factor controls physical parameters of the modulation scheme, and thus can result in significant processing gains and range improvements, the LoRa coding rate essentially controls the amount of Forward Error Correction that is added to the payload data. As such, a higher coding rate will not increase range, but will make a link more reliable if interference is present. As one would expect, it also comes at the cost of decreasing the data rate. LoRa can be configured for four different coding rates.

The coding rate describes the ratio of actual data to error-correcting data added. In choosing the LoRa coding rate, it is important to consider whether it’s necessary to permanently employ a high coding rate, with subsequent loss of data rate, or whether it is more efficient over-all to allow for the occasional dropped packet due to interference. 

Since the coding rate does not modify the physical parameters of the modulation, two (or more) LoRa transceivers configured with different coding rates can still decode each others signals. This might be useful if a receiver is located in an area with high amounts of interference, but the other end of the link is not. In such a case, the two transceivers can be configured with asymmetric coding rates, such that optimal transfer rates can be achieved, even though interference levels vary at the different sites.

1. An Adaptive Spreading Factor Selection Scheme for a Single Channel LoRa Modem:
https://www.ncbi.nlm.nih.gov/pmc/articles/PMC7070984/#B5-sensors-20-01008

2. Spreading Factor, Bandwidth, Coding Rate and Bit Rate in LoRa (English)
https://josefmtd.com/2018/08/14/spreading-factor-bandwidth-coding-rate-and-bit-rate-in-lora-english/#:~:text=The%20value%20of%20Spreading%20Factor,gain%20from%20the%20receiver%20side.

Webtool - LoRa bitrate calculator and understanding LoRa parameters:
https://unsigned.io/understanding-lora-parameters/
