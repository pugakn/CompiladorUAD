using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
namespace WindowsFormsApplication1
{
    public partial class Form1 : Form
    {
        [DllImport("compilador.dll",  CharSet = CharSet.Ansi, CallingConvention = CallingConvention.Cdecl)]
        private extern static int Compile(string code, byte[] str, int len);
        public Form1()
        {
            InitializeComponent();
        }
        private void compilkarToolStripMenuItem_Click(object sender, EventArgs e)
        {
            //Compilar
            errorList.Clear();
            StringBuilder sb = new StringBuilder(10);
            byte[] buf = new byte[2048];
            Compile(codeText.Text, buf, 2048);
            errorList.Text = (System.Text.Encoding.ASCII.GetString(buf));

        }

        private void openToolStripMenuItem_Click(object sender, EventArgs e)
        {
            //Abrir
            OpenFileDialog openDialog = new OpenFileDialog();
            openDialog.Filter = "Text |*";
            openDialog.Title = "Save";
            openDialog.ShowDialog();
            if (openDialog.FileName != "")
            {
                System.IO.StreamReader fs = new
                   System.IO.StreamReader(openDialog.OpenFile());
                codeText.Text = fs.ReadToEnd();
                fs.Close();
            }
        }

        private void closeToolStripMenuItem_Click(object sender, EventArgs e)
        {
            SaveFileDialog saveFileDialog1 = new SaveFileDialog();
            saveFileDialog1.Filter = "Text |*.txt";
            saveFileDialog1.Title = "Save";
            saveFileDialog1.ShowDialog();

            if (saveFileDialog1.FileName != "")
            {
                System.IO.StreamWriter fs = new
                   System.IO.StreamWriter(saveFileDialog1.OpenFile());
                fs.Write(codeText.Text);
                fs.Close();
            }
        }
    }
}
