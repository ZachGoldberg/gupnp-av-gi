/*
 * Copyright (C) 2007 Zeeshan Ali <zeenix@gstreamer.net>
 *
 * Authors: Zeeshan Ali <zeenix@gstreamer.net>
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

#include <string.h>

#include "xml-util.h"
#include "gupnp-didl-lite-object.h"

#define CONTAINER_CLASS_NAME "object.container"
#define ITEM_CLASS_NAME      "object.item"

gboolean
gupnp_didl_lite_object_is_container (xmlNode *object_node)
{
        char     *class_name;
        gboolean is_container;

        g_return_val_if_fail (object_node != NULL, FALSE);

        class_name = gupnp_didl_lite_object_get_upnp_class (object_node);
        if (class_name == NULL) {
                return FALSE;
        }

        if (0 == strncmp (class_name,
                          CONTAINER_CLASS_NAME,
                          strlen (CONTAINER_CLASS_NAME))) {
                is_container = TRUE;
        } else {
                is_container = FALSE;
        }

        g_free (class_name);

        return is_container;
}

gboolean
gupnp_didl_lite_object_is_item (xmlNode *object_node)
{
        char     *class_name;
        gboolean is_item;

        g_return_val_if_fail (object_node != NULL, FALSE);

        class_name = gupnp_didl_lite_object_get_upnp_class (object_node);
        if (class_name == NULL) {
                return FALSE;
        }

        if (0 == strncmp (class_name,
                          ITEM_CLASS_NAME,
                          strlen (ITEM_CLASS_NAME))) {
                is_item = TRUE;
        } else {
                is_item = FALSE;
        }

        g_free (class_name);

        return is_item;
}

char *
gupnp_didl_lite_object_get_upnp_class (xmlNode *object_node)
{
        g_return_val_if_fail (object_node != NULL, NULL);

        return gupnp_didl_lite_object_get_value (object_node, "class");
}

char *
gupnp_didl_lite_object_get_id (xmlNode *object_node)
{
        g_return_val_if_fail (object_node != NULL, NULL);

        return xml_util_get_attribute_content (object_node, "id");
}

char *
gupnp_didl_lite_object_get_parent_id (xmlNode *object_node)
{
        g_return_val_if_fail (object_node != NULL, NULL);

        return xml_util_get_attribute_content (object_node,
                                                "parentID");
}

char *
gupnp_didl_lite_object_get_title (xmlNode *object_node)
{
        g_return_val_if_fail (object_node != NULL, NULL);

        return gupnp_didl_lite_object_get_value (object_node, "title");
}

char *
gupnp_didl_lite_object_get_value (xmlNode    *object_node,
                                  const char *value_id)
{
        g_return_val_if_fail (object_node != NULL, NULL);

        return xml_util_get_child_element_content (object_node, value_id);
}

gboolean
gupnp_didl_lite_object_get_restricted (xmlNode *object_node)
{
        g_return_val_if_fail (object_node != NULL, FALSE);

        return xml_util_get_boolean_attribute (object_node, "restricted");
}

gboolean
gupnp_didl_lite_object_get_never_playable (xmlNode *object_node)
{
        g_return_val_if_fail (object_node != NULL, FALSE);

        return xml_util_get_boolean_attribute (object_node, "neverPlayable");
}

GList *
gupnp_didl_lite_object_get_descriptors (xmlNode *object_node)
{
        g_return_val_if_fail (object_node != NULL, FALSE);

        return xml_util_get_child_elements_by_name (object_node, "desc");
}

GList *
gupnp_didl_lite_object_get_resources (xmlNode *object_node)
{
        g_return_val_if_fail (object_node != NULL, NULL);

        return xml_util_get_child_elements_by_name (object_node, "res");
}

GHashTable *
gupnp_didl_lite_object_get_resource_hash (xmlNode *object_node)
{
   GHashTable *resource_hash;
   GList *resources;
   GList *iter;

   resources = gupnp_didl_lite_object_get_resources (object_node);

   if (resources == NULL)
           return NULL;

   resource_hash = g_hash_table_new_full (g_str_hash,
                                          g_str_equal,
                                          g_free,
                                          g_free);

   for (iter = resources; iter; iter = iter->next) {
           xmlNode *res_node;
           char *uri;
           char *proto_info;

           res_node = (xmlNode *) iter->data;
           proto_info = gupnp_didl_lite_resource_get_protocol_info (res_node);
           if (proto_info == NULL)
                   continue;

           uri = gupnp_didl_lite_resource_get_contents (res_node);
           if (uri == NULL) {
                   g_free (proto_info);
                   continue;
           }

           g_hash_table_insert (resource_hash, proto_info, uri);
   }

   if (g_hash_table_size (resource_hash) == 0) {
           /* No point in keeping empty hash tables here */
           g_hash_table_destroy (resource_hash);
           resource_hash = NULL;
   }

   return resource_hash;
}

gboolean
gupnp_didl_lite_container_is_searchable (xmlNode *container_node)
{
        g_return_val_if_fail (container_node != NULL, FALSE);

        return xml_util_get_boolean_attribute (container_node, "searchable");
}

guint
gupnp_didl_lite_container_get_child_count (xmlNode *container_node)
{
        g_return_val_if_fail (container_node != NULL, 0);

        return xml_util_get_uint_attribute (container_node, "childCount");
}

char *
gupnp_didl_lite_item_get_ref_id (xmlNode *item_node)
{
        g_return_val_if_fail (item_node != NULL, NULL);

        return xml_util_get_attribute_content (item_node, "refID");
}

char *
gupnp_didl_lite_descriptor_get_type (xmlNode *desc_node)
{
        g_return_val_if_fail (desc_node != NULL, NULL);

        return xml_util_get_attribute_content (desc_node, "type");
}

char *
gupnp_didl_lite_descriptor_get_name_space (xmlNode *desc_node)
{
        g_return_val_if_fail (desc_node != NULL, NULL);

        return xml_util_get_attribute_content (desc_node, "nameSpace");
}

char *
gupnp_didl_lite_resource_get_contents (xmlNode *res_node)
{
        g_return_val_if_fail (res_node != NULL, NULL);

        return xml_util_get_element_content (res_node);
}

char *
gupnp_didl_lite_resource_get_import_uri (xmlNode *res_node)
{
        g_return_val_if_fail (res_node != NULL, NULL);

        return xml_util_get_attribute_content (res_node, "importUri");
}

char *
gupnp_didl_lite_resource_get_protocol_info (xmlNode *res_node)
{
        g_return_val_if_fail (res_node != NULL, NULL);

        return xml_util_get_attribute_content (res_node, "protocolInfo");
}

guint
gupnp_didl_lite_resource_get_size (xmlNode *res_node)
{
        g_return_val_if_fail (res_node != NULL, 0);

        return xml_util_get_uint_attribute (res_node, "size");
}

char *
gupnp_didl_lite_resource_get_duration (xmlNode *res_node)
{
        g_return_val_if_fail (res_node != NULL, NULL);

        return xml_util_get_attribute_content (res_node, "duration");
}

guint
gupnp_didl_lite_resource_get_bitrate (xmlNode *res_node)
{
        g_return_val_if_fail (res_node != NULL, 0);

        return xml_util_get_uint_attribute (res_node, "bitrate");
}

guint
gupnp_didl_lite_resource_get_sample_frequency (xmlNode *res_node)
{
        g_return_val_if_fail (res_node != NULL, 0);

        return xml_util_get_uint_attribute (res_node, "frequency");
}

guint
gupnp_didl_lite_resource_get_bits_per_sample (xmlNode *res_node)
{
        g_return_val_if_fail (res_node != NULL, 0);

        return xml_util_get_uint_attribute (res_node, "bitsPerSample");
}

guint
gupnp_didl_lite_resource_get_nr_audio_channels (xmlNode *res_node)
{
        g_return_val_if_fail (res_node != NULL, 0);

        return xml_util_get_uint_attribute (res_node, "nrAudioChannels");
}

char *
gupnp_didl_lite_resource_get_resolution (xmlNode *res_node)
{
        g_return_val_if_fail (res_node != NULL, NULL);

        return xml_util_get_attribute_content (res_node, "resolution");
}

guint
gupnp_didl_lite_resource_get_color_depth (xmlNode *res_node)
{
        g_return_val_if_fail (res_node != NULL, 0);

        return xml_util_get_uint_attribute (res_node, "colorDepth");
}

char *
gupnp_didl_lite_resource_get_protection (xmlNode *res_node)
{
        g_return_val_if_fail (res_node != NULL, NULL);

        return xml_util_get_attribute_content (res_node, "protection");
}
