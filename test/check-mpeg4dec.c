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
#include <goo-ti-mpeg4dec.h>
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
	component = goo_component_factory_get_component (factory, GOO_TI_MPEG4_DECODER);
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
process (gchar *infile, gint width, gint height, gint outcolor, GooTiVideoDecoderProcessMode process_mode)
{
	fail_unless (infile != NULL, "unspecified filename in test");
	fail_unless (g_file_test (infile, G_FILE_TEST_IS_REGULAR),
		     "file don't exist");

	gchar outfile[100];
	gchar *fn, *fn1;
	gboolean vopparser;

	fn = g_path_get_basename (infile);
	fn1 = strchr (fn, '.');
	fn1[0] = '\0';
	g_snprintf (outfile, 100, "/tmp/%s.yuv", fn);
	g_free (fn);

	/* input port */
	{
		GooPort* port = goo_component_get_port (component, "input0");
		g_assert (port != NULL);
		OMX_PARAM_PORTDEFINITIONTYPE* param =
			GOO_PORT_GET_DEFINITION (port);

		param->nBufferCountActual = 4;
		param->format.video.nFrameWidth = width;
		param->format.video.nFrameHeight = height;
		param->format.video.eColorFormat = OMX_COLOR_FormatCbYCrY;

		g_object_unref (G_OBJECT (port));
	}

	/* output port */
	{
		GooPort* port = goo_component_get_port (component, "output0");
		g_assert (port != NULL);
		OMX_PARAM_PORTDEFINITIONTYPE* param =
			GOO_PORT_GET_DEFINITION (port);

		param->nBufferCountActual = 4;
		param->format.video.nFrameWidth = width;
		param->format.video.nFrameHeight = height;
		param->format.video.eColorFormat = outcolor;

		g_object_unref (G_OBJECT (port));
	}

	/* video properties */
	  g_object_set (G_OBJECT (component),
					"process-mode", process_mode,
					NULL);
	vopparser = (process_mode == GOO_TI_VIDEO_DECODER_FRAMEMODE) ? TRUE : FALSE;

/*	GooEngine* engine = goo_engine_new_vop (component, infile, outfile, vopparser); */
	GooEngine* engine = goo_engine_new_vop (component, infile, "/dev/null", vopparser);


	goo_component_set_state_idle (component);
	goo_component_set_state_executing (component);

	goo_engine_play (engine);

	goo_component_set_state_idle (component);
	goo_component_set_state_loaded (component);

	g_object_unref (G_OBJECT (engine));

	return;
}

START_TEST (BF0016)
{
	process ("/omx/patterns/carphone_qcif_vsp0_15fps_64kbps.m4v",
		 176, 144, OMX_COLOR_FormatYUV420PackedPlanar, GOO_TI_VIDEO_DECODER_FRAMEMODE);
	return;
}
END_TEST

START_TEST (BF0017)
{
	process ("/omx/patterns/carphone_qcif_vsp0_15fps_64kbps.m4v",
		 176, 144, OMX_COLOR_FormatYUV420PackedPlanar, GOO_TI_VIDEO_DECODER_FRAMEMODE);
	return;
}
END_TEST

START_TEST (BF0018)
{
	process ("/omx/patterns/carphone_qcif_vsp0_15fps_64kbps.m4v",
		 176, 144, OMX_COLOR_FormatYUV420PackedPlanar, GOO_TI_VIDEO_DECODER_FRAMEMODE);
	return;
}
END_TEST

START_TEST (BF0019)
{
	process ("/omx/patterns/carphone_qcif_vsp0b_15fps_128kbps.m4v",
		 176, 144, OMX_COLOR_FormatYUV420PackedPlanar, GOO_TI_VIDEO_DECODER_FRAMEMODE);
	return;
}
END_TEST

START_TEST (BF0020)
{
	process ("/omx/patterns/carphone_qcif_vsp1_15fps_64kbps.m4v",
		 176, 144, OMX_COLOR_FormatYUV420PackedPlanar, GOO_TI_VIDEO_DECODER_FRAMEMODE);
	return;
}
END_TEST

START_TEST (BF0022)
{
	process ("/omx/patterns/foreman300_cif_vsp3_30fps_384kbps.m4v",
		 352, 288, OMX_COLOR_FormatYUV420PackedPlanar, GOO_TI_VIDEO_DECODER_FRAMEMODE);
	return;
}
END_TEST

START_TEST (BF0027)
{
	process ("/omx/patterns/foreman_vga_vsp4a_30fps_4mbps.m4v",
		 640, 480, OMX_COLOR_FormatYUV420PackedPlanar, GOO_TI_VIDEO_DECODER_FRAMEMODE);
	return;
}
END_TEST

START_TEST (BF0028)
{
	process ("/omx/patterns/carphone_qcif_vsp0_15fps_64kbps.m4v",
		 176, 144, OMX_COLOR_FormatYUV420PackedPlanar, GOO_TI_VIDEO_DECODER_FRAMEMODE);
	return;
}
END_TEST

START_TEST (BF0029)
{
	process ("/omx/patterns/carphone_qcif_vsp0_15fps_64kbps.m4v",
		 176, 144, OMX_COLOR_FormatCbYCrY, GOO_TI_VIDEO_DECODER_FRAMEMODE);
	return;
}
END_TEST

START_TEST (BF0030)
{
	process ("/omx/patterns/carphone_qcif_vsp0_15fps_64kbps.m4v",
		 176, 144, OMX_COLOR_FormatYUV420PackedPlanar, GOO_TI_VIDEO_DECODER_STREAMMODE);
	return;
}
END_TEST

START_TEST (BF0031)
{
	process ("/omx/patterns/carphone_qcif_vsp0_15fps_64kbps.m4v",
		 176, 144, OMX_COLOR_FormatYUV420PackedPlanar, GOO_TI_VIDEO_DECODER_FRAMEMODE);
	return;
}
END_TEST

START_TEST (BF0032)
{
	process ("/omx/patterns/carphone_qcif_vsp0_15fps_64kbps.m4v",
		 176, 144, OMX_COLOR_FormatYUV420PackedPlanar, GOO_TI_VIDEO_DECODER_FRAMEMODE);
	return;
}
END_TEST

START_TEST (BF0033)
{
	process ("/omx/patterns/carphone_qcif_vsp0_15fps_64kbps.m4v",
		 176, 144, OMX_COLOR_FormatYUV420PackedPlanar, GOO_TI_VIDEO_DECODER_FRAMEMODE);
	return;
}
END_TEST

START_TEST (BF0116)
{
	process ("/omx/patterns/foreman_cif_vsp2_15fps_128kbps.m4v",
		 352, 288, OMX_COLOR_FormatYUV420PackedPlanar, GOO_TI_VIDEO_DECODER_STREAMMODE);
	return;
}
END_TEST

START_TEST (BF0117)
{
	process ("/omx/patterns/foreman_cif_vsp2_15fps_128kbps.m4v",
		 352, 288, OMX_COLOR_FormatCbYCrY, GOO_TI_VIDEO_DECODER_STREAMMODE);
	return;
}
END_TEST

START_TEST (BF0118)
{
	process ("/omx/patterns/foreman_cif_vsp2_15fps_128kbps.m4v",
		 352, 288, OMX_COLOR_FormatYUV420PackedPlanar, GOO_TI_VIDEO_DECODER_FRAMEMODE);
	return;
}
END_TEST

START_TEST (BF0122)
{
	process ("/omx/patterns/mi3_qcif.m4v",
		 176, 144, OMX_COLOR_FormatCbYCrY, GOO_TI_VIDEO_DECODER_STREAMMODE);
	return;
}
END_TEST

START_TEST (BF0123)
{
	process ("/omx/patterns/mi3_qcif.m4v",
		 176, 144, OMX_COLOR_FormatYUV420PackedPlanar, GOO_TI_VIDEO_DECODER_FRAMEMODE);
	return;
}
END_TEST

START_TEST (BF0149)
{
	process ("/omx/patterns/CTIA_2007_PAL_8mbps_25fps.m4v",
		 720, 576, OMX_COLOR_FormatYUV420PackedPlanar, GOO_TI_VIDEO_DECODER_FRAMEMODE);
	return;
}
END_TEST

START_TEST (BF0150)
{
	process ("/omx/patterns/CTIA_2007_NTSC_8mbps_30fps.m4v",
		 720, 480, OMX_COLOR_FormatYUV420PackedPlanar, GOO_TI_VIDEO_DECODER_FRAMEMODE);
	return;
}
END_TEST

START_TEST (BF0151)
{
	process ("/omx/patterns/carphone_qcif_vsp0_15fps_64kbps.m4v",
		 176, 144, OMX_COLOR_FormatYUV420PackedPlanar, GOO_TI_VIDEO_DECODER_FRAMEMODE);
	return;
}
END_TEST

START_TEST (BF0159)
{
	process ("/omx/patterns/carphone_qcif_vsp0_15fps_64kbps.m4v",
		 176, 144, OMX_COLOR_FormatYUV420PackedPlanar, GOO_TI_VIDEO_DECODER_FRAMEMODE);
	return;
}
END_TEST

START_TEST (BF0160)
{
	process ("/omx/patterns/foreman300_cif_vsp3_30fps_384kbps.m4v",
		 352, 288, OMX_COLOR_FormatYUV420PackedPlanar, GOO_TI_VIDEO_DECODER_FRAMEMODE);
	return;
}
END_TEST

START_TEST (BF0161)
{
	process ("/omx/patterns/foreman_vga_vsp4a_30fps_4mbps.m4v",
		 640, 480, OMX_COLOR_FormatYUV420PackedPlanar, GOO_TI_VIDEO_DECODER_FRAMEMODE);
	return;
}
END_TEST

START_TEST (BF0182)
{
	process ("/omx/patterns/CTIA_2007_PAL_8mbps_25fps.m4v",
		 720, 576, OMX_COLOR_FormatYUV420PackedPlanar, GOO_TI_VIDEO_DECODER_STREAMMODE);
	return;
}
END_TEST

START_TEST (BF0183)
{
	process ("/omx/patterns/CTIA_2007_NTSC_8mbps_30fps.m4v",
		 720, 480, OMX_COLOR_FormatYUV420PackedPlanar, GOO_TI_VIDEO_DECODER_STREAMMODE);
	return;
}
END_TEST

START_TEST (BF0184)
{
	process ("/omx/patterns/CTIA_2007_PAL_8mbps_25fps.m4v",
		 720, 576, OMX_COLOR_FormatCbYCrY, GOO_TI_VIDEO_DECODER_FRAMEMODE);
	return;
}
END_TEST

START_TEST (BF0185)
{
	process ("/omx/patterns/CTIA_2007_NTSC_8mbps_30fps.m4v",
		 720, 480, OMX_COLOR_FormatCbYCrY, GOO_TI_VIDEO_DECODER_FRAMEMODE);
	return;
}
END_TEST

START_TEST (BF0186)
{
	process ("/omx/patterns/CTIA_2007_PAL_8mbps_25fps.m4v",
		 720, 576, OMX_COLOR_FormatCbYCrY, GOO_TI_VIDEO_DECODER_STREAMMODE);
	return;
}
END_TEST

START_TEST (BF0187)
{
	process ("/omx/patterns/CTIA_2007_NTSC_8mbps_30fps.m4v",
		 720, 480, OMX_COLOR_FormatCbYCrY, GOO_TI_VIDEO_DECODER_STREAMMODE);
	return;
}
END_TEST

START_TEST (BF0188)
{
	process ("/omx/patterns/H264_160x120_M.m4v",
		 160, 128, OMX_COLOR_FormatYUV420PackedPlanar, GOO_TI_VIDEO_DECODER_STREAMMODE);
	return;
}
END_TEST

START_TEST (BF0189)
{
	process ("/omx/patterns/MPEG4SP_160s.m4v",
		 160, 128, OMX_COLOR_FormatCbYCrY, GOO_TI_VIDEO_DECODER_FRAMEMODE);
	return;
}
END_TEST

START_TEST (BF0190)
{
	process ("/omx/patterns/MPEG4SP_160x120_15fps_0.m4v",
		 160, 128, OMX_COLOR_FormatYUV420PackedPlanar, GOO_TI_VIDEO_DECODER_STREAMMODE);
	return;
}
END_TEST

START_TEST (BF0191)
{
	process ("/omx/patterns/MPEG4SP_160x120_15fps_0160kbps.m4v",
		 160, 128, OMX_COLOR_FormatYUV420PackedPlanar, GOO_TI_VIDEO_DECODER_STREAMMODE);
	return;
}
END_TEST

START_TEST (BF0198)
{
	process ("/omx/patterns/H264_160x120_M.m4v",
		 160, 128, OMX_COLOR_FormatCbYCrY, GOO_TI_VIDEO_DECODER_STREAMMODE);
	return;
}
END_TEST

START_TEST (BF0199)
{
	process ("/omx/patterns/MPEG4SP_160x120_15fps_0.m4v",
		 160, 128, OMX_COLOR_FormatCbYCrY, GOO_TI_VIDEO_DECODER_STREAMMODE);
	return;
}
END_TEST



START_TEST (SR11619)
{
	process ("/multimedia/patterns/SR11619.m4v",
		 176, 144, OMX_COLOR_FormatCbYCrY, GOO_TI_VIDEO_DECODER_STREAMMODE);

	return;
}
END_TEST


START_TEST (SR11620)
{
	process ("/multimedia/patterns/SR11620.m4v",
		 176, 144, OMX_COLOR_FormatCbYCrY, GOO_TI_VIDEO_DECODER_STREAMMODE);

	return;
}
END_TEST

START_TEST (SR11621)
{
	process ("/multimedia/patterns/SR11621.m4v",
		 176, 144, OMX_COLOR_FormatCbYCrY, GOO_TI_VIDEO_DECODER_STREAMMODE);

	return;
}
END_TEST

START_TEST (SR11622)
{
	process ("/multimedia/patterns/SR11622.m4v",
		 352, 288, OMX_COLOR_FormatCbYCrY, GOO_TI_VIDEO_DECODER_STREAMMODE);

	return;
}
END_TEST

START_TEST (SR11623)
{
	process ("/multimedia/patterns/SR11623.m4v",
		 352, 288, OMX_COLOR_FormatCbYCrY, GOO_TI_VIDEO_DECODER_STREAMMODE);

	return;
}
END_TEST

void
fill_tcase (gchar* srd, gpointer func, TCase* tc_mpeg4)
{
	tcase_add_test (tc_mpeg4, func);

	return;
}

Suite *
goo_suite (gchar* srd)
{
	Suite *s = suite_create ("Goo");
	TCase *tc_mpeg4 = tcase_create ("MPEG4");

	GHashTable *ht = g_hash_table_new (g_str_hash, g_str_equal);

	g_hash_table_insert (ht, "BF0016", BF0016);
	g_hash_table_insert (ht, "BF0017", BF0017);
	g_hash_table_insert (ht, "BF0018", BF0018);
	g_hash_table_insert (ht, "BF0019", BF0019);
	g_hash_table_insert (ht, "BF0020", BF0020);
	g_hash_table_insert (ht, "BF0022", BF0022);
	g_hash_table_insert (ht, "BF0027", BF0027);
	g_hash_table_insert (ht, "BF0028", BF0028);
	g_hash_table_insert (ht, "BF0029", BF0029);
	g_hash_table_insert (ht, "BF0030", BF0030);
	g_hash_table_insert (ht, "BF0031", BF0031);
	g_hash_table_insert (ht, "BF0032", BF0032);
	g_hash_table_insert (ht, "BF0033", BF0033);
	g_hash_table_insert (ht, "BF0116", BF0116);
	g_hash_table_insert (ht, "BF0117", BF0117);
	g_hash_table_insert (ht, "BF0118", BF0118);
	g_hash_table_insert (ht, "BF0122", BF0122);
	g_hash_table_insert (ht, "BF0123", BF0123);
	g_hash_table_insert (ht, "BF0149", BF0149);
	g_hash_table_insert (ht, "BF0150", BF0150);
	g_hash_table_insert (ht, "BF0151", BF0151);
	g_hash_table_insert (ht, "BF0159", BF0159);
	g_hash_table_insert (ht, "BF0160", BF0160);
	g_hash_table_insert (ht, "BF0161", BF0161);
	g_hash_table_insert (ht, "BF0182", BF0182);
	g_hash_table_insert (ht, "BF0183", BF0183);
	g_hash_table_insert (ht, "BF0184", BF0184);
	g_hash_table_insert (ht, "BF0185", BF0185);
	g_hash_table_insert (ht, "BF0186", BF0186);
	g_hash_table_insert (ht, "BF0187", BF0187);
	g_hash_table_insert (ht, "BF0188", BF0188);
	g_hash_table_insert (ht, "BF0189", BF0189);
	g_hash_table_insert (ht, "BF0190", BF0190);
	g_hash_table_insert (ht, "BF0191", BF0191);
	g_hash_table_insert (ht, "BF0198", BF0198);
	g_hash_table_insert (ht, "BF0199", BF0199);
	g_hash_table_insert (ht, "SR11619", SR11619);
	g_hash_table_insert (ht, "SR11620", SR11620);
	g_hash_table_insert (ht, "SR11621", SR11621);
	g_hash_table_insert (ht, "SR11622", SR11622);
	g_hash_table_insert (ht, "SR11623", SR11623);
	g_hash_table_insert (ht, "SR11623", SR11623);

	if (g_ascii_strncasecmp ("all", srd, 3) == 0)
	{
		g_hash_table_foreach (ht, (GHFunc) fill_tcase, tc_mpeg4);
	}
	else
	{
		tcase_add_test (tc_mpeg4, g_hash_table_lookup (ht, srd));
	}

	g_hash_table_destroy (ht);

	tcase_add_checked_fixture (tc_mpeg4, setup, teardown);
	tcase_set_timeout (tc_mpeg4, 0);
	suite_add_tcase (s, tc_mpeg4);

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
		  "Test option: (BF0016/BF0017/BF0018/BF0019/BF0020/BF0022/BF0027/BF0028/BF0029/BF0030/BF0031/BF0032/BF0033/BF0116/BF0117/BF0118/BF0122/BF0123/BF0149/BF0150/BF0151/BF0159/BF0160/BF0161/BF0182/BF0183/BF0184/BF0185/BF0186/BF0187/BF0188/BF0189/BF0190/BF0191/BF0198/BF0199/SR11619/SR11620/SR11621/SR11622/SR11623)", "S" },
		{ NULL }
	};

	ctx = g_option_context_new ("- MPEG4 Tests.");
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

