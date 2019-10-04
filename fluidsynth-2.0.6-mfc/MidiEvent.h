/************************************************************************/
/*                                                                      */
/*	Programmer:		Songark <arksong123@gmail.com>						*/
/*	Filename:		MidiEvent.h											*/
/*	Syntax:			C++ v11												*/
/*	Date:			2017-10-26											*/
/*	Description:	A class which stores a MidiEvent and a timestamp	*/
/*					for MidiTrack class									*/
/************************************************************************/


#ifndef _MIDIEVENT_H_INCLUDED
#define _MIDIEVENT_H_INCLUDED

#include "MidiMessage.h"
#include <vector>
#include <list>

using namespace std;

typedef unsigned char byte;
typedef unsigned short ushort;
typedef unsigned long  ulong;

namespace ns_midi {
	class MidiEvent {
	public:
		MidiEvent     (void);
		MidiEvent     (int starttick, int tracknum, byte eventid);
		MidiEvent     (int starttick, int tracknum, byte eventid, byte param1);
		MidiEvent     (int starttick, int tracknum, byte eventid, byte param1, int param2);
		MidiEvent     (int starttick, int tracknum, vector<byte>& message);
		MidiEvent     (const MidiEvent& mfevent);

		~MidiEvent     ();

		MidiEvent& operator=     (const MidiEvent& mfevent);

		byte eventGroupId();
		byte eventChannel();
		void copyMessage(byte* data, int length);
		void addMessage(byte* data, int length);

	public:
		byte eventId;
		byte eventMeta;

		int trackNum;
		int startTick;
		int deltaTick;
		int startMillsecond;
		int tempo;
		int portNo;
		int sortCompareId;

		byte numerator;
		byte denominator;

		vector<byte> messageArray;
	};
}


#endif /* _MIDIEVENT_H_INCLUDED */



