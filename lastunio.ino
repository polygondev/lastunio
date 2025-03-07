// LASTUINO - Code to upload to Arduino

#include <Wire.h>
#include <LiquidCrystal.h>

// Initialize LCD (DFRobot LCD Keypad Shield uses these pins)
LiquidCrystal lcd(8, 9, 4, 5, 6, 7);

String songName = "";
String artistAlbum = "";
unsigned long scrollTimer = 0;
int scrollIndex = 0;

void setup() {
    Serial.begin(9600);
    lcd.begin(16, 2);
    lcd.print("Waiting for data");
}

void loop() {
    if (Serial.available()) {
        String data = "";

        // Read incoming serial data
        while (Serial.available()) {
            char c = Serial.read();
            data += c;
            delay(5);
        }

        // Parse received data: "Song Name|Artist|Album"
        int firstSplit = data.indexOf("|");
        int secondSplit = data.indexOf("|", firstSplit + 1);

        if (firstSplit > 0 && secondSplit > firstSplit) {
            songName = data.substring(0, firstSplit);
            String artist = data.substring(firstSplit + 1, secondSplit);
            String album = data.substring(secondSplit + 1);

            artistAlbum = artist + " - " + album; // Combine for bottom row
            scrollIndex = 0; // Reset scrolling
        }

        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print(songName.substring(0, 16)); // Top line (song name)
    }

    // Handle scrolling for artist + album if it's too long
    if (artistAlbum.length() > 16) {
        if (millis() - scrollTimer > 500) { // Scroll every 500ms
            scrollTimer = millis();
            scrollIndex++;
            if (scrollIndex > artistAlbum.length() - 16) {
                scrollIndex = 0; // Reset scrolling
            }
            lcd.setCursor(0, 1);
            lcd.print(artistAlbum.substring(scrollIndex, scrollIndex + 16));
        }
    } else {
        lcd.setCursor(0, 1);
        lcd.print(artistAlbum); // No scrolling needed
    }
}
