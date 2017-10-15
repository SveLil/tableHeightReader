#include "logicdata.hpp"
#include <Arduino.h>

const unsigned char MARKER_ARRAY_LENGTH = 23;
const unsigned char markerArray[MARKER_ARRAY_LENGTH] = {1, 0, 1, 1, 1, 1, 1, 1, 1, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1};

LogicData::LogicData(unsigned char pin)
{
  _pin = pin;
  _currentPos = 0;
  _lastTime = micros();
}

bool LogicData::checkTime()
{
  unsigned long currentTime = micros();
  if (currentTime - _lastTime < 1000)
  {
    return false;
  }
  _lastTime = currentTime;
  return true;
}

bool LogicData::checkParity(unsigned char currentBit) {
  bool parity = true;
  for (char i = 0; i < 8; i++)
  {
    if (_heightArray[i]) {
      parity = !parity;
    }
  }
  return parity == currentBit;
}

unsigned char LogicData::calculateHeight() {
  unsigned char value = 0;
  for (unsigned char i = 0; i < 8; ++i) {
      value |= (_heightArray[i] << (i));
  }

  //Invert bits
  return ~value;
}

bool LogicData::checkArray() {
  for (char i = 0; i < MARKER_ARRAY_LENGTH; i++)
  {
    if (!_receiveArray[i] == markerArray[i]) {
      return false;
    }
  }
  return true;
}

unsigned char LogicData::process()
{
  if (!checkTime())
  {
    return;
  }

  unsigned char currentBit = digitalRead(_pin);
  for (char i = 0; i < MARKER_ARRAY_LENGTH - 1; i++)
  {
    _receiveArray[i] = _receiveArray[i + 1];
  }
  _receiveArray[MARKER_ARRAY_LENGTH - 1] = currentBit;

  //checkArray
  if (checkArray())
  {
    if (_currentPos < 8)
    {
      _heightArray[_currentPos] = currentBit;
      _currentPos++;
    }
    else
    {
      // Parity bit
      if (checkParity(currentBit)) {
        return calculateHeight();
      }
      _currentPos = 0;
    }
  }
  return 0;
}