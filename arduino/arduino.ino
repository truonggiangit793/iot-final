#include <SPI.h>
#include <MFRC522.h>
#include <SoftwareSerial.h>

#define BAUD_RATE 115200
#define SS_PIN 10
#define RST_PIN 9
#define SIGNAL_LED 8
#define BUZZER 7
#define SERVO 6
#define ESP_RX_PIN 3
#define ESP_TX_PIN 5

SoftwareSerial espSerial(ESP_TX_PIN, ESP_RX_PIN);
MFRC522 rfid(SS_PIN, RST_PIN); // Instance of the class

MFRC522::MIFARE_Key key;

// Init array that will store new NUID 
String UIDPICC = "";
byte UID[4];
// Init KEY_CARD 
int KEY_CARD[4] = {163, 35, 126, 163};
//Init access temp
int access_tmp = 0;

void setup() { 
  access_tmp = 0;
  Serial.begin(BAUD_RATE);
  espSerial.begin(BAUD_RATE);
  pinMode(SIGNAL_LED, OUTPUT);
  pinMode(BUZZER, OUTPUT);
  pinMode(SERVO, OUTPUT);
  SPI.begin(); // Init SPI bus
  rfid.PCD_Init(); // Init MFRC522
}

void LedSignal(){
  digitalWrite(SIGNAL_LED, HIGH);
  delay(300);
  digitalWrite(SIGNAL_LED, LOW);
}

void ServoTrigger(){
  BUZZER_OFF();
  digitalWrite(SERVO, HIGH);
  delay(3000);
  digitalWrite(SERVO, LOW);
}

void BUZZER_ON(){
  digitalWrite(BUZZER, HIGH);
}

void BUZZER_OFF(){
  digitalWrite(BUZZER, LOW);
}

void checkValidUID(){
  if (UID[0] == KEY_CARD[0] && UID[1] == KEY_CARD[1] && UID[2] == KEY_CARD[2] && UID[3] == KEY_CARD[3]) {
    access_tmp = 0;
    espSerial.print("?uid="+UIDPICC+"&status=1");
    ServoTrigger();
  } else {
    espSerial.print("?uid="+UIDPICC+"&status=0");
    access_tmp ++;
  };
  Serial.print("Access times: ");
  Serial.println(access_tmp);
}

void checkAccess(){
  if (access_tmp >= 3) {
    BUZZER_ON();
  } else {
    BUZZER_OFF();
  }
}

void RFIDReader(){
  UIDPICC = "";
  // Reset the loop if no new card present on the sensor/reader. 
  // This saves the entire process when idle.
  if ( ! rfid.PICC_IsNewCardPresent()) return;
  // Verify if the NUID has been readed
  if ( ! rfid.PICC_ReadCardSerial()) return;

  // Store NUID into nuidPICC array
  for (byte i = 0; i < 4; i++) {
    UID[i] = rfid.uid.uidByte[i];
    UIDPICC.concat(String(rfid.uid.uidByte[i],HEX));
  };
  
  // Turn LED Signal on and print UID on Serial
  LedSignal();
  printDec(rfid.uid.uidByte, rfid.uid.size);
  Serial.println();
  Serial.println(UIDPICC);

  // Check whether an UID is valid or not
  checkValidUID();

  // Halt PICC
  rfid.PICC_HaltA();
  // Stop encryption on PCD
  rfid.PCD_StopCrypto1();
}

// LOOP main
void loop() {
  RFIDReader();
  // Check time access dennied
  checkAccess();
}

// Helper routine to dump a byte array as HEX values to Serial.
void printHex(byte * buffer, byte bufferSize) {
  for (byte i = 0; i < bufferSize; i++) {
    Serial.print(buffer[i] < 0x10 ? " 0" : " ");
    Serial.print(buffer[i], HEX);
  }
}

// Helper routine to dump a byte array as DEC values to Serial.
void printDec(byte * buffer, byte bufferSize) {
  for (byte i = 0; i < bufferSize; i++) {
    Serial.print(buffer[i] < 0x10 ? " 0" : " ");
    Serial.print(buffer[i], DEC);
  }
}
