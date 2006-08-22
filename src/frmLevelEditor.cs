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
        private Map map;
        
        public frmLevelEditor()
        {
            InitializeComponent();
            map = new Map();
            map.New();
            UpdateLayerList();
        }

        private void frmLevelEditor_Paint(object sender, EventArgs e)
        {
            map.Draw();
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
            map.TargetGraphics = pbxLevelDisplay.CreateGraphics();
            UpdateLayerList();

            // "C:\\Program Files\\Pirates Vs Ninjas\\data\\level1.xml"
        }

        private void exitToolStripMenuItem_Click(object sender, EventArgs e)
        {
            Close();
        }

        private void saveToolStripMenuItem_Click(object sender, EventArgs e)
        {
            New();
        }

        private void pbxLevelDisplay_Paint(object sender, PaintEventArgs e)
        {
            if (map != null)
                map.Draw();
        }

        private void LoadMap(string filename)
        {
            /*try
            {
                map.Open(filename);
            }
            catch (Exception ex)
            {
                statusBar.Text = "Failed to load: " + filename + " : ex.Message;
            }*/
            map.LoadLevel(filename);
        }

        private void openToolStripMenuItem_Click(object sender, EventArgs e)
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
    }
}