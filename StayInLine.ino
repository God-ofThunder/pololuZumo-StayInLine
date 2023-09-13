#include <Wire.h>
#include <Zumo32U4.h>

Zumo32U4LCD display;
Zumo32U4Motors motor;
Zumo32U4ButtonA buttonA;
Zumo32U4ButtonB buttonB;
Zumo32U4ButtonC buttonC;
Zumo32U4LineSensors lineSensors;
Zumo32U4ProximitySensors proxSensors;

#define NUM_SENSORS 5
uint16_t lineSensorValues[NUM_SENSORS];
bool useEmitters = true;
uint16_t surfaceBrightnes[2];

void setup() {
	lineSensors.initFiveSensors();
	buttonA.waitForButton();
	lineSensors.read(lineSensorValues, useEmitters ? QTR_EMITTERS_ON: QTR_EMITTERS_OFF);
	surfaceBrightnes[0] = lineSensorValues[0];
	surfaceBrightnes[1] = lineSensorValues[4];
}

void printReadingsToDisplay(){
	display.clear();
	display.gotoXY(0,0);
	display.print(surfaceBrightnes[0]);
	display.print(" ");
	display.print(surfaceBrightnes[1]);
	display.gotoXY(0,1);
	display.print(lineSensorValues[0]);
	display.print(lineSensorValues[4]);
}

void driveAwayFromLine(){
	motor.setSpeeds(-200, -200);
	delay(500);
	motor.setSpeeds(-200, 200);
	delay(500);
}

void checkForLine(){
	uint8_t lineTreshHold = 200;
	if(lineSensorValues[0] < surfaceBrightnes[0]-lineTreshHold || lineSensorValues[4] < surfaceBrightnes[1]-lineTreshHold){
		driveAwayFromLine();
	}
	else if(lineSensorValues[0] > surfaceBrightnes[0]+lineTreshHold || lineSensorValues[4] > surfaceBrightnes[1]+lineTreshHold){
		driveAwayFromLine();
	}
}

void loop() {
	lineSensors.read(lineSensorValues, useEmitters ? QTR_EMITTERS_ON: QTR_EMITTERS_OFF);
	printReadingsToDisplay();
	motor.setSpeeds(200, 200);
	checkForLine();
}
