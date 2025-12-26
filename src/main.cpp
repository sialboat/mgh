#include <Arduino.h>
#include <Adafruit_MCP4728.h>
#include <Adafruit_LSM6DSOX.h>
#include <ILI9341_T4.h>
#include <tgx.h>
#include <IntervalTimer.h>
#include <MIDI.h>
#include <Adafruit_TinyUSB.h>
// #include "tusb.h"
// #include <USBHost_t36.h>

Adafruit_USBD_CDC usb_serial;
Adafruit_USBD_MIDI usb_midi;

MIDI_CREATE_INSTANCE(Adafruit_USBD_MIDI, usb_midi, MIDI);

// put function declarations here:
int myFunction(int, int);

void setup() {
  // put your setup code here, to run once.
  usb_serial.begin(115200);
  usb_midi.setStringDescriptor("TinyUSB MIDI");
  MIDI.begin(MIDI_CHANNEL_OMNI);
  if(USBDevice.mounted()) {
    USBDevice.detach();
    delay(10);
    USBDevice.attach();
  }

  while(!USBDevice.mounted()) delay(10);
  usb_serial.println("morning!");
}

void loop() {
  // put your main code here, to run repeatedly:
}

// put function definitions here:
int myFunction(int x, int y) {
  return x + y;
}