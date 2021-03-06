/*
 * Copyright (C) 2009 Nokia Corporation.
 *
 * Authors: Zeeshan Ali <zeeshanak@gnome.org>
 *                      <zeeshan.ali@nokia.com>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public
 * License along with this library; if not, write to the
 * Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 */

#ifndef __GUPNP_DLNA_PRIVATE_H__
#define __GUPNP_DLNA_PRIVATE_H__

#include <glib.h>
#include "gupnp-didl-lite-resource.h"

G_BEGIN_DECLS

G_GNUC_INTERNAL const char *
dlna_guess_profile                      (GUPnPDIDLLiteResource *resource,
                                         const char            *mime_type);

G_END_DECLS

#endif /* __GUPNP_DLNA_PRIVATE_H__ */
