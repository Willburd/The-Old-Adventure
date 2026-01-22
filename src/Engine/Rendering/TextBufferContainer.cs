using Assets;
using Engine;
using Silk.NET.OpenGL;
using System.Numerics;

namespace Rendering
{
    public class TextBufferContainer(uint width, uint height) : FrameBufferContainer(width, height)
    {
        /// <summary>
        /// Used to convert string characters to font atlas UV positions
        /// </summary>
        static Dictionary<char, Vector2> decode = [];

        /// <summary>
        /// Character sequence that matches the font atlas
        /// </summary>
        static string seq = " ☺☻♥♦♣♠•◘○◙♂♀♪♫☼►◄↕‼¶§▬↨↑↓→←∟↔▲▼ !\"#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_`abcdefghijklmnopqrstuvwxyz{¦}~⌂ÇüéâäàåçêëèïîìÄÅÉæÆôöòûùÿÖÜ¢£¥₧ƒáíóúñÑªº¿⌐¬½¼¡«»░▒▓│┤╡╢╖╕╣║╗╝╜╛┐└┴┬├─┼╞╟╚╔╩╦╠═╬╧╨╤╥╙╘╒╓╫╪┘┌█▄▌▐▀αßΓπΣσµτΦΘΩδ∞φε∩≡±≥≤⌠⌡÷≈°∙·√ⁿ²■□";

        static TextBufferContainer()
        {
            int tex_col_count = 16;
            for (int index = 0; index < seq.Length; index++)
            {
                decode.Add(seq[index], new((float)index % tex_col_count / tex_col_count, MathF.Floor(index / tex_col_count) / tex_col_count));
            }
        }

        /// <summary>
        /// Bind the buffer, clears it, and draws text to it. Used as a quick utility function for text rendering.
        /// </summary>
        public void DrawTextToBuffer(string text, Vector2 offset, bool clear_buffer = true)
        {
            BindFrameBuffer();
            if (clear_buffer)
            {
                Core.OpenGLContext.ClearColor(System.Drawing.Color.FromArgb(0, 0, 0, 0));
                Core.OpenGLContext.Clear(ClearBufferMask.ColorBufferBit | ClearBufferMask.DepthBufferBit | ClearBufferMask.StencilBufferBit);
            }
            

        }
    }
}