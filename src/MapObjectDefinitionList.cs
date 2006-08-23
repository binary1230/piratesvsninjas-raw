using System;
using System.Collections;
using System.Text;

namespace Ninjeditor
{
    class MapObjectDefinitionList : ArrayList
    {
        // Find an object definition based on it's name
        public MapObjectDefinition FindByName(string objectDefName)
        {
            foreach (MapObjectDefinition objectDef in this)
            {
                if (objectDefName == objectDef.Name)
                {
                    return objectDef;
                }
            }

            return null;
        }
    }
}
