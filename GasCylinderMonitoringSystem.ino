// C++ code
//

#include <LiquidCrystal.h>

// initializations
LiquidCrystal lcd(2, 3, 4, 5, 6, 7);
const int animationSpeed = 1000;
int buzzerPin = 10, trigPin = 8, echoPin = 9;
const int RED = 11, GREEN = 13, BLUE = 12;
int redValue, greenValue, blueValue;
int soundSpeed = 34000; // speed in cm/s

void setup()
{
    pinMode(buzzerPin, OUTPUT);
    pinMode(trigPin, OUTPUT);
    pinMode(echoPin, INPUT);
    pinMode(GREEN, OUTPUT);
    pinMode(BLUE, OUTPUT);
    pinMode(RED, OUTPUT);

    Serial.begin(9600);
    lcd.begin(16, 2);
}

// this method changes rgb color based on
// gas level modeled by ultrasonic sensor
void lightRGBBasedOnModeledGaslevel(int distance)
{
    // map the distance to rgb colors
    int B = map(distance, 0, -308, 0, 255);

    if (distance >= -100)
    {
        // Setting Green to be the color of choice
        // Setting brightness to 55 + light insensity
        int G = map(distance, 0, -100, 0, 255);
        redValue = 0;
        greenValue = G;
        blueValue = 0;
        analogWrite(RED, redValue);
        analogWrite(GREEN, greenValue);
        analogWrite(BLUE, blueValue);
    }
    else if (distance < -100 && distance >= -150)
    {
        // Using a mix of green and blue, starting off
        // with a high Green intensity
        // Slowly increasing blue, while reducing green
        // and maintaining RED as zero
        int B = map(distance, -100, -150, 0, 255);
        redValue = 0;
        greenValue = 255 - B;
        blueValue = B;
        analogWrite(RED, redValue);
        analogWrite(GREEN, greenValue);
        analogWrite(BLUE, blueValue);
    }
    else
    {
        // Using a mix of green and blue, starting off
        // with a high Green intensity
        // Slowly increasing blue, while reducing green
        // and maintaining RED as zero
        int R = map(distance, -150, -308, 0, 255);
        redValue = R;
        greenValue = 0;
        blueValue = 255 - R;
        analogWrite(RED, redValue);
        analogWrite(GREEN, greenValue);
        analogWrite(BLUE, blueValue);
    }

    displayColorValues(redValue, greenValue, blueValue);
    delay(animationSpeed);
}

// this method lights rgb based on gas value
void lightRGBBasedOnActualGasValue(float gasValue)
{
    // map the gas value to rgb colors
    int B = map(gasValue, 85, 375, 0, 255);

    if (gasValue >= 275)
    {
        // Setting Green to be the color of choice
        // Setting brightness to 55 + light insensity
        int G = map(gasValue, 275, 375, 0, 255);
        redValue = 0;
        greenValue = G;
        blueValue = 0;
        analogWrite(RED, redValue);
        analogWrite(GREEN, greenValue);
        analogWrite(BLUE, blueValue);
    }
    else if (gasValue < 275 && gasValue >= 180)
    {
        // Using a mix of green and blue, starting off
        // with a high Green intensity
        // Slowly increasing blue, while reducing green
        // and maintaining RED as zero
        int B = map(gasValue, 180, 275, 0, 255);
        redValue = 0;
        greenValue = 255 - B;
        blueValue = B;
        analogWrite(RED, redValue);
        analogWrite(GREEN, greenValue);
        analogWrite(BLUE, blueValue);
    }
    else
    {
        // Using a mix of green and blue, starting off
        // with a high Green intensity
        // Slowly increasing blue, while reducing green
        // and maintaining RED as zero
        int R = map(gasValue, 85, 180, 0, 255);
        redValue = R;
        greenValue = 0;
        blueValue = 255 - R;
        analogWrite(RED, redValue);
        analogWrite(GREEN, greenValue);
        analogWrite(BLUE, blueValue);
    }

    displayColorValues(redValue, greenValue, blueValue);
    delay(animationSpeed);
}

// this method formats the display of the rgb color
// referenced by the modeled gas level
void displayColorValues(int redValue, int greenValue, int blueValue)
{
    // Print the values to the Serial Monitor
    Serial.print("color code= (");
    Serial.print(redValue);
    Serial.print(",");
    Serial.print(greenValue);
    Serial.print(",");
    Serial.print(blueValue);
    Serial.println(")");
}

// this method sounds the buzzer based on frequency of gas value
void soundBuzzer(int buzzerPin, int gasValue)
{
    // tone buzzer based on gas value
    tone(buzzerPin, gasValue);
    delay(gasValue);
    noTone(buzzerPin);

    delay(animationSpeed);
}

// this method reads the actual gas level from gas sensor
void getActualGasLevel(float gasValue)
{
    // low gas
    if (gasValue >= 85 && gasValue < 230)
    {
        lcd.setCursor(0, 0);
        lcd.print("Actual gas level: "); // No extra space after "at"
        lcd.print(gasValue);             // Print the actual distance value
        lcd.scrollDisplayLeft();
        lcd.print(" mg. Gas is low!!!"); // Add units and exclamation mark

        // sound buzzer frequency based on gas value
        soundBuzzer(buzzerPin, gasValue);
    }

    // enough gas
    else
    {
        lcd.setCursor(0, 0);
        lcd.print("Actual gas level: "); // No extra space after "at"
        lcd.print(gasValue);             // Print the actual distance value
        lcd.scrollDisplayLeft();
        lcd.print(" mg"); // Add units and exclamation mark
    }
}

// this method reads the modeled gas level from ultrasonic sensor
void getModeledGasLevel(int distance)
{
    if (distance < -100)
    {
        // tone sound buzzer a certain freq
        tone(buzzerPin, animationSpeed);
        delay(500);
        noTone(buzzerPin);

        lcd.setCursor(0, 0);
        lcd.print("Gas is low!!! Gas is at "); // No extra space after "at"
        lcd.print(distance);                   // Print the actual distance value
        lcd.scrollDisplayLeft();
        lcd.print(" cm"); // Add units and exclamation mark
    }

    else
    {
        lcd.setCursor(0, 0);
        lcd.print("Gas is at "); // No extra space after "at"
        lcd.print(distance);     // Print the actual distance value
        lcd.scrollDisplayLeft();
        lcd.print(" cm"); // Add units and exclamation mark
    }

    delay(animationSpeed / 1000);
}

// this method sends the real time gas data
// to the bluetooth connected device when prompted
void sendData(float gasValue)
{
    // low gas
    if (gasValue >= 85 && gasValue < 230)
    {
        Serial.print("Gas level: ");
        Serial.println(gasValue);
        Serial.println(" mg. ");
        Serial.println("Plan for a refill soon!!!");
    }

    // enough gas
    else
    {
        Serial.print("Gas level: "); // No extra space after "at"
        Serial.println(gasValue);    // Print the actual distance value
        Serial.println(" mg");       // Add units and exclamation mark
    }
}

void loop()
{
    // Send a 10 microsecond pulse to trigger the ultrasonic sensor
    digitalWrite(trigPin, LOW);
    delayMicroseconds(2);
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);

    // calculate time in microseconds
    long timeTaken = pulseIn(echoPin, HIGH);

    // calculate distance in cm
    int distance = (soundSpeed * timeTaken * 1e-6) / 2;

    // read gas value from gas sensor
    float gasValue = analogRead(A0);

    String choice = "actual";

    if (choice == "modeled")
    {
        // display modeled gas levels at regular intervals
        getModeledGasLevel(distance);

        // light rgb based on distance
        lightRGBBasedOnModeledGaslevel(distance);
    }
    else if (choice == "actual")
    {
        // actual gas level is part of data
        getActualGasLevel(gasValue);

        // light rgb based on distance
        lightRGBBasedOnActualGasValue(gasValue);
    }

    if (Serial.available() > 0)
    {
        // read serial input
        String data = Serial.readString();

        // Convert data to lowercase for case insensitive comparison
        data.toLowerCase();

        if (data == "off")
        {
            // mute the buzzer for 10 seconds
            noTone(buzzerPin);
            delay(animationSpeed * 10);
        }

        else if (data == "send")
        {
            // send the data to bluetooth connected device when prompted
            sendData(gasValue);
            Serial.println();
        }
    }
}
