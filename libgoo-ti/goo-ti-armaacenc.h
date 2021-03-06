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

#ifndef _GOO_TI_ARMAACENC_H_
#define _GOO_TI_ARMAACENC_H_

#include <goo-ti-audio-component.h>
#include <goo-ti-audio-encoder.h>

G_BEGIN_DECLS

#define GOO_TYPE_TI_ARMAACENC			\
	(goo_ti_armaacenc_get_type ())
#define GOO_TI_ARMAACENC(obj)						\
	(G_TYPE_CHECK_INSTANCE_CAST ((obj), GOO_TYPE_TI_ARMAACENC, GooTiArmAacEnc))
#define GOO_TI_ARMAACENC_CLASS(klass)					\
	(G_TYPE_CHECK_CLASS_CAST ((klass), GOO_TYPE_TI_ARMAACENC, GooTiArmAacEncClass))
#define GOO_IS_TI_ARMAACENC(obj)						\
	(G_TYPE_CHECK_INSTANCE_TYPE ((obj), GOO_TYPE_TI_ARMAACENC))
#define GOO_IS_TI_ARMAACENC_CLASS(klass)				\
	(G_TYPE_CHECK_CLASS_TYPE ((klass), GOO_TYPE_TI_ARMAACENC))
#define GOO_TI_ARMAACENC_GET_CLASS(obj)					\
	(G_TYPE_INSTANCE_GET_CLASS ((obj), GOO_TI_ARMAACENC_GET_CLASS, GooTiArmAacEncClass))

typedef struct _GooTiArmAacEnc GooTiArmAacEnc;
typedef struct _GooTiArmAacEncClass GooTiArmAacEncClass;

struct _GooTiArmAacEnc
{
	GooTiAudioEncoder parent;

	guint output_frames;

	OMX_AUDIO_PARAM_AACPROFILETYPE* output_port_param;
	OMX_AUDIO_PARAM_PCMMODETYPE* input_port_param;
};

struct _GooTiArmAacEncClass
{
	GooTiAudioEncoderClass parent_class;
};

/**
 * GOO_TI_ARMAACENC_GET_INPUT_PARAM:
 * @enc: a #GooTiArmAacEnc instance
 *
 * Gets the #OMX_AUDIO_PARAM_PCMMODETYPE structure of the AAC component.
 *
 * Return value: The #OMX_AUDIO_PARAM_PCMMODETYPE pointer.
 */
#define GOO_TI_ARMAACENC_GET_INPUT_PORT_PARAM(enc) \
	(GOO_TI_ARMAACENC (enc)->input_port_param)

/**
 * GOO_TI_ARMAACENC_GET_OUTPUT_PARAM:
 * @enc: a #GooTiAacEnc instance
 *
 * Gets the #OMX_AUDIO_PARAM_AACPROFILETYPE structure of the AAC component.
 *
 * Return value: The #OMX_AUDIO_PARAM_AACPROFILETYPE pointer.
 */
#define GOO_TI_ARMAACENC_GET_OUTPUT_PORT_PARAM(enc)	\
	(GOO_TI_ARMAACENC (enc)->output_port_param)

#define GOO_TI_ARMAACENC_INPUT_BUFFER_SIZE	 8192

GType goo_ti_armaacenc_get_type (void);

typedef enum _GooTiArmAacEncBitRateMode GooTiArmAacEncBitRateMode;

enum _GooTiArmAacEncBitRateMode
{
	GOO_TI_ARMAACENC_BR_CBR = 0,
	GOO_TI_ARMAACENC_BR_VBR1,
	GOO_TI_ARMAACENC_BR_VBR2,
	GOO_TI_ARMAACENC_BR_VBR3,
	GOO_TI_ARMAACENC_BR_VBR4,
	GOO_TI_ARMAACENC_BR_VBR5
};

GType goo_ti_armaacenc_bitratemode_get_type (void);

G_END_DECLS

#endif /* _GOO_TI_ARMAACENC_H_ */
