using System.Net;
using System.Net.Sockets;

public class UdpReceiver
{
    private UdpClient udpClient;
    private Thread receiveThread;
    private PictureBox pictureBox;
    private bool isRunning;

    public UdpReceiver(PictureBox pictureBox)
    {
        this.pictureBox = pictureBox;
        isRunning = true;

        // UDPクライアントを作成し、ポート番号を指定
        udpClient = new UdpClient(1024);

        // 受信用スレッドを開始
        receiveThread = new Thread(ReceiveData);
        receiveThread.Start();
    }

    private void ReceiveData()
    {
        while (isRunning)
        {
            try
            {
                // データを受信
                IPEndPoint remoteEP = null;
                byte[] data = udpClient.Receive(ref remoteEP);

                // 受信したデータをPictureBoxに表示するために、別スレッドでInvokeメソッドを使用する
                pictureBox.Invoke(new Action(() =>
                {
                    // バイト配列をビットマップに変換
                    Bitmap bmp = new Bitmap(320, 240);
                    int index = 0;
                    for (int y = 0; y < 240; y++)
                    {
                        for (int x = 0; x < 320; x++)
                        {
                            // RGB565のバイナリデータを解析して、カラー情報を取得
                            int color = data[index] << 8 | data[index + 1];
                            int r = (color >> 11) & 0x1f;
                            int g = (color >> 5) & 0x3f;
                            int b = color & 0x1f;
                            r = (r * 255 / 31);
                            g = (g * 255 / 63);
                            b = (b * 255 / 31);

                            // ピクセルにカラーをセット
                            bmp.SetPixel(x, y, Color.FromArgb(r, g, b));

                            index += 2;
                        }
                    }

                    // PictureBoxにビットマップを表示
                    pictureBox.Image = bmp;
                }));
            }
            catch (Exception e)
            {
                Console.WriteLine(e.ToString());
            }
        }
        udpClient.Close();
    }

    public void Stop()
    {
        isRunning = false;
    }
}
