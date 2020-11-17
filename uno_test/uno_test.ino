/* This code works with MLX90614 (GY906) and LCD ic screen
 * It measures both ambient and object temperature in Celsius and display it on the screen
 * Please visit www.surtrtech.com for more details
 */
#include <Adafruit_MLX90614.h>

Adafruit_MLX90614 mlx = Adafruit_MLX90614();

void setup() {

  Serial.begin(9600);
  Serial.println("Start IR Monitor");
  mlx.begin();

}

void loop() {
 Serial.print(String(mlx.readAmbientTempF(),2)+" "+String(mlx.readObjectTempF(),2));
 Serial.println();
 delay(1000);

}
