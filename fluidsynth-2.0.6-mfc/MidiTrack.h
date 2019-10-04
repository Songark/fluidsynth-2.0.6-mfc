/************************************************************************/
/*                                                                      */
/*	Programmer:		Songark <arksong123@gmail.com>						*/
/*	Filename:		MidiTrack.h											*/
/*	Syntax:			C++ v11												*/
/*	Date:			2017-10-26											*/
/*	Description:	A class which stores and processes one set of Midi	*/
/*					Messages in one track.								*/
/************************************************************************/

#ifndef PROJ_WINDOWS_MIDITRACK_H
#define PROJ_WINDOWS_MIDITRACK_H

#include "MidiEvent.h"

namespace ns_midi {
	class MidiTrack {
	public:
		MidiTrack(void);
		MidiTrack(int tracknum);
		MidiTrack(int tracknum, string strname);
		MidiTrack(const MidiTrack& mtrack);

		~MidiTrack();

		MidiTrack& operator= (const MidiTrack& mtrack);

		bool readTrack(byte* buffer, int length);
		bool isDrumtrack();
		void setDrumtrack();
		void setPort(bool isFilled);

		static int read_2byte(byte* pbuffer, bool littleendian = true);
		static int read_3byte(byte* pbuffer, bool littleendian = true);
		static int read_4byte(byte* pbuffer, bool littleendian = true);
		static int read_variable_data_length(byte* buffer, int& nreadlen);

	public:
		int trackNum;
		byte port;
		byte channel;
		string nameTrack;

		vector<MidiEvent*> eventArray;
	};
}

#endif //PROJ_WINDOWS_MIDITRACK_H
