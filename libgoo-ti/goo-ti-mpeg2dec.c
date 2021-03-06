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

#include <goo-ti-mpeg2dec.h>
#include <goo-utils.h>

G_DEFINE_TYPE (GooTiMpeg2Dec, goo_ti_mpeg2dec, GOO_TYPE_TI_VIDEO_DECODER)

static void
goo_ti_mpeg2dec_validate_ports_definitions (GooComponent* component)
{
	g_assert (GOO_IS_TI_MPEG2DEC (component));
	GooTiMpeg2Dec* self = GOO_TI_MPEG2DEC (component);
	g_assert (component->cur_state == OMX_StateLoaded);

	OMX_PARAM_PORTDEFINITIONTYPE *param;

	GOO_OBJECT_DEBUG (self, "Entering");

	/* input */
	{
		GooIterator* iter =
			goo_component_iterate_input_ports (component);
		goo_iterator_nth (iter, 0);
		GooPort* port = GOO_PORT (goo_iterator_get_current (iter));
		g_assert (port != NULL);

		param = GOO_PORT_GET_DEFINITION (port);

		param->format.video.cMIMEType = "MPEG2";
		param->format.video.eCompressionFormat = OMX_VIDEO_CodingMPEG2;
		param->format.video.bFlagErrorConcealment = OMX_FALSE;
		param->format.video.nFrameWidth = GOO_ROUND_UP_16 (param->format.video.nFrameWidth);
		param->format.video.nFrameHeight = GOO_ROUND_UP_16 (param->format.video.nFrameHeight);

		switch (param->format.video.eColorFormat)
		{
			case OMX_COLOR_FormatCbYCrY:
			case OMX_COLOR_FormatYCbYCr:
				param->nBufferSize =
					param->format.video.nFrameWidth *
					param->format.video.nFrameHeight / 2;
				break;
			case OMX_COLOR_FormatYUV420PackedPlanar:
				param->nBufferSize =
					(param->format.video.nFrameWidth *
					param->format.video.nFrameHeight * 3) / 8;
				break;
			default:
				GOO_OBJECT_ERROR (self, "Not valid color format");
				g_assert (FALSE);
		}

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

		param = GOO_PORT_GET_DEFINITION (port);
		param->format.video.nFrameWidth = GOO_ROUND_UP_16 (param->format.video.nFrameWidth);
		param->format.video.nFrameHeight = GOO_ROUND_UP_16 (param->format.video.nFrameHeight);

		switch (param->format.video.eColorFormat)
		{
			case OMX_COLOR_FormatCbYCrY:
			case OMX_COLOR_FormatYCbYCr:
				param->nBufferSize =
					param->format.video.nFrameWidth *
					param->format.video.nFrameHeight * 2;
				break;
			case OMX_COLOR_FormatYUV420PackedPlanar:
				param->nBufferSize =
					param->format.video.nFrameWidth *
					param->format.video.nFrameHeight * 1.5;
				break;
			default:
				GOO_OBJECT_ERROR (self, "Not valid color format");
				g_assert (FALSE);
		}

		param->format.video.cMIMEType = "YUV";
		param->format.video.pNativeRender = NULL;
		param->format.video.bFlagErrorConcealment = OMX_FALSE;
		param->format.video.eCompressionFormat =
			OMX_VIDEO_CodingUnused;

		/** @todo validate frame size, bitrate and framerate */

		g_object_unref (iter);
		g_object_unref (port);

	}

	GOO_OBJECT_DEBUG (self, "Exit");

	return;
}

static void
goo_ti_mpeg2dec_init (GooTiMpeg2Dec* self)
{
	return;
}

static void
goo_ti_mpeg2dec_class_init (GooTiMpeg2DecClass* klass)
{
	GooComponentClass* o_klass = GOO_COMPONENT_CLASS (klass);
	o_klass->validate_ports_definition_func =
		goo_ti_mpeg2dec_validate_ports_definitions;

	return;
}


