/* -*- Mode: C; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 8 -*- */

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

#include <goo-ti-g722dec.h>
#include <goo-ti-component-factory.h>

#include <goo-engine.h>
#include <goo-utils.h>

#include <check.h>
#include <stdlib.h>

START_TEST (test_g722dec_1)
{
        GooComponentFactory* factory =
                goo_ti_component_factory_get_instance ();
        GooComponent* component =
                goo_component_factory_get_component (factory,
                                                     GOO_TI_G722_DECODER);
        /* audio properties */

        g_object_set (G_OBJECT (component),
                      "dasf-mode", TRUE,
                      "data-path", NULL,
                      "mute", FALSE,
                      "volume", 100,
                      NULL);

        /* input component parameters */
        {
                GOO_TI_G722DEC_GET_INPUT_PARAM (component)->nChannels = 1;
                GOO_TI_G722DEC_GET_INPUT_PARAM (component)->nSamplingRate = 48000;
        }

        /* output component parameters */
        {
                GOO_TI_G722DEC_GET_OUTPUT_PARAM (component)->nChannels                = 1; /* mono */
                GOO_TI_G722DEC_GET_OUTPUT_PARAM (component)->nSamplingRate            = 16000;
                GOO_TI_G722DEC_GET_OUTPUT_PARAM (component)->ePCMMode                 = OMX_AUDIO_PCMModeLinear;

        }
        GOO_OBJECT_WARNING (component, "stream ID = %d",
                            goo_ti_audio_component_get_stream_id
                            (GOO_TI_AUDIO_COMPONENT (component)));

        goo_component_set_state_idle (component);
        goo_component_set_state_executing (component);

        GooEngine* engine = goo_engine_new (
                component,
                "/omx/patterns/F116_48kbps.enc",
                "/tmp/F116_48kbps.pcm"
                );

        goo_engine_play (engine);

        goo_component_set_state_idle (component);
        goo_component_set_state_loaded (component);

        g_object_unref (G_OBJECT (engine));
        g_object_unref (G_OBJECT (component));
        g_object_unref (G_OBJECT (factory));

}
END_TEST

Suite *
goo_suite (void)
{
        Suite *s = suite_create ("Goo");

        TCase *tc_g722dec = tcase_create ("G722Decoder");
        tcase_add_test (tc_g722dec, test_g722dec_1);
        tcase_set_timeout (tc_g722dec, 0);
        suite_add_tcase (s, tc_g722dec);

        return s;
}

gint
main (void)
{
        int number_failed;

        g_type_init ();
        if (!g_thread_supported ())
        {
                g_thread_init (NULL);
        }

        Suite *s = goo_suite ();
        SRunner *sr = srunner_create (s);
        srunner_run_all (sr, CK_NORMAL);
        number_failed = srunner_ntests_failed (sr);
        srunner_free (sr);

        return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}
