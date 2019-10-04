/************************************************************************/
/*                                                                      */
/*	Programmer:		Songark <arksong123@gmail.com>						*/
/*	Filename:		MidiEvent.cpp										*/
/*	Syntax:			C++ v11												*/
/*	Date:			2017-10-26											*/
/*	Description:	A class which stores a MidiEvent and a timestamp	*/
/*					for MidiTrack class									*/
/************************************************************************/

#include "stdafx.h"
#include "MidiEvent.h"
#include <stdlib.h>

using namespace std;
using namespace ns_midi;

MidiEvent::MidiEvent(void)
{
    eventId = 0;
    eventMeta = 0;
    trackNum = 0;
    startTick = 0;
    deltaTick = 0;
    startMillsecond = 0;
    tempo = 0;
    portNo = 0;
    sortCompareId = 0;
    numerator = 0;
    denominator = 0;

    messageArray.clear();
}

MidiEvent::MidiEvent(int starttick, int tracknum, byte eventid)
{
    eventMeta = 0;
    deltaTick = 0;
    startMillsecond = 0;
    tempo = 0;
    portNo = 0;
    sortCompareId = 0;
    numerator = 0;
    denominator = 0;
    startTick = starttick;
    trackNum = tracknum;
    eventId = eventid;
    messageArray.resize(1);
    messageArray[0] = eventid;
}

MidiEvent::MidiEvent(int starttick, int tracknum, byte eventid, byte param1)
{
    eventMeta = 0;
    deltaTick = 0;
    startMillsecond = 0;
    tempo = 0;
    portNo = 0;
    sortCompareId = 0;
    numerator = 0;
    denominator = 0;

    startTick = starttick;
    trackNum = tracknum;
    eventId = eventid;

    messageArray.resize(2);
    messageArray[0] = eventid;
    messageArray[1] = param1;
}

MidiEvent::MidiEvent(int starttick, int tracknum, byte eventid, byte param1, int param2)
{
    eventMeta = 0;
    deltaTick = 0;
    startMillsecond = 0;
    tempo = 0;
    portNo = 0;
    sortCompareId = 0;
    numerator = 0;
    denominator = 0;

    startTick = starttick;
    trackNum = tracknum;
    eventId = eventid;

    messageArray.resize(3);
    messageArray[0] = eventid;
    messageArray[1] = param1;
    messageArray[2] = param2;
}

MidiEvent::MidiEvent(int starttick, int tracknum, vector<byte>& message)
{
    MidiEvent();
    if (message.size() > 0) {
        startTick = starttick;
        trackNum = tracknum;
        eventId = message[0];
        messageArray = message;
    }
}

MidiEvent::MidiEvent(const MidiEvent& mfevent)
{
    eventId = mfevent.eventId;
    eventMeta = mfevent.eventMeta;
    trackNum = mfevent.trackNum;
    startTick = mfevent.startTick;
    deltaTick = mfevent.deltaTick;
    startMillsecond = mfevent.startMillsecond;
    tempo = mfevent.tempo;
    portNo = mfevent.portNo;
    sortCompareId = mfevent.sortCompareId;
    numerator = mfevent.numerator;
    denominator = mfevent.denominator;

    this->messageArray.resize(mfevent.messageArray.size());
    for (int i=0; i<(int)this->messageArray.size(); i++) {
        this->messageArray[i] = mfevent.messageArray[i];
    }
}

MidiEvent::~MidiEvent()
{

}

MidiEvent& MidiEvent::operator=(const MidiEvent& mfevent) {
    if (this == &mfevent) {
        return *this;
    }
    eventId = mfevent.eventId;
    eventMeta = mfevent.eventMeta;
    trackNum = mfevent.trackNum;
    startTick = mfevent.startTick;
    deltaTick = mfevent.deltaTick;
    startMillsecond = mfevent.startMillsecond;
    tempo = mfevent.tempo;
    portNo = mfevent.portNo;
    sortCompareId = mfevent.sortCompareId;
    numerator = mfevent.numerator;
    denominator = mfevent.denominator;

    this->messageArray.resize(mfevent.messageArray.size());
    for (int i=0; i<(int)this->messageArray.size(); i++) {
        this->messageArray[i] = mfevent.messageArray[i];
    }
    return *this;
}

byte MidiEvent::eventGroupId()
{
    return (byte)eventId & 0xF0;
}

byte  MidiEvent::eventChannel()
{
    return (byte)eventId & 0x0F;
}

void MidiEvent::copyMessage(byte* data, int length)
{
    messageArray.resize(length);
    for (int nIndex = 0; nIndex < length; nIndex++)
        messageArray[nIndex] = data[nIndex];
}

void MidiEvent::addMessage(byte* data, int length)
{
    int prevSize = (int)messageArray.size();
    messageArray.resize(prevSize + length);
    for (int nIndex = 0; nIndex < length; nIndex++)
        messageArray[prevSize + nIndex] = data[nIndex];
}




