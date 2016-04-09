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
using Windows.UI.Core;
using Windows.ApplicationModel.Activation;
using Windows.UI.ViewManagement;
using Windows.UI;


//“空白页”项模板在 http://go.microsoft.com/fwlink/?LinkId=402352&clcid=0x409 上有介绍

namespace wjq_hw2
{
    /// <summary>
    /// 可用于自身或导航至 Frame 内部的空白页。
    /// </summary>
    public sealed partial class MainPage : Page
    {
        private object coreTitleBar;

        public object SuspensionManager { get; private set; }

        public MainPage()
        {
            this.InitializeComponent();

            /// TODO
            /// 
            /// 标题栏与内容融合， 类似酷狗。 然而要重写return， 留坑， 以后填。
            /*var coreTitleBar = Windows.ApplicationModel.Core.CoreApplication.GetCurrentView().TitleBar;
            coreTitleBar.ExtendViewIntoTitleBar = true;
            var appTitleBar = Windows.UI.ViewManagement.ApplicationView.GetForCurrentView().TitleBar;
            appTitleBar.ButtonBackgroundColor = Colors.Transparent;*/

            var view = ApplicationView.GetForCurrentView();
            view.TitleBar.BackgroundColor = Colors.PaleTurquoise;
            view.TitleBar.ButtonBackgroundColor = Colors.PaleTurquoise;

        }

        private void add_buttonclick(object sender, RoutedEventArgs e)
        {
            Frame.Navigate(typeof(EditPage));
        }
        protected override void OnNavigatedTo(NavigationEventArgs e)
        {

            Frame rootFrame = Window.Current.Content as Frame;
            if (rootFrame.CanGoBack)
            {
                // Show UI in title bar if opted-in and in-app backstack is not empty.
                SystemNavigationManager.GetForCurrentView().AppViewBackButtonVisibility =
                    AppViewBackButtonVisibility.Visible;
            }
            else
            {
                // Remove the UI from the title bar if in-app back stack is empty.
                SystemNavigationManager.GetForCurrentView().AppViewBackButtonVisibility =
                    AppViewBackButtonVisibility.Collapsed;
            }
        }

        private void add_line(object sender, RoutedEventArgs e)
        {
            line.Visibility = Visibility.Visible;
        }

        private void del_line(object sender, RoutedEventArgs e)
        {
            line.Visibility = Visibility.Collapsed;
        }

        private void add_line1(object sender, RoutedEventArgs e)
        {
            line1.Visibility = Visibility.Visible;
        }

        private void del_line1(object sender, RoutedEventArgs e)
        {
            line1.Visibility = Visibility.Collapsed;
        }

    }
}
