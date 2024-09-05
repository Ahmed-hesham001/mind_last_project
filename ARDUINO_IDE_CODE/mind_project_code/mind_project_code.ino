bool door_state = false;
bool face_rec = false;
bool password = false;
bool door_key = false;
int choice = 0;

void setup() {
  // put your setup code here, to run once:
}

void loop() {
  // put your main code here, to run repeatedly:
  if (face_rec) {
    door_state = true;
  } else {
    //3 choices
    //1)open the door and add face to data base
    //2)just open the door
    //3)add password
    //get choice from serial monitor
    switch (choice) {
      case 1:
        //add face to database
        door_state = true;
        break;
      case 2:
        //just open the door
        door_state = true;
        break;
      case 3:
        //read password
        if (password) {
          //open the door
          door_state = true;
        } else {
          //buzzer and warning message
          door_state = false;
        }
        break;
    }
  }

  if (door_state){
    //open the door and start all functions
  }
  else {
    //pir sensor code
  }
}