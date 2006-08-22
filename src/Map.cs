using System;
using System.Collections.Generic;
using System.Text;
using System.Collections;
using System.Drawing;
using System.Xml;

namespace Ninjeditor
{
    class Map
    {
        private ArrayList layers = null;
        private ArrayList objectDefinitions = null;
        private RGBValue bgColor;
        private Graphics graphics = null;

        private string filename;    // full filename
        private string dirname;     // directory name (from filename)
        private string mapfile;     // map name (from filename)

        private string mapName;

        private uint width, height;

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


        public ArrayList Layers 
        {
            get 
            {
                return layers;
            }
            set 
            {
                layers = value;
            }
        }

        public Graphics TargetGraphics
        {
            get
            {
                return graphics;
            }
            set
            {
                graphics = value;

                if (layers != null)
                {
                    foreach (MapLayer layer in layers)
                    {
                        if (layer != null)
                            layer.TargetGraphics = graphics;
                    }
                }
            }
        }

        public RGBValue BackgroundColor
        {
            get
            {
                return bgColor;
            }
            set
            {
                bgColor = value;
            }
        }

        public Map() {}

        public void Clear()
        {
            if (layers != null)
            {
                foreach (MapLayer layer in layers)
                {
                    if (layer != null)
                        layer.Clear();
                }

                layers.Clear();
                layers = null;
            }

            if (objectDefinitions != null)
            {
                foreach (MapLayer objectDefinition in objectDefinitions)
                {
                    if (objectDefinition != null)
                        objectDefinition.Clear();
                }

                objectDefinitions.Clear();
                objectDefinitions = null;
            }

            bgColor = null;
        }

        public void New()
        {
            Clear();

            layers = new ArrayList();
            layers.Add(new MapLayer("Default Layer"));

            objectDefinitions = new ArrayList();

            bgColor = new RGBValue();
            bgColor.SetColor(0, 0, 0);
            
            // create the first layer
            MapLayer layer = (MapLayer)layers[0];
            layer.Name = "Default";
        }

        public void NewLayer(string name)
        {
            if (name == "")
                name = "New layer";

            layers.Add(new MapLayer(name));
        }

        public void Draw()
        {
            if (layers == null)
                return;

            foreach (MapLayer layer in layers)
                if (layer != null)
                    layer.Draw();
        }

        public void LoadLevel(string full_filename)
        {
            filename = full_filename;
            mapfile = full_filename.Substring(full_filename.LastIndexOf("\\") + 1);
            dirname = full_filename.Substring(0, full_filename.LastIndexOf("\\") + 1);

            XmlDocument xml = new XmlDocument();

            xml.Load(filename);

            XmlNode xGameMode = xml.DocumentElement;
            if (xGameMode.Name != "gameMode")
                throw new Exception("Root element \"gameMode\" not found!");

            LoadGameMode(xGameMode);
        }

        void LoadGameMode(XmlNode xGameMode)
        {
            string modeType = "";

            foreach (XmlAttribute attrib in xGameMode.Attributes)
            {
                if (attrib.Name == "type")
                    modeType = attrib.Value;

                if (attrib.Name == "name")
                    mapName = attrib.Value;
            }

            if (modeType != "simulation")
                throw new Exception("That XML file is NOT a level. (gameMode.type != \"simulation\")");

            LoadProperties(xGameMode.SelectSingleNode("properties").ChildNodes);
            LoadObjectDefinitions(xGameMode.SelectSingleNode("objectDefinitions").SelectNodes("include_xml_file"));
        }

        void LoadProperties(XmlNodeList xProperties)
        {
            foreach (XmlNode xProperty in xProperties)
            {
                switch (xProperty.Name)
                {
                    case "width":
                        width = UInt32.Parse(xProperty.InnerText);
                        break;
                    case "height":
                        height = UInt32.Parse(xProperty.InnerText);
                        break;
                    case "bgcolor":
                        bgColor.LoadFromXml(xProperty);
                        break;
                }
            }
        }

        void LoadObjectDefinitions(XmlNodeList xIncludes)
        {
            // find all the <include_xml_file>'s and include them
            foreach (XmlNode xInclude in xIncludes)
            {
                if (xInclude.Name != "include_xml_file")
                    throw new Exception("Expected <include_xml_file>");

                MapObjectDefinition objectDef = new MapObjectDefinition();
                objectDef.LoadFromFile(dirname + xInclude.InnerText);
            }
        }
    }
}
