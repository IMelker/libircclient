/*
 * Copyright (C) 2004-2009 Georgy Yunaev gyunaev@ulduzsoft.com
 *
 * This example is free, and not covered by LGPL license. There is no 
 * restriction applied to their modification, redistribution, using and so on.
 * You can study them, modify them, use them in your own program - either 
 * completely or partially. By using it you may give me some credits in your
 * program, but you don't have to.
 *
 *
 * This program will test automatic color replacement features. It uses the 
 * colors itself, generates colored string in reaction to 'test' word, and
 * dumps the de-colored messages.
 */

#include <string>

#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <stdlib.h>

#include "libircclient.h"


/*
 * We store data in IRC session context.
 */
typedef struct
{
	char 	* channel;
	char 	* nick;

} irc_ctx_t;



void event_join (irc_session_t * session, const char * event, const char * origin, const char ** params, unsigned int count)
{
	char * str = irc_color_convert_to_mirc ("[B]Hi[/B] [U]all[/U]. Could someone say 'test'?");
	irc_cmd_msg (session, params[0], str);
	free (str);
}


void event_login (irc_session_t * session, const char * event, const char * origin, const char ** params, unsigned int count)
{
	irc_ctx_t * ctx = (irc_ctx_t *) irc_get_ctx (session);
	irc_cmd_join (session, ctx->channel, 0);
}


void event_channel (irc_session_t * session, const char * event, const char * origin, const char ** params, unsigned int count)
{
	printf ("'%s' said in channel %s: %s\n", 
		origin ? origin : "someone",
		params[0], params[1] );

	char * stripped = irc_color_strip_from_mirc (params[1]);
	irc_cmd_msg (session, params[0], stripped);
	free (stripped);

	stripped = irc_color_convert_from_mirc (params[1]);
	irc_cmd_msg (session, params[0], stripped);
	free (stripped);

	if ( !strcmp (params[1], "test") )
	{
	char * str = irc_color_convert_to_mirc ("normal, [B]bold[/B], [I]reverse[/I], [U]underline[/U], [COLOR=RED]red on white[/COLOR], [COLOR=YELLOW/BLACK]yellow on black[/COLOR]");
	irc_cmd_msg (session, params[0], str);
	free (str);
	}

	if ( strstr (params[1], "testme") == params[1] )
	{
	char * str = irc_color_convert_to_mirc (params[1] + 7);
	irc_cmd_msg (session, params[0], str);
	free (str);
	}

}


void event_numeric (irc_session_t * session, unsigned int event, const char * origin, const char ** params, unsigned int count)
{
	if ( event > 400 )
	{
		std::string fulltext;
		for ( unsigned int i = 0; i < count; i++ )
		{
			if ( i > 0 )
				fulltext += " ";

			fulltext += params[i];
		}

		printf ("ERROR %d: %s: %s\n", event, origin ? origin : "?", fulltext.c_str());
	}
}


int main (int argc, char **argv)
{
	irc_callbacks_t	callbacks;
	irc_ctx_t ctx;
	irc_session_t * s;
	unsigned short port = 6667;

	if ( argc != 4 )
	{
		printf ("Usage: %s <server> <nick> <channel>\n", argv[0]);
		return 1;
	}

	memset (&callbacks, 0, sizeof(callbacks));

	callbacks.event_login = event_login;
	callbacks.event_join = event_join;
	callbacks.event_channel = event_channel;
	callbacks.event_numeric = event_numeric;

	s = irc_create_session (&callbacks);

	if ( !s )
	{
		printf ("Could not create session\n");
		return 1;
	}

	ctx.channel = argv[3];
    ctx.nick = argv[2];

	irc_set_ctx (s, &ctx);
	
	// If the port number is specified in the server string, use the port 0 so it gets parsed
	if ( strchr( argv[1], ':' ) != 0 )
		port = 0;

	// To handle the "SSL certificate verify failed" from command line we allow passing ## in front 
	// of the server name, and in this case tell libircclient not to verify the cert
	if ( argv[1][0] == '#' && argv[1][1] == '#' )
	{
		// Skip the first character as libircclient needs only one # for SSL support, i.e. #irc.freenode.net
		argv[1]++;
		
		irc_option_set( s, LIBIRC_OPTION_SSL_NO_VERIFY );
	}
	
	// Initiate the IRC server connection
	if ( irc_connect (s, argv[1], port, 0, argv[2], 0, 0) )
	{
		printf ("Could not connect: %s\n", irc_strerror (irc_errno(s)));
		return 1;
	}

	// and run into forever loop, generating events
	if ( irc_run (s) )
	{
		printf ("Could not connect or I/O error: %s\n", irc_strerror (irc_errno(s)));
		return 1;
	}

	return 1;
}
