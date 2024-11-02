#include "gtest/gtest.h"
#include "typewise-alert.h"

// Define mock variables
BreachType MockControllerBreachType;
BreachType MockEmailBreachType;
bool isControllerAlertCalled = false;
bool isEmailAlertCalled = false;

// Define mock functions
void mockSendToController(BreachType breachType) {
  MockControllerBreachType = breachType;
  isControllerAlertCalled = true;
}

void mockSendToEmail(BreachType breachType) {
  MockEmailBreachType = breachType;
  isEmailAlertCalled = true;
}

// Redirect function pointers to mock functions
void (*sendToControllerPtr)(BreachType) = mockSendToController;
void (*sendToEmailPtr)(BreachType) = mockSendToEmail;

// Reset function for mock variables
void resetMocks() {
  isControllerAlertCalled = false;
  isEmailAlertCalled = false;
  MockControllerBreachType = NORMAL;
  MockEmailBreachType = NORMAL;
}

// Test fixture class
class TypewiseAlertTest : public ::testing::Test {
protected:
    void testAlertForCoolingType(AlertTarget alertTarget, CoolingType coolingType, double temperatureInC, BreachType expectedBreach) {
        BatteryCharacter batteryChar = {coolingType, "BrandX"};
        testing::internal::CaptureStdout();
        checkAndAlert(alertTarget, batteryChar, temperatureInC);
        std::string output = testing::internal::GetCapturedStdout();
        ASSERT_EQ(expectedBreach, classifyTemperatureBreach(coolingType, temperatureInC));
        if (alertTarget == TO_CONTROLLER) {
            validateControllerOutput(expectedBreach, output);
        } else if (alertTarget == TO_EMAIL) {
            validateEmailOutput(expectedBreach, output);
        }
    }

    void validateControllerOutput(BreachType expectedBreach, const std::string& output) {
        const unsigned short expectedHeader = 0xfeed;
        std::string expectedOutput = "";
        if (expectedBreach == TOO_LOW) {
            expectedOutput = "feed : 1\n";
        } else if (expectedBreach == TOO_HIGH) {
            expectedOutput = "feed : 2\n";
        }else {
            expectedOutput = "feed : 0\n";
        }
        ASSERT_EQ(output, expectedOutput);
    }

    void validateEmailOutput(BreachType expectedBreach, const std::string& output) {
        const char* recipient = "a.b@c.com";
        std::string expectedOutput = "";
        if (expectedBreach == TOO_LOW) {
            expectedOutput = "To: a.b@c.com\nHi, the temperature is too low\n";
        } else if (expectedBreach == TOO_HIGH) {
            expectedOutput = "To: a.b@c.com\nHi, the temperature is too high\n";
        } // No action required for NORMAL; expectedOutput remains empty.
        ASSERT_EQ(output, expectedOutput);
    }
};
// Test Cases
TEST_F(TypewiseAlertTest, PassiveCooling_ToController_WhenTemperatureIsNormal_ShouldReturnNormal) {
    testAlertForCoolingType(TO_CONTROLLER, PASSIVE_COOLING, 20, NORMAL);
}

TEST_F(TypewiseAlertTest, PassiveCooling_ToController_WhenTemperatureIsTooLow_ShouldReturnTooLow) {
    testAlertForCoolingType(TO_CONTROLLER, PASSIVE_COOLING, -1, TOO_LOW);
}

TEST_F(TypewiseAlertTest, PassiveCooling_ToController_WhenTemperatureIsTooHigh_ShouldReturnTooHigh) {
    testAlertForCoolingType(TO_CONTROLLER, PASSIVE_COOLING, 36, TOO_HIGH);
}

TEST_F(TypewiseAlertTest, HiActiveCooling_ToController_WhenTemperatureIsNormal_ShouldReturnNormal) {
    testAlertForCoolingType(TO_CONTROLLER, HI_ACTIVE_COOLING, 30, NORMAL);
}

TEST_F(TypewiseAlertTest, HiActiveCooling_ToController_WhenTemperatureIsTooLow_ShouldReturnTooLow) {
    testAlertForCoolingType(TO_CONTROLLER, HI_ACTIVE_COOLING, -1, TOO_LOW);
}

TEST_F(TypewiseAlertTest, HiActiveCooling_ToController_WhenTemperatureIsTooHigh_ShouldReturnTooHigh) {
    testAlertForCoolingType(TO_CONTROLLER, HI_ACTIVE_COOLING, 46, TOO_HIGH);
}

TEST_F(TypewiseAlertTest, MedActiveCooling_ToController_WhenTemperatureIsNormal_ShouldReturnNormal) {
    testAlertForCoolingType(TO_CONTROLLER, MED_ACTIVE_COOLING, 30, NORMAL);
}

TEST_F(TypewiseAlertTest, MedActiveCooling_ToController_WhenTemperatureIsTooLow_ShouldReturnTooLow) {
    testAlertForCoolingType(TO_CONTROLLER, MED_ACTIVE_COOLING, -1, TOO_LOW);
}

TEST_F(TypewiseAlertTest, MedActiveCooling_ToController_WhenTemperatureIsTooHigh_ShouldReturnTooHigh) {
    testAlertForCoolingType(TO_CONTROLLER, MED_ACTIVE_COOLING, 41, TOO_HIGH);
}

TEST_F(TypewiseAlertTest, PassiveCooling_ToEmail_WhenTemperatureIsNormal_ShouldReturnNormal) {
    testAlertForCoolingType(TO_EMAIL, PASSIVE_COOLING, 20, NORMAL);
}

TEST_F(TypewiseAlertTest, PassiveCooling_ToEmail_WhenTemperatureIsTooLow_ShouldReturnTooLow) {
    testAlertForCoolingType(TO_EMAIL, PASSIVE_COOLING, -1, TOO_LOW);
}

TEST_F(TypewiseAlertTest, PassiveCooling_ToEmail_WhenTemperatureIsTooHigh_ShouldReturnTooHigh) {
    testAlertForCoolingType(TO_EMAIL, PASSIVE_COOLING, 36, TOO_HIGH);
}

TEST_F(TypewiseAlertTest, HiActiveCooling_ToEmail_WhenTemperatureIsNormal_ShouldReturnNormal) {
    testAlertForCoolingType(TO_EMAIL, HI_ACTIVE_COOLING, 30, NORMAL);
}

TEST_F(TypewiseAlertTest, HiActiveCooling_ToEmail_WhenTemperatureIsTooLow_ShouldReturnTooLow) {
    testAlertForCoolingType(TO_EMAIL, HI_ACTIVE_COOLING, -1, TOO_LOW);
}

TEST_F(TypewiseAlertTest, HiActiveCooling_ToEmail_WhenTemperatureIsTooHigh_ShouldReturnTooHigh) {
    testAlertForCoolingType(TO_EMAIL, HI_ACTIVE_COOLING, 46, TOO_HIGH);
}

TEST_F(TypewiseAlertTest, MedActiveCooling_ToEmail_WhenTemperatureIsNormal_ShouldReturnNormal) {
    testAlertForCoolingType(TO_EMAIL, MED_ACTIVE_COOLING, 30, NORMAL);
}

TEST_F(TypewiseAlertTest, MedActiveCooling_ToEmail_WhenTemperatureIsTooLow_ShouldReturnTooLow) {
    testAlertForCoolingType(TO_EMAIL, MED_ACTIVE_COOLING, -1, TOO_LOW);
}

TEST_F(TypewiseAlertTest, MedActiveCooling_ToEmail_WhenTemperatureIsTooHigh_ShouldReturnTooHigh) {
    testAlertForCoolingType(TO_EMAIL, MED_ACTIVE_COOLING, 41, TOO_HIGH);
}
