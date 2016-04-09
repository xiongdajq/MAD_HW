using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Runtime.InteropServices.WindowsRuntime;
using Windows.Foundation;
using Windows.Foundation.Collections;
using Windows.UI.Xaml;
using Windows.UI.Xaml.Controls;
using Windows.UI.Xaml.Controls.Primitives;
using Windows.UI.Xaml.Data;
using Windows.UI.Xaml.Input;
using Windows.UI.Xaml.Media;
using Windows.UI.Xaml.Navigation;

//“空白页”项模板在 http://go.microsoft.com/fwlink/?LinkId=402352&clcid=0x409 上有介绍

namespace wjq_hw1
{
    /// <summary>
    /// 可用于自身或导航至 Frame 内部的空白页。
    /// </summary>
    public sealed partial class MainPage : Page
    {
        private delegate string animal_speak(object sender, EventArgs e);
        private event animal_speak Speak;
       
        public MainPage()
        {
            this.InitializeComponent();
        }
        interface Animal
        {
            string saying(object sender, EventArgs e);
        }
        class pig : Animal
        {
            TextBlock word;
            public pig (TextBlock a)
            {
                this.word = a;
            }
            public string saying(object sender, EventArgs e)
            {
                this.word.Text += "pig: I am a pig" + "\n";
                return "";
            }
        }
        class dog : Animal
        {
            TextBlock word;
            public dog(TextBlock a)
            {
                this.word = a;
            }
            public string saying(object sender, EventArgs e)
            {
                this.word.Text += "dog: I am a dog" + "\n";
                return "";
            }
        }

        class cat : Animal
        {
            TextBlock word;
            public cat(TextBlock a)
            {
                this.word = a;
            }
            public string saying(object sender, EventArgs e)
            {
                this.word.Text += "cat: I am a cat" + "\n";
                return "";
            }
        }

        private pig p;
        private dog d;
        private cat c;

       
        private void speak_btn_Click(object sender, RoutedEventArgs e)
        {
            Random ran = new Random();
            int n = ran.Next(0, 3);
          
            textBlock.Text = "";
            if (n == 0)
            {
                p = new pig(textBlock);
                Speak += new animal_speak(p.saying);
            } else if (n == 1)
            {
                d = new dog(textBlock);
                Speak += new animal_speak(d.saying);
            } else
            {
                c = new cat(textBlock);
                Speak += new animal_speak(c.saying);
            }
            Speak(this, new EventArgs());
            switch (n) {
                case 0: Speak -= new animal_speak(p.saying); break;
                case 1: Speak -= new animal_speak(d.saying); break;
                case 2: Speak -= new animal_speak(c.saying); break;
            }
        }

        private void sure_btn_Click(object sender, RoutedEventArgs e)
        {
            this.textBlock.Text = "";
            string name = this.textBox.Text;
            switch(name) {
                case "pig": p = new pig(textBlock); Speak += new animal_speak(p.saying); break;
                case "dog": d = new dog(textBlock); Speak += new animal_speak(d.saying); break;
                case "cat": c = new cat(textBlock); Speak += new animal_speak(c.saying); break;
                default: this.textBox.Text = ""; return;
            }
            Speak(this, new EventArgs());
            switch(name) {
                case "pig": Speak -= new animal_speak(p.saying); break;
                case "dog": Speak -= new animal_speak(d.saying); break;
                case "cat": Speak -= new animal_speak(c.saying); break;
                default: return;
            }
            this.textBox.Text = "";
        }
    }
    }
