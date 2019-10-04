#include "stdafx.h"
#include "CfluidsynthWrapper.h"


CfluidsynthWrapper::CfluidsynthWrapper()
{
	settings = NULL;
	player = NULL;
	router = NULL;
	mdriver = NULL;
	adriver = NULL;
	synth = NULL;
	server = NULL;	
	cmd_handler = NULL;

	with_server = 0;
	midi_in = 1;
	config_file = NULL;
	audio_groups = 0;
	audio_channels = 0;
	dump = 0;
	fast_render = 0;
}


CfluidsynthWrapper::~CfluidsynthWrapper()
{
}

static void fast_render_loop(fluid_settings_t *settings, fluid_synth_t *synth, fluid_player_t *player)
{
	fluid_file_renderer_t *renderer;

	renderer = new_fluid_file_renderer(synth);

	if (!renderer)
	{
		return;
	}

	while (fluid_player_get_status(player) == FLUID_PLAYER_PLAYING)
	{
		if (fluid_file_renderer_process_block(renderer) != FLUID_OK)
		{
			break;
		}
	}

	delete_fluid_file_renderer(renderer);
}

bool CfluidsynthWrapper::init(string strsf2path, string strmidipath)
{
	int result = -1;
	int arg1 = 1;
	int c, i;
	int interactive = 1;

	strmidi = strmidipath;
	strsf2 = strsf2path;

	/* create the settings */
	settings = new_fluid_settings();

#ifdef WIN32
	SetPriorityClass(GetCurrentProcess(), REALTIME_PRIORITY_CLASS);
#endif

	if ((audio_groups == 0) && (audio_channels != 0))
	{
		audio_groups = audio_channels;
	}

	if (audio_groups != 0)
	{
		fluid_settings_setint(settings, "synth.audio-groups", audio_groups);
	}

	if (fast_render)
	{
		midi_in = 0;		/* disable MIDI driver creation */
		interactive = 0;	/* disable user shell creation */
#ifdef NETWORK_SUPPORT
		with_server = 0;	/* disable tcp server shell creation */
#endif
		fluid_settings_setstr(settings, "player.timing-source", "sample");
		fluid_settings_setint(settings, "synth.lock-memory", 0);
	}

	/* create the synthesizer */
	synth = new_fluid_synth(settings);

	if (synth == NULL)
	{
		fprintf(stderr, "Failed to create the synthesizer\n");
		goto cleanup;
	}

	if (fluid_is_soundfont(strsf2path.c_str()))
	{
		if (fluid_synth_sfload(synth, strsf2path.c_str(), 1) == -1)
		{
			fprintf(stderr, "Failed to load the SoundFont\n");
			goto cleanup;
		}
	}
	
	if (!fluid_is_midifile(strmidipath.c_str()))
	{
		fprintf(stderr, "Parameter is not a SoundFont or MIDI file or error occurred identifying it.\n");
		goto cleanup;
	}

	router = new_fluid_midi_router(
		settings,
		dump ? fluid_midi_dump_postrouter : fluid_synth_handle_midi_event,
		(void *)synth);

	if (router == NULL)
	{
		fprintf(stderr, "Failed to create the MIDI input router; no MIDI input\n"
			"will be available. You can access the synthesizer \n"
			"through the console.\n");
	}

	/* start the midi router and link it to the synth */
	if (midi_in && router != NULL)
	{
		/* In dump mode, text output is generated for events going into and out of the router.
		 * The example dump functions are put into the chain before and after the router..
		 */
		mdriver = new_fluid_midi_driver(
			settings,
			dump ? fluid_midi_dump_prerouter : fluid_midi_router_handle_midi_event,
			(void *)router);

		if (mdriver == NULL)
		{
			fprintf(stderr, "Failed to create the MIDI thread; no MIDI input\n"
				"will be available. You can access the synthesizer \n"
				"through the console.\n");
		}
	}

	return true;

cleanup:

#ifdef NETWORK_SUPPORT

	if (server != NULL)
	{
		/* if the user typed 'quit' in the shell, kill the server */
		if (!interactive)
		{
			fluid_server_join(server);
		}

#ifdef SYSTEMD_SUPPORT
		sd_notify(0, "STOPPING=1");
#endif
		delete_fluid_server(server);
	}

#endif	/* NETWORK_SUPPORT */

	if (cmd_handler != NULL)
	{
		delete_fluid_cmd_handler(cmd_handler);
	}

	if (player != NULL)
	{
		/* if the user typed 'quit' in the shell, stop the player */
		if (interactive)
		{
			fluid_player_stop(player);
		}

		if (adriver != NULL || !fluid_settings_str_equal(settings, "player.timing-source", "sample"))
		{
			/* if no audio driver and sample timers are used, nothing makes the player advance */
			fluid_player_join(player);
		}
	}

	delete_fluid_audio_driver(adriver);
	delete_fluid_player(player);
	delete_fluid_midi_driver(mdriver);
	delete_fluid_midi_router(router);
	delete_fluid_synth(synth);
	delete_fluid_settings(settings);

	return false;
}

int CfluidsynthWrapper::get_status()
{
	if (player) {
		return fluid_player_get_status(player);
	}
	
	return -1;
}

bool CfluidsynthWrapper::play()
{
	char buf[512];

	if (player == NULL)
	{
		player = new_fluid_player(synth);

		if (player == NULL)
		{
			fprintf(stderr, "Failed to create the midifile player.\n"
				"Continuing without a player.\n");
			return false;
		}

		if (router != NULL)
		{
			fluid_player_set_playback_callback(player, fluid_midi_router_handle_midi_event, router);
		}
	}

	fluid_player_add(player, strmidi.c_str());
	first_tempo = fluid_player_get_midi_tempo(player);

	/* start the player */
	if (player != NULL)
	{
		/* Try to load the default soundfont, if no soundfont specified */
		if (fluid_synth_get_sfont(synth, 0) == NULL)
		{
			char *s;

			if (fluid_settings_dupstr(settings, "synth.default-soundfont", &s) != FLUID_OK)
			{
				s = NULL;
			}

			if ((s != NULL) && (s[0] != '\0'))
			{
				fluid_synth_sfload(synth, s, 1);
			}

			FLUID_FREE(s);
		}

		fluid_player_play(player);
	}

	/* try to load and execute the user or system configuration file */
	cmd_handler = new_fluid_cmd_handler(synth, router);

	if (cmd_handler == NULL)
	{
		fprintf(stderr, "Failed to create the command handler\n");
		return false;
	}

	if (config_file != NULL)
	{
		if (fluid_source(cmd_handler, config_file) < 0)
		{
			fprintf(stderr, "Failed to execute user provided command configuration file '%s'\n", config_file);
		}
	}
	else if (fluid_get_userconf(buf, sizeof(buf)) != NULL)
	{
		fluid_source(cmd_handler, buf);
	}
	else if (fluid_get_sysconf(buf, sizeof(buf)) != NULL)
	{
		fluid_source(cmd_handler, buf);
	}
	
	/* fast rendering audio file, if requested */
	if (fast_render)
	{
		char *filename;

		if (player == NULL)
		{
			fprintf(stderr, "No midi file specified!\n");
			return false;
		}

		fluid_settings_dupstr(settings, "audio.file.name", &filename);
		printf("Rendering audio to file '%s'..\n", filename);

		if (filename)
		{
			FLUID_FREE(filename);
		}

		fast_render_loop(settings, synth, player);
	}
	else /* start the synthesis thread */
	{
		adriver = new_fluid_audio_driver(settings, synth);

		if (adriver == NULL)
		{
			fprintf(stderr, "Failed to create the audio driver\n");
			return false;
		}
	}

	return true;
}

bool CfluidsynthWrapper::pause()
{
	if (player && adriver) {
		fluid_player_stop(player);
		delete_fluid_audio_driver(adriver);
		adriver = NULL;
		return true;
	}
	return false;
}

bool CfluidsynthWrapper::stop()
{
	if (player && adriver) {
		fluid_player_stop(player);
		fluid_player_join(player);
		delete_fluid_audio_driver(adriver);
		delete_fluid_player(player);
		delete_fluid_midi_router(router);
		adriver = NULL;
		player = NULL;
		router = NULL;
		return true;
	}
	return false;
}

void CfluidsynthWrapper::setfilter(int ntrack, bool flag)
{
	if (player) {
		fluid_player_set_midi_track_mute(player, ntrack, flag ? 0 : 1);
	}
}

void CfluidsynthWrapper::seek(int ticks)
{
	if (get_status() == FLUID_PLAYER_PLAYING) {
		fluid_player_seek(player, ticks);
	}
}

int CfluidsynthWrapper::gettempo()
{	
	return first_tempo;
}

int CfluidsynthWrapper::settempo(int ntempo)
{
	if (player) {
		fluid_player_set_midi_tempo(player, ntempo);
		return 1;
	}
	return 0;
}

int CfluidsynthWrapper::gettickscount()
{
	if (player) {
		return fluid_player_get_total_ticks(player);
	}
	return 0;
}

int CfluidsynthWrapper::getcurrentticks()
{
	if (player) {
		return fluid_player_get_current_tick(player);
	}
	return 0;
}

