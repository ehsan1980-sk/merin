// -------------------------------------------------
// Copyright (c) 2023 HiBit <https://www.hibit.dev>
// -------------------------------------------------

void hornOn()
{
  tone(BUZZER_PIN, 1000); //Send 1KHz sound signal
}

void hornOff()
{
  noTone(BUZZER_PIN); //Stop sound
}
