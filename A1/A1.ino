#include <ESP8266WiFi.h>                                          //Include the WiFi library for the microcontroller.
#include <ESP8266HTTPClient.h>                                    //Include the client library for the microcontroller.
#include <ArduinoJson.h>                                          //Include the library to parse JSON.

const char* ssid = "YONG_2.4GHz";                                 //Create a constant character array for the WiFi name.
const char* pass = "4254028948";                                  //Create a constant character array for the WiFi password.

typedef struct {                                                  //Create a struct type.
  String text;                                                    //Put a string called text in the struct.
  String author;                                                  //Put a string called author in the struct.
} quoteData;                                                      //Name the struct quoteData.

quoteData randomQuote;                                            //Create an instance of quoteData struct called randomQuote.

typedef struct {                                                  //Create a struct type.
  String number;                                                  //Put a string called number in the struct.
  String text;                                                    //Put a string called text in the struct.
} factData;                                                       //Name the struct factData.

factData randomFact;                                              //Create an instance of factData struct called randomFact.

void setup() {
  Serial.begin(115200);                                           //Initialize the serial monitor at a rate of 115200 baud.
  delay(10);                                                      //Delay by 10 milliseconds.
  Serial.print("This board is running: ");                        //Print this in the serial monitor.
  Serial.println(F(__FILE__));                                    //Print the file name in the serial monitor.
  Serial.print("Compiled: ");                                     //Print this in the serial monitor.
  Serial.println(F(__DATE__ " " __TIME__));                       //Print the date and time that the file was uploaded.
  
  Serial.print("Connecting to "); Serial.println(ssid);           //Print this in the serial monitor.

  WiFi.mode(WIFI_STA);                                            //Connect to the WiFi in station mode.
  WiFi.begin(ssid, pass);                                         //Connect to the WiFi using the constant character arrays previously created.

  while (WiFi.status() != WL_CONNECTED) {                         //If the WiFi does not connect do this.
    delay(500);                                                   //Delay by 500 milliseconds.
    Serial.print(".");                                            //Print this in the serial monitor.
  }

  Serial.println();                                                                                 //Print this in the serial monitor.
  Serial.println("WiFi connected");                                                                 //Print this in the serial monitor.
  Serial.print("Your ESP has been assigned the internal IP address ");                              //Print this in the serial monitor.
  Serial.println(WiFi.localIP());                                                                   //Print the local IP address in the serial monitor.
  Serial.println();                                                                                 //Print this in the serial monitor.
  
  getQuote();                                                                                       //Call the getQuote function.

  Serial.println();                                                                                 //Print this in the serial monitor.
  Serial.println("RANDOM QUOTE");                                                                   //Print this in the serial monitor.
  Serial.println("\"" + randomQuote.text + "\"");                                                   //Retrieve the random quote and print this in the serial monitor.
  Serial.println(" – " + randomQuote.author);                                                       //Retrieve the author and print this in the serial monitor.
  Serial.println();                                                                                 //Print this in the serial monitor.
  
  getFact();                                                                                        //Call the getFact function.
  
  Serial.println("RANDOM NUMBER FACT");                                                             //Print this in the serial monitor.
  Serial.println("Number: " + randomFact.number);                                                   //Retrieve the random number and print this in the serial monitor.
  Serial.println("Fact: " + randomFact.text);                                                       //Retrieve the fact and print this in the serial monitor.
  Serial.println();                                                                                 //Print this in the serial monitor.

}

void loop() {
}

void getQuote() {                                                                                                     //Define the getQuote function.
  HTTPClient theClient;                                                                                               //Create a mini browser on the microcontroller.
  Serial.println("Making HTTP request to forismatic API");                                                            //Print this in the serial monitor.
  theClient.begin("http://api.forismatic.com/api/1.0/?method=getQuote&format=json&lang=en");                          //Open this link in the mini browser.
  int httpCode = theClient.GET();                                                                                     //Retrieve the HTTP code.

  if (httpCode > 0) {                                                                     //If the HTTP code is greater than 0, do this.
    if (httpCode == 200) {                                                                //If the HTTP code is 200, do this.
      Serial.println("Received HTTP QUOTE payload");                                      //Print this in the serial monitor.
      DynamicJsonBuffer jsonBuffer;                                                       //Create a dynamic JSON buffer.
      String payload = theClient.getString();                                             //Store what the mini browser gives back as one big string.
      Serial.println("Parsing QUOTE...");                                                 //Print this in the serial monitor.
      Serial.println();                                                                   //Print this in the serial monitor.
      JsonObject& root = jsonBuffer.parse(payload);                                       //Parse the string.

      if (!root.success()) {                                                              //If the parsing was not successful, do this.
        Serial.println("parseObject() failed");                                           //Print this in the serial monitor.
        Serial.println(payload);                                                          //Print the unparsed string in the serial monitor.
        return;                                                                           //Return nothing.
      }

      randomQuote.text = root["quoteText"].as<String>();                                  //Cast the quote text as a string and put it into the struct.
      randomQuote.author = root["quoteAuthor"].as<String>();                              //Cast the author as a string and put it into the struct.
      
    } else {                                                                              //If the HTTP code is not 200, do this.
      Serial.println("Something went wrong with connecting to the endpoint.");            //Print this in the serial monitor.
    }
  }
}

void getFact() {                                                                          //Define the getFact function.
  HTTPClient theClient;                                                                   //Create a mini browser on the microcontroller.
  Serial.println("Making HTTP request to Numbers API");                                   //Print this in the serial monitor.
  theClient.begin("http://numbersapi.com/random/trivia?json");                            //Open this link in the mini browser.
  int httpCode = theClient.GET();                                                         //Retrieve the HTTP code.

  if (httpCode > 0) {                                                                     //If the HTTP code is greater than 0, do this.
    if (httpCode == 200) {                                                                //If the HTTP code is 200, do this.
      Serial.println("Received HTTP FACT payload");                                       //Print this in the serial monitor.
      DynamicJsonBuffer jsonBuffer;                                                       //Create a dynamic JSON buffer.
      String payload = theClient.getString();                                             //Store what the mini browser gives back as one big string.
      Serial.println("Parsing FACT...");                                                  //Print this in the serial monitor.
      JsonObject& root = jsonBuffer.parse(payload);                                       //Parse the string.

      if (!root.success()) {                                                              //If the parsing was not successful, do this.
        Serial.println("parseObject() failed");                                           //Print this in the serial monitor.
        Serial.println(payload);                                                          //Print the unparsed string in the serial monitor.
        return;                                                                           //Return nothing.
      }

      randomFact.number = root["number"].as<String>();                                    //Cast the number as a string and put it into the struct.
      randomFact.text = root["text"].as<String>();                                        //Cast the fact text as a string and put it into the struct.
      
    } else {                                                                              //If the HTTP code is not 200, do this.
      Serial.println("Something went wrong with connecting to the endpoint.");            //Print this in the serial monitor.
    }
  }
}
