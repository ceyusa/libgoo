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

#ifndef _GOO_TI_MPEG4DEC_H_
#define _GOO_TI_MPEG4DEC_H_

#include <goo-ti-video-decoder.h>

G_BEGIN_DECLS

#define GOO_TYPE_TI_MPEG4DEC \
        (goo_ti_mpeg4dec_get_type ())
#define GOO_TI_MPEG4DEC(obj) \
        (G_TYPE_CHECK_INSTANCE_CAST ((obj), GOO_TYPE_TI_MPEG4DEC, GooTiMpeg4Dec))
#define GOO_TI_MPEG4DEC_CLASS(klass) \
        (G_TYPE_CHECK_CLASS_CAST ((klass), GOO_TYPE_TI_MPEG4DEC, GooTiMpeg4DecClass))
#define GOO_IS_TI_MPEG4DEC(obj) \
        (G_TYPE_CHECK_INSTANCE_TYPE ((obj), GOO_TYPE_TI_MPEG4DEC))
#define GOO_IS_TI_MPEG4DEC_CLASS(klass) \
        (G_TYPE_CHECK_CLASS_TYPE ((klass), GOO_TYPE_TI_MPEG4DEC))
#define GOO_TI_MPEG4DEC_GET_CLASS(obj) \
        (G_TYPE_INSTANCE_GET_CLASS ((obj), GOO_TI_MPEG4DEC_GET_CLASS, GooTiMpeg4DecClass))

typedef struct _GooTiMpeg4Dec GooTiMpeg4Dec;
typedef struct _GooTiMpeg4DecClass GooTiMpeg4DecClass;

struct _GooTiMpeg4Dec
{
        GooTiVideoDecoder parent;

};

struct _GooTiMpeg4DecClass
{
        GooTiVideoDecoderClass parent_class;
};

/* functions */
GType goo_ti_mpeg4dec_get_type (void);
GooComponent* goo_ti_mpeg4dec_new ();

G_END_DECLS


#endif /* _GOO_TI_MPEG4DEC_H_ */
