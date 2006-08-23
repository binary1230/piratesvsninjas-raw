using System;
using System.Collections;
using System.Text;
using System.Drawing;
using System.Xml;

namespace Ninjeditor
{
    class MapObject : IMapObject
    {
        #region "Private vars"

        // The object definition (e.g. 'red-flower' 'wall3' 'palm-tree' etc)
        private MapObjectDefinition objectDefinition;
        private int m_x, m_y;

        #endregion

        #region "Properties"

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
        }
        public uint Height
        {
            get
            {
                return objectDefinition.Height;
            }
        }
#endregion

        #region "Public Methods"
        public void Clear()
        {
            m_x = m_y = 0;
        }

        public void New()
        {
            Clear();
        }

        // Draw this MapObject in it's current location
        public void Draw(Graphics g, int scroll_x, int scroll_y, int screen_w, int screen_h)
        {
            int x = m_x - scroll_x;
            int y = m_y - scroll_y;

            y = screen_h - y - (int)Height;

            g.DrawImage(objectDefinition.Image, x, y);
        }

        #endregion

        // Load this object from XML
        // xObject - the <object> tag in XML
        // objectDefinitions - a list of loaded object Definitions (NOT TO BE MODIFIED)
        public void LoadFromXML(XmlNode xObject, MapObjectDefinitionList objectDefinitions)
        {
            //<object objectDef="s_sign_arrow">
            //  <position type="fixed">
			//      <x>30</x>
            //      <y>20</y>
            //  </position>
            //</object>

            string objectDefName;

            try
            {
                New();

                objectDefName = xObject.Attributes["objectDef"].Value;
                objectDefinition = objectDefinitions.FindByName(objectDefName);

                if (objectDefinition == null)
                    throw new Exception("Object definition '" + objectDefName + "' not loaded (try including it in the XML).");

                XmlNode xPos = xObject.SelectSingleNode("position");
                if (xPos.Attributes["type"].Value != "fixed")
                    throw new Exception("Error: Map editor only supports <position type=\"fixed\">");

                X = Int32.Parse(xPos.SelectSingleNode("x").InnerText);
                Y = Int32.Parse(xPos.SelectSingleNode("y").InnerText);
            }
            catch (Exception ex)
            {
                throw new Exception("Error loading object.", ex);
            }
        }
    }
}
