/************************************************************************/
/*                                                                      */
/*	Programmer:		Songark <arksong123@gmail.com>						*/
/*	Filename:		TimeSignature.h										*/
/*	Syntax:			C++ v11												*/
/*	Date:			2017-10-26											*/
/*	Description:	A class which processes timestamps for MIDI playing.*/
/************************************************************************/

#ifndef PROJ_WINDOWS_TIMESIGNATURE_H
#define PROJ_WINDOWS_TIMESIGNATURE_H

#include <vector>

using namespace std;

namespace ns_midi {
	class TimeSignature {
	public:
		TimeSignature(){
			numerator = 0;
			denominator = 0;
			quarternote = 0;
			measure = 0;
		};
		~TimeSignature(){};

		static TimeSignature* GetInstance();
		static TimeSignature* m_staticInst;

		void set_numerator(int value) {numerator = value;};
		void set_denominator(int value) {denominator = value;};
		void set_quarternote(int value) {quarternote = value;};
		int get_tick2measure(int tick);
		int get_measure2tick(int value);
		void clear_tempo() {tempotickArray.clear(); tempovalueArray.clear();};
		void set_tempo(int tick, int newtempo);
		int get_tempo(int tick);
		int get_tick2millsecond(int tick);
		int gettickcount();

	private:
		int numerator;
		int denominator;
		int quarternote;
		int measure;
		vector<int> tempotickArray;
		vector<int> tempovalueArray;
	};
}


#endif //PROJ_WINDOWS_TIMESIGNATURE_H
