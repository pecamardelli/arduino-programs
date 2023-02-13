# 1 "/home/pablin/arduino_programs/relayTimer/relayTimer.ino"

# 3 "/home/pablin/arduino_programs/relayTimer/relayTimer.ino" 2

System sys;

void setup()
{
    Serial.begin(9600);
    Serial.println("Hello World");
}

void loop()
{
    if (Serial.available())
    {
        // sys.getSerialInput();
    }
}
