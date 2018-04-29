from PyQt4 import QtCore, QtGui
import random
import serial
from widgets import Slider, Meter, Value
import random

globalVref = 3.3
COM_port = "COM7"
baudRate = 115200
samplingPeriod_ms = 25
meterUpdateRate_ms = 200
adcRange = 4095
pwmRange = 2400

def serial_write(string):
    if ser:
        ser.write(str(string))
        ser.write(",+")
    print string

def test_str():
    sin = "k"
    for i in range(0, 8):
        sin = sin + "," + str(random.randint(0, adcRange))
    sin = sin + "\n"
    return sin

def conv_str_to_int(strArray):
    intArray = []
    for item in strArray:
        intArray.append(int(item))

    return intArray

class Window(QtGui.QWidget):
    def __init__(self):
        super(Window, self).__init__()

        # variable to store the number of received samples
        self.sampleNumber = 0

        # Move, resize and set window title
        self.move(300, 15)
        self.resize(600, 100)
        self.setWindowTitle("Power Supply")

        # Create objects for each section of the GUI
        self.enableBox = EnableBox()
        self.silderBox = SilderBox()
        self.measurementBox = MeasurementBox()
        self.statisticsBox = StatisticsBox()

        # Place objects in UI in vertical layout
        layout = QtGui.QVBoxLayout()
        layout.addWidget(self.enableBox.enableGroup)
        layout.addWidget(self.silderBox.sliderGroup)
        layout.addWidget(self.measurementBox.measureGroup)
        layout.addWidget(self.statisticsBox.statisticsGroup)
        self.setLayout(layout)

        # Set a timer to handle serial inputs. 
        # Timer expires and calls the timerCallback function every samplingPeriod_ms
        self.timer = QtCore.QTimer()
        self.timer.timeout.connect(self.timerCallback)
        self.timer.start(samplingPeriod_ms)

    def timerCallback(self):
        # handle Serial input every time timer expires
        if ser:
            sin = ser.readline()    # read line from serial input. EOL is denoted by new line character "\n"
            # Test String
            #sin = test_str()

            if sin:
                values = sin.split(',') # split values into list delimited by comma
                values[-1] = values[-1][:-1]  # Get rid of the newline character on the last value

                # if the serial input is valid, denoted by first character "k"
                if values[0] == 'k':
                    values = values[1:]
                    
                    # convert values to integer and print
                    values = conv_str_to_int(values)

                    # incrememt sample number 
                    self.sampleNumber += 1
                    
                    if self.sampleNumber % int(meterUpdateRate_ms/samplingPeriod_ms) == 0:
                        self.sampleNumber = 0   # reset the sample number
                        self.update_meters(values)

                        # calculate real quantities and update statistics box
                        calculatedValues = self.statisticsBox.calculate_values(values)
                        self.update_statistics(calculatedValues)
                        #print values
                    
                else:
                    print str(sin)

    def update_meters(self, values):
        # Update the ADC values in the meters
        self.measurementBox.vref_adcMeter.set_value(values[0])
        self.measurementBox.vin_adcMeter.set_value(values[1])
        self.measurementBox.vsw_adcMeter.set_value(values[2])
        self.measurementBox.vout_adcMeter.set_value(values[3])
        self.measurementBox.isense_adcMeter.set_value(values[4])
        self.measurementBox.vinl_adcMeter.set_value(values[5])            
        self.measurementBox.isrc_adcMeter.set_value(values[6])
        #self.measurementBox.temperature_adcMeter.set_value(values[7])

    def update_statistics(self, values):
        # Update the statistics on the GUI - these are the real quantities of voltage and current derived from the ADC values
        self.statisticsBox.referenceVoltage.update_value(values[0])
        self.statisticsBox.inputVoltage.update_value(values[1])
        self.statisticsBox.preregulatorVoltage.update_value(values[2])
        self.statisticsBox.linearRegInVoltage.update_value(values[3])
        self.statisticsBox.outputVoltage.update_value(values[4])
        self.statisticsBox.senseCurrent.update_value(values[5])
        self.statisticsBox.quiescentCurrent.update_value(values[6])
        self.statisticsBox.outputCurrent.update_value(values[7])
        self.statisticsBox.inputPower.update_value(values[8])
        self.statisticsBox.outputPower.update_value(values[9])
        self.statisticsBox.efficiency.update_value(values[10])
        #self.statisticsBox.temperatureSensor.update_value(values[11])

class StatisticsBox(object):
    def __init__(self):
        self.statisticsGroup = QtGui.QGroupBox("Statistics")
        layout = QtGui.QGridLayout()

        self.referenceVoltage = Value("Reference Voltage (2.048V +/- 0.5%)", layout, 0, "V")
        self.inputVoltage = Value("Input Voltage", layout, 1, "V")
        self.preregulatorVoltage = Value("Preregulator Voltage", layout, 2, "V")
        self.linearRegInVoltage = Value("LinReg Input Voltage", layout, 3, "V")
        self.outputVoltage = Value("Output Voltage", layout, 4, "V")
        self.senseCurrent = Value("Sense Current", layout, 5, "mA")
        self.quiescentCurrent = Value("Quiescent Current", layout, 6, "mA")
        self.outputCurrent = Value("Output Current", layout, 7, "mA")
        self.inputPower = Value("Input Power", layout, 8, "W")
        self.outputPower = Value("Output Power", layout, 9, "W")
        self.efficiency = Value("Efficiency", layout, 10, "%")
        #self.temperatureSensor = Value("MCU Temperature", layout, 11, "deg C")

        self.statisticsGroup.setLayout(layout)

    def calculate_values(self, values):
        # function to pass convert ADC values to real quantities
        # returns list of real calculated and calibrated values

        calculatedValues = []

        # reference voltage
        #referenceVoltage = (globalVref * values[0]/float(adcRange)) 

        globalVref = 2.021/(values[0]/float(adcRange))
        calculatedValues.append(globalVref)
        

        # input voltage
        inputVoltage = 9.95 * (globalVref * values[1]/float(adcRange))
        calculatedValues.append(inputVoltage)

        # preregulator output voltage
        preregulatorVoltage = 10.2 * (globalVref * values[2]/float(adcRange))
        calculatedValues.append(preregulatorVoltage)

        # Linear Regulator Inuput Voltage
        linearRegInVoltage = 10.2 * (globalVref * values[5]/float(adcRange))
        calculatedValues.append(linearRegInVoltage)

        # output voltage
        outputVoltage = 10.09 * (globalVref * values[3]/float(adcRange))
        calculatedValues.append(outputVoltage)

        # sense current
        # 2V/A = 500mA per Volt
        senseCurrent_mA = 500 * (globalVref * values[4]/float(adcRange)) 
        calculatedValues.append(senseCurrent_mA)

        # quiescent current
        # 10mA per Volt
        quiescentCurrent_mA = 10 * (globalVref * values[6]/float(adcRange))
        calculatedValues.append(quiescentCurrent_mA)

        # output current
        #outputCurrent_mA = senseCurrent_mA - quiescentCurrent_mA
        outputCurrent_mA = senseCurrent_mA
        #outputCurrent2 = (preregulatorVoltage - linearRegInVoltage)/0.5    # alternative way of calculating current
        calculatedValues.append(outputCurrent_mA)

        # input power in W
        inputPower = (senseCurrent_mA * inputVoltage)/1000.0 
        calculatedValues.append(inputPower)

        # output power
        outputPower = (outputCurrent_mA * outputVoltage)/1000.0
        calculatedValues.append(outputPower)

        # efficiency
        if inputPower != 0:
            efficiency = (outputPower/inputPower) * 100.0
        else:
            efficiency = 0
        calculatedValues.append(efficiency)

        # temperature sensor
        #temperature = ((globalVref * values[7]/float(adcRange)) / gainError)
        #calculatedValues.append(temperature)

        # calibrate everything to the reference voltage? (is there an offset and gain error or just gain error)
        return calculatedValues


class MeasurementBox(object):
    def __init__(self):
        self.measureGroup = QtGui.QGroupBox("ADC Measurement")

        layout = QtGui.QGridLayout()

        # Define Meters
        self.vref_adcMeter = Meter("VREF", layout, 0, adcRange, globalVref)
        self.vin_adcMeter = Meter("VIN ADC", layout, 1, adcRange, globalVref)
        self.vsw_adcMeter = Meter("VSW ADC", layout, 2, adcRange, globalVref)
        self.vinl_adcMeter = Meter("VINL ADC", layout, 3, adcRange, globalVref)
        self.vout_adcMeter = Meter("VOUT ADC", layout, 4, adcRange, globalVref)
        self.isense_adcMeter = Meter("ISENSE ADC", layout, 5, adcRange, globalVref)
        self.isrc_adcMeter = Meter("ISRC ADC", layout, 6, adcRange, globalVref)
        #self.temperature_adcMeter = Meter("MCU Temp ADC", layout, 7, adcRange, globalVref)

        self.measureGroup.setLayout(layout)


class SilderBox(object):
    def __init__(self):
        self.sliderGroup = QtGui.QGroupBox("PWM Control")

        layout = QtGui.QGridLayout()

        #Define Sliders
        self.vsw_pwmSlider  = Slider("VSW PWM" , layout, 1, pwmRange, self.update_vswset)
        self.vset_pwmSlider = Slider("VSET PWM", layout, 2, pwmRange, self.update_vset)
        self.iset_pwmSlider = Slider("ISET PWM", layout, 3, pwmRange, self.update_iset)

        self.sliderGroup.setLayout(layout)

    def update_vswset(self):
        sliderValue = str(self.vsw_pwmSlider.get_slider_value())
        print "VSW Slider: " + sliderValue
        serial_write("cmd,1," + sliderValue)

    def update_vset(self):
        sliderValue = str(self.vset_pwmSlider.get_slider_value())
        print "VSET Slider: " + sliderValue
        serial_write("cmd,2," + sliderValue)

    def update_iset(self):
        sliderValue = str(self.iset_pwmSlider.get_slider_value())
        print "ISET Slider: " + sliderValue
        serial_write("cmd,3," + sliderValue)


class EnableBox(object):
    def __init__(self):
        self.enableGroup = QtGui.QGroupBox("Enable")

        layout = QtGui.QGridLayout()
        
        # Output Enable Check Box
        self.output_enCheck = QtGui.QCheckBox("Output ON/OFF")
        self.output_enCheck.clicked.connect(self.update_output_en)
        layout.addWidget(self.output_enCheck, 0, 0)

        # Output Enable Check Box
        self.vsw_enCheck = QtGui.QCheckBox("VSW ON/OFF")
        self.vsw_enCheck.clicked.connect(self.update_vsw_en)
        layout.addWidget(self.vsw_enCheck, 0, 1)

        # Output Enable Check Box
        self.bklight_enCheck = QtGui.QCheckBox("BACKLIGHT ON/OFF")
        self.bklight_enCheck.clicked.connect(self.update_bklight_en)
        layout.addWidget(self.bklight_enCheck, 0, 2)

        self.enableGroup.setLayout(layout)

    def update_output_en(self):
        value = str(int(self.output_enCheck.isChecked()))
        print "Output = " + value
        serial_write("cmd,4," + value)
        

    def update_vsw_en(self):
        value = str(int(self.vsw_enCheck.isChecked()))
        print "Vsw enable = " + value 
        serial_write("cmd,5," + value)

    def update_bklight_en(self):
        value = str(int(self.bklight_enCheck.isChecked()))
        print "Backlight enable = " + value 
        serial_write("cmd,6," + value)



if __name__ == '__main__':

    import sys

    ser = serial.Serial(COM_port, baudRate, timeout=samplingPeriod_ms)
    if ser.isOpen():
        print "Connected on " + COM_port
       
    app = QtGui.QApplication(sys.argv)
    window = Window()
    window.show()
    sys.exit(app.exec_())
