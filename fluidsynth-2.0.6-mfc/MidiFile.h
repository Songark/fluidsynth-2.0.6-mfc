/************************************************************************/
/*                                                                      */
/*	Programmer:		Songark <arksong123@gmail.com>						*/
/*	Filename:		MidiFile.h											*/
/*	Syntax:			C++ v11												*/
/*	Date:			2017-10-26											*/
/*	Description:	A class which can read / write Standard MIDI files.	*/
/*					MIDI data is stored by track in an array.			*/
/************************************************************************/

#ifndef _MIDIFILE_H_INCLUDED
#define _MIDIFILE_H_INCLUDED

#include "MidiTrack.h"
#include <vector>
#include <list>
#include <istream>
#include <fstream>

using namespace std;

namespace ns_midi {
	class MidiFile {
	public:
		MidiFile(){};
		~MidiFile(){};

		string filename;
		string filename_sf2;

		void init();
		string getFilename();
		int getNumTracks();
		MidiTrack* getTrack(int trackIndex);
		string getNameTrack(int trackIndex);		
		bool readFile(string midifile);
		bool sortEventArray();
		int sortEventArraySize();
		MidiEvent* getsortEvent(int eventIndex);
		int getTotalTicks() {return totalTicks;};
		void setEnableTrack(int trackIndex, bool flag);
		bool isEnableTrack(MidiEvent* pevent);

	private:
		vector<MidiTrack*> trackArray;
		vector<int> channelArray;
		list<MidiEvent*> sortedEventsList;
		bool trackFilter[32];

		int totalTicks;		
		int format;
	};
}
#endif /* _MIDIFILE_H_INCLUDED */



