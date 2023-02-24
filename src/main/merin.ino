// -------------------------------------------------
// Copyright (c) 2023 HiBit <https://www.hibit.dev>
// -------------------------------------------------

void control()
{
  bool hasLightsOn = airControl.toggles.upper.on;

  // Driving dark places?
  if (airControl.buttons.right.upper.pressed || hasLightsOn) {
    headlightsOn();
  } else {
    lightsOff();
  }

  // Setting direction
  if (airControl.toggles.lower.on) {
    if (isMotorDirectionUnset(motorSpeed) || isMotorDirectionForward(motorSpeed)) {
      brakelightsOn();
      switchDirectionBackward();
    }
  } else {
    if (isMotorDirectionUnset(motorSpeed) || isMotorDirectionBackward(motorSpeed)) {
      brakelightsOn();
      switchDirectionForward();
    }
  }

  // Turning left or right?
  if (airControl.analogs.left.x > 50) {
    turnRight();
  } else if (airControl.analogs.left.x < -50) {
    turnLeft();
  } else {
    turnNone();
  }

  // Setting speed
  if (airControl.analogs.right.y < -10 ) {
    brakelightsOn();
    brake(airControl.analogs.right.y);
  } else {
    // Recheck lights
    if (hasLightsOn) {
      lightsOn();
    } else {
      brakelightsOff();
    }

    if (airControl.analogs.right.y > 15) {
      accelerate(airControl.analogs.right.y, airControl.potentiometers.left.level);
    } else {
      brakeGradually(random(2));
    }
  }


  // Make some noise!
  if (airControl.buttons.right.lower.pressed) {
    hornOn();
  } else {
    hornOff();
  }

  delay(30);
}

void switchDirectionBackward()
{
  brakeUntilStop();
  setMotorDirectionBackward(motorSpeed);
  sendToSpeedMotor();
}

void switchDirectionForward()
{
  brakeUntilStop();
  setMotorDirectionForward(motorSpeed);
  sendToSpeedMotor();
}

void accelerate(int speed, byte precision)
{
  int speedToApply = map(speed, 0, 127, 0, precision);

  if (speedToApply < motorSpeed.speed) {
    return;
  }

  setMotorSpeed(motorSpeed, speedToApply);
  sendToSpeedMotor();
}

void brake(int speed)
{
  int value = map(speed, 0, -127, 0, motorSpeed.speed);

  setMotorSpeed(motorSpeed, motorSpeed.speed - value);
  sendToSpeedMotor();
}

void brakeGradually(int delta)
{
  int value = max(0, motorSpeed.speed - delta);

  setMotorSpeed(motorSpeed, value > 30 ? value : 0);
  sendToSpeedMotor();
}

void brakeUntilStop()
{
  setMotorSpeed(motorSpeed, 0);
  unsetMotorDirection(motorSpeed);
  sendToSpeedMotor();

  delay(100);
}

void turnNone()
{
  unsetMotorDirection(motorDirection);
  sendToDirectionMotor();
}

void turnLeft()
{
  setMotorDirectionForward(motorDirection);
  sendToDirectionMotor();
}

void turnRight()
{
  setMotorDirectionBackward(motorDirection);
  sendToDirectionMotor();
}
