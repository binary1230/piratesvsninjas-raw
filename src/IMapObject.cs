using System;
using System.Collections.Generic;
using System.Text;

namespace Ninjeditor
{
    interface IMapObject
    {
        // The object definition (e.g. 'red-flower' 'wall3' 'palm-tree' etc)
        MapObjectDefinition ObjectDefinition{get; set;}

        // Position and width/height of this object
        int X {get; set;}
        int Y {get; set;}
        uint Width { get; set;}
        uint Height { get; set;}
        
        // Draw the object in its current position
        void Draw();
    }
}
