using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;

namespace BeeAPI
{
    public class Native
    {
        [DllImport(@".\BeeCplus.dll", CallingConvention = CallingConvention.Cdecl, CharSet = CharSet.Ansi)]
       unsafe public static extern IntPtr GetImage(ref int rows, ref int cols, ref int Type);
        [DllImport(@".\BeeCplus.dll", CallingConvention = CallingConvention.Cdecl, CharSet = CharSet.Ansi)]
        unsafe public static extern IntPtr GetResultImage(ref int rows, ref int cols, ref int Type);

        [DllImport(@".\BeeCplus.dll", CallingConvention = CallingConvention.Cdecl, CharSet = CharSet.Ansi)]
        unsafe public static extern IntPtr ScanBasler();

        [DllImport(@".\BeeCplus.dll", CallingConvention = CallingConvention.Cdecl, CharSet = CharSet.Ansi)]
        unsafe public static extern void GrabBasler();

        [DllImport(@".\BeeCplus.dll", CallingConvention = CallingConvention.Cdecl, CharSet = CharSet.Ansi)]
        unsafe public static extern IntPtr ConnectBasler(int width, int height, string device);

        [DllImport(@".\BeeCplus.dll", CallingConvention = CallingConvention.Cdecl, CharSet = CharSet.Ansi)]
        unsafe public static extern IntPtr SetPara(string Para, float Value);

        [DllImport(@".\BeeCplus.dll", CallingConvention = CallingConvention.Cdecl, CharSet = CharSet.Ansi)]
        unsafe public static extern IntPtr GetPara(string Para);

        [DllImport(@".\BeeCplus.dll", CallingConvention = CallingConvention.Cdecl, CharSet = CharSet.Ansi)]
        unsafe public static extern IntPtr DisconnectBasler();

     
    }
}
