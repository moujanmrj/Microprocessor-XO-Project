#include <LiquidCrystal.h>
#include <Keypad.h>
const int rs = 13, en = 8, d4 = 12, d5 = 11, d6 = 10, d7 = 9, ld = 22;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

//keypad
const byte ROWS = 4;
const byte COLS = 3;
char keys[ROWS][COLS] = {
  {'1','2','3'},
  {'4','5','6'},
  {'7','8','9'},
  {'*','0','#'}
};
byte rowPins[ROWS] = {A12, A11, A10, A9};
byte colPins[COLS] = {A13, A14, A15}; 
Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS);

char board[9];
boolean flag=true;
void setup(){
  digitalWrite(ld, 0);
  Serial.begin(9600);
  lcd.begin(16, 4);
  lcd.clear();
  for(int i=0;i<9;i++){
    board[i]='N';
  }
  printboard();
}

void loop(){    
  if(Serial.available()){
    char ch = Serial.read();
    switch(ch){
      case 'e':
        flag = false;
        printresult(0); 
        break;
      case 'x':
        flag = false;                   
        printresult(1);
        break;
      case 'o':
        flag = false;                    
        printresult(2);
        break;
      default:
        flag = true;
        int i = index(ch);
        if(board[i - 1] == 'N'){
          board[i - 1] = 'x';
          printboard();
        }
        break;
    }
  }     
  if(flag == true){
    char key = keypad.getKey();  
            if (key){
              if((key =='1' || key =='2' || key=='3' || key =='4' || key =='5' || key =='6' || key =='7' || key =='8'|| key =='9' )){  
                int i = index(key);
                if(board[i - 1] == 'N'){
                  board[i - 1] = 'o';
                  printboard();
                  flag = false;
                  char win = checkresult();
                  Serial.write(key); 
                  switch(win){
                    case 'e':
                      Serial.write('e');
                      printresult(0);
                      break;
                    case 'x':
                      Serial.write('x');
                      printresult(1);
                      break;
                    case 'o':
                      Serial.write('o');
                      printresult(2);
                      break;
                  }
              }
          }     
      }
   }
}

char checkresult(){
  char winner='n';
  boolean check=false;
  for (int i=0; i<3; i++){
    if(board[i] == board[i+3] && board[i] == board[i+6] && board[i] != 'N' && board[i+3] != 'N' && board[i+6] != 'N'){
      winner=board[0];
      check=true;
    }
  }
  for (int i=0; i<9; i=i+3){
    if(board[i] == board[i+1] && board[i] == board[i+2] && board[i] != 'N' && board[i+1] != 'N' && board[i+2] != 'N'){
      winner=board[0];
      check=true;
    }
  }
  if(board[0] == board[4] && board[0] == board[8] && board[0] != 'N'){
    winner=board[0];
    check=true;
  }if(board[2] == board[4] && board[2] == board[6] && board[2] != 'N'){
    winner=board[2];
    check=true;
  }if(board[0] != 'N'  && board[1] != 'N'  && board[2] != 'N' && board[3] != 'N' && board[4] != 'N' && board[5] != 'N' && board[6] != 'N' && board[7] != 'N' && board[8] != 'N' && check == false){
    winner='e';
  }else if(check==false){
    winner='n';
  }
  return winner;
}

int index(char key){
  return (int)(key - 48);
}

void printboard(){
  lcd.clear();
  lcd.setCursor(0 , 0);
  lcd.print("O");
  int b=0;
  for (int i=1; i<4; i++){
    for(int j=6; j<=8; j++){
      lcd.setCursor(j , i);
      lcd.write(board[b]);
      b++;
    }
  }
}

void printresult(int gamer){
  switch(gamer){
    case 0:
      lcd.setCursor(2 , 0);
      lcd.print("Equal!");
      break;
    case 1:
      lcd.setCursor(2 , 0);
      lcd.print("X won!");
      break;
    case 2:
      lcd.setCursor(2 , 0);
      lcd.print("O won!");
      digitalWrite(ld, 1);
      break;     
  }        
}
