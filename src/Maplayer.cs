using System;
using System.Collections.Generic;
using System.Text;
using System.Collections;
using System.Drawing;
using System.Drawing.Drawing2D;
using System.Xml;

namespace Ninjeditor
{
    class MapLayer
    {
        #region "Private vars"

        private ArrayList mapObjects = null;
        private float scroll_speed = 1.0f;
        private string name = "Unnamed Layer";

        #endregion

        #region "Properties"

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
#endregion

        #region "Public Methods"
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

        public void Draw(Graphics g, int scroll_x, int scroll_y, int screen_w, int screen_h)
        {
            if (mapObjects == null)
                return;

            foreach (MapObject mapObject in mapObjects)
                if (mapObject != null)
                    mapObject.Draw(g, scroll_x, scroll_y, screen_w, screen_h);
        }

        public MapLayer(string _name)
        {
            New(_name);
        }

        public MapLayer()
        {
            New("Unnamed Layer");
        }
#endregion

        #region "XML Loading Stuff"

        // Load this layer from XML
        // xLayer - the <layer> tag in XML
        // objectDefinitions - a list of loaded object Definitions (NOT TO BE MODIFIED)
        public void LoadFromXml(XmlNode xLayer, MapObjectDefinitionList objectDefinitions)
        {
            New(xLayer.Attributes["name"].Value);
            scroll_speed = float.Parse(xLayer.Attributes["scroll_speed"].Value);

            XmlNodeList xObjects = xLayer.SelectNodes("object");

            foreach (XmlNode xObject in xObjects)
            {
                MapObject obj = new MapObject();

                // sleazy hack
                if (Map.SleazyObjectLoadingHack(xObject.Attributes["objectDef"].Value))
                    continue;

                obj.LoadFromXML(xObject, objectDefinitions);
                mapObjects.Add(obj);
            }
        }

        #endregion
    }
}
