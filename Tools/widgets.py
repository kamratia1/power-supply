from PyQt4 import QtCore, QtGui
import random


class Value(object):
    def __init__(self, label, layout, row, unit):

        valueLabel = QtGui.QLabel(str(label))
        self.value = QtGui.QLabel("0 " + str(unit))
        self.unit = str(unit)

        layout.addWidget(valueLabel, row, 0)
        layout.addWidget(self.value, row, 1)

    def update_value(self, value):
        limitedValue = "{:.3f}".format(value)
        self.value.setText(str(limitedValue) + " " + self.unit)


class Slider(object):
    def __init__(self, label, layout, row, maxValue, releaseCallback):
        
        self.maxValue = maxValue

        # Label
        self.sliderLabel = QtGui.QLabel(str(label))
        
        # Slider
        self.slider = QtGui.QSlider(QtCore.Qt.Horizontal)
        self.slider.setRange(0, maxValue)
        # Set callback function when slider is released
        self.slider.sliderReleased.connect(releaseCallback)  
    
        # Slider Value
        self.sliderValue = QtGui.QLabel()
        self.sliderValue.setText("0/4095 (0V)")
        self.slider.valueChanged.connect(self.update_slider_value)

        layout.addWidget(self.sliderLabel, row, 0)
        layout.addWidget(self.slider, row, 1)
        layout.addWidget(self.sliderValue, row, 2)

    def get_slider_value(self):
        return self.slider.sliderPosition()

    def update_slider_value(self, value):
        voltage = "{:.3f}".format(value * 3.3/float(self.maxValue))
        strval = str(value) + "/" + str(self.maxValue) + " (" + str(voltage) + "V)"
        self.sliderValue.setText(strval)

class Meter(object):
    def __init__(self, label, layout, row, maxValue, vref):
        
        self.maxValue = maxValue
        self.vref = vref
        #Label
        meterLabel = QtGui.QLabel(str(label))

        # Meter
        self.meter = QtGui.QProgressBar()
        self.meter.setFormat("%v / " + str(maxValue))
        self.meter.setStyleSheet("QProgressBar { border: 1px solid #d0d0d0; border-radius: 0px; text-align: center; } QProgressBar::chunk {background-color: #3add36; width: 1px;}")
        self.meter.setRange(0, maxValue)
        # initialise random value
        self.set_value(random.randint(0,maxValue))

        layout.addWidget(meterLabel, row, 0)
        layout.addWidget(self.meter, row, 1)
    
    def set_value(self, value):
        self.meter.setFormat("%v / " + str(self.maxValue) + " ({:.3f} V)".format(value * self.vref / self.maxValue))
        self.meter.setValue(value) 