/*
 * Copyright (C) 2007 Zeeshan Ali <zeenix@gstreamer.net>
 * Copyright (C) 2007 OpenedHand Ltd
 *
 * Authors: Zeeshan Ali <zeenix@gstreamer.net>
 *          Jorn Baayen <jorn@openedhand.com>
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

#include <gobject/gvaluecollector.h>

#include "gupnp-av-util.h"
#include "gvalue-util.h"
#include "xml-util.h"

/* Reads a value of state variable @variable_name to an initialised GValue pair
 * from the InstanceID node of a LastChange xml doc */
static void
read_state_variable (const char *variable_name,
                     GValue     *value,
                     xmlNode    *instance_node)
{
        xmlNode *variable_node;
        char    *val_str;

        variable_node = xml_util_get_element (instance_node,
                                              "InstanceID",
                                              variable_name,
                                              NULL);
        if (!variable_node)
                return;

        val_str = xml_util_get_attribute_content (variable_node, "val");
        if (!val_str) {
                g_warning ("No value provided for variable \"%s\" in "
                           "LastChange event",
                           variable_name);

                return;
        }

        gvalue_util_set_value_from_string (value, (char *) val_str);

        g_free (val_str);
}

static xmlNode *
get_instance_node (xmlDoc *doc,
                   guint   instance_id)
{
        xmlNode *node;

        for (node = doc->children;
             node;
             node = node->next) {
                guint id;

                id = xml_util_get_uint_attribute (node, "val");
                if (id == instance_id)
                        break;
        }

        return node;
}

/**
 * gupnp_av_util_parse_last_change_valist
 * @last_change_xml: The xml from the "LastChange" event to parse
 * @instance_id: The ID of instance the caller is interested in
 * @error: The location where to store any error, or NULL
 * @Varargs: A va_list of tuples of state variable name, state variable type,
 * and state variable value location, terminated with NULL. The state variable
 * values should be freed after use
 *
 * See gupnp_av_util_parse_last_change(); this version takes a va_list for use
 * by language bindings.
 *
 * Return value: TRUE on success.
 **/
gboolean
gupnp_av_util_parse_last_change_valist (const char *last_change_xml,
                                        guint       instance_id,
                                        GError    **error,
                                        va_list     var_args)
{
        const char *variable_name;
        xmlDoc  *doc;
        xmlNode *instance_node;

        g_return_val_if_fail (last_change_xml, FALSE);

        doc = xmlParseDoc ((const xmlChar *) last_change_xml);
        if (doc == NULL) {
                g_set_error (error,
                             GUPNP_SERVER_ERROR,
                             GUPNP_SERVER_ERROR_INVALID_RESPONSE,
                             "Could not parse LastChange xml");

                return FALSE;
        }

        instance_node = get_instance_node (doc, instance_id);
        if (instance_node == NULL) {
                /* This is not an error since the caller of this function
                 * doesn't (need to) know if the instance of his interest is
                 * part of the LastChange event received.
                 */
                xmlFreeDoc (doc);

                return FALSE;
        }

        /* Variables */
        variable_name = va_arg (var_args, const char *);
        while (variable_name) {
                GType variable_type;
                GValue value = { 0, };
                char *copy_error = NULL;

                variable_type = va_arg (var_args, GType);

                g_value_init (&value, variable_type);

                read_state_variable (variable_name, &value, instance_node);

                G_VALUE_LCOPY (&value, var_args, 0, &copy_error);

                g_value_unset (&value);

                if (copy_error) {
                        g_warning ("Error copying value: %s", copy_error);

                        g_free (copy_error);
                }

                variable_name = va_arg (var_args, const char *);
        }

        /* Cleanup */
        xmlFreeDoc (doc);

        return TRUE;
}

/**
 * gupnp_av_util_parse_last_change
 * @last_change_xml: The xml from the "LastChange" event to parse
 * @instance_id: The ID of instance the caller is interested in
 * @error: The location where to store any error, or NULL
 * @Varargs: tuples of state variable name, state variable type, and state
 * variable value location, terminated with NULL. The state variable values
 * should be freed after use.
 *
 * Parses the xml fragment from a LastChange event.
 *
 * Return value: TRUE on success.
 **/
gboolean
gupnp_av_util_parse_last_change (const char *last_change_xml,
                                 guint       instance_id,
                                 GError    **error,
                                 ...)
{
        va_list var_args;
        gboolean ret;

        va_start (var_args, error);
        ret = gupnp_av_util_parse_last_change_valist (last_change_xml,
                                                      instance_id,
                                                      error,
                                                      var_args);
        va_end (var_args);

        return ret;
}

