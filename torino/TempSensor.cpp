#include "TempSensor.h"

// Inicializamos el sensor DHT11
DHT dht(DHTPIN, DHTTYPE);

TempSensor::TempSensor(/* args */)
{
    lastUnixtime = 0;
    refreshInterval = 1; // Amount of seconds between display refreshes.
}

TempSensor::~TempSensor()
{
}

/**************************************************************************/
/*!
    @brief  Initializes the DHT temperature sensor.
*/
/**************************************************************************/
void TempSensor::begin()
{
    dht.begin();
}

/**************************************************************************/
/*!
    @brief  Prints the temperature and the humidity on the small display.
*/
/**************************************************************************/
void TempSensor::printTemp()
{
    // now = clock.RTC.now();

    // if (lastUnixtime + refreshInterval > now.unixtime())
    // {
    //     return;
    // }

    // float h = dht.readHumidity();
    // float t = dht.readTemperature();

    // if (isnan(h) || isnan(t))
    // {
    //     Serial.println("Error reading data from DHT11 sensor.");
    //     return;
    // }

    // // smallDisplay.clearDisplay();

    // // Print time
    // smallDisplay.setCursor(0, 50);
    // smallDisplay.setTextSize(2);
    // smallDisplay.setTextColor(WHITE);

    // smallDisplay.print(t, h);

    // smallDisplay.display();

    // lastUnixtime = now.unixtime();
}

/**************************************************************************/
/*!
    @brief  Returns sensor readings.
    @return A struct containing the values of the temperature and humidity.
*/
/**************************************************************************/
readings TempSensor::getReadings()
{
    readings values = {dht.readTemperature(),
                       dht.readHumidity()};

    return values;
}