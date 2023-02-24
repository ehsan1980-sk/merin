// -------------------------------------------------
// Copyright (c) 2023 HiBit <https://www.hibit.dev>
// -------------------------------------------------

void lightsOn()
{
  headlightsOn();
  brakelightsOn(100);
}

void lightsOff()
{
  headlightsOff();
  brakelightsOff();
}

void headlightsOn()
{
  digitalWrite(HEADLIGHT_PIN , HIGH);
}

void headlightsOff()
{
  digitalWrite(HEADLIGHT_PIN , LOW);
}

void brakelightsOn()
{
  brakelightsOn(255);
}

void brakelightsOn(int intensity)
{
  analogWrite(BACKLIGHT_PIN , intensity);
}

void brakelightsOff()
{
  analogWrite(BACKLIGHT_PIN , 0);
}
