using System;
using System.Collections.Generic;
using System.Text;
using System.Drawing;
using System.Xml;

namespace Ninjeditor
{
    class MapObject : IMapObject
    {
        // The object definition (e.g. 'red-flower' 'wall3' 'palm-tree' etc)
        private MapObjectDefinition objectDefinition;
        private int m_x, m_y;

        private Graphics graphics = null;

        public Graphics TargetGraphics
        {
            get
            {
                return graphics;
            }
            set
            {
                graphics = value;
            }
        }

        public MapObjectDefinition ObjectDefinition
        {
            get
            {
                return objectDefinition;
            }
            set
            {
                objectDefinition = value;
            }
        }

        public int X
        {
            get
            {
                return m_x;
            }
            set
            {
                m_x = value;
            }
        }
        public int Y
        {
            get
            {
                return m_y;
            }
            set
            {
                m_y = value;
            }
        }
        public uint Width
        {
            get
            {
                return objectDefinition.Width;
            }
            set
            {
                objectDefinition.Width = value;
            }
        }
        public uint Height
        {
            get
            {
                return objectDefinition.Height;
            }
            set
            {
                objectDefinition.Height = value;
            }
        }

        public void Clear()
        {
            m_x = m_y = 0;
        }

        public void New()
        {
            m_x = 0;
            m_y = 0;
        }

        // Draw this MapObject in it's current location
        public void Draw()
        {
            
        }
    }
}
