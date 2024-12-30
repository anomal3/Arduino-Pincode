//Работа с циферблатом и паролем
#include <Keypad.h>

 //digitalWrite(LED_BUILTIN, HIGH);
 //pinMode(LED_BUILTIN, OUTPUT);
const byte ROWS = 4; //four rows
const byte COLS = 3; //three columns
char keys[ROWS][COLS] = {
	{'1','2','3'},
	{'4','5','6'},
	{'7','8','9'},
	{'*','0','#'}
};

byte rowPins[ROWS] = { D1, D2, D3, D4 }; // сюда ряды
byte colPins[COLS] = { 14, 12, 13 }; // сюда столбцы

Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);
byte ledPin = D8;
byte buzze = D0;

boolean blink = false;
boolean ledPin_state;
const String pass = "1829921230";
String stringPasswd = "";
bool SuccessPass = false;

void setup() {
	Serial.begin(9600);
	pinMode(ledPin, OUTPUT);              // Sets the digital pin as output.
	pinMode(buzze, OUTPUT);
	digitalWrite(ledPin, LOW);           // Turn the LED on.
	ledPin_state = digitalRead(ledPin);   // Store initial LED state. HIGH when LED is on.
	keypad.addEventListener(keypadEvent); // Add an event listener for this keypad
}

void loop() {
	char key = keypad.getKey();

	if (blink) {
		digitalWrite(ledPin, !digitalRead(ledPin));    // Change the ledPin from Hi2Lo or Lo2Hi.
		delay(100);
	}
}

// Taking care of some special events.
void keypadEvent(KeypadEvent key) {
	switch (keypad.getState()) {
	case PRESSED:
		if (key == '#') {
			if (stringPasswd == pass) {
				digitalWrite(ledPin, !digitalRead(ledPin));
				ledPin_state = digitalRead(ledPin);        // Remember LED state, lit or unlit.
			}
			else {
				blink = true;
				tone(buzze, 2000);
				delay(750);
				noTone(buzze); 
			}
		}
		else if (key == '*') {
			blink = false;
			ledPin_state = false;
			// Если нажата клавиша *, очищаем строку
			Serial.println("Clear pass " + stringPasswd);
			stringPasswd = "";

		}
		else {
			tone(buzze, 1000);
			// Добавляем символ к строке
			stringPasswd += key;
			Serial.println("Add symbols: " + String(key));
			delay(100);        // ...for 1 sec
			noTone(buzze);     // Stop sound...
		}
		break;

	case RELEASED:
		if (key == '*') {
			digitalWrite(ledPin, ledPin_state);    // Restore LED state from before it started blinking.
			blink = false;
		}
		break;
	case HOLD:
		if (key == '*') {
			blink = true;    // Blink the LED when holding the * key.
		}
		break;
	}
}