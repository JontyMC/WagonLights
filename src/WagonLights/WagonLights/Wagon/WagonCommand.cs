namespace WagonLights.Wagon
{
    public class WagonCommand
    {
        public WagonCommandType Type { get; set; }
        public byte[] Command { get; set; }
    }
}