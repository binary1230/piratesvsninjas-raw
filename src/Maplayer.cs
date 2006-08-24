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

        protected ArrayList mapObjects = null;
        protected float scroll_speed = 1.0f;
        protected string name = "Unnamed Layer";

        protected bool isSelected = false;

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

        public bool Selected
        {
            get
            {
                return isSelected;
            }
            set
            {
                isSelected = value;
            }
        }

        // Returns the selected MapObject or NULL if nothing is selected
        public MapObject SelectedObject
        {
            get
            {
                if (mapObjects == null)
                    return null;

                foreach (MapObject obj in mapObjects)
                    if (obj != null && obj.Selected)
                        return obj;

                return null;
            }
            set
            {
                // unselect current object
                MapObject selectedObj = (MapObject)SelectedObject;
                if (selectedObj != null)
                    selectedObj.Selected = false;

                // select new one
                value.Selected = true;
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

        public bool SelectObjectAtPosition
            (   int mouse_x, int mouse_y, 
                int scroll_x, int scroll_y, 
                int screen_w, int screen_h)
        {
            int world_x = 0, world_y = 0;

            // Get the world coordinates, store in world_x, world_y
            Map.TransformFromScreenToWorld(
                mouse_x, mouse_y,
                screen_w, screen_h,
                scroll_x, scroll_y,
                ref world_x, ref world_y    );

            // See if this point matches an object
            foreach (MapObject obj in mapObjects)
            {
                if (obj.ContainsPoint(world_x, world_y))
                {
                    SelectedObject = obj;
                    return true;
                }
            }

            return false;
        }
    }
}
