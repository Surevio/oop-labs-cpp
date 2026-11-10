#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include <string>
#include <vector>
#include <memory>
#include <fstream>
#include <locale>
#include <cwchar>

#include <boost/archive/text_woarchive.hpp>
#include <boost/archive/text_wiarchive.hpp>
#include <boost/archive/codecvt_null.hpp>
#include <boost/serialization/access.hpp>
#include <boost/serialization/base_object.hpp>
#include <boost/serialization/export.hpp>
#include <boost/serialization/vector.hpp>
#include <boost/serialization/shared_ptr.hpp>
#include <boost/serialization/string.hpp>

class Car
{
public:
    Car() = default;
    Car(const std::wstring& team, const std::wstring& driver, int year, int power, double weight)
        : team_(team), driver_(driver), year_(year), power_(power), weight_(weight) {}
    virtual ~Car() = default;

    const std::wstring& Team() const { return team_; }
    const std::wstring& Driver() const { return driver_; }
    int Year() const { return year_; }
    int Power() const { return power_; }
    double Weight() const { return weight_; }
    virtual bool IsExtended() const { return false; }

private:
    friend class boost::serialization::access;
    template<class Archive>
    void serialize(Archive& ar, const unsigned int /*version*/)
    {
        ar & team_;
        ar & driver_;
        ar & year_;
        ar & power_;
        ar & weight_;
    }

    std::wstring team_;
    std::wstring driver_;
    int year_ = 0;
    int power_ = 0;
    double weight_ = 0.0;
};

class ExtendedCar : public Car
{
public:
    ExtendedCar() = default;
    ExtendedCar(const std::wstring& team, const std::wstring& driver, int year, int power, double weight,
        const std::wstring& principal, double budget, const std::wstring& sponsors)
        : Car(team, driver, year, power, weight), principal_(principal), budget_(budget)
    {
        SetSponsors(sponsors);
    }

    const std::wstring& Principal() const { return principal_; }
    double Budget() const { return budget_; }

    // спонсоры передаются в C# одной строкой через "|"
    std::wstring Sponsors() const
    {
        std::wstring result;
        for (size_t i = 0; i < sponsors_.size(); ++i) {
            if (i != 0) result += L"|";
            result += sponsors_[i];
        }
        return result;
    }

    void SetSponsors(const std::wstring& joined)
    {
        sponsors_.clear();
        std::wstring current;
        for (wchar_t ch : joined) {
            if (ch == L'|' || ch == L',') {
                if (!current.empty()) sponsors_.push_back(current);
                current.clear();
            } else if (!(current.empty() && ch == L' ')) {
                current += ch;
            }
        }
        if (!current.empty()) sponsors_.push_back(current);
    }

    bool IsExtended() const override { return true; }

private:
    friend class boost::serialization::access;
    template<class Archive>
    void serialize(Archive& ar, const unsigned int /*version*/)
    {
        ar & boost::serialization::base_object<Car>(*this);
        ar & principal_;
        ar & budget_;
        ar & sponsors_;
    }

    std::wstring principal_;
    double budget_ = 0.0;
    std::vector<std::wstring> sponsors_;
};

BOOST_CLASS_EXPORT(ExtendedCar)
BOOST_CLASS_EXPORT(Car)

struct F1Garage
{
    std::vector<std::shared_ptr<Car>> cars;
};

namespace
{
    void CopyToBuffer(const std::wstring& src, wchar_t* dst, int dstSize)
    {
        if (dst == nullptr || dstSize <= 0) return;
        int n = static_cast<int>(src.size());
        if (n >= dstSize) n = dstSize - 1;
        if (n > 0) wmemcpy(dst, src.c_str(), n);
        dst[n] = L'\0';
    }

    std::shared_ptr<Car> MakeCar(const wchar_t* team, const wchar_t* driver, int year, int power, double weight,
        bool isExtended, const wchar_t* principal, double budget, const wchar_t* sponsors)
    {
        const std::wstring teamStr = team ? team : L"";
        const std::wstring driverStr = driver ? driver : L"";
        if (isExtended) {
            return std::make_shared<ExtendedCar>(teamStr, driverStr, year, power, weight,
                principal ? principal : L"", budget, sponsors ? sponsors : L"");
        }
        return std::make_shared<Car>(teamStr, driverStr, year, power, weight);
    }

    // codecvt_null пишет wchar_t в файл как есть, без преобразования по локали
    template<class Stream>
    void ImbueArchiveLocale(Stream& stream)
    {
        stream.imbue(std::locale(stream.getloc(), new boost::archive::codecvt_null<wchar_t>));
    }
}

extern "C"
{
    __declspec(dllexport) void* __cdecl CreateGarage()
    {
        return new (std::nothrow) F1Garage();
    }

    __declspec(dllexport) void __cdecl DeleteGarage(void* garage)
    {
        delete static_cast<F1Garage*>(garage);
    }

    __declspec(dllexport) void __cdecl ClearGarage(void* garage)
    {
        if (garage) static_cast<F1Garage*>(garage)->cars.clear();
    }

    __declspec(dllexport) int __cdecl GetCarCount(void* garage)
    {
        if (!garage) return 0;
        return static_cast<int>(static_cast<F1Garage*>(garage)->cars.size());
    }

    __declspec(dllexport) void __cdecl AddCar(void* garage,
        const wchar_t* team, const wchar_t* driver, int year, int power, double weight,
        bool isExtended, const wchar_t* principal, double budget, const wchar_t* sponsors)
    {
        if (!garage) return;
        static_cast<F1Garage*>(garage)->cars.push_back(
            MakeCar(team, driver, year, power, weight, isExtended, principal, budget, sponsors));
    }

    __declspec(dllexport) bool __cdecl UpdateCar(void* garage, int index,
        const wchar_t* team, const wchar_t* driver, int year, int power, double weight,
        bool isExtended, const wchar_t* principal, double budget, const wchar_t* sponsors)
    {
        if (!garage) return false;
        auto* g = static_cast<F1Garage*>(garage);
        if (index < 0 || static_cast<size_t>(index) >= g->cars.size()) return false;

        auto car = MakeCar(team, driver, year, power, weight, isExtended, principal, budget, sponsors);
        g->cars[index] = car;
        return true;
    }

    __declspec(dllexport) void __cdecl DeleteCar(void* garage, int index)
    {
        if (!garage) return;
        auto* g = static_cast<F1Garage*>(garage);
        if (index < 0 || static_cast<size_t>(index) >= g->cars.size()) return;
        g->cars.erase(g->cars.begin() + index);
    }

    __declspec(dllexport) void __cdecl GetCarInfo(void* garage, int index,
        wchar_t* team, int teamSize, wchar_t* driver, int driverSize,
        int* year, int* power, double* weight, bool* isExtended,
        wchar_t* principal, int principalSize, double* budget,
        wchar_t* sponsors, int sponsorsSize)
    {
        if (!garage) return;
        auto* g = static_cast<F1Garage*>(garage);
        if (index < 0 || static_cast<size_t>(index) >= g->cars.size()) return;

        const auto& car = g->cars[index];
        CopyToBuffer(car->Team(), team, teamSize);
        CopyToBuffer(car->Driver(), driver, driverSize);
        if (year) *year = car->Year();
        if (power) *power = car->Power();
        if (weight) *weight = car->Weight();

        if (isExtended) *isExtended = car->IsExtended();

        if (auto ext = std::dynamic_pointer_cast<ExtendedCar>(car); ext) {
            CopyToBuffer(ext->Principal(), principal, principalSize);
            if (budget) *budget = ext->Budget();
            CopyToBuffer(ext->Sponsors(), sponsors, sponsorsSize);
        } else {
            CopyToBuffer(L"", principal, principalSize);
            if (budget) *budget = 0.0;
            CopyToBuffer(L"", sponsors, sponsorsSize);
        }
    }

    __declspec(dllexport) bool __cdecl SaveToFile(void* garage, const wchar_t* filename)
    {
        if (!garage || !filename) return false;
        auto* g = static_cast<F1Garage*>(garage);
        try {
            std::wofstream ofs(filename, std::ios::out | std::ios::trunc);
            if (!ofs.is_open()) return false;
            ImbueArchiveLocale(ofs);
            boost::archive::text_woarchive oa(ofs);
            oa << g->cars;
            return true;
        }
        catch (...) {
            return false;
        }
    }

    __declspec(dllexport) bool __cdecl LoadFromFile(void* garage, const wchar_t* filename)
    {
        if (!garage || !filename) return false;
        auto* g = static_cast<F1Garage*>(garage);
        try {
            std::wifstream ifs(filename);
            if (!ifs.is_open()) return false;
            ImbueArchiveLocale(ifs);
            std::vector<std::shared_ptr<Car>> loaded;
            boost::archive::text_wiarchive ia(ifs);
            ia >> loaded;
            g->cars.swap(loaded);
            return true;
        }
        catch (...) {
            return false;
        }
    }
}

BOOL APIENTRY DllMain(HMODULE, DWORD, LPVOID)
{
    return TRUE;
}
