/************************************************************************/
/*                                                                      */
/*	Programmer:		Songark <arksong123@gmail.com>						*/
/*	Filename:		MidiFile.cpp										*/
/*	Syntax:			C++ v11												*/
/*	Date:			2017-10-26											*/
/*	Description:	A class which can read / write Standard MIDI files.	*/
/*					MIDI data is stored by track in an array.			*/
/************************************************************************/


#include "stdafx.h"
#include "MidiFile.h"
#include "TimeSignature.h"

#include <string.h>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <iterator>

using namespace std;
using namespace ns_midi;

void MidiFile::init()
{
    sortedEventsList.clear();
    for (int nIndex = 0; nIndex < (int)trackArray.size(); nIndex++) {
        MidiTrack* track = trackArray[nIndex];
        if (track)
            delete track;
    }
    trackArray.clear();

    for (int nIndex = 0; nIndex < 0x10; nIndex++)
        channelArray.insert(channelArray.end(), 0);

    totalTicks = 0;
}

string MidiFile::getFilename()
{
    return filename;
}

int MidiFile::getNumTracks()
{
    return (int)trackArray.size();
}

MidiTrack* MidiFile::getTrack(int trackIndex)
{
	if (trackIndex >= 0 && trackIndex < (int)trackArray.size())
		return trackArray[trackIndex];
	return NULL;
}

string MidiFile::getNameTrack(int trackIndex)
{
	if (trackIndex >= 0 && trackIndex < (int)trackArray.size())
		return trackArray[trackIndex]->nameTrack;
	return "";
}

struct membuf : std::streambuf
{
    membuf(char* begin, char* end) {
        this->setg(begin, begin, end);
    }
};

bool MidiFile::readFile(string midifile)
{
    if (filename == midifile)
        return true;
    filename = midifile;

    TimeSignature* ptimesig = TimeSignature::GetInstance();
    ptimesig->clear_tempo();

    init();

	std::ifstream input(midifile.c_str(), std::ifstream::ate | std::ios::binary);
	int filesize = (int)input.tellg();

    if (filesize) {
        byte* buffer = new byte[filesize];
        byte* buffer_begin = buffer;
        byte* buffer_end = buffer + filesize;
		input.seekg(input.beg);
		input.read((char*)buffer, filesize);

        if (buffer[0] == 'M' && buffer[1] == 'T' && buffer[2] == 'h' && buffer[3] == 'd') {
            buffer += 8;

			format = MidiTrack::read_2byte(buffer, false); buffer += 2;
            int ntracks = MidiTrack::read_2byte(buffer, false); buffer += 2;
            int quaternote = MidiTrack::read_2byte(buffer, false); buffer += 2;
            ptimesig->set_quarternote(quaternote);

            int trackIndex = 0;
            while (buffer < buffer_end) {
                if (buffer[0] == 'M' && buffer[1] == 'T' && buffer[2] == 'r' && buffer[3] == 'k') {
                    buffer += 4;

                    int trackLen = MidiTrack::read_4byte(buffer, false); buffer += 4;
                    if (buffer + trackLen > buffer_end)
                        break;

                    MidiTrack* trackInfo = new MidiTrack(trackIndex);   trackIndex++;
                    trackInfo->readTrack(buffer, trackLen);
                    if (trackInfo->channel >= 0 && trackInfo->channel < 0x10) {
                        bool isFilled = channelArray[trackInfo->channel] == 1 ? true : false;
                        channelArray[trackInfo->channel] = 1;
                        trackInfo->setPort(isFilled);
                    }

                    trackArray.insert(trackArray.end(), trackInfo);
                    buffer += trackLen;
                }
                else {
                    break;
                }
            }
            return true;
        }

        delete[] buffer_begin;
    }
    return false;
}

bool MidiFile::sortEventArray()
{
    struct CompareEventTime {
        bool operator()(MidiEvent * lhs, MidiEvent * rhs) {
            if (lhs->startTick == rhs->startTick)
                return lhs->sortCompareId < rhs->sortCompareId;
            else
                return lhs->startTick < rhs->startTick;
        }
    };

    if (sortedEventsList.size() > 0)
        return true;

    totalTicks = 0;
    TimeSignature* pTimeSig = TimeSignature::GetInstance();
    for (int ntracks = 0; ntracks < (int)trackArray.size(); ntracks++) {
        MidiTrack* pTrack = trackArray[ntracks];
        for (int nevents = 0; nevents < (int)pTrack->eventArray.size(); nevents++) {
            MidiEvent* pEvent = pTrack->eventArray[nevents];
            pEvent->startMillsecond = pTimeSig->get_tick2millsecond(pEvent->startTick);
            pEvent->portNo = pTrack->port;
            if (pEvent->eventGroupId() == EventNoteOff || pEvent->eventGroupId() == EventNoteOn) {
                if (pEvent->startTick > totalTicks)
                    totalTicks = pEvent->startTick;
            }
            sortedEventsList.insert(sortedEventsList.end(), pEvent);
        }
    }
    sortedEventsList.sort(CompareEventTime());
    return true;
}

int MidiFile::sortEventArraySize()
{
    return (int)sortedEventsList.size();
}

MidiEvent* MidiFile::getsortEvent(int eventIndex)
{
    if (eventIndex < (int)sortedEventsList.size()) {
        std::list<MidiEvent*>::iterator it = sortedEventsList.begin();
        std::advance(it, eventIndex);
        MidiEvent* pEvent = *it;
        return pEvent;
    }
    return NULL;
}

void MidiFile::setEnableTrack(int trackIndex, bool flag)
{
	if (trackIndex < _countof(trackFilter))
		trackFilter[trackIndex] = flag;
}

bool MidiFile::isEnableTrack(MidiEvent* pevent)
{
	if (pevent->eventGroupId() == EventNoteOn || pevent->eventGroupId() == EventNoteOff ) {
		if (pevent->messageArray.size() >= 3 && pevent->messageArray[2] == 0)
			return true;
	}

	int trackIndex = pevent->trackNum;
	if (trackIndex < _countof(trackFilter))
		return trackFilter[trackIndex];
	return true;
}