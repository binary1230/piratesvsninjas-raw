using System;
using System.Collections.Generic;
using System.Text;
using System.Drawing;
using System.Drawing.Drawing2D;
using System.Xml;

namespace Ninjeditor
{
    class MapObjectDefinition
    {
        // Stuff that goes back out to XML
        private string name;            // name of this object definition (e.g. 'sonic' or 'flower3' etc)
        private string type;            // name of this object definition (e.g. 'sonic' or 'flower3' etc)
        private string xml_filename;    // the filename of the XML file (e.g. data/evilguy.xml)

        // Stuff just for the map editor
        private string dirname;         // the root data directory (e.g. c:/ninjas/data/)
        private string image_filename;  // the full filename of the image (e.g. c:/ninjas/data/test.png)
        Bitmap image = null;            // the image data itself

        public string Type
        {
            get
            {
                return type;
            }
        }

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
                return (uint)image.Width;
            }
        }
        public uint Height
        {
            get
            {
                return (uint)image.Height;
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

        public Bitmap Image
        {
            get
            {
                return image;
            }
        }

        public void Clear()
        {
            if (image != null)
                image.Dispose();

            name = "";
            type = "";
            xml_filename = "";

            dirname = "";
            image_filename = "";
            image = null;
        }

        public void LoadFromXml(XmlNode xObjectDef)
        {
            // Sleazy hack time.
            // Really, object definitions have multiple animations associated with them
            // However, we only want the first image, so we'll 'sort of' pick it out of the XML

            name = xObjectDef.Attributes["name"].Value;
            type = xObjectDef.Attributes["type"].Value;

            image_filename = xObjectDef.SelectSingleNode("animations/animation/frames/img").Attributes["name"].Value;
            LoadImage();
        }

        // PRE: image_filename is set
        private void LoadImage()
        {
            string filename = dirname + image_filename;

            try
            {
                image = new Bitmap(filename);
            }
            catch (Exception ex)
            {
                throw new Exception("Can't load image file: " + filename, ex);
            }
        }

        // Load an object definition from another XML file in the specified directory
        public void LoadFromFile(string filename, string _dirname)
        {
            xml_filename = filename;
            dirname = _dirname;

            XmlDocument xml = new XmlDocument();

            // XXX Left off making sure that this gets to the right path and not data/
            xml.Load(dirname + xml_filename);

            XmlNode xObjectDef = xml.DocumentElement;
            if (xObjectDef.Name != "objectDef")
                throw new Exception("Root element \"objectDef\" not found!");

            LoadFromXml(xObjectDef);
        }
    }
}
