using System.Windows.Forms;

namespace udp_cam
{
    public partial class Form1 : Form
    {
        private UdpReceiver udpReceiver;

        public Form1()
        {
            InitializeComponent();
            udpReceiver = new UdpReceiver(pictureBox1);
        }

        private void Form1_FormClosing(object sender, FormClosingEventArgs e)
        {
            udpReceiver.Stop();
        }
    }
}