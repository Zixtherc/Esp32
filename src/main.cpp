#include <Arduino.h>

using namespace std;
#define BUZZER_PIN 15

struct Morse{
  char letter;
  string morseCode;
};

Morse alphabet[] = {
    {'A', ".-"},    {'B', "-..."},  {'C', "-.-."},  {'D', "-.."},
    {'E', "."},     {'F', "..-."},  {'G', "--."},   {'H', "...."},
    {'I', ".."},    {'J', ".---"},  {'K', "-.-"},   {'L', ".-.."},
    {'M', "--"},    {'N', "-."},    {'O', "---"},   {'P', ".--."},
    {'Q', "--.-"},  {'R', ".-."},   {'S', "..."},   {'T', "-"},
    {'U', "..-"},   {'V', "...-"},  {'W', ".--"},   {'X', "-..-"},
    {'Y', "-.--"},  {'Z', "--.."},

    {'0', "-----"}, {'1', ".----"}, {'2', "..---"}, {'3', "...--"},
    {'4', "....-"}, {'5', "....."}, {'6', "-...."}, {'7', "--..."},
    {'8', "---.."}, {'9', "----."}
};
const int alphabetSize = sizeof(alphabet) / sizeof(alphabet[0]);

void textToMorse(string text);
void morseToText(string morseCode);
void beepBuzz(string morse);

void setup() {
  Serial.begin(115200);
  Serial.println("ESP32 Started!");
  pinMode(BUZZER_PIN, OUTPUT);
  digitalWrite(BUZZER_PIN,HIGH);
}

void loop() {
  static String inputArduino = "";
  
  while (Serial.available() > 0){
    char c = Serial.read();

    if(c == '\n' || c == '\r'){
      inputArduino.trim();
      if (inputArduino.length() > 0) {
        string input = inputArduino.c_str();

        Serial.print("Вы ввели: ");
        Serial.println(input.c_str());

        if (input[0] == '.' || input[0] == '-') {
          morseToText(input);
          beepBuzz(input);
        } 
        else {
          textToMorse(input);
        }

        Serial.println("============================");
        
        inputArduino = "";
      }
    } 
    else {
      inputArduino += c;
    }
  }
} 

void textToMorse(string text){
  string morseCode = "";
  for(int i = 0; i < text.length(); i++){
    char c = toupper(text[i]);
    if(c == ' '){
        morseCode += "/ ";
        continue;
      }
    for(int j = 0; j < alphabetSize; j++){
      if(alphabet[j].letter == c){
        morseCode += alphabet[j].morseCode + " ";
        break;
      }
    }
  }
  Serial.println(morseCode.c_str());
}

void morseToText(string morse){
  string fullText = "";
  string currSymbol = "";
  for(int i = 0; i < morse.length(); i++){
    if(morse[i] != ' '){
      currSymbol += morse[i];
    }
    if (morse[i] == ' ' || i == morse.length() - 1){
      if(currSymbol == "/"){
        fullText += " ";
      }
      else if(currSymbol.length() > 0){
        for(int j = 0; j < alphabetSize; j++){
          if(alphabet[j].morseCode == currSymbol){
            fullText += alphabet[j].letter;
            break;
          }
        }
      }
      currSymbol = "";
    }
  }
  Serial.print("Перевод: ");
  Serial.println(fullText.c_str());
}

void beepBuzz(string morse){
  int dot = 100;
  int dash = 300;
  int pause = 100;
  int letterPause = 200; 
  int wordsPause = 600;

  for(int i = 0; i < morse.length(); i++){
    if(morse[i] == '.'){
      digitalWrite(BUZZER_PIN, LOW);
      delay(dot);
      digitalWrite(BUZZER_PIN, HIGH);
      delay(pause);
    }
    else if(morse[i] == '-'){
      digitalWrite(BUZZER_PIN, LOW);
      delay(dash);
      digitalWrite(BUZZER_PIN, HIGH);
      delay(pause);
    }
    else if(morse[i] == ' '){
      delay(letterPause);
    }
    else if(morse[i] == '/'){
      delay(wordsPause);
    }
  }
}