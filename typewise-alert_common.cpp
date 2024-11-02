// this file contains all the common functions
#include "typewise-alert.h"
#include <stdio.h>

BreachType inferBreach(double value, double lowerLimit, double upperLimit) {
  if(value < lowerLimit) {
    return TOO_LOW;
  }
  if(value > upperLimit) {
    return TOO_HIGH;
  }
  return NORMAL;
}

BreachType classifyTemperatureBreach(CoolingType coolingType, double temperatureInC) {
    int lowerLimit = 0, upperLimit = 0;
    if(coolingType <= 2)
        getCoolingLimits(coolingType, lowerLimit, upperLimit);
    return inferBreach(temperatureInC, lowerLimit, upperLimit);
}

void getCoolingLimits(CoolingType coolingType, int &lowerLimit, int &upperLimit) {
    switch (coolingType) {
        case PASSIVE_COOLING:
            lowerLimit = 0;
            upperLimit = 35;
            break;
        case HI_ACTIVE_COOLING:
            lowerLimit = 0;
            upperLimit = 45;
            break;
        case MED_ACTIVE_COOLING:
            lowerLimit = 0;
            upperLimit = 40;
            break;
    }
}

void checkAndAlert(AlertTarget alertTarget, BatteryCharacter batteryChar, double temperatureInC) {
  BreachType breachType = classifyTemperatureBreach(batteryChar.coolingType, temperatureInC);
  switch(alertTarget) {
    case TO_CONTROLLER:
      sendToController(breachType);
      break;
    case TO_EMAIL:
      sendToEmail(breachType);
      break;
  }
}
