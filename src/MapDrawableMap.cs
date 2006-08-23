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
            if (layers == null)
                return;

            foreach (MapLayer layer in layers)
                if (layer != null)
                    layer.Draw(g, scroll_x, scroll_y, screen_w, screen_h);
        }
    }
}
