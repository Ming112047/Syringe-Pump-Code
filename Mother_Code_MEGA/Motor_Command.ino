double DoRoll (long int a, double b, int Direction) { // a is step

    if (Direction == 0) {
      digitalWrite(dirPin1, LOW); //forward
    }

    if (Direction == 1) {
      digitalWrite(dirPin1, HIGH); //backward
      b = 1;
    }
    
  STIME = micros();//oled.clear();oled.println(TimeStepDelay);oled.println(b);
  for (long j = 1; j <= a; j++){

    digitalWrite(stepPin1, HIGH);
    delayMicroseconds(b);         
    digitalWrite(stepPin1, LOW);
    delayMicroseconds(b);

  }ETIME = micros();
  //TimeSpeed = 0;
  EnterDataState = 6;
}
