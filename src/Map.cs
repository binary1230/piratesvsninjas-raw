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
        #region "Private vars"

        protected ArrayList layers = null;
        protected MapObjectDefinitionList objectDefinitions = null;
        protected RGBValue bgColor;

        protected string filename;    // full filename
        protected string dirname;     // directory name (from filename)
        protected string mapfile;     // map name (from filename)

        protected string mapName;

        private uint width, height;

        #endregion

        #region "Properties"

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

        public MapObjectDefinitionList ObjectDefinitions
        {
            get
            {
                return objectDefinitions;
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

        #endregion

        #region "Public Members"

        public Map() 
        {
        }

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
                foreach (MapObjectDefinition objectDefinition in objectDefinitions)
                {
                    if (objectDefinition != null)
                        objectDefinition.Clear();
                }

                objectDefinitions.Clear();
                objectDefinitions = null;
            }

            bgColor = null;
            filename = "";
        }

        public void New()
        {
            Clear();

            layers = new ArrayList();
            layers.Add(new MapLayer("Default Layer"));

            objectDefinitions = new MapObjectDefinitionList();

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

        #endregion

        #region "XML Loading Stuff"

        public void LoadLevel(string full_filename)
        {
            filename = full_filename;
            mapfile = Map.GetFilePart(full_filename);
            dirname = Map.GetDirPart(full_filename);

            XmlDocument xml = new XmlDocument();

            xml.Load(filename);

            XmlNode xGameMode = xml.DocumentElement;
            if (xGameMode.Name != "gameMode")
                throw new Exception("Root element \"gameMode\" not found!");

            LoadGameMode(xGameMode);
        }

        protected void LoadGameMode(XmlNode xGameMode)
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
            LoadLayers(xGameMode.SelectSingleNode("map").SelectNodes("layer"));
        }

        protected void LoadLayers(XmlNodeList xLayers)
        {
            bool foundDefaultLayer = false;

            foreach (XmlNode xLayer in xLayers)
            {
                MapLayer layer = new MapLayer();

                layer.LoadFromXml(xLayer, objectDefinitions);

                if (!foundDefaultLayer)
                {
                    foundDefaultLayer = true;
                    layer.Selected = true;
                }

                layers.Add(layer);
            }
        }

        protected void LoadProperties(XmlNodeList xProperties)
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

        protected void LoadObjectDefinitions(XmlNodeList xIncludes)
        {
            string objectDefFilename;

            // find all the <include_xml_file>'s and include them
            foreach (XmlNode xInclude in xIncludes)
            {
                if (xInclude.Name != "include_xml_file")
                    throw new Exception("Expected <include_xml_file>");

                objectDefFilename = xInclude.InnerText;

                // This will skip certain kinds of objects we can't deal with now.
                if (SleazyObjectLoadingHack(objectDefFilename))
                    continue;

                MapObjectDefinition objectDef = new MapObjectDefinition();
                objectDef.LoadFromFile(objectDefFilename, dirname);
                objectDefinitions.Add(objectDef);
            }
        }
        #endregion

        #region "Static Helpers"

        // Transform coordinates from Screen coordinates (e.g. mouse clicking on the picturebox)
        // To world coordinates (e.g. the XY of an object IN THE WORLD)
        // The transformed coordinates are stored in out_x and out_y
        static public void TransformFromWorldToScreen
            (   int world_x, int world_y, 
                int object_w, int object_h,
                int scroll_x, int scroll_y, 
                int screen_w, int screen_h,
                ref int out_x, ref int out_y)
        {
            out_x = world_x - scroll_x;
            out_y = screen_h - (world_y - scroll_y) - (int)object_h;
        }

        static public void TransformFromWorldToScreen
            (   MapObject obj, Graphics g,
                int scroll_x, int scroll_y,
                int screen_w, int screen_h,
                ref int out_x, ref int out_y)
        {
            TransformFromWorldToScreen(
                obj.X, obj.Y, (int)obj.Width, (int)obj.Height, 
                scroll_x, scroll_y, screen_w, screen_h,
                ref out_x, ref out_y);
        }

        // Transform screen coordinates (e.g. mouse clicks) to world coordinates (e.g. XY in the world)
        static public void TransformFromScreenToWorld
            (   int screen_x, int screen_y,
                int screen_w, int screen_h,
                int scroll_x, int scroll_y,
                ref int out_x, ref int out_y) 
        {
            out_x = screen_x + scroll_x;
            out_y = screen_h + scroll_y - screen_y;
        }

        // Gets the directory name from a full pathname
        static public string GetDirPart(string full_filename)
        {
            return full_filename.Substring(0, full_filename.LastIndexOf("\\") + 1);
        }

        // Gets just the filename from a full pathname
        static public string GetFilePart(string full_filename)
        {
            return full_filename.Substring(full_filename.LastIndexOf("\\") + 1);
        }

        #region "Don't look - it's a Seriously Sleazy Hack"

        // XXX  SERIOUSLY SLEAZY HACK TIME XXX XXX XXX XXX XXX XXX XXX XXX
        // This map editor cannot currently handle certain types of objects
        // If we get them, we will just ignore them. [sigh]
        // XXX XXX XXX XXX XXX XXX XXX XXX XXX XXX XXX XXX XXX XXX XXX XXX
        static public bool SleazyObjectLoadingHack(string name)
        {
            string o = name.ToLower();
            if (o.Contains("controller") || o.Contains("opener"))
                return true;

            return false;
        }
        // XXX END OF SERIOUSLY SLEAZY HACK XXX
        #endregion

        #endregion
    }
}
