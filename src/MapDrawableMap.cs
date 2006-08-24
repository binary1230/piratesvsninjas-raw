using System;
using System.Collections.Generic;
using System.Text;
using System.Drawing;

namespace Ninjeditor
{
    // A drawable map
    // This class contains all things related to drawing maps on the screen
    class MapDrawableMap : Map
    {
        public void Draw(Graphics g, int scroll_x, int scroll_y, int screen_w, int screen_h)
        {
            g.Clear(bgColor.GetSystemColor());

            if (layers == null)
                return;

            // Draw each layer
            foreach (MapLayer layer in layers)
                if (layer != null)
                    layer.Draw(g, scroll_x, scroll_y, screen_w, screen_h);

            // Draw the selected object
            if (SelectedLayer != null && SelectedLayer.SelectedObject != null)
                SelectedLayer.SelectedObject.DrawRect(g, scroll_x, scroll_y, screen_w, screen_h);
        }

        // Returns the currently selected layer, or NULL if none selected
        public MapLayer SelectedLayer
        {
            get
            {
                if (layers == null)
                    return null;

                foreach (MapLayer layer in layers)
                    if (layer != null && layer.Selected)
                        return layer;

                return null;
            }
            set
            {
                if (SelectedLayer != null)
                    SelectedLayer.Selected = false;

                value.Selected = true;
            }
        }

        // Returns the currently selected object, or NULL if none selected
        public MapObject SelectedObject
        {
            get
            {
                MapLayer layer = SelectedLayer;
                if (layer == null)
                    return null;

                return layer.SelectedObject;
            }
        }

        // Select the object at the current mouse position, or return NULL if none exists
        public bool SelectObjectAtPosition
            (   int mouse_x, int mouse_y, 
                int scroll_x, int scroll_y, 
                int screen_w, int screen_h  )
        {
            MapLayer layer = SelectedLayer;
            if (layer == null)
                return false;
        
            return layer.SelectObjectAtPosition(mouse_x, mouse_y, scroll_x, scroll_y, screen_w, screen_h);
        }
    }
}
