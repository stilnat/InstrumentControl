#include "Arduino.h"
#include "InstrumentControl.h"

InstrumentControl::InstrumentControl()
{
	
}

// convert an angle into a readable byte for the MIDI format
// the number must be between 0 and 127.
byte InstrumentControl::AngleToMidi(float angle)
{
    return (byte)((angle + 180) / 2.83);
}

// convert a pitch 
float InstrumentControl::AngleToPitchBend(float angle)
{
    return angle / 180;
}