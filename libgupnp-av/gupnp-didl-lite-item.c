/*
 * Copyright (C) 2009 Nokia Corporation.
 *
 * Authors: Zeeshan Ali (Khattak) <zeeshan.ali@nokia.com>
 *                                <zeeshanak@gnome.org>
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

/**
 * SECTION:gupnp-didl-lite-item
 * @short_description: DIDL-Lite Item
 *
 * #GUPnPDIDLLiteItem respresents a DIDL-Lite item element.
 */

#include <string.h>

#include "gupnp-didl-lite-item.h"
#include "xml-util.h"

G_DEFINE_TYPE (GUPnPDIDLLiteItem,
               gupnp_didl_lite_item,
               GUPNP_TYPE_DIDL_LITE_OBJECT);

enum {
        PROP_0,
        PROP_REF_ID
};

static void
gupnp_didl_lite_item_init (GUPnPDIDLLiteItem *item)
{
        /* Nothing to initialize, yay! */
}

static void
gupnp_didl_lite_item_get_property (GObject    *object,
                                   guint       property_id,
                                   GValue     *value,
                                   GParamSpec *pspec)
{
        GUPnPDIDLLiteItem *item;

        item = GUPNP_DIDL_LITE_ITEM (object);

        switch (property_id) {
        case PROP_REF_ID:
                g_value_set_string
                        (value,
                         gupnp_didl_lite_item_get_ref_id (item));
                break;
        default:
                G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
                break;
        }
}

static void
gupnp_didl_lite_item_constructed (GObject *object)
{
        GObjectClass *object_class;
        xmlNode      *node;

        /* Chain-up first */
        object_class = G_OBJECT_CLASS (gupnp_didl_lite_item_parent_class);
        if (object_class->constructed != NULL)
                object_class->constructed (object);

        /* Now override the root element name */
        node = gupnp_didl_lite_object_get_xml_node
                                        (GUPNP_DIDL_LITE_OBJECT (object));
        xmlNodeSetName (node, (unsigned char *) "item");
}

static void
gupnp_didl_lite_item_class_init (GUPnPDIDLLiteItemClass *klass)
{
        GObjectClass *object_class;

        object_class = G_OBJECT_CLASS (klass);

        object_class->get_property = gupnp_didl_lite_item_get_property;
        object_class->constructed = gupnp_didl_lite_item_constructed;

        /**
         * GUPnPDIDLLiteItem:ref-id
         *
         * The ref ID of this item.
         **/
        g_object_class_install_property
                (object_class,
                 PROP_REF_ID,
                 g_param_spec_string ("ref-id",
                                      "RefID",
                                      "The ref ID of this item.",
                                      NULL,
                                      G_PARAM_READABLE |
                                      G_PARAM_STATIC_NAME |
                                      G_PARAM_STATIC_NICK |
                                      G_PARAM_STATIC_BLURB));
}

/**
 * gupnp_didl_lite_item_get_ref_id
 * @item: #GUPnPDIDLLiteItem
 *
 * Get the ref ID of the @item.
 *
 * Return value: The ref ID of the @item, or %NULL. #g_free after usage.
 **/
char *
gupnp_didl_lite_item_get_ref_id (GUPnPDIDLLiteItem *item)
{
        xmlNode *xml_node;

        g_return_val_if_fail (item != NULL, 0);
        g_return_val_if_fail (GUPNP_IS_DIDL_LITE_ITEM (item), NULL);

        xml_node = gupnp_didl_lite_object_get_xml_node
                                        (GUPNP_DIDL_LITE_OBJECT (item));

        return xml_util_get_attribute_content (xml_node, "refID");
}