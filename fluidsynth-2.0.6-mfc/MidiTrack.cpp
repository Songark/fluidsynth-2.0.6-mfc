/************************************************************************/
/*                                                                      */
/*	Programmer:		Songark <arksong123@gmail.com>						*/
/*	Filename:		MidiTrack.cpp										*/
/*	Syntax:			C++ v11												*/
/*	Date:			2017-10-26											*/
/*	Description:	A class which stores and processes one set of Midi	*/
/*					Messages in one track.								*/
/************************************************************************/

#include "stdafx.h"
#include "MidiTrack.h"
#include "TimeSignature.h"
#include <math.h>

using namespace ns_midi;

MidiTrack::MidiTrack(void)
{
    trackNum = 0;
    port = 0;
    channel = 0xFF;

    eventArray.clear();
}

MidiTrack::MidiTrack(int tracknum)
{
    port = 0;
    channel = 0xFF;
    trackNum = tracknum;
}

MidiTrack::MidiTrack(int tracknum, string strname)
{
    port = 0;
    channel = 0xFF;
    trackNum = tracknum;
    nameTrack = strname;
}

MidiTrack::MidiTrack(const MidiTrack& mtrack)
{
    trackNum = mtrack.trackNum;
    port = mtrack.port;
    channel = mtrack.channel;
    nameTrack = mtrack.nameTrack;

    for (int i=0; i<(int)this->eventArray.size(); i++) {
        delete this->eventArray[i];
    }
    this->eventArray.resize(mtrack.eventArray.size());
    for (int i=0; i<(int)this->eventArray.size(); i++) {
        this->eventArray[i] = mtrack.eventArray[i];
    }
}

MidiTrack::~MidiTrack()
{
    for (int i=0; i<(int)this->eventArray.size(); i++) {
        delete this->eventArray[i];
    }
    this->eventArray.clear();
}

MidiTrack& MidiTrack::operator= (const MidiTrack& mtrack)
{
    if (this == &mtrack) {
        return *this;
    }

    trackNum = mtrack.trackNum;
    port = mtrack.port;
    channel = mtrack.channel;
    nameTrack = mtrack.nameTrack;

    for (int i=0; i<(int)this->eventArray.size(); i++) {
        delete this->eventArray[i];
    }
    this->eventArray.resize(mtrack.eventArray.size());
    for (int i=0; i<(int)this->eventArray.size(); i++) {
        this->eventArray[i] = mtrack.eventArray[i];
    }

    return *this;
}

bool MidiTrack::readTrack(byte* buffer, int length)
{
    TimeSignature* ptimesig = TimeSignature::GetInstance();
    eventArray.clear();

    int readlen = 0, variablelen = 0;
    int sortid = 0, prevmillsecond = 0;
    int starttick = 0, deltatick = 0;
    byte eventid = 0;
    byte* buff_begin = buffer;
    byte* buff_end = buffer + length;

    while (buffer < buff_end) {
        sortid++;

        deltatick = MidiTrack::read_variable_data_length(buffer, readlen);  buffer += readlen;
        starttick += deltatick;

        eventid = buffer[0]; buffer++;
        if (eventid < EventNoteOff) {
            int nlast = (int)eventArray.size();
            if (nlast > 0) {
                eventid = eventArray[nlast - 1]->eventId;
                buffer -= 1;
            }
            else {
                return false;
            }
        }

        MidiEvent* new_event = new MidiEvent(starttick, trackNum, eventid);
        new_event->deltaTick = deltatick;
        new_event->sortCompareId = sortid;
        if (eventid == MetaEvent) {
            new_event->eventMeta = buffer[0];   buffer++;
            variablelen = MidiTrack::read_variable_data_length(buffer, readlen);
            if (new_event->eventMeta == MetaEventSequenceName) {
                std::string s( reinterpret_cast<char const*>(buffer + 1), buffer[0]) ;
                nameTrack = s;
            }
            else if (new_event->eventMeta == MetaEventTimeSignature) {
                new_event->numerator = (byte)buffer[readlen];
                new_event->denominator = (byte)pow(2.0, (int)buffer[readlen + 1]);
                ptimesig->set_numerator(new_event->numerator);
                ptimesig->set_denominator(new_event->denominator);
            }
            else if (new_event->eventMeta == MetaEventTempo) {
                if (variablelen == 3) {
                    new_event->tempo = MidiTrack::read_3byte(buffer + readlen, false);
                    ptimesig->set_tempo(new_event->startTick, new_event->tempo);
                }
            }
			else if (new_event->eventMeta == MetaEventPort) {
				if (variablelen == 1) {
					port = buffer[readlen];
				}
			}
            new_event->addMessage(buffer, variablelen + readlen);
            buffer += (variablelen + readlen);
        }
        else if (eventid == SysexEvent1 || eventid == SysexEvent2) {
            variablelen = MidiTrack::read_variable_data_length(buffer, readlen);
            new_event->addMessage(buffer, variablelen + readlen);
            buffer += (variablelen + readlen);
        }
        else {
            if (channel == 0xFF)
                channel = new_event->eventChannel();
            switch (new_event->eventGroupId()) {
                case EventNoteOn:
                case EventNoteOff:
                    new_event->addMessage(buffer, 2);
                    buffer += 2;
                    break;
                case EventControlChange:
                case EventKeyPressure:
                case EventPitchBend:
                    new_event->addMessage(buffer, 2);
                    buffer += 2;
                    break;
                case EventProgramChange:
                case EventChannelPressure:
                    new_event->addMessage(buffer, 1);
                    buffer ++;
                    break;
            }
        }
        eventArray.insert(eventArray.end(), new_event);
    }

    return true;
}

bool MidiTrack::isDrumtrack()
{
    if (channel == DrumChannel1)
        return true;
    return false;
}

void MidiTrack::setDrumtrack()
{
    channel = DrumChannel1;
}

void MidiTrack::setPort(bool isFilled)
{
}

int MidiTrack::read_2byte(byte* pbuffer, bool littleendian)
{
    int nValue = 0;
    if (!littleendian)
    {
        int nValue1 = pbuffer[0x00];
        int nValue2 = pbuffer[0x01];
        nValue = (nValue1 << 8) + nValue2;
    }
    else
    {
        int nValue1 = pbuffer[0x00];
        int nValue2 = pbuffer[0x01];
        nValue = (nValue2 << 8) + nValue1;
    }
    return nValue;
}

int MidiTrack::read_3byte(byte* pbuffer, bool littleendian)
{
    int nValue = 0;
    if (!littleendian)
    {
        int nValue1 = pbuffer[0x00];
        int nValue2 = pbuffer[0x01];
        int nValue3 = pbuffer[0x02];
        nValue = (nValue1 << 16) + (nValue2 << 8) + nValue3;
    }
    else
    {
        int nValue1 = pbuffer[0x00];
        int nValue2 = pbuffer[0x01];
        int nValue3 = pbuffer[0x02];
        nValue = (nValue3 << 16) + (nValue2 << 8) + nValue1;
    }
    return nValue;
}

int MidiTrack::read_4byte(byte* pbuffer, bool littleendian)
{
    int nValue = 0;
    if (!littleendian)
    {
        int nValue1 = pbuffer[0x00];
        int nValue2 = pbuffer[0x01];
        int nValue3 = pbuffer[0x02];
        int nValue4 = pbuffer[0x03];
        nValue = (nValue1 << 24) + (nValue2 << 16) + (nValue3 << 8) + nValue4;
    }
    else
    {
        nValue = *(int*)pbuffer;
    }
    return nValue;
}

int MidiTrack::read_variable_data_length(byte* buffer, int& nreadlen)
{
    int nLength = (int)buffer[0];
    nreadlen = 1;
    if ((byte)(nLength & 0x80) > 0)
    {
        nLength = (int)(byte)(nLength & 0x7F);
        do
        {
            nLength = (int)(buffer[nreadlen] & 0x7F) + (nLength << 7);
            nreadlen++;
        } while ((byte)(buffer[nreadlen - 1] & 0x80) > 0);
    }
    return nLength;
}