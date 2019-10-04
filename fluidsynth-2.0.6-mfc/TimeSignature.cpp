/************************************************************************/
/*                                                                      */
/*	Programmer:		Songark <arksong123@gmail.com>						*/
/*	Filename:		TimeSignature.cpp									*/
/*	Syntax:			C++ v11												*/
/*	Date:			2017-10-26											*/
/*	Description:	A class which processes timestamps for MIDI playing.*/
/************************************************************************/
#include "stdafx.h"
#include "TimeSignature.h"

using namespace ns_midi;

TimeSignature* TimeSignature::m_staticInst = 0;
TimeSignature* TimeSignature::GetInstance()
{
    if (!m_staticInst)
        m_staticInst = new TimeSignature();
    return m_staticInst;
}

int TimeSignature::get_tick2measure(int tick)
{
    int beat;
    if (denominator < 4)
        beat = (int)quarternote * 2;
    else
        beat = (int)quarternote / (denominator/4);

    measure = (int)numerator * beat;
    return (int) tick / measure;
}

int TimeSignature::get_measure2tick(int value)
{
    int beat;
    if (denominator < 4)
        beat = (int)quarternote * 2;
    else
        beat = (int)quarternote / (denominator/4);

    measure = (int)numerator * beat;
    return (int) measure * value;
}

void TimeSignature::set_tempo(int tick, int newtempo)
{
    int cursize = (int)tempotickArray.size();
    tempotickArray.resize(cursize + 1);
    tempotickArray[cursize] = tick;

    tempovalueArray.resize(cursize + 1);
    tempovalueArray[cursize] = newtempo;
}

int TimeSignature::get_tempo(int tick)
{
    for (int nIndex = 0; nIndex < (int)tempotickArray.size(); nIndex++) {
        if (tick > tempotickArray[nIndex]){
            return tempovalueArray[nIndex];
        }
        else {
            break;
        }
    }
    return 500000;
}

int TimeSignature::get_tick2millsecond(int tick)
{
    float fRetMs = 0;
    int nTempo = 500000;
    int nCurTick = 0, nPrevTick = 0, nSumTick = 0;
    int nIndex = 0;

    for (nIndex = 0; nIndex < (int)tempotickArray.size(); nIndex++) {
        nCurTick = tempotickArray[nIndex];
        if (nIndex > 0) {
            nPrevTick = tempotickArray[nIndex - 1];
            nTempo = tempovalueArray[nIndex - 1];
        }

        float tempo = (float)nTempo;
        float ppqn = (float)quarternote;
        float bpm = 60000000.0f / tempo;
        float ftick2ms = 1000.0f * (60.0f / (bpm * ppqn));
        if (tick < nCurTick) {
            fRetMs += (float)(tick - nPrevTick) * ftick2ms;
            break;
        }
        else {
            fRetMs += (float)(nCurTick - nPrevTick) * ftick2ms;
        }
    }

    if (tick > nCurTick && nIndex) {
        nTempo = tempovalueArray[nIndex - 1];
        float tempo = (float)nTempo;
        float ppqn = (float)quarternote;
        float bpm = 60000000.0f / tempo;
        float ftick2ms = 1000.0f * (60.0f / (bpm * ppqn));
        fRetMs += (float)(tick - nCurTick) * ftick2ms;
    }
    return (int)fRetMs;
}

int TimeSignature::gettickcount()
{
	return ::GetTickCount();
}