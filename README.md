# Training Fruit Feather Transmitters
The Adafruit Feather is a fantastic microcontroller and when coupled with a variety of transmitters makes an impressive training aid. The code found in this repository covers five different models of Feather Transmitters:

Feather M0 WiFi ATSAMD21+ATWINC1500 https://www.adafruit.com/product/3010

Feather 32u4 RFM96 LoRa Radio - 433MHz https://www.adafruit.com/product/3079

Feather 32u4 RFM69HCW - 433MHz https://www.adafruit.com/product/3077

Feather 32u4 RFM96 LoRa Radio - 900MHz https://www.adafruit.com/product/3078

Feather 32u4 RFM69HCW - 900MHz https://www.adafruit.com/product/3076

Feather RP2040 RFM69 - 900MHz https://www.adafruit.com/product/5712

These transmitters, minus the WiFi, can be given five different scenarios.
1. Single Continuous Transmission
2. Burst
3. Spread Spectrum
4. Spread Spectrum Burst
5. Spread Spectrum, Frequency Hoping, Burst

How-To article can be found here: http://skinnyrd.com/using-the-adafruit-feather-for-fun-and-training/

The WiFi transmitter, M0 WiFi ATSAMD21+ATWINC1500, can be given 2 scenarios.
1. Burst
2. Randomized Burst

How-To for the WiFi transmitter can be found here: http://skinnyrd.com/wifi-training-transmitter/

## RP2040 RFM69 Notes
The RP2040 RFM69 version of Adafruit's RadioFruit Feather transmitter can easily be loaded with CircuitPython, allowing you to run the Training Fruit Feather Transmitter code without the need to recompile it each time you alter the scenario it is running, or any of the variables.  Below is how to initialize your Adafruit RP2040 RFM69 to be able to run the Python code:

1. Install CircuitPython - Follow the instructions on Adafruit's website to install CircuitPython 8.X for the RP2040 RFM69: https://learn.adafruit.com/feather-rp2040-rfm69/circuitpython
2. Download the necessary libraries:
   - Download the Adafruit CircuitPython Bundle 8.X located here: https://circuitpython.org/libraries
   - Unzip the library bundle into a folder easy for you to remember
3. Connect your RP2040 RFM69 Feather to your computer, if it is not already.  After installing CircuitPython - it should show up as a removable media drive on your computer.
4. From the unzipped Adafruit CircuitPython Bundle folder, copy over both the "adafruit_rfm69.mpy" file, as well as the "adafruit_bus_device" folder into the "lib" folder located on the RP2040 RFM69 (the lib folder may need to be created).
5. Copy the contents of the RFM69_RP2040_900MHz.py file in this repository into the "cody.py" file in the root of your RP2040 RFM69 Feather's drive, code.py should already be present - just overwrite the current contents. NOTE: The python file in the root of the Feather must be named "code.py" to work.

After populating "code.py" with the training code, it should immediately start transmitting.  To make a change to the scenario or any of the variables, simply open "code.py" in a text editor (Adafruit recommends the Mu editor for CircuitPython, but any text editor should work), make the change, then save the file.  The feather will immediately reboot and begin running the new scenario. 


