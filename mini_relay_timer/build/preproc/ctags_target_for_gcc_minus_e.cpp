# 1 "/home/pablin/arduino_programs/mini_relay_timer/mini_relay_timer.ino"
# 2 "/home/pablin/arduino_programs/mini_relay_timer/mini_relay_timer.ino" 2

Commander commander;
Clock clock;
Relay relays;

void setup()
{
  Serial.begin(9600);
  Serial.println((reinterpret_cast<const __FlashStringHelper *>(
# 10 "/home/pablin/arduino_programs/mini_relay_timer/mini_relay_timer.ino" 3
                (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 10 "/home/pablin/arduino_programs/mini_relay_timer/mini_relay_timer.ino"
                "MINI RELAY TIMER STARTED"
# 10 "/home/pablin/arduino_programs/mini_relay_timer/mini_relay_timer.ino" 3
                ); &__c[0];}))
# 10 "/home/pablin/arduino_programs/mini_relay_timer/mini_relay_timer.ino"
                )));

  Wire.begin();
  clock.begin();
  relays.begin();
}

void loop()
{
  if (Serial.available())
  {
    String input = commander.getInput();
    commander.exec(input);
  }

  relays.relay_check();
  relays.relay_watch();
}
