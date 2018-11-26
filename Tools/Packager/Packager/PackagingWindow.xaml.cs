using System;
using System.Windows;
using System.Collections.Generic;
using System.Windows.Forms;
using System.IO;
using System.Text.RegularExpressions;
using System.Drawing;
using System.Drawing.Imaging;
using System.Windows.Media.Imaging;
using System.Linq;
using System.Windows.Interop;
using Microsoft.VisualBasic.FileIO;
using MahApps.Metro.Controls;
using Brew;
using Brew.Pack;

namespace bjs.Packager
{
    public enum LogMode
    {
        Information,
        Error,
    }

    public partial class PackagingWindow : MetroWindow
    {
        public readonly string logopath = Brew.Pack.Utils.TemporaryDirectory + "\\logo.jpg";
        public static readonly string Program = "Brew.js packager";

        public PackagingWindow()
        {
            InitializeComponent();
            Brew.Pack.Utils.InitializeTemporaryDirectory();
            if (File.Exists(logopath)) File.Delete(logopath);
            Properties.Resources.BaseIcon_nx.Save(logopath, ImageFormat.Jpeg);
            if (File.Exists(Brew.Pack.Utils.TemporaryDirectory + "\\bin\\hacbrewpack.exe")) File.Delete(Brew.Pack.Utils.TemporaryDirectory + "\\bin\\hacbrewpack.exe");
            if (File.Exists(Brew.Pack.Utils.TemporaryDirectory + "\\bin\\Pack.exe")) File.Delete(Brew.Pack.Utils.TemporaryDirectory + "\\bin\\Pack.exe");
            File.WriteAllBytes(Brew.Pack.Utils.TemporaryDirectory + "\\bin\\hacbrewpack.exe", Properties.Resources.hacbrewpack);
            Image_Icon.Source = Imaging.CreateBitmapSourceFromHBitmap(Properties.Resources.BaseIcon_nx.GetHbitmap(), IntPtr.Zero, Int32Rect.Empty, BitmapSizeOptions.FromEmptyOptions());
        }

        public static byte[] StringToByteArray(string hex)
        {
            int NumberChars = hex.Length;
            byte[] bytes = new byte[NumberChars / 2];
            for(int i = 0; i < NumberChars; i += 2) bytes[i / 2] = Convert.ToByte(hex.Substring(i, 2), 16);
            return bytes;
        }

        public static byte[] ReverseApplicationId(string ApplicationId)
        {
            byte[] appid = Enumerable.Range(0, ApplicationId.Length).Where(x => x % 2 == 0).Select(x => Convert.ToByte(ApplicationId.Substring(x, 2), 16)).ToArray();
            Array.Reverse(appid);
            return appid;
        }

        protected override void OnClosing(System.ComponentModel.CancelEventArgs e)
        {
            Brew.Pack.Utils.FinalizeTemporaryDirectory();
            base.OnClosing(e);
        }

        public static void Log(string Message, LogMode Mode = LogMode.Error)
        {
            System.Windows.MessageBox.Show(Message, Program + " - " + Mode.ToString(), MessageBoxButton.OK, ((Mode == LogMode.Error) ? MessageBoxImage.Error : MessageBoxImage.Information));
        }

        private void Button_SourceBrowse_Click(object sender, RoutedEventArgs e)
        {
            FolderBrowserDialog src = new FolderBrowserDialog()
            {
                Description = "Select JavaScript sources and assets directory",
                ShowNewFolderButton = true,
            };
            DialogResult res = src.ShowDialog();
            if (res == System.Windows.Forms.DialogResult.OK)
            {
                if (!File.Exists(src.SelectedPath + "\\index.js"))
                {
                    Log("Directory does not contain a index.js file. It is required, as will be the entrypoint of the title.");
                    return;
                }
                Box_SourcePath.Text = src.SelectedPath;
            }
        }

        private void Button_IconBrowse_Click(object sender, RoutedEventArgs e)
        {
            OpenFileDialog icon = new OpenFileDialog()
            {
                Title = "Select icon for the NSP package",
                Filter = "Common image types (*.bmp, *.png, *.jpg, *.jpeg, *.dds, *.tga)|*.bmp;*.png;*.jpg;*.jpeg;*.dds;*.tga",
                InitialDirectory = Environment.GetFolderPath(Environment.SpecialFolder.Desktop),
            };
            DialogResult res = icon.ShowDialog();
            if (res is System.Windows.Forms.DialogResult.OK)
            {
                Bitmap logo = new Bitmap(icon.FileName);
                if (File.Exists(logopath)) File.Delete(logopath);
                logo.Save(logopath, ImageFormat.Jpeg);
                Image_Icon.Source = Imaging.CreateBitmapSourceFromHBitmap(logo.GetHbitmap(), IntPtr.Zero, Int32Rect.Empty, BitmapSizeOptions.FromEmptyOptions());
            }
        }

        private void Button_KeySetBrowse_Click(object sender, RoutedEventArgs e)
        {
            OpenFileDialog kset = new OpenFileDialog()
            {
                Title = "Select keyset file",
                InitialDirectory = Environment.GetFolderPath(Environment.SpecialFolder.Desktop),
            };
            DialogResult res = kset.ShowDialog();
            if(res is System.Windows.Forms.DialogResult.OK) Box_KeySet.Text = kset.FileName;
        }

        private void Button_BuildNSP_Click(object sender, RoutedEventArgs e)
        {
            string appid = Box_ApplicationId.Text;
            if(string.IsNullOrEmpty(appid))
            {
                Log("No application Id was set.");
                return;
            }
            else if(appid.Length != 16)
            {
                Log("Application Id is not 16 characters long.");
                return;
            }
            else if(!Regex.IsMatch(appid, @"\A\b[0-9a-fA-F]+\b\Z"))
            {
                Log("Application Id is not a valid hexadecimal string.");
                return;
            }
            byte keygen = 5;
            string kgen = Combo_KeyGen.Text;
            if(kgen is "1 (1.0.0 - 2.3.0)") keygen = 1;
            else if(kgen is "2 (3.0.0)") keygen = 2;
            else if(kgen is "3 (3.0.1 - 3.0.2)") keygen = 3;
            else if(kgen is "4 (4.0.0 - 4.1.0)") keygen = 4;
            else if(kgen is "5 (5.0.0 - 5.1.0)") keygen = 5;
            else if(kgen is "6 (6.0.0 - Latest)") keygen = 6;
            if(string.IsNullOrEmpty(Box_Name.Text))
            {
                Log("No application name was set.");
                return;
            }
            if(string.IsNullOrEmpty(Box_Version.Text))
            {
                Log("No application version string was set.");
                return;
            }
            if (string.IsNullOrEmpty(Box_ProductCode.Text))
            {
                Log("No product code was set.");
                return;
            }
            bool screen = (Check_AllowScreenshots.IsChecked.HasValue) ? false : Check_AllowScreenshots.IsChecked.Value;
            bool video = (Check_AllowVideo.IsChecked.HasValue) ? false : Check_AllowVideo.IsChecked.Value;
            byte user = Convert.ToByte((Check_UserAccount.IsChecked.HasValue) ? false : Check_UserAccount.IsChecked.Value);
            byte[] bappid = StringToByteArray(appid);
            Array.Reverse(bappid, 0, bappid.Length);
            byte[] npdm = Properties.Resources.main;
            int aci0offset = 0;
            for(int i = 0; i < npdm.Length; i++)
            {
                char c1 = Convert.ToChar(npdm[i]);
                char c2 = Convert.ToChar(npdm[i + 1]);
                char c3 = Convert.ToChar(npdm[i + 2]);
                char c4 = Convert.ToChar(npdm[i + 3]);
                string str = new string(new char[]{ c1, c2, c3, c4 });
                if(str == "ACI0")
                {
                    aci0offset = i;
                    break;
                }
            }
            int appidoffset = aci0offset + 0x10;
            byte[] rappid = ReverseApplicationId(appid);
            foreach(byte rb in rappid)
            {
                npdm[appidoffset] = rb;
                appidoffset++;
            }
            if(Directory.Exists(Brew.Pack.Utils.TemporaryDirectory + "\\gen")) Directory.Delete(Brew.Pack.Utils.TemporaryDirectory + "\\gen", true);
            Directory.CreateDirectory(Brew.Pack.Utils.TemporaryDirectory + "\\gen");
            Directory.CreateDirectory(Brew.Pack.Utils.TemporaryDirectory + "\\gen\\exefs");
            Directory.CreateDirectory(Brew.Pack.Utils.TemporaryDirectory + "\\gen\\romfs");
            File.WriteAllBytes(Brew.Pack.Utils.TemporaryDirectory + "\\gen\\exefs\\main", Properties.Resources.Packager);
            File.WriteAllBytes(Brew.Pack.Utils.TemporaryDirectory + "\\gen\\exefs\\main.npdm", npdm);
            FileSystem.CopyDirectory(Box_SourcePath.Text, Brew.Pack.Utils.TemporaryDirectory + "\\gen\\romfs");
            Directory.CreateDirectory(Brew.Pack.Utils.TemporaryDirectory + "\\gen\\out");
            NACP n = new NACP();
            LanguageEntry en = new LanguageEntry();
            en.Name = Box_Name.Text;
            en.Author = Box_Author.Text;
            n.Version = Box_Version.Text;
            n.ApplicationId = appid;
            n.Screenshot = screen;
            n.VideoCapture = video;
            n.StartupUserAccount = user;
            n.ProductCode = Box_ProductCode.Text;
            n.Entries.Add(en);
            n.Entries.Add(en);
            n.Entries.Add(en);
            n.Entries.Add(en);
            n.Entries.Add(en);
            n.Entries.Add(en);
            n.Entries.Add(en);
            n.Entries.Add(en);
            n.Entries.Add(en);
            n.Entries.Add(en);
            n.Entries.Add(en);
            n.Entries.Add(en);
            n.Entries.Add(en);
            n.Entries.Add(en);
            n.Entries.Add(en);
            n.Entries.Add(en);
            Directory.CreateDirectory(Brew.Pack.Utils.TemporaryDirectory + "\\gen\\control");
            ControlIcons.Save(logopath, Brew.Pack.Utils.TemporaryDirectory + "\\gen\\control");
            n.Save(Brew.Pack.Utils.TemporaryDirectory + "\\gen\\control");
            string cmd = "-k \"" + Box_KeySet.Text + "\"";
            cmd += " --exefsdir=\"" + Brew.Pack.Utils.TemporaryDirectory + "\\gen\\exefs\"";
            cmd += " --romfsdir=\"" + Brew.Pack.Utils.TemporaryDirectory + "\\gen\\romfs\"";
            cmd += " --controldir=\"" + Brew.Pack.Utils.TemporaryDirectory + "\\gen\\control\"";
            cmd += " --nologo";
            cmd += " --keygeneration=" + keygen.ToString();
            cmd += " --tempdir=\"" + Brew.Pack.Utils.TemporaryDirectory + "\\bin\\temp\"";
            cmd += " --nspdir=\"" + Brew.Pack.Utils.TemporaryDirectory + "\\out\"";
            SaveFileDialog nsp = new SaveFileDialog()
            {
                Title = "Package as an installable NSP package",
                Filter = "Nintendo Submission Package (*.nsp)|*.nsp",
                AddExtension = true,
                InitialDirectory = Environment.GetFolderPath(Environment.SpecialFolder.Desktop),
            };
            DialogResult res = nsp.ShowDialog();
            if (res is System.Windows.Forms.DialogResult.OK)
            {
                string outnsp = nsp.FileName;
                Brew.Pack.Utils.ExecuteCommand(Brew.Pack.Utils.TemporaryDirectory + "\\bin\\hacbrewpack.exe", cmd, true);
                if (File.Exists(Brew.Pack.Utils.TemporaryDirectory + "\\out\\" + appid + ".nsp")) File.Move(Brew.Pack.Utils.TemporaryDirectory + "\\out\\" + appid + ".nsp", outnsp);
                if(File.Exists(outnsp))
                {
                    long nspsize = new FileInfo(outnsp).Length;
                    if(nspsize is 0) Log("The build failed. The built NSP seems to be empty.");
                    else Log("The NSP was successfully built:\n" + outnsp + " (" + Brew.Pack.Utils.FormatSize(nspsize) + ")", LogMode.Information);
                }
                else Log("The build failed. The built NSP does not exist.");
            }
        }
    }
}