using System;
using System.Collections;
using System.Text;
using System.Drawing;
using System.Drawing.Imaging;
using System.Xml;

namespace Ninjeditor
{
    class MapObject : IMapObject
    {
        #region "Private vars"

        // The object definition (e.g. 'red-flower' 'wall3' 'palm-tree' etc)
        protected MapObjectDefinition objectDefinition = null;

        protected int m_x = 0, m_y = 0;
        protected bool isSelected = false;

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

        #endregion

        #region "Public Methods"

        public void Clear()
        {
            m_x = m_y = 0;
            isSelected = false;
            objectDefinition = null;
        }

        public void New()
        {
            Clear();
        }

        // Returns true if this object contains the specified point
        public bool ContainsPoint(int x, int y)
        {
            return (m_x < x && x < m_x + Width &&
                    m_y < y && y < m_y + Height);
        }

        public void DrawRect(Graphics g, int scroll_x, int scroll_y, int screen_w, int screen_h)
        {
            // Will hold the final transformed coordinates
            int transformed_x = 0, transformed_y = 0;

            // Compute the screen coordinates from the world coordinates
            Map.TransformFromWorldToScreen
                (   this, g,
                    scroll_x, scroll_y,
                    screen_w, screen_h,
                    ref transformed_x, ref transformed_y);

            Pen pen = new Pen(Color.White);

            g.DrawRectangle(pen, new Rectangle(transformed_x, transformed_y, (int)Width, (int)Height));
        }

        // Draw this MapObject in it's current location
        public void Draw(Graphics g, int scroll_x, int scroll_y, int screen_w, int screen_h)
        {
            // Will hold the final transformed coordinates
            int transformed_x = 0, transformed_y = 0;

            // Compute the screen coordinates from the world coordinates
            Map.TransformFromWorldToScreen
                (   this, g, 
                    scroll_x, scroll_y, 
                    screen_w, screen_h,
                    ref transformed_x, ref transformed_y);

            // Setup color key to make pink (0xff00ff) a transparent color
            ImageAttributes ia = new ImageAttributes();
            Bitmap image = objectDefinition.Image;
            Color c = Color.FromArgb(255, 255, 0, 255);
            ia.SetColorKey(c, c);

            // Draw the image
            g.DrawImage(    image, new Rectangle(transformed_x, transformed_y, 
                            (int)Width, (int)Height), 0, 0, 
                            Width, Height, GraphicsUnit.Pixel, ia               );
        }

        #endregion

        // Load this object from XML
        // xObject - the <object> tag in XML
        // objectDefinitions - a list of loaded object Definitions (NOT TO BE MODIFIED)
        public void LoadFromXML(XmlNode xObject, MapObjectDefinitionList objectDefinitions)
        {
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

                // handle some special cases
                if (xPos.SelectSingleNode("alignTop") != null)
                    Y = Y - (int)Height;

                if (xPos.SelectSingleNode("alignRight") != null)
                    X = X - (int)Width;
            }
            catch (Exception ex)
            {
                throw new Exception("Error loading object.", ex);
            }
        }
    }
}
