#include <Arduino.h>
#include <SoftwareSerial.h>
#include <stdio.h>
#include <ThinkGearStreamParser.h>

#define LED D5
#define RXPIN D7

SoftwareSerial MYSERIAL(RXPIN, D8); //UART
unsigned char streamByte;
ThinkGearStreamParser parser;

void handleDataValueFunc(
    unsigned char extendedCodeLevel,
    unsigned char code,
    unsigned char valueLength,
    const unsigned char *value,
    void *customData)
{
    if (extendedCodeLevel == 0)
    {
        switch (code)
        {
        case (0x02):
            Serial.printf("Poor Signal. Quality: %d\n", value[0] & 0xFF);
            break;
        /* [CODE]: ATTENTION eSense */
        case (0x04):
            Serial.printf("Attention Level: %d\n", value[0] & 0xFF);
            break;
            /* [CODE]: MEDITATION eSense */
        case (0x05):
            Serial.printf("Meditation Level: %d\n", value[0] & 0xFF);
            break;
            /* Other [CODE]s */
        default:
            Serial.printf("\nEXCODE level: %d CODE: 0x%02X vLength: %d\n", extendedCodeLevel, code, valueLength);
            Serial.printf("Data value(start):[");
            for (int i = 0; i < valueLength; i++)
                Serial.printf("%02X ", value[i] & 0xFF);
            Serial.printf("]Data value(end)");
            Serial.printf("\n\n");
        }
    }
}

void setup()
{
    // put your setup code here, to run once:
    pinMode(LED, OUTPUT);
    pinMode(RXPIN, INPUT);

    Serial.begin(115200);
    MYSERIAL.begin(57600);
    Serial.println("Program Started");

    digitalWrite(LED, HIGH);

    THINKGEAR_initParser(&parser, PARSER_TYPE_PACKETS, handleDataValueFunc, NULL);
}

void loop()
{
    while (MYSERIAL.available())
    {
        streamByte = MYSERIAL.read(); // If anything comes in Serial1 (pins 0 & 1)
                                      //Serial.print(streamByte, HEX); // read it and send it out Serial (USB)
                                      // Serial.print(" ");

        // THINKGEAR_parseByte(&parser, streamByte);
        THINKGEAR_parseByte(&parser, streamByte);
    }
}