// -------------------------------------------------
// Copyright (c) 2023 HiBit <https://www.hibit.dev>
// -------------------------------------------------

void sendToSpeedMotor()
{
  sendToMotor(motorSpeed, L298N_ENA_PIN, L298N_IN1_PIN, L298N_IN2_PIN);
}

void sendToDirectionMotor()
{
  sendToMotor(motorDirection, L298N_IN3_PIN, L298N_IN4_PIN);
}
