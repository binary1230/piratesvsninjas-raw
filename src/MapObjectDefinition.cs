using System;
using System.Collections.Generic;
using System.Text;
using System.Drawing;
using System.Xml;

namespace Ninjeditor
{
    class MapObjectDefinition
    {
        private string xml_filename;
        private string name;
        private string image_filename;
        private uint width, height;

        public string XmlFilename
        {
            get
            {
                return xml_filename;
            }
            set
            {
                xml_filename = value;
            }
        }

        public uint Width
        {
            get
            {
                return width;
            }
            set
            {
                width = value;
            }
        }
        public uint Height
        {
            get
            {
                return height;
            }
            set
            {
                height = value;
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

        public string ImageFileName
        {
            get
            {
                return image_filename;
            }
            set
            {
                image_filename = value;
            }
        }

        public void LoadFromXml(XmlNode xObjectDef)
        {

        }

        public void LoadFromFile(string filename)
        {
            XmlDocument xml = new XmlDocument();

            // XXX Left off making sure that this gets to the right path and not data/
            xml.Load(filename);

            XmlNode xObjectDef = xml.DocumentElement;
            if (xObjectDef.Name != "objectDef")
                throw new Exception("Root element \"objectDef\" not found!");

            LoadFromXml(xObjectDef);

            xml_filename = filename;
        }
    }
}
