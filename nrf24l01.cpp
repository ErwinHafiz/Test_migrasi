#include "dhht.h"
#include "nrf24l01.h"
#include "rainfall.h"
#include "soilMoisture.h"
#include "automation.h"
#include "npk.h"
#include "phSensor.h"
#include "soilMoisture.h"


RF24 radio(CePin, CsnPin);  //ce, csn --> radio object

uint16_t indeks = 0; // indeks penampung counter


void setupNrf24l01() {
  Serial.begin(115200);
  radio.begin();
  if (!radio.begin()) {
    Serial.println("NRF24L01 uno failed to initialize!");
    Serial.println("Trying to connect ... ");
    radio.printDetails();
    // delay(300);
    while (1)
      ;
  } else {
    Serial.println("NRF24L01 initialized successfully.");
  }

  radio.setChannel(108);  // 0-124 channel
  radio.openWritingPipe(address);

  radio.setPALevel(RF24_PA_HIGH);  //RF24_PA_MIN, RF24_PA_LOW, RF24_PA_HIGH, RF24_PA_MAX
  radio.setDataRate(RF24_250KBPS); // Set a lower data rate for more reliable communication

  // radio.setDataRate (RF24_2MBPS); //exchange rate. To choose RF24_2MBPS, RF24_1MBPS, RF24_250KBPS
  // radio.setRetries(0, 15); //(time between attempts to reach out, number of attempts)

  radio.printDetails();
  radio.stopListening();


}

void readAndSend() {
  indeks ++;
  SensorData dataToSend;



    // soil moisture 

  // readSoilMoisture();
  // soilMoisture soilMoistureValue;
   soilMoisture soilMoistureValue = readSoilMoisture();  // Ambil struct dari fungsi readSoilMoisture


  // npk 
  byte nitrogen_value, phosphorus_value, potassium_value;

  readNPKSensor(nitrogen_value, phosphorus_value, potassium_value);
  npk npkValue ; 

  
  // Automation 
  Automation automationValue;

  // DHT
  readTemp();
  
  readHum();

  //PH
  readPh();

  // Mengisi nilai untuk dikirim (NRF)
  dataToSend.counter = indeks;                        //  counter
  dataToSend.temperature = readTemp();  //  suhu
  dataToSend.humidity = readHum();        //  kelembaban
  dataToSend.rainfall = readRainfall();             //  raindfall

  // dataToSend.adc = soilMoistureValue.adc;             //  soil moisture // ga dipake buat save nrf byte maximum
  // dataToSend.tegangan = soilMoistureValue.tegangan;             //  soil moisture// ga dipake buat save nrf byte maximum
  dataToSend.soilMoisturePercent = soilMoistureValue.soilMoisturePercent;             //  soil moisture

  dataToSend.automation = waterPinState;             // status pompa air
  //npk
    dataToSend.nitrogen = npkValue.nitrogen;
  dataToSend.phosporus = npkValue.phosporus;
  dataToSend.potassium = npkValue.potassium;
  // dataToSend.nitrogen = nitrogen_value;
  // dataToSend.phosporus = phosphorus_value;
  // dataToSend.potassium = potassium_value;
  //ph sensor
  // dataToSend.phSensorValue =phSensorValue; // ga dipake buat save nrf byte maximum
  dataToSend.phTanah = phTanah;


  if (isnan(dataToSend.temperature) || isnan(dataToSend.humidity) || isnan(dataToSend.rainfall)) {
    Serial.println("Gagal membaca dari sensor !");
  }

  bool ok = false;
  ok = radio.write(&dataToSend, sizeof(dataToSend));
  if (!isnan(dataToSend.temperature) && !isnan(dataToSend.humidity)&& !isnan(dataToSend.rainfall) &&  ok) {  // data dikirim ke nrf jika data sensor tidak = nan dan nrf receiver tersedia
    Serial.println("Data Berhasil Dikirim dari NRF24L01");
    Serial.print("data send counter = ");
    Serial.println(dataToSend.counter);
    Serial.print("data send temperature = ");
    Serial.println(dataToSend.temperature);
    Serial.print("data send humidity = ");
    Serial.println(dataToSend.humidity);
    Serial.print("data send rainfal = ");
    Serial.println(dataToSend.rainfall);
    // Serial.print("Soil Moisture ADC: ");
    // Serial.println(dataToSend.adc);
    // Serial.print("Soil Moisture Tegangan (Voltage): ");
    // Serial.println(dataToSend.tegangan);
    Serial.print("Soil Moisture Percentage: ");
    Serial.println(dataToSend.soilMoisturePercent);
    Serial.print("Waterpum Status : ");
    Serial.println(dataToSend.automation);
    Serial.print("Nitrogen : ");
    Serial.println(dataToSend.nitrogen);
    Serial.print("Phosporus : ");
    Serial.println(dataToSend.phosporus);
    Serial.print("Potassium : ");
    Serial.println(dataToSend.potassium);
    // Serial.print("PH Sensor Value : ");
    // Serial.println(dataToSend.phSensorValue);
    Serial.print("PH Tanah : ");
    Serial.println(dataToSend.phTanah);
    Serial.print("Size of data being sent: ");
    Serial.println(sizeof(dataToSend));
    

  }

  else {
    Serial.println("Gagal mengirim data dari NRF24L01 oioioi");
    // // Serial.println(dataToSend.phSensorValue);
    // // Serial.println(dataToSend.phTanah);
    // // Serial.println(dataToSend.phosporus);
    // // Serial.println(dataToSend.nitrogen);
    // // Serial.println(dataToSend.potassium);
    // Serial.println(dataToSend.automation);

    //debuging :
    Serial.print("data send counter = ");
    Serial.println(dataToSend.counter);
    Serial.print("data send temperature = ");
    Serial.println(dataToSend.temperature);
    Serial.print("data send humidity = ");
    Serial.println(dataToSend.humidity);
    Serial.print("data send rainfal = ");
    Serial.println(dataToSend.rainfall);
    // Serial.print("Soil Moisture ADC: ");
    // Serial.println(dataToSend.adc);
    // Serial.print("Soil Moisture Tegangan (Voltage): ");
    // Serial.println(dataToSend.tegangan);
    Serial.print("Soil Moisture Percentage: ");
    Serial.println(dataToSend.soilMoisturePercent);
    Serial.print("Waterpum Status : ");
    Serial.println(dataToSend.automation);
    Serial.print("Nitrogen : ");
    Serial.println(dataToSend.nitrogen);
    Serial.print("Phosporus : ");
    Serial.println(dataToSend.phosporus);
    Serial.print("Potassium : ");
    Serial.println(dataToSend.potassium);
    // Serial.print("PH Sensor Value : ");
    // Serial.println(dataToSend.phSensorValue);
    Serial.print("PH Tanah : ");
    Serial.println(dataToSend.phTanah);

    Serial.print("Size of data being sent: ");
    Serial.println(sizeof(dataToSend));
    delay(200);  // delay


  } 

}
