/* Author: Devon Friend
 * Date: May 1, 2020
 *
 * This is a program for a mini security system using an arduino board. The user is prompted to enter the
 * password, and if they are correct, a green LED will turn on. Otherwise, the red LED remains on and the
 * user gets a "access denied" message to indicate that they were incorrect. The user may try again until
 * they are successful, and when they are they get a "access granted" message and the green LED turns on
 * and the red LED turns off. The user is then prompted to continue by pressing any key on the keypad and
 * the program starts again.
 *
 * The following youtube videos were used to help me initialize the keypad and LCD screen, as well as some of the
 * circuit schematics:
 *
 * LCD Screen: https://youtu.be/dZZynJLmTn8
 * Keypad: https://youtu.be/ZRNttZgPUzI
 *
 */

 /* *********Libraries and Constants********** */

#include <Arduino.h>
#include <LiquidCrystal.h>
#include <Keypad.h>

#define TRUE 1
#define FALSE 0

#define GREEN_LED_PIN 11
#define RED_LED_PIN 13

/* **************LCD Variables*************** */

//Pins for LCD
LiquidCrystal lcd(A0, A1, A2, A3, A4, A5);

/* *************Keypad Variables************* */

const byte ROWS = 4; //Number of rows
const byte COLS = 4; //Number of columns
char keys[ROWS][COLS] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};

byte rowPins[ROWS] = { 53, 49, 45, 41 }; //Connects the row pinouts of the keypad
byte colPins[COLS] = { 37, 33, 29, 25 }; //Connects the column pinouts of the keypad

Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);

/* *************Password**************** */

char password[] = { 'A', '9', 'D', '4', '5', '0', '\0' };

/* **********Function Prototypes************ */

int passwordLength(char);
int checkPassword(char, int);
void correct(void);
void tryAgain(void);

void setup() {

	//Turns on LCD screen
	lcd.begin(16, 2);

	//Input from the keypad in displayed on the serial monitor
	Serial.begin(9600);

	//LED initialization 
	pinMode(GREEN_LED_PIN, OUTPUT);
	pinMode(RED_LED_PIN, OUTPUT);

}

void loop() {
	int LENGTH = passwordLength(password);

	digitalWrite(RED_LED_PIN, HIGH);

	if (checkPassword(password, LENGTH)) {
		correct();
	} else {
		tryAgain();
	}
}

/* This method determines the length of the password.
 *
 * @param password: A character array representing the password.
 *
 * @return: An integer value i representing the length of
 *          password such that i >= 0.
 */
int passwordLength(char password[]) {
	int len = 0;

	while (password[len] != '\0') {
		len++;
	}
	return len;
}

/* This method verifies if the user input of the password is correct.
 *
 * @param password: A character array representing the password.
 * @param LENGTH: An integer value representing the length of the password.
 *
 * @return: Returns TRUE (1) if the user is correct and FALSE (0) if the user is incorrect
 */
int checkPassword(char password[], int LENGTH) {
	int index = 0;
	char key;
	char guess[LENGTH];

	lcd.clear();
	lcd.print("Password:");
	lcd.setCursor(0, 1);
	lcd.blink();

	//Continuously checks if a key is pressed then stores the input and prints it on the LCD screen
	while (index < LENGTH) {
		key = keypad.getKey();

		if (key != NO_KEY) {
			guess[index] = key;
			index += 1;
			lcd.print(key);
		}
	}
	delay(1000);

	//Verifies the password
	for (index = 0; index < LENGTH; index++) {
		if (guess[index] != password[index]) {
			return FALSE;
		}
	}
	return TRUE;
}

/* 
 * This method displays a message on the LCD screen to indicate a correct password input.
 */
void correct(void) {
	char key;

	digitalWrite(RED_LED_PIN, LOW);
	digitalWrite(GREEN_LED_PIN, HIGH);

	lcd.clear();
	lcd.print("Access Granted");
	delay(3000);

	lcd.clear();
	lcd.print("Press any key");
	lcd.setCursor(0, 1);
	lcd.print("to continue");

	while (TRUE) {
		key = keypad.getKey();
		if (key) {
			lcd.clear();
			digitalWrite(GREEN_LED_PIN, LOW);
			return;
		}
	}
}

/* 
 * This method displays a message on the LCD screen to indicate an incorrect password input.
 */
void tryAgain(void) {
	lcd.clear();
	
	lcd.print("Access Denied");
	delay(3000);

	lcd.clear();

	return;
}