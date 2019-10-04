/************************************************************************/
/*                                                                      */
/*	Programmer:		Songark <arksong123@gmail.com>						*/
/*	Filename:		MidiMessage.h										*/
/*	Syntax:			C++ v11												*/
/*	Date:			2017-10-26											*/
/*	Description:	Defines factors of MIDI message for Standard		*/
/************************************************************************/

#ifndef _MIDIMESSAGE_H_INCLUDED
#define _MIDIMESSAGE_H_INCLUDED

#include <vector>
#include <string>

using namespace std;

typedef unsigned char  uchar;
typedef unsigned short ushort;
typedef unsigned long  ulong;

/* The list of Midi Events */
#define EventNoteOff         0x80
#define EventNoteOn          0x90
#define EventKeyPressure     0xA0
#define EventControlChange   0xB0
#define EventProgramChange   0xC0
#define EventChannelPressure 0xD0
#define EventPitchBend       0xE0
#define SysexEvent1          0xF0
#define SysexEvent2          0xF7
#define MetaEvent            0xFF

/* The list of Meta Events */
#define MetaEventSequence      0x0
#define MetaEventText          0x1
#define MetaEventCopyright     0x2
#define MetaEventSequenceName  0x3
#define MetaEventInstrument    0x4
#define MetaEventLyric         0x5
#define MetaEventMarker        0x6
#define MetaEventPort		   0x21
#define MetaEventEndOfTrack    0x2F
#define MetaEventTempo         0x51
#define MetaEventSMPTEOffset   0x54
#define MetaEventTimeSignature 0x58
#define MetaEventKeySignature  0x59

#define DrumChannel1			0x09
#define DrumChannel2			0x0A

#endif /* _MIDIMESSAGE_H_INCLUDED */



