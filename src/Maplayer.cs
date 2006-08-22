using System;
using System.Collections.Generic;
using System.Text;
using System.Collections;
using System.Drawing;
using System.Xml;

namespace Ninjeditor
{
    class MapLayer
    {
        private ArrayList mapObjects = null;
        private float scroll_speed = 1.0f;
        private string name = "Unnamed Layer";
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

                if (mapObjects != null)
                {
                    foreach (MapObject mapObject in mapObjects)
                    {
                        if (mapObject != null)
                            mapObject.TargetGraphics = graphics;
                    }
                }
            }
        }

        public string Name
        {
            get
            {
                return name;
            }
            set
            {
                name = value;
            }
        }

        public ArrayList MapObjects 
        {
            get
            {
                return mapObjects;
            }
            set
            {
                mapObjects = value;
            }
        }

        public float ScrollSpeed
        {
            get
            {
                return scroll_speed;
            }
            set
            {
                scroll_speed = value;
            }
        }

        public void Clear()
        {
            if (mapObjects != null)
            {
                foreach (MapObject mapObject in mapObjects)
                    if (mapObject != null)
                        mapObject.Clear();

                mapObjects = null; 
            }
        }

        public void New(string _name)
        {
            if (_name == "")
                name = "New Layer";
            else
                name = _name;

            Clear();
            mapObjects = new ArrayList();
        }

        public void Draw()
        {
            if (mapObjects == null)
                return;

            foreach (MapObject mapObject in mapObjects)
                if (mapObject != null)
                    mapObject.Draw();
        }

        public MapLayer(string _name)
        {
            New(_name);
        }
    }
}
