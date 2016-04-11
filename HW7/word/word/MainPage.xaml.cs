using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Net;
using System.Runtime.InteropServices.WindowsRuntime;
using System.Text;
using System.Xml;
using Windows.Foundation;
using Windows.Foundation.Collections;
using Windows.UI.Xaml;
using Windows.UI.Xaml.Controls;
using Windows.UI.Xaml.Controls.Primitives;
using Windows.UI.Xaml.Data;
using Windows.UI.Xaml.Input;
using Windows.UI.Xaml.Media;
using Windows.UI.Xaml.Navigation;

// The Blank Page item template is documented at http://go.microsoft.com/fwlink/?LinkId=402352&clcid=0x409

namespace word
{
    /// <summary>
    /// An empty page that can be used on its own or navigated to within a Frame.
    /// </summary>
    public sealed partial class MainPage : Page
    {
        public MainPage()
        {
            this.InitializeComponent();
        }

        private async void GetPinyin()
        {
            string GetP = "http://apis.baidu.com/sillystudio/service/topy";
            string param = "words=" + word.Text + "&accent=1&traditional=0&letter=0&oc=0&type=xml";
            string url = GetP + "?" + param;
            HttpWebRequest request = (HttpWebRequest)WebRequest.Create(url);
            request.Method = "GET";
            request.Headers["apikey"] = "4c86c76e669262c052d04fd6828e1525";
            WebResponse response = await request.GetResponseAsync();

            Stream s = response.GetResponseStream();
            //XmlReader.Create(s);
            //XmlDocument doc = XmlDocument.Load(s);
            StreamReader reader = new StreamReader(s, Encoding.UTF8);
            string StrDate = "";
            string strValue = "";
            while ((StrDate = reader.ReadLine()) != null) //读取数据流
            {
                strValue += StrDate;
            }
            XmlDocument doc = new XmlDocument();
            doc.LoadXml(strValue);
            XmlNodeList elem = doc.GetElementsByTagName("py");
            pinyin.Text = elem[0].InnerText;

            //XmDocumnt.Parse(strValue);
        }
        private void button_Click(object sender, RoutedEventArgs e)
        {
            pinyin.Text = "";
            GetPinyin();
        }
    }
}
