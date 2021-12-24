#ifndef InstrumentControl_h
#define InstrumentControl_h

#include "Arduino.h"

class InstrumentControl
{
public:
    InstrumentControl();
    byte AngleToMidi(float angle);
    float AngleToPitchBend(float angle);

};

#endif