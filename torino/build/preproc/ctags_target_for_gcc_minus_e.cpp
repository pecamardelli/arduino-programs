# 1 "/home/pablin/arduino-programs/torino/torino.ino"
# 2 "/home/pablin/arduino-programs/torino/torino.ino" 2

Commander commander;

void setup()
{
    Serial.begin(9600);
    Serial.println("Initializing...");
    initScreen();
}

void loop()
{
    if (Serial.available())
    {
        String input = commander.getInput();
        commander.parse(input);
    }
}
