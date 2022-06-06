/* This file is an image processing operation for GEGL
 *
 * GEGL is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 3 of the License, or (at your option) any later version.
 *
 * GEGL is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with GEGL; if not, see <https://www.gnu.org/licenses/>.
 *
 * Copyright 2006 Øyvind Kolås <pippin@gimp.org>
 * 2022 Beaver GEGL cut out
 */

#include "config.h"
#include <glib/gi18n-lib.h>

#ifdef GEGL_PROPERTIES



property_color (color, _("Color"), "#52ff6f")
    description (_("The color to paint over the input"))


property_color (color2, _("Color 2"), "#f587ff")
    description (_("The color to paint over the input"))


property_double (x, _("X"), -2.5)
  description   (_("Horizontal shadow offset"))
  ui_range      (-16.0, 16.0)
  ui_steps      (1, 10)
  ui_meta       ("unit", "pixel-distance")
  ui_meta       ("axis", "x")

property_double (y, _("Y"), 9.12)
  description   (_("Vertical shadow offset"))
  ui_range      (-16.0, 16.0)
  ui_steps      (1, 10)
  ui_meta       ("unit", "pixel-distance")
  ui_meta       ("axis", "y")

property_double (radius, _("Blur radius"), 4.1)
  value_range   (0.0, G_MAXDOUBLE)
  ui_range      (0.0, 20.0)
  ui_steps      (1, 5)
  ui_gamma      (1.5)
  ui_meta       ("unit", "pixel-distance")

property_double (growradius, _("Grow radius"), -19.0)
  value_range   (-100.0, 100.0)
  ui_range      (-50.0, 50.0)
  ui_digits     (0)
  ui_steps      (1, 5)
  ui_gamma      (1.5)
  ui_meta       ("unit", "pixel-distance")
  description (_("The distance to expand the shadow before blurring; a negative value will contract the shadow instead"))

property_color  (colorshadow, _("Color"), "black")
    /* TRANSLATORS: the string 'black' should not be translated */
  description   (_("The shadow's color (defaults to 'black')"))

/* It does make sense to sometimes have opacities > 1 (see GEGL logo
 * for example)
 */
property_double (opacity, _("Opacity"), 0.5)
  value_range   (0.0, 2.0)
  ui_steps      (0.01, 0.10)





#else

#define GEGL_OP_META
#define GEGL_OP_NAME     cutout
#define GEGL_OP_C_SOURCE cutout.c

#include "gegl-op.h"

static void attach (GeglOperation *operation)
{
  GeglNode *gegl = operation->node;
  GeglNode *input, *output, *erase, *layer, *color, *crop, *ds, *behind, *it, *layer2, *color2, *nop, *nop2;


  input    = gegl_node_get_input_proxy (gegl, "input");
  output   = gegl_node_get_output_proxy (gegl, "output");

  color    = gegl_node_new_child (gegl,
                                  "operation", "gegl:color-overlay",
                                  NULL);

  color2    = gegl_node_new_child (gegl,
                                  "operation", "gegl:color",
                                  NULL);

  behind    = gegl_node_new_child (gegl,
                                  "operation", "gegl:dst-atop",
                                  NULL);


  erase   = gegl_node_new_child (gegl,
                                  "operation", "gegl:xor",
                                  NULL);


  crop  = gegl_node_new_child (gegl,
                                  "operation", "gegl:crop",
                                  NULL);

  ds  = gegl_node_new_child (gegl,
                                  "operation", "gegl:dropshadow",
                                  NULL);

  layer  = gegl_node_new_child (gegl,
                                  "operation", "gegl:layer",
                                  NULL);


  layer2  = gegl_node_new_child (gegl,
                                  "operation", "gegl:layer",
                                  NULL);

  nop  = gegl_node_new_child (gegl,
                                  "operation", "gegl:nop",
                                  NULL);

  nop2  = gegl_node_new_child (gegl,
                                  "operation", "gegl:nop",
                                  NULL);

  it  = gegl_node_new_child (gegl,
                                  "operation", "gegl:invert-transparency",
                                  NULL);







 gegl_node_link_many (input, it, nop, color, layer, crop, ds, behind, output, NULL);

 gegl_node_link_many (behind, color2, layer2, NULL);
gegl_node_connect_from (behind, "aux", color2, "output"); 



  gegl_operation_meta_redirect (operation, "color", color, "value");
  gegl_operation_meta_redirect (operation, "upload", layer, "src");
  gegl_operation_meta_redirect (operation, "upload2", layer2, "src");
  gegl_operation_meta_redirect (operation, "opacity", ds, "opacity");
  gegl_operation_meta_redirect (operation, "x", ds, "x");
  gegl_operation_meta_redirect (operation, "y", ds, "y");
  gegl_operation_meta_redirect (operation, "radius", ds, "radius");
  gegl_operation_meta_redirect (operation, "growradius", ds, "grow-radius");
  gegl_operation_meta_redirect (operation, "colorshadow", ds, "color");
  gegl_operation_meta_redirect (operation, "color2", color2, "value");




}

static void
gegl_op_class_init (GeglOpClass *klass)
{
  GeglOperationClass *operation_class;

  operation_class = GEGL_OPERATION_CLASS (klass);

  operation_class->attach = attach;

  gegl_operation_class_set_keys (operation_class,
    "name",        "gegl:cut-out",
    "title",       _("Cut Out Text Effect"),
    "categories",  "Artistic",
    "reference-hash", "10agg1fkzaz10aavx65421xc254001b2ac",
    "description", _("Cut out text effect using GEGL. "
                     ""),
    NULL);
}

#endif
