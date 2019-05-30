#include "MQUnifiedsensor.h"

 MQUnifiedsensor::MQUnifiedsensor(int pin, int type) {

  this->_pin = pin;
  this->_type = type; //2,3,4,5,6,7,8,9,131,135,303,309
  //Set _MQ variable with MQ sensor selected
  if(_type == 2)
  {
    memcpy(_MQ, _MQ2, sizeof(_MQ2));
    //_MQ = MQ2;
  }
  else if(_type == 3)
  {
    memcpy(_MQ, _MQ3, sizeof(_MQ3));
    //_MQ = MQ3;
  }
  else if(_type == 4)
  {
    memcpy(_MQ, _MQ4, sizeof(_MQ4));
    //_MQ = MQ4;
  }
  else if(_type == 5)
  {
    memcpy(_MQ, _MQ5, sizeof(_MQ5));
    //_MQ = MQ5;
  }
  else if(_type == 6)
  {
    memcpy(_MQ, _MQ6, sizeof(_MQ6));
    //_MQ = MQ6;
  }
  else if(_type == 7)
  {
    memcpy(_MQ, _MQ7, sizeof(_MQ7));
    //_MQ = MQ7;
  }
  else if(_type == 8)
  {
    memcpy(_MQ, _MQ8, sizeof(_MQ8));
    //_MQ = MQ8;
  }
  else if(_type == 9)
  {
    memcpy(_MQ, _MQ9, sizeof(_MQ9));
    //_MQ = MQ9;
  }
  else if(_type == 131)
  {
    memcpy(_MQ, _MQ131, sizeof(_MQ131));
    //_MQ = MQ131;
  }
  else if(_type == 135)
  {
    memcpy(_MQ, _MQ135, sizeof(_MQ135));
    //_MQ = MQ135;
  }
  else if(_type == 303)
  {
    memcpy(_MQ, _MQ303A, sizeof(_MQ303A));
    //_MQ = MQ303A;
  }
  else if(_type == 309)
  {
    memcpy(_MQ, _MQ309A, sizeof(_MQ309A));
    //_MQ = MQ309;
  }
}
void MQUnifiedsensor::setVoltResolution(float voltaje)
{
  VOLT_RESOLUTION = voltaje;
}
void MQUnifiedsensor::inicializar()
{
  pinMode(_pin, INPUT);
}
int MQUnifiedsensor::readSensor(String nameLectureRequeired, bool print)
{
  setSensorCharacteristics(nameLectureRequeired, print);
  _PPM =readPPM(_m, _b);
  if(print)
  {
    String nameLecture = getnameLecture();
    Serial.println("**********************");
    Serial.println("* Sensor: MQ-" + String(_type));
    Serial.println("* m =" + String(_m) + " ,b =" + String(_b));
    Serial.println("* RS/R0 = " + String(_ratio) + " ,Voltaje leido(ADC): " + String(this->getVoltage()));
    Serial.println("* Lectura(" + nameLecture + ")=" + String(_PPM) + " PPM");
    Serial.println("**********************");
  }
  return _PPM;
}
String MQUnifiedsensor::getnameLecture()
{
  return _nameLectureRequeired;
}
void MQUnifiedsensor::setSensorCharacteristics(String nameLectureRequeired, bool print)
{
  //Defaults index
  if(nameLectureRequeired == "")
  {
    //Set default
    setDefaultGas();
    //Set ratio in clean air to calc R0
    setRatioInCleanAir();
    //Put  the default into variable internally used
    nameLectureRequeired = _nameLectureRequeired;
  }

  //Dinamic index search
  for (int i=0; i<sizeof(_MQ); i++) {
      if (nameLectureRequeired == _MQ[i]) {    //modified here
        _lecturePosInArray = i;
        break;
      }
    }
  //Serial debugging
  _m = stringToDouble(_MQ[_lecturePosInArray+1]);
  _b = stringToDouble(_MQ[_lecturePosInArray+2]);
  //Save the name for future calls
  nameLectureRequeired = _nameLectureRequeired;
}
int MQUnifiedsensor::readPPM(int m, int b) {
  /**
  * Returns the PPM concentration
  */
  double sensor_volt = this->getVoltage();
  double RS_gas; //Define variable for sensor resistance

  RS_gas = ((5.0 * 10.0) / sensor_volt) - 10.0; //Get value of RS in a gas

  _ratio = RS_gas / this->_R0;   // Get ratio RS_gas/RS_air

  double ppm_log = (log10(_ratio) - b) / m; //Get ppm value in linear scale according to the the ratio value
  double ppm = pow(10, ppm_log); //Convert ppm value to log scale
  return floor(ppm);
}
double MQUnifiedsensor::calibrate() {
    float sensor_volt; //Define variable for sensor voltage
    float RS_air; //Define variable for sensor resistance
    float R0; //Define variable for R0
    float sensorValue; //Define variable for analog readings
    sensor_volt = this->getVoltage(); //Convert average to voltage
    R0 = sensor_volt / _ratioInCleanAir; //Calculate R0
    return R0;
}
double MQUnifiedsensor::getVoltage() {
  double avg = 0.0;
  for (int i = 0; i < retries; i ++) {
    avg += analogRead(this->_pin) / retries;
    delay(retry_interval);
  }

  double voltage = avg * VOLT_RESOLUTION / (pow(2, ADC_RESOLUTION) - 1);

  return voltage;
}
void MQUnifiedsensor::setR0(double R0) {
  this->_R0 = R0;
}
void MQUnifiedsensor::setDefaultGas()
{
    if(_type == 2)
    {
      _nameLectureRequeired = defaultMQ2;
    }
    else if(_type == 3)
    {
      _nameLectureRequeired = defaultMQ3;
    }
    else if(_type == 4)
    {
      _nameLectureRequeired = defaultMQ4;
    }
    else if(_type == 5)
    {
      _nameLectureRequeired = defaultMQ5;
    }
    else if(_type == 6)
    {
      _nameLectureRequeired = defaultMQ6;
    }
    else if(_type == 7)
    {
      _nameLectureRequeired = defaultMQ7;
    }
    else if(_type == 8)
    {
      _nameLectureRequeired = defaultMQ8;
    }
    else if(_type == 9)
    {
      _nameLectureRequeired = defaultMQ9;
    }
    else if(_type == 131)
    {
      _nameLectureRequeired = defaultMQ131;
    }
    else if(_type == 135)
    {
      _nameLectureRequeired = defaultMQ135;
    }
    else if(_type == 303)
    {
      _nameLectureRequeired = defaultMQ303;
    }
    else if(_type == 309)
    {
      _nameLectureRequeired = defaultMQ309;
    }
}
void MQUnifiedsensor::setRatioInCleanAir()
{
    if(_type == 2)
    {
      _ratioInCleanAir = RatioMQ2CleanAir;
    }
    else if(_type == 3)
    {
      _ratioInCleanAir = RatioMQ3CleanAir;
    }
    else if(_type == 4)
    {
      _ratioInCleanAir = RatioMQ4CleanAir;
    }
    else if(_type == 5)
    {
      _ratioInCleanAir = RatioMQ5CleanAir;
    }
    else if(_type == 6)
    {
      _ratioInCleanAir = RatioMQ6CleanAir;
    }
    else if(_type == 7)
    {
      _ratioInCleanAir = RatioMQ7CleanAir;
    }
    else if(_type == 8)
    {
      _ratioInCleanAir = RatioMQ8CleanAir;
    }
    else if(_type == 9)
    {
      _ratioInCleanAir = RatioMQ9CleanAir;
    }
    else if(_type == 131)
    {
      _ratioInCleanAir = RatioMQ131CleanAir;
    }
    else if(_type == 135)
    {
      _ratioInCleanAir = RatioMQ135CleanAir;
    }
    else if(_type == 303)
    {
      _ratioInCleanAir = RatioMQ303CleanAir;
    }
    else if(_type == 309)
    {
      _ratioInCleanAir = RatioMQ309CleanAir;
    }
}
double MQUnifiedsensor::stringToDouble(String & str)
{
  return atof( str.c_str() );
}