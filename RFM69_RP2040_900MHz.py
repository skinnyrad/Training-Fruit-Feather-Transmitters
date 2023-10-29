import time
import random
import digitalio
import board
import busio
import adafruit_rfm69

# Choose the number of the option you want.
WhatToDo = 2

# FOR OPTION 1 - Single Set Frequency
# Center frequency must be in MHz with a frequency between 809MHz AND 1023MHz
centerFrequency = 915.0

# FOR OPTION 2 - Burst
# The burst center frequency must be in MHz with a frequency between 809MHz AND 1023MHz
burstCenterFrequency = 915.0
# How long should the transmission occur? Time is in milliseconds: Ex: 5000 = 5 seconds
burstLength = 500
# How often should this burst occur? Time is in milliseconds
burstInterval = 2000

# FOR OPTION 3 - Spread Spectrum
# The start frequency for the spread in MHz between 809MHz AND 1023MHz
startFrequency = 915.0
# The stop frequency for the spread in MHz between 809MHz AND 1023MHz
stopFrequency = 918.0
# How often should the signal jump to another frequency? Time in milliseconds. NOT ZERO!
hopInterval = 1

# FOR OPTION 4 - Spread Spectrum Burst
# Make sure all of the OPTION 3 variables are assigned and the burstLength and
# burstInterval variables from OPTION 2.

# FOR OPTION 5 - Spread Spectrum, Frequency Hopping, Burst (AKA the kitchen sink)
# Fill out hopInterval in OPTION 3. Fill out burstLength and burstInterval variables in
# OPTION 2. Also, fill out the variables below.

# How wide should the spread spectrum signal be? Bandwidth is in MHz.
signalBandwidth = 1

# As the spread spectrum signal jumps around, what is the uppermost frequency it should
# bound itself by. The limit is 1023MHz. The number must be in MHz.
upperLimit = 920.0

# Define the lower limit as well
lowerLimit = 910.0

# SIGNAL AMPLITUDE
# Specify an amplitude level between 14 (weakest) and 19 (strongest).
amplitude = 14

# Defines pins needed for RF Tx & Rx to function
CS = digitalio.DigitalInOut(board.RFM_CS)
RESET = digitalio.DigitalInOut(board.RFM_RST)

# Initialize the RFM69 module
rf69 = adafruit_rfm69.RFM69(board.SPI(), CS, RESET, centerFrequency)
rf69.tx_power = amplitude

while True:
    if WhatToDo == 1:
        # Option 1: Single Set Frequency
        rf69.frequency_mhz = centerFrequency
        rf69.send(bytes("Hello World", "utf-8"))


    elif WhatToDo == 2:
        # Option 2: Burst
        time.sleep(burstInterval)
        rf69.frequency_mhz = burstCenterFrequency
        start_time = time.monotonic()
        while (time.monotonic() - start_time) <= (burstLength / 1000):
            rf69.send(bytes("Hello World", "utf-8"))


    elif WhatToDo == 3:
        # Option 3: Spread Spectrum
        rf69.frequency_mhz = random.uniform(startFrequency, stopFrequency)
        time.sleep(hopInterval / 1000)
        rf69.send(bytes("Hello World", "utf-8"))


    elif WhatToDo == 4:
        # Option 4: Spread Spectrum Burst
        time.sleep(burstInterval)
        start_time = time.monotonic()
        while (time.monotonic() - start_time) <= (burstLength / 1000):
            time.sleep(hopInterval / 1000)
            rf69.frequency_mhz = random.uniform(startFrequency, stopFrequency)
            rf69.send(bytes("Hello World", "utf-8"))


    elif WhatToDo == 5:
        # Option 5: Kitchen Sink
        time.sleep(burstInterval)
        start_time = time.monotonic()
        while (time.monotonic() - start_time) <= (burstLength / 1000):
            time.sleep(hopInterval / 1000)
            random_freq = random.uniform(lowerLimit, upperLimit)
            rf69.frequency_mhz = random.uniform(
                random_freq - signalBandwidth / 2, random_freq + signalBandwidth / 2)
            rf69.send(bytes("Hello World", "utf-8"))

