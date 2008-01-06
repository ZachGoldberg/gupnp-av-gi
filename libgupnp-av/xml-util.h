/*
 * Copyright (C) 2006, 2007 OpenedHand Ltd.
 * Copyright (C) 2007 Zeeshan Ali.
 *
 * Author: Jorn Baayen <jorn@openedhand.com>
 * Author: Zeeshan Ali <zeenix@gstreamer.net>
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

#ifndef __XML_UTIL_H__
#define __XML_UTIL_H__

#include <glib.h>
#include <libxml/tree.h>
#include <stdarg.h>

/* Misc utilities for inspecting xmlNodes */
xmlNode *
xml_util_get_element                    (xmlNode    *node,
                                         ...) G_GNUC_NULL_TERMINATED;

GList *
xml_util_get_child_elements_by_name     (xmlNode *node,
                                         const char *name);

char *
xml_util_get_element_content            (xmlNode    *node);

char *
xml_util_get_child_element_content      (xmlNode    *node,
                                         const char *child_name);

char *
xml_util_get_attribute_content         (xmlNode    *node,
                                         const char *attribute_name);

gboolean
xml_util_get_boolean_attribute          (xmlNode    *node,
                                         const char *attribute_name);

guint
xml_util_get_uint_attribute             (xmlNode    *node,
                                         const char *attribute_name);

#endif /* __XML_UTIL_H__ */
