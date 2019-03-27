//L293D
//Motor A
const int motorPin1  = 5;  // Pin 14 of L293
const int motorPin2  = 6;  // Pin 10 of L293
//Motor B.
const int motorPin3  = 11; // Pin  7 of L293
const int motorPin4  = 10;  // Pin  2 of L293

const int LeftButt = 12;
const int RightButt = 3;

int stateRunning = 1;

void LeftForward(){
   digitalWrite(motorPin1, HIGH);
   digitalWrite(motorPin2, LOW);
   digitalWrite(motorPin3, LOW);
   digitalWrite(motorPin4, LOW);
}

void LeftBackwords(){
   digitalWrite(motorPin1, LOW);
   digitalWrite(motorPin2, HIGH);
   digitalWrite(motorPin3, LOW);
   digitalWrite(motorPin4, LOW);
    
}

void RightForward(){
   digitalWrite(motorPin1, LOW);
   digitalWrite(motorPin2, LOW);
   digitalWrite(motorPin3, HIGH);
   digitalWrite(motorPin4, LOW); 
}

void RightBackwards(){
    digitalWrite(motorPin1, LOW);
    digitalWrite(motorPin2, LOW);
    digitalWrite(motorPin3, LOW);
    digitalWrite(motorPin4, HIGH);
}

void StopBoth(){
    digitalWrite(motorPin1, LOW);
    digitalWrite(motorPin2, LOW);
    digitalWrite(motorPin3, LOW);
    digitalWrite(motorPin4, LOW);
}

void BothForward(){
   digitalWrite(motorPin1, HIGH);
   digitalWrite(motorPin2, LOW);
   
   digitalWrite(motorPin3, LOW);
   digitalWrite(motorPin4, HIGH);
}

void BothBackwards(){
   digitalWrite(motorPin1, LOW);
   digitalWrite(motorPin2, HIGH);
   
   digitalWrite(motorPin3, HIGH);
   digitalWrite(motorPin4, LOW);
}

void TurnLeft(){
   digitalWrite(motorPin1, HIGH);
   digitalWrite(motorPin2, LOW);
   
   digitalWrite(motorPin3, HIGH);
   digitalWrite(motorPin4, LOW);
}

void TurnRight(){
   digitalWrite(motorPin1, LOW);
   digitalWrite(motorPin2, HIGH);
   
   digitalWrite(motorPin3, LOW);
   digitalWrite(motorPin4, HIGH);
}

void setup(){
 
    //Set pins as outputs
    pinMode(motorPin1, OUTPUT);
    pinMode(motorPin2, OUTPUT);
    pinMode(motorPin3, OUTPUT);
    pinMode(motorPin4, OUTPUT);
    
    Serial.begin(9600);
    
    pinMode(LeftButt, INPUT_PULLUP);
    pinMode(RightButt, INPUT_PULLUP);
    
    BothForward();
}


void loop(){
    int stateLeft = digitalRead(LeftButt);
    int stateRight = digitalRead(RightButt);

    if(stateRunning){
    
    if(stateLeft == LOW){
      if(stateRight == LOW) {
        StopBoth();
        stateRunning = 0;
      }else{
      BothBackwards();
      delay(1000);
      TurnRight();
      delay(500);
      BothForward();
      }
    } else if(stateRight == LOW){

      if(stateLeft == LOW) {
        StopBoth();
        stateRunning = 0;
      }else{
      BothBackwards();
      delay(500);
      TurnLeft();
      delay(300);
      BothForward();
      }
    }
    }
    delay(50);
}
