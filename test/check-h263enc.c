/* -*- Mode: C; tab-width: 8; indent-tabs-mode: t; c-basic-offset: 8 -*- */

/*
 * Copyright (C) 2008 Texas Instruments - http://www.ti.com/
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation
 * version 2.1 of the License.
 *
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 */

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <goo-ti-component-factory.h>
#include <goo-engine.h>
#include <goo-utils.h>
#include <goo-ti-h263enc.h>
#include <check.h>
#include <stdlib.h>

#include <stdlib.h>
#include <time.h>
#include <string.h>

#include <glib.h>

GooComponentFactory* factory;
GooComponent* component;
GRand* rnd;

void
setup (void)
{
	factory = goo_ti_component_factory_get_instance ();
	component = goo_component_factory_get_component (factory, GOO_TI_H263_ENCODER);
	rnd = g_rand_new_with_seed (time (0));

	return;
}

void
teardown (void)
{
	g_object_unref (component);
	g_object_unref (factory);
	g_rand_free (rnd);

	return;
}

static void
process (gchar *infile, gint width, gint height, gint incolor,
	gint framerate, gint bitrate, gint level)
{
	fail_unless (infile != NULL, "unspecified filename in test");
	fail_unless (g_file_test (infile, G_FILE_TEST_IS_REGULAR),
		     "file don't exist");

	gchar outfile[100];
	gchar *fn, *fn1;

	fn = g_path_get_basename (infile);
	fn1 = strchr (fn, '.');
	fn1[0] = '\0';
	g_snprintf (outfile, 100, "/tmp/%s.263", fn);
	g_free (fn);

	/* video properties */
	g_object_set (G_OBJECT (component),
		"level", level,
		"control-rate", GOO_TI_VIDEO_ENCODER_CR_VARIABLE,
		NULL);

	/* input port */
	{
		GooPort* port = goo_component_get_port (component, "input0");
		g_assert (port != NULL);
		OMX_PARAM_PORTDEFINITIONTYPE* param =
			GOO_PORT_GET_DEFINITION (port);

		param->nBufferCountActual = 4;
		param->format.video.cMIMEType = "yuv";
		param->format.video.nFrameWidth = width;
		param->format.video.nFrameHeight = height;
		param->format.video.eColorFormat = incolor;
		param->format.video.xFramerate = framerate;

		g_object_unref (G_OBJECT (port));
	}

	/* output port */
	{
		GooPort* port = goo_component_get_port (component, "output0");
		g_assert (port != NULL);
		OMX_PARAM_PORTDEFINITIONTYPE* param =
			GOO_PORT_GET_DEFINITION (port);

		param->nBufferCountActual = 4;
		param->format.video.cMIMEType = "263";
		param->format.video.nFrameWidth = width;
		param->format.video.nFrameHeight = height;
		param->format.video.nBitrate = bitrate;
		param->format.video.eCompressionFormat = OMX_VIDEO_CodingAVC;

		g_object_unref (G_OBJECT (port));
	}

	GooEngine* engine = goo_engine_new (component, infile, outfile);

	goo_component_set_state_idle (component);
	goo_component_set_state_executing (component);

	g_object_set (component, "frame-interval", 30, NULL);

	goo_engine_play (engine);

	goo_component_set_state_idle (component);
	goo_component_set_state_loaded (component);

	g_object_unref (G_OBJECT (engine));

	return;
}

START_TEST (SR11614)
{
	process ("/multimedia/patterns/SR11614.yuv",
		 176, 144, OMX_COLOR_FormatYCbYCr, 15, 64000, 1);

	return;
}
END_TEST

START_TEST (SR11615)
{
	process ("/multimedia/patterns/SR11615.yuv",
		 352, 288, OMX_COLOR_FormatYCbYCr, 15, 128000, 2);

	return;
}
END_TEST


START_TEST (SR11616)
{
	process ("/multimedia/patterns/SR11616.yuv",
		 352, 288, OMX_COLOR_FormatYCbYCr, 30, 384000, 4);

	return;
}
END_TEST

void
fill_tcase (gchar* srd, gpointer func, TCase* tc_h263)
{
	tcase_add_test (tc_h263, func);

	return;
}

Suite *
goo_suite (gchar* srd)
{
	Suite *s = suite_create ("Goo");
	TCase *tc_h263 = tcase_create ("H263");

	GHashTable *ht = g_hash_table_new (g_str_hash, g_str_equal);

	g_hash_table_insert (ht, "SR11614", SR11614);
	g_hash_table_insert (ht, "SR11615", SR11615);
	g_hash_table_insert (ht, "SR11616", SR11616);

	if (g_ascii_strncasecmp ("all", srd, 3) == 0)
	{
		g_hash_table_foreach (ht, (GHFunc) fill_tcase, tc_h263);
	}
	else
	{
		tcase_add_test (tc_h263, g_hash_table_lookup (ht, srd));
	}

	g_hash_table_destroy (ht);

	tcase_add_checked_fixture (tc_h263, setup, teardown);
	tcase_set_timeout (tc_h263, 0);
	suite_add_tcase (s, tc_h263);

	return s;
}

static gchar*
parse_options (int *argc, char **argv[])
{
	GOptionContext* ctx;
	GError *error = NULL;
	gchar* testopt = "all";
	GOptionEntry options[] = {
		{ "test", 't', 0, G_OPTION_ARG_STRING, &testopt,
		  "Test option: (SR11614/SR11616/SR11617)", "S" },
		{ NULL }
	};

	ctx = g_option_context_new ("- H263 Tests.");
	g_option_context_add_main_entries (ctx, options, NULL);

	if (!g_option_context_parse (ctx, argc, argv, &error))
	{
		g_print ("Failed to initialize: %s\n", error->message);
		g_error_free (error);
		return NULL;
	}

	g_option_context_free (ctx);

	return testopt;
}

gint
main (int argc, char *argv[])
{
	int number_failed;

	g_type_init ();
	if (!g_thread_supported ())
	{
		g_thread_init (NULL);
	}

	gchar* srd = parse_options (&argc, &argv);

	if (srd == NULL)
	{
		return EXIT_FAILURE;
	}

	Suite *s = goo_suite (srd);
	SRunner *sr = srunner_create (s);
	srunner_run_all (sr, CK_NORMAL);
	number_failed = srunner_ntests_failed (sr);
	srunner_free (sr);

	return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;

}
