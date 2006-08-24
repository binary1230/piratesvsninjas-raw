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
                int i = lbLayers.Items.Add(layer.Name);
                if (layer.Selected)
                    lbLayers.SelectedIndex = i;
            }
        }

        private void New()
        {
            map.New();
            UpdateLayerList();
            UpdateObjectDefinitionList();
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

        private void SetStatusBarText(string text)
        {
            statusBar.Text = text;
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
                SetStatusBarText("Failed to load: " + filename + " : " + ex.Message);
            }

            UpdateLayerList();
            UpdateObjectDefinitionList();
            UpdateLevelSize();

            hscrollMap.Value = 0;
            vscrollMap.Value = vscrollMap.Maximum;

            Draw(pbxLevelDisplay.CreateGraphics());

            SetStatusBarText("Loaded map: " + filename);
        }

        private void UpdateLevelSize()
        {
            int xmax = (int)map.Width - pbxLevelDisplay.Width;
            int ymax = (int)map.Height - pbxLevelDisplay.Height;

            hscrollMap.Minimum = 0;
            vscrollMap.Minimum = 0;

            if (xmax > 0)
                hscrollMap.Maximum = xmax;
            else
                hscrollMap.Enabled = false;

            if (ymax > 0)
                vscrollMap.Maximum = ymax;
            else
                vscrollMap.Enabled = false;
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

        // The real drawing function
        private void Draw(Graphics g)
        {
            map.Draw(g, GetScrollX(), GetScrollY(), pbxLevelDisplay.Width, pbxLevelDisplay.Height);
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

        private bool right_mouse_down = false;
        private int mouse_down_x = 0;
        private int mouse_down_y = 0;
        private int mouse_scroll_x = 0;
        private int mouse_scroll_y = 0;

        new private void MouseDown(object sender, MouseEventArgs e)
        {
            if (e.Button == MouseButtons.Right)
            {
                right_mouse_down = true;
                mouse_down_x = e.X;
                mouse_down_y = e.Y;
                mouse_scroll_x = hscrollMap.Value;
                mouse_scroll_y = vscrollMap.Value;
            } else if (e.Button == MouseButtons.Left) {
                bool selectedNewObject = SelectObjectAtPosition(e.X, e.Y);

                if (selectedNewObject)
                    Draw(pbxLevelDisplay.CreateGraphics());
            }
        }

        private int GetScrollX()
        {
            return hscrollMap.Value;
        }

        private int GetScrollY()
        {
            return vscrollMap.Maximum - vscrollMap.Value;
        }

        // returns true if we selected a new object
        private bool SelectObjectAtPosition(int x, int y)
        {
            return map.SelectObjectAtPosition(x, y, GetScrollX(), GetScrollY(), 
                                              pbxLevelDisplay.Width, pbxLevelDisplay.Height   );
        }

        new private void MouseUp(object sender, MouseEventArgs e)
        {
            if (e.Button == MouseButtons.Right)
            {
                right_mouse_down = false;
            }
        }

        new private void MouseMove(object sender, MouseEventArgs e)
        {
            const int mouse_scroll_speed = 5;

            if (right_mouse_down == true)
            {
                SafeSetScrollX(mouse_scroll_x + ((e.X - mouse_down_x)*mouse_scroll_speed));
                SafeSetScrollY(mouse_scroll_y + ((e.Y - mouse_down_y)*mouse_scroll_speed));
                Draw(pbxLevelDisplay.CreateGraphics());
            }
        }

        private void SafeSetScrollX(int val)
        {
            hscrollMap.Value = Math.Max(Math.Min(val, hscrollMap.Maximum), 0);
        }

        private void SafeSetScrollY(int val)
        {
            vscrollMap.Value = Math.Max(Math.Min(val, vscrollMap.Maximum), 0);
        }

        private void OnPbxSizeChange(object sender, EventArgs e)
        {
            UpdateLevelSize();
        }

        private void OnLayerChanged(object sender, EventArgs e)
        {
            map.SelectedLayer = (MapLayer)map.Layers[lbLayers.SelectedIndex];
            Draw(pbxLevelDisplay.CreateGraphics());
        }
    }
}