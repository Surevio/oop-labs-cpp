using System.Runtime.InteropServices;
using System.Text;

namespace Lab6;

// обертка над функциями из f1garage_dll.dll
public sealed class F1GarageWrapper : IDisposable
{
    private const string Dll = "f1garage_dll.dll";

    [DllImport(Dll, CallingConvention = CallingConvention.Cdecl)]
    private static extern IntPtr CreateGarage();

    [DllImport(Dll, CallingConvention = CallingConvention.Cdecl)]
    private static extern void DeleteGarage(IntPtr garage);

    [DllImport(Dll, CallingConvention = CallingConvention.Cdecl, CharSet = CharSet.Unicode)]
    private static extern void AddCar(IntPtr garage, string team, string driver, int year, int power, double weight,
        [MarshalAs(UnmanagedType.I1)] bool isExtended, string principal, double budget, string sponsors);

    [DllImport(Dll, CallingConvention = CallingConvention.Cdecl, CharSet = CharSet.Unicode)]
    [return: MarshalAs(UnmanagedType.I1)]
    private static extern bool UpdateCar(IntPtr garage, int index, string team, string driver, int year, int power,
        double weight, [MarshalAs(UnmanagedType.I1)] bool isExtended, string principal, double budget, string sponsors);

    [DllImport(Dll, CallingConvention = CallingConvention.Cdecl)]
    private static extern void DeleteCar(IntPtr garage, int index);

    [DllImport(Dll, CallingConvention = CallingConvention.Cdecl)]
    private static extern int GetCarCount(IntPtr garage);

    [DllImport(Dll, CallingConvention = CallingConvention.Cdecl, CharSet = CharSet.Unicode)]
    private static extern void GetCarInfo(IntPtr garage, int index,
        StringBuilder team, int teamSize, StringBuilder driver, int driverSize,
        out int year, out int power, out double weight, [MarshalAs(UnmanagedType.I1)] out bool isExtended,
        StringBuilder principal, int principalSize, out double budget,
        StringBuilder sponsors, int sponsorsSize);

    [DllImport(Dll, CallingConvention = CallingConvention.Cdecl, CharSet = CharSet.Unicode)]
    [return: MarshalAs(UnmanagedType.I1)]
    private static extern bool SaveToFile(IntPtr garage, string filename);

    [DllImport(Dll, CallingConvention = CallingConvention.Cdecl, CharSet = CharSet.Unicode)]
    [return: MarshalAs(UnmanagedType.I1)]
    private static extern bool LoadFromFile(IntPtr garage, string filename);

    [DllImport(Dll, CallingConvention = CallingConvention.Cdecl)]
    private static extern void ClearGarage(IntPtr garage);

    private IntPtr garagePtr;

    public F1GarageWrapper()
    {
        garagePtr = CreateGarage();
        if (garagePtr == IntPtr.Zero) throw new InvalidOperationException("Не удалось создать гараж");
    }

    public void AddCar(CarInfo c) =>
        AddCar(garagePtr, c.Team, c.Driver, c.Year, c.Power, c.Weight, c.IsExtended, c.Principal, c.Budget, c.Sponsors);

    public bool UpdateCar(int index, CarInfo c) =>
        UpdateCar(garagePtr, index, c.Team, c.Driver, c.Year, c.Power, c.Weight, c.IsExtended, c.Principal, c.Budget, c.Sponsors);

    public void DeleteCar(int index) => DeleteCar(garagePtr, index);
    public int Count => GetCarCount(garagePtr);
    public void Clear() => ClearGarage(garagePtr);
    public bool Save(string filename) => SaveToFile(garagePtr, filename);
    public bool Load(string filename) => LoadFromFile(garagePtr, filename);

    public CarInfo GetCar(int index)
    {
        const int size = 1024;
        var team = new StringBuilder(size);
        var driver = new StringBuilder(size);
        var principal = new StringBuilder(size);
        var sponsors = new StringBuilder(size);
        GetCarInfo(garagePtr, index, team, size, driver, size, out var year, out var power, out var weight,
            out var extended, principal, size, out var budget, sponsors, size);

        return new CarInfo
        {
            Team = team.ToString(),
            Driver = driver.ToString(),
            Year = year,
            Power = power,
            Weight = weight,
            IsExtended = extended,
            Principal = principal.ToString(),
            Budget = budget,
            Sponsors = sponsors.ToString()
        };
    }

    public void Dispose()
    {
        if (garagePtr != IntPtr.Zero)
        {
            DeleteGarage(garagePtr);
            garagePtr = IntPtr.Zero;
        }
        GC.SuppressFinalize(this);
    }

    ~F1GarageWrapper() => Dispose();
}

public sealed class CarInfo
{
    public string Team { get; set; } = "";
    public string Driver { get; set; } = "";
    public int Year { get; set; }
    public int Power { get; set; }
    public double Weight { get; set; }
    public bool IsExtended { get; set; }
    public string Principal { get; set; } = "";
    public double Budget { get; set; }
    public string Sponsors { get; set; } = "";
}
