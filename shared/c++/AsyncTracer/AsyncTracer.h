#ifndef AsyncTracer_h
#define AsyncTracer_h

#include <ModbusMaster.h>
#include <functional>
#include <deque>

#include <Log.h>
#include <TimeUtils.h>

class AsyncTracer
{
    public:
        typedef std::function<void(float pvVoltage)> PVVoltageCallback;
        typedef std::function<void(float pvCurrent)> PVCurrentCallback;
        typedef std::function<void(float pvPower)> PVPowerCallback;
        typedef std::function<void(float batteryChargingVoltage)> BatteryChargingVoltageCallback;
        typedef std::function<void(float batteryChargingCurrent)> BatteryChargingCurrentCallback;
        typedef std::function<void(float batteryChargingPower)> BatteryChargingPowerCallback;
        typedef std::function<void(float loadVoltage)> LoadVoltageCallback;
        typedef std::function<void(float loadCurrent)> LoadCurrentCallback;
        typedef std::function<void(float loadPower)> LoadPowerCallback;
        typedef std::function<void(float batteryTemperature)> BatteryTemperatureCallback;
        typedef std::function<void(float caseTemperature)> CaseTemperatureCallback;
        typedef std::function<void(float heatSinkTemperature)> HeatSinkTemperatureCallback;
        typedef std::function<void(float batteryPercentage)> BatteryPercentageCallback;
        typedef std::function<void(float remoteBatteryTemperature)> RemoteBatteryTemperatureCallback;
        typedef std::function<void(float systemVoltage)> SystemVoltageCallback;

        float NULL_RESPONSE_VALUE = -1000.0f;

        enum ADDRESSES {
            // Real-time data
            PV_VOLTAGE = 0x3100,
            PV_CURRENT = 0x3101,
            PV_POWER = 0x3102, // 16-bit
            BATTERY_CHARGING_VOLTAGE = 0x3104,
            BATTERY_CHARGING_CURRENT = 0x3105,
            BATTERY_CHARGING_POWER = 0x3106, // 16-bit
            LOAD_VOLTAGE = 0x310C,
            LOAD_CURRENT = 0x310D,
            LOAD_POWER = 0x310E, // 16-bit
            BATTERY_TEMPERATURE = 0x3110,
            CASE_TEMPERATURE = 0x3111,
            HEAT_SINK_TEMPERATURE = 0x3112,
            BATTERY_PERCENTAGE = 0x311A,
            REMOTE_BATTERY_TEMPERATURE = 0x311B,
            SYSTEM_VOLTAGE = 0x311D,
        };

        ModbusMaster* client;
        HardwareSerial* serial;
        uint8_t tx;
        uint8_t rx;
        uint8_t address;

        float pvVoltage = NULL_RESPONSE_VALUE;
        float pvCurrent = NULL_RESPONSE_VALUE;
        float pvPower = NULL_RESPONSE_VALUE;
        float batteryChargingVoltage = NULL_RESPONSE_VALUE;
        float batteryChargingCurrent = NULL_RESPONSE_VALUE;
        float batteryChargingPower = NULL_RESPONSE_VALUE;
        float loadVoltage = NULL_RESPONSE_VALUE;
        float loadCurrent = NULL_RESPONSE_VALUE;
        float loadPower = NULL_RESPONSE_VALUE;
        float batteryTemperature = NULL_RESPONSE_VALUE;
        float caseTemperature = NULL_RESPONSE_VALUE;
        float heatSinkTemperature = NULL_RESPONSE_VALUE;
        float batteryPercentage = NULL_RESPONSE_VALUE;
        float remoteBatteryTemperature = NULL_RESPONSE_VALUE;
        float systemVoltage = NULL_RESPONSE_VALUE;

        PVVoltageCallback pvVoltageCallback = nullptr;
        PVCurrentCallback pvCurrentCallback = nullptr;
        PVPowerCallback pvPowerCallback = nullptr;
        BatteryChargingVoltageCallback batteryChargingVoltageCallback = nullptr;
        BatteryChargingCurrentCallback batteryChargingCurrentCallback = nullptr;
        BatteryChargingPowerCallback batteryChargingPowerCallback = nullptr;
        LoadVoltageCallback loadVoltageCallback = nullptr;
        LoadCurrentCallback loadCurrentCallback = nullptr;
        LoadPowerCallback loadPowerCallback = nullptr;
        BatteryTemperatureCallback batteryTemperatureCallback = nullptr;
        CaseTemperatureCallback caseTemperatureCallback = nullptr;
        HeatSinkTemperatureCallback heatSinkTemperatureCallback = nullptr;
        BatteryPercentageCallback batteryPercentageCallback = nullptr;
        RemoteBatteryTemperatureCallback remoteBatteryTemperatureCallback = nullptr;
        SystemVoltageCallback systemVoltageCallback = nullptr;
        
        int readPeriod = 2000;
        int lastReadMillis = 0;
        int readCount = 0;

    private:
        std::deque<std::string> jobQueue = {};

    public:
        AsyncTracer(
            HardwareSerial* _serial,
            uint8_t _tx,
            uint8_t _rx,
            uint8_t _address
        );

        void setPVVoltageCallback(
            PVVoltageCallback _pvVoltageCallback = nullptr
        );
        void setPVCurrentCallback(
            PVCurrentCallback _pvCurrentCallback = nullptr
        );
        void setPVPowerCallback(
            PVPowerCallback _pvPowerCallback = nullptr
        );
        void setBatteryChargingVoltageCallback(
            BatteryChargingVoltageCallback _batteryChargingVoltageCallback = nullptr
        );
        void setBatteryChargingCurrentCallback(
            BatteryChargingCurrentCallback _batteryChargingCurrentCallback = nullptr
        );
        void setBatteryChargingPowerCallback(
            BatteryChargingPowerCallback _batteryChargingPowerCallback = nullptr
        );
        void setLoadVoltageCallback(
            LoadVoltageCallback _loadVoltageCallback = nullptr
        );
        void setLoadCurrentCallback(
            LoadCurrentCallback _loadCurrentCallback = nullptr
        );
        void setLoadPowerCallback(
            LoadPowerCallback _loadPowerCallback = nullptr
        );
        void setBatteryTemperatureCallback(
            BatteryTemperatureCallback _batteryTemperatureCallback = nullptr
        );
        void setCaseTemperatureCallback(
            CaseTemperatureCallback _caseTemperatureCallback = nullptr
        );
        void setHeatSinkTemperatureCallback(
            HeatSinkTemperatureCallback _heatSinkTemperatureCallback = nullptr
        );
        void setBatteryPercentageCallback(
            BatteryPercentageCallback _batteryPercentageCallback = nullptr
        );
        void setRemoteBatteryTemperatureCallback(
            RemoteBatteryTemperatureCallback _remoteBatteryTemperatureCallback = nullptr
        );
        void setSystemVoltageCallback(
            SystemVoltageCallback _systemVoltageCallback = nullptr
        );

        void setup(void (*preTransmission)() = nullptr, void (*postTransmission)() = nullptr);
        void loop();

    private:
        boolean getCoil(uint16_t address, boolean defaultValue = false);
        float get8BitRegister(
            uint16_t address, 
            float multiplier = 1.0f,
            float defaultValue = -1000.0f
        );
        float get16BitRegister(
            uint16_t address, 
            float multiplier = 1.0f,
            float defaultValue = -1000.0f
        );

        void checkPVVoltage();
        void checkPVCurrent();
        void checkPVPower();
        void checkBatteryChargingVoltage();
        void checkBatteryChargingCurrent();
        void checkBatteryChargingPower();
        void checkLoadVoltage();
        void checkLoadCurrent();
        void checkLoadPower();
        void checkBatteryTemperature();
        void checkCaseTemperature();
        void checkHeatSinkTemperature();
        void checkBatteryPercentage();
        void checkRemoteBatteryTemperature();
        void checkSystemVoltage();

        void checkJobQueue();
};

#endif