using System;
using System.Collections.Generic;
using System.Text;
using System.Drawing;
using System.Drawing.Drawing2D;

namespace Ninjeditor
{
    interface IMapObject
    {
        // The object definition (e.g. 'red-flower' 'wall3' 'palm-tree' etc)
        MapObjectDefinition ObjectDefinition{get; set;}

        // Position and width/height of this object
        int X {get; set;}
        int Y {get; set;}
        uint Width {get;}
        uint Height {get;}
        
        // Draw the object in its current position
        void Draw(Graphics g, int scroll_x, int scroll_y, int screen_w, int screen_h);
    }
}
