using System;
using System.Windows.Forms;

namespace BeeForm
{
    public class DecimalTrackBar
    {
        private TrackBar trackBar;
        private float scale;

        public DecimalTrackBar(TrackBar trackBar, float scale = 10.0f)
        {
            this.trackBar = trackBar;
            this.scale = scale;
        }

        public float Value
        {
            get => trackBar.Value / scale;
            set => trackBar.Value = (int)(value * scale);
        }

        public float Minimum
        {
            get => trackBar.Minimum / scale;
            set => trackBar.Minimum = (int)(value * scale);
        }

        public float Maximum
        {
            get => trackBar.Maximum / scale;
            set => trackBar.Maximum = (int)(value * scale);
        }
    }
}
