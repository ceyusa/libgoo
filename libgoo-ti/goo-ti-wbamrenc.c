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

#include <goo-ti-wbamrenc.h>
#include <goo-utils.h>

#define ID "OMX.TI.WBAMR.encode"
#define DASF_PARAM_NAME "OMX.TI.index.config.tispecific"
#define DATAPATH_PARAM_NAME "OMX.TI.index.config.wbamr.datapath"
#define ACOUSTIC_PARAM_NAME "OMX.TI.index.config.tispecific"
#define STREAMID_PARAM_NAME "OMX.TI.index.config.wbamrstreamIDinfo"

G_DEFINE_TYPE (GooTiWbAmrEnc, goo_ti_wbamrenc, GOO_TYPE_TI_AUDIO_ENCODER)

static void
goo_ti_wbamrenc_init (GooTiWbAmrEnc* self)
{
        GOO_COMPONENT (self)->id = g_strdup (ID);
        GOO_TI_AUDIO_COMPONENT (self)->dasf_param_name =
                g_strdup (DASF_PARAM_NAME);
        GOO_TI_AUDIO_COMPONENT (self)->acoustic_param_name =
                g_strdup (ACOUSTIC_PARAM_NAME);
        GOO_TI_AUDIO_COMPONENT (self)->streamid_param_name =
                g_strdup (STREAMID_PARAM_NAME);
        GOO_TI_AUDIO_COMPONENT (self)->datapath_param_name =
                g_strdup (DATAPATH_PARAM_NAME);

        self->output_port_param = NULL;

        return;
}

static void
goo_ti_wbamrenc_finalize (GObject* object)
{
        g_assert (GOO_IS_TI_WBAMRENC (object));
        GooTiWbAmrEnc* self = GOO_TI_WBAMRENC (object);

        if (G_LIKELY (self->output_port_param))
        {
                g_free (self->output_port_param);
                self->output_port_param = NULL;
        }

        (*G_OBJECT_CLASS (goo_ti_wbamrenc_parent_class)->finalize) (object);

        return;
}

static void
goo_ti_wbamrenc_load_parameters (GooComponent* component)
{
	g_assert (GOO_IS_TI_WBAMRENC (component));
	GooTiWbAmrEnc* self = GOO_TI_WBAMRENC (component);
	g_assert (self->output_port_param == NULL);

	self->output_port_param = g_new0 (OMX_AUDIO_PARAM_AMRTYPE, 1);
	GOO_INIT_PARAM (self->output_port_param, OMX_AUDIO_PARAM_AMRTYPE);

	goo_component_get_parameter_by_index (component,
					      OMX_IndexParamAudioAmr,
					      self->output_port_param);

	goo_ti_audio_component_audio_manager_activate
		(GOO_TI_AUDIO_COMPONENT (component));

	GOO_OBJECT_DEBUG (self, "");
	return;
}

static void
goo_ti_wbamrenc_set_parameters (GooComponent* component)
{
	g_assert (GOO_IS_TI_WBAMRENC (component));
	GooTiWbAmrEnc* self = GOO_TI_WBAMRENC (component);
	g_assert (self->output_port_param != NULL);

	/* @todo move it to goo-utils */
	GOO_OBJECT_DEBUG (self, "\n"
				"nSize: %d\n"
				"nPortIndex : %d\n"
				"nChannels: %d\n"
				"eAMRBandMode: %d\n"
				"eAMRFrameFormat: %d\n"
				"eAMRDTXMode: %d",
			  self->output_port_param->nSize,
			  self->output_port_param->nPortIndex,
			  self->output_port_param->nChannels,
			  self->output_port_param->eAMRBandMode,
			  self->output_port_param->eAMRFrameFormat,
			  self->output_port_param->eAMRDTXMode
			  );

	goo_component_set_parameter_by_index (component,
					      OMX_IndexParamAudioAmr,
					      self->output_port_param);
	GOO_OBJECT_DEBUG (self, "Exit");
	return;
}

static void
goo_ti_wbamrenc_validate_ports (GooComponent* component)
{
	g_assert (GOO_IS_TI_WBAMRENC (component));
	GooTiWbAmrEnc* self = GOO_TI_WBAMRENC (component);
	g_assert (self->output_port_param != NULL);

	OMX_PARAM_PORTDEFINITIONTYPE *def = NULL;

	GOO_OBJECT_DEBUG (self, "Enter");

	/* input */
	{
		GooIterator* iter =
			goo_component_iterate_input_ports (component);
		goo_iterator_nth (iter, 0);
		GooPort* port = GOO_PORT (goo_iterator_get_current (iter));
		g_assert (port != NULL);

		def = GOO_PORT_GET_DEFINITION (port);

		def->format.audio.eEncoding = OMX_AUDIO_CodingPCM;
		def->format.audio.cMIMEType = "audio/x-raw-int";

		g_object_unref (iter);
		g_object_unref (port);
	}

	/* output */
	{
		GooIterator* iter =
			goo_component_iterate_output_ports (component);
		goo_iterator_nth (iter, 0);
		GooPort* port = GOO_PORT (goo_iterator_get_current (iter));
		g_assert (port != NULL);

		def = GOO_PORT_GET_DEFINITION (port);

		def->nBufferSize = GOO_TI_WBAMRENC_OUTPUT_BUFFER_SIZE;
		def->format.audio.eEncoding = OMX_AUDIO_CodingAMR;
		def->format.audio.cMIMEType = "audio/AMR-WB";

		g_object_unref (iter);
		g_object_unref (port);
	}

	/* param */
	{
		OMX_AUDIO_PARAM_AMRTYPE *param;

		param = GOO_TI_WBAMRENC_GET_OUTPUT_PORT_PARAM (component);
		param->nChannels = 1; /* Always MONO */
		/* should validate the rest ? */
	}

	GOO_OBJECT_DEBUG (self, "Exit");

	return;
}

static void
goo_ti_wbamrenc_class_init (GooTiWbAmrEncClass* klass)
{

	GObjectClass* g_klass = G_OBJECT_CLASS (klass);
	g_klass->finalize = goo_ti_wbamrenc_finalize;

	GooComponentClass* o_klass = GOO_COMPONENT_CLASS (klass);
	o_klass->load_parameters_func = goo_ti_wbamrenc_load_parameters;
	o_klass->set_parameters_func = goo_ti_wbamrenc_set_parameters;
	o_klass->validate_ports_definition_func =
		goo_ti_wbamrenc_validate_ports;

	return;
}

