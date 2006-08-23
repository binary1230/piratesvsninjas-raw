using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using System.Xml;

namespace Ninjeditor
{
    public partial class frmLevelEditor : Form
    {
        private MapDrawableMap map;
        
        public frmLevelEditor()
        {
            InitializeComponent();
            map = new MapDrawableMap();
            map.New();
            UpdateLayerList();
        }

        private void menuBtnNew_Click(object sender, EventArgs e)
        {
            New();
        }

        private void UpdateLayerList()
        {
            lbLayers.Items.Clear();
            foreach (MapLayer layer in map.Layers)
            {
                lbLayers.Items.Add(layer.Name);
            }
        }

        private void New()
        {
            map.New();
            UpdateLayerList();
            Draw(pbxLevelDisplay.CreateGraphics());
        }

        private void exitToolStripMenuItem_Click(object sender, EventArgs e)
        {
            Close();
        }

        private void saveToolStripMenuItem_Click(object sender, EventArgs e)
        {
            New();
        }

        private void LoadMap(string filename)
        {
            try
            {
                map.Layers.Clear();
                map.LoadLevel(filename);
            }
            catch (Exception ex)
            {
                New();
                statusBar.Text = "Failed to load: " + filename + " : " + ex.Message;
            }

            UpdateLayerList();
            UpdateObjectDefinitionList();
            UpdateLevelSize();

            hscrollMap.Value = 0;
            vscrollMap.Value = vscrollMap.Maximum;

            Draw(pbxLevelDisplay.CreateGraphics());

            statusBar.Text = "Loaded map: " + filename;
        }

        private void UpdateLevelSize()
        {
            hscrollMap.Minimum = 0;
            hscrollMap.Maximum = (int)map.Width;
            
            vscrollMap.Minimum = 0;
            vscrollMap.Maximum = (int)map.Height;
        }

        private void UpdateObjectDefinitionList()
        {
            lbObjects.Items.Clear();
            if (map.ObjectDefinitions != null) {
                foreach (MapObjectDefinition def in map.ObjectDefinitions)
                {
                    lbObjects.Items.Add(def.Name);
                }
            }
        }

        private void Draw(Graphics g)
        {
            Color c = new Color();
            g.Clear(Color.Black);
            map.Draw(g, hscrollMap.Value, vscrollMap.Maximum - vscrollMap.Value, pbxLevelDisplay.Width, pbxLevelDisplay.Height);
        }

        private void OnPaint(object sender, PaintEventArgs e)
        {
            Draw(e.Graphics);
        }

        private void DoOpen()
        {
            OpenFileDialog openDialog = new OpenFileDialog();

            openDialog.DefaultExt = "xml";
            openDialog.Filter = "XML Level Files (*.xml)|*.xml";

            // openDialog.InitialDirectory = "C:\\Program Files\\Pirates Vs Ninjas\\Data\\";
            openDialog.InitialDirectory = "C:\\dom\\ninjas\\data\\";

            DialogResult result = (DialogResult)openDialog.ShowDialog(this);
            if (result == DialogResult.OK)
            {
                LoadMap(openDialog.FileName);
            }

            openDialog.Dispose();
        }

        private void btn_open_Click(object sender, EventArgs e)
        {
            DoOpen();
        }

        private void openToolStripMenuItem_Click(object sender, EventArgs e)
        {
            DoOpen();
        }

        private void hscrollMap_Scroll(object sender, ScrollEventArgs e)
        {
            Draw(pbxLevelDisplay.CreateGraphics());
        }

        private void vscrollMap_Scroll(object sender, ScrollEventArgs e)
        {
            Draw(pbxLevelDisplay.CreateGraphics());
        }
    }
}