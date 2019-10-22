#pragma once

#include <string>
#include "fluid_sys.h"

#if !defined(WIN32) && !defined(MACINTOSH)
#define _GNU_SOURCE
#endif

#if defined(HAVE_GETOPT_H)
#include <getopt.h>
#define GETOPT_SUPPORT 1
#endif

#include "fluid_lash.h"

#ifdef SYSTEMD_SUPPORT
#include <systemd/sd-daemon.h>
#endif


using namespace std;

class CfluidsynthWrapper
{
public:
	CfluidsynthWrapper();
	~CfluidsynthWrapper();

	bool init(string strsf2path, string strmidipath);
	int get_status();
	bool play();
	bool pause();
	bool stop();
	void setfilter(int ntrack, bool flag);
	void seek(int ticks);
	int gettempo();
	int settempo(int ntempo);
	double getvolume();
	int setvolume(double dbvolume);
	int gettickscount();
	int getcurrentticks();

private:
	fluid_settings_t *settings;
	fluid_player_t *player;
	fluid_midi_router_t *router;
	fluid_midi_driver_t *mdriver;
	fluid_audio_driver_t *adriver;
	fluid_synth_t *synth;
	fluid_server_t *server;
	int with_server;
	fluid_cmd_handler_t *cmd_handler;

	int midi_in;
	char *config_file;
	int audio_groups;
	int audio_channels;
	int dump;
	int fast_render;
	int first_tempo;
	double first_volume;

	string strmidi;
	string strsf2;
};

