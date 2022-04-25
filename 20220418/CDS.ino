
int cds = A0;

int led = 3;

void setup() {
  Serial.begin(9600);
  pinMode(3, OUTPUT);
}

void loop() {
  int cdsValue = analogRead(cds);

  Serial.print("cds =  ");
  Serial.println(cdsValue);

  if (cdsValue > 100) {
    digitalWrite(led, HIGH);
    Serial.println("LED ON (cds > 100)");
  }
  else {
    digitalWrite(led, LOW);
    Serial.println("LED OFF (cds < 100)");
  }
  delay(200);
}
