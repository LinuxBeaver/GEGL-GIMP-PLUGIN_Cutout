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
 * Credit to Øyvind Kolås (pippin) for major GEGL contributions
 * 2022 Beaver GEGL cut out
 */

/*
Rough recreation of GEGL Graph. Requires Invert Transparency

invert-transparency value=#00ff22 id=image1 #src-atop aux=[ ref=image1  layer src= ]
id=2 dst-over aux=[ ref=2 color value=#ff000b crop #layer src  ]
 */

#include "config.h"
#include <glib/gi18n-lib.h>

#ifdef GEGL_PROPERTIES



property_color (color, _("Color"), "#52ff6f")
    description (_("The color to paint over the input"))


property_color (color2, _("Color 2"), "#f587ff")
    description (_("The color to paint over the input"))


property_double (x, _("Shadow X"), -2.5)
  description   (_("Horizontal shadow offset"))
  ui_range      (-16.0, 16.0)
  ui_steps      (1, 10)
  ui_meta       ("unit", "pixel-distance")
  ui_meta       ("axis", "x")

property_double (y, _("Shadow  Y"), 9.12)
  description   (_("Vertical shadow offset"))
  ui_range      (-16.0, 16.0)
  ui_steps      (1, 10)
  ui_meta       ("unit", "pixel-distance")
  ui_meta       ("axis", "y")

property_double (radius, _("Shadow Blur radius"), 5.0)
  value_range   (0.0, G_MAXDOUBLE)
  ui_range      (0.0, 20.0)
  ui_steps      (1, 5)
  ui_gamma      (1.5)
  ui_meta       ("unit", "pixel-distance")

property_double (growradius, _("Shadow Grow radius"), 9.0)
  value_range   (0.0, 100.0)
  ui_range      (0.0, 50.0)
  ui_digits     (0)
  ui_steps      (1, 5)
  ui_gamma      (1.5)
  ui_meta       ("unit", "pixel-distance")
  description (_("The distance to expand the shadow before blurring; a negative value will contract the shadow instead"))

property_color  (colorshadow, _("Shadow Color"), "black")
    /* TRANSLATORS: the string 'black' should not be translated */
  description   (_("The shadow's color (defaults to 'black')"))

/* It does make sense to sometimes have opacities > 1 (see GEGL logo
 * for example)
 */
property_double (opacity, _("Shadow Opacity"), 0.5)
  value_range   (0.0, 1.0)
  ui_steps      (0.1, 0.10)


property_file_path(upload, _("Top image Overlay"), "")
    description (_("Source image file path (png, jpg, raw, svg, bmp, tif, ...)"))

property_file_path(upload2, _("Bottom image overlay"), "")
    description (_("Source image file path (png, jpg, raw, svg, bmp, tif, ...)"))

property_double (black_level, _("Black level on bottom image layer"), 0.0)
    description (_("Adjust the black level"))
    ui_range (-0.1, 0.1)

property_double (exposure, _("Exposure on bottom image layer"), 0.0)
    description (_("Relative brightness change in stops"))
    ui_range    (-10.0, 10.0)

property_double (hue, _("Color Rotation on bottom image layer"),  0.0)
   description  (_("Hue adjustment"))
   value_range  (-180.0, 180.0)




#else

#define GEGL_OP_META
#define GEGL_OP_NAME     cutout
#define GEGL_OP_C_SOURCE cutout.c

#include "gegl-op.h"

static void attach (GeglOperation *operation)
{
  GeglNode *gegl = operation->node;
  GeglNode *input, *output, *layer, *ontop, *nop, *color, *crop, *ds, *behind, *exposure, *hue, *behind2, *it, *layer2, *color2;


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

  behind2    = gegl_node_new_child (gegl,
                                  "operation", "gegl:dst-atop",
                                  NULL);

  ontop   = gegl_node_new_child (gegl,
                                  "operation", "gegl:src-atop",
                                  NULL);

   hue = gegl_node_new_child (gegl,
                                  "operation", "gegl:hue-chroma",
                                  NULL);

  nop  = gegl_node_new_child (gegl,
                                  "operation", "gegl:nop",
                                  NULL);

  crop  = gegl_node_new_child (gegl,
                                  "operation", "gegl:crop",
                                  NULL);

  ds  = gegl_node_new_child (gegl,
                                  "operation", "gegl:dropshadow",
                                  NULL);

  layer  = gegl_node_new_child (gegl,
                                  "operation", "port:load",
                                  NULL);


  layer2  = gegl_node_new_child (gegl,
                                  "operation", "port:load",
                                  NULL);

  it  = gegl_node_new_child (gegl,
                                  "operation", "lb:invert-transparency",
                                  NULL);

  exposure  = gegl_node_new_child (gegl,
                                  "operation", "gegl:exposure",
                                  NULL);

gegl_node_link_many (input, it, color, nop, crop, ontop, ds, behind2, behind, output, NULL);
gegl_node_link_many (layer2, exposure, hue, NULL);
gegl_node_connect (ontop, "aux", layer, "output"); 
gegl_node_connect (behind, "aux", color2, "output"); 
gegl_node_connect (behind2, "aux", hue, "output"); 
gegl_node_connect (crop, "aux", nop, "output"); 

/*
invert-transparency value=#00ff22 id=image1 #src-atop aux=[ ref=image1  layer src= ]
id=2 dst-over aux=[ ref=2 color value=#ff000b crop #layer src  ]
*/


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
  gegl_operation_meta_redirect (operation, "black_level", exposure, "black-level");
  gegl_operation_meta_redirect (operation, "exposure", exposure, "exposure");
  gegl_operation_meta_redirect (operation, "hue", hue, "hue");

}

static void
gegl_op_class_init (GeglOpClass *klass)
{
  GeglOperationClass *operation_class;

  operation_class = GEGL_OPERATION_CLASS (klass);

  operation_class->attach = attach;

  gegl_operation_class_set_keys (operation_class,
    "name",        "lb:cut-out",
    "title",       _("Cut Out Text Effect"),
    "categories",  "Artistic",
    "reference-hash", "10agg1fkzaz10aavx65421xc254001b2ac",
    "description", _("Cut out text effect using GEGL. "
                     ""),
    "gimp:menu-path", "<Image>/Filters/Text Styling",
    "gimp:menu-label", _("Cut Out Text Effect..."),
    NULL);
}

#endif
