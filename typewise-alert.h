#pragma once
typedef enum {
  PASSIVE_COOLING,
  HI_ACTIVE_COOLING,
  MED_ACTIVE_COOLING
} CoolingType;

typedef enum {
  NORMAL,
  TOO_LOW,
  TOO_HIGH
} BreachType;

BreachType inferBreach(double value, double lowerLimit, double upperLimit);
void getCoolingLimits(CoolingType coolingType, int &lowerLimit, int &upperLimit);
BreachType classifyTemperatureBreach(CoolingType coolingType, double temperatureInC);
typedef enum {
  TO_CONTROLLER,
  TO_EMAIL
} AlertTarget;

typedef struct {
  CoolingType coolingType;
  char brand[48];
} BatteryCharacter;

void checkAndAlert(
  AlertTarget alertTarget, BatteryCharacter batteryChar, double temperatureInC);

void sendToController(BreachType breachType);
void sendToEmail(BreachType breachType);

// Extern declarations for mock variables
extern BreachType MockControllerBreachType;
extern BreachType MockEmailBreachType;
extern bool isControllerAlertCalled;
extern bool isEmailAlertCalled;

extern void (*sendToControllerPtr)(BreachType);
extern void (*sendToEmailPtr)(BreachType);
