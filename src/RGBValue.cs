using System;
using System.Collections.Generic;
using System.Text;
using System.Xml;
using System.Drawing;

namespace Ninjeditor
{
    class RGBValue
    {
        private Byte r = 0, g = 0, b = 0;

        public Byte R
        {
            get
            {
                return r;
            }
            set
            {
                r = value;
            }
        }

        public Byte G
        {
            get
            {
                return g;
            }
            set
            {
                g = value;
            }
        }

        public Byte B
        {
            get
            {
                return b;
            }
            set
            {
                b = value;
            }
        }

        public void LoadFromXml(XmlNode xColor) 
        {
            R = Byte.Parse(xColor.SelectSingleNode("r").InnerText);
            G = Byte.Parse(xColor.SelectSingleNode("g").InnerText);
            B = Byte.Parse(xColor.SelectSingleNode("b").InnerText);
        }

        public Color GetSystemColor()
        {
            return Color.FromArgb(R, G, B);
        }

        public void SetColor(Byte r, Byte g, Byte b)
        {
            R = r; G = g; B = b;
        }

        public RGBValue()
        {
            r = g = b = 0;
        }

        public RGBValue(Byte r, Byte g, Byte b)
        {
            SetColor(r, g, b);
        }

        ~RGBValue()
        { 

        }
    }
}
