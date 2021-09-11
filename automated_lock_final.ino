/*   This program is made by Rahul Guglani*/




#include <EEPROM.h>      //to save password in eeprom

#include <string.h>      

#include<math.h>

#include <LiquidCrystal.h>

// initialize the library with the numbers of the interface pins
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);
int keypad=A0;
int key_o=9876; //this is by default password of door lock
int key_u;      //this is the password which is stored in eeprom of arduino and can be changed by pressing C in keypad
int m1=6,m2=7,mp2=8,mp1=9; //initializing motor And motor position pins
int buzzer=10;
int adr=0;             //address where password is to saved

char keypadval(void);   //    F
void welcome(int);      //    U
int entrkey(void);      //    N
void matchkey(int);     //    C
void lock(void);        //    T    used in program
void Delay(int);        //    I
int getpin(void);       //    O
void putpin(int);       //    N'
void updatepin(void);   //    S

void setup() 
{
  // set up the LCD's number of columns and rows:
  lcd.begin(16, 2);   
  // set up other pin's
  pinMode(keypad,INPUT);
  pinMode(m1,OUTPUT);
  pinMode(m2,OUTPUT);
  pinMode(mp1,INPUT);
  pinMode(mp2,INPUT);
  pinMode(buzzer,OUTPUT);
  delay(200);
  
  key_u=getpin();   //getting password from EEPROM and storing it in key_u

  while(digitalRead(mp1)==LOW)  //closes the gate in beggining
  {
  digitalWrite(m1,LOW);
  digitalWrite(m2,HIGH);
  }
  digitalWrite(m1,LOW);
  digitalWrite(m2,LOW);
  
  lcd.clear();     //clears lcd screen
}


void loop()
{
 
  if(keypadval()=='n')  //when no key is pressed it will greet us with welcome message
  welcome(1);
  
  else
  if(keypadval()=='C')  //when C is pressed password change menu will be opened
  {
   updatepin();
  }
  else                  //if any other key is pressed it will move us to gate opening menu and asks us for password
  {
   lcd.print("key =");
   lcd.print(keypadval()); //displays which key is pressed
   delay(500);
    
   matchkey(entrkey());    //entrkey function asks us for password and return password as integer value,
                           //matchkey function takes entered password as parameter and compare it with the
                           //correct password and calls lock function, if password entered is correct.
     
   delay(2000);
   noTone(buzzer);
  }
  
}

int getpin()                //this function returns password stored in eeprom
{
int a;
  EEPROM.get(adr,a);
  return a;
}

void putpin(int a)          //this function stores password in eeprom
{
 EEPROM.put(adr,a);
}

void updatepin()            //this function changes password stored in eeprom
{
  lcd.clear();
  lcd.print("to change pin");
  lcd.setCursor(0,1);
  lcd.print("enter old pin");  //in order to change password first it asks old password
  delay(1000);                 //and compares it with correct password if password entered 
  int c=entrkey();             //is correct it asks for new password and updates it
  lcd.clear();
  if(key_o==c||c==key_u)
  { 
    lcd.print("now entr new pin");
    delay(600);
    int d=entrkey();
    key_u=d;
    putpin(d);
    lcd.clear();
    lcd.print("pin changed");
    delay(1000);
  }
  else
  {
  lcd.print("Wrong pin");
    tone(buzzer,1000);
    delay(1000);
    noTone(buzzer);
    
  }

}

void Delay(int a)                  //this function acts similar to delay function but it also reverse counts
{int b=a;                          //the time left and prints it on screen.
 int i;
 B:for(i=0;;)
 {
 if((b/1000)>=(pow(10,i)-1))
 i++;
 else
 break;
 }int c=0;
 lcd.setCursor((16-i),1);
c=b/1000;
 if(c>=0)
   
 {lcd.print(c);
  if(c==0)
 delay((b%1000)+1);
 else
 delay(1000);
 b=b-1000;
  lcd.setCursor(16-i,1);
  for(int j=0;j<i;j++)
    lcd.print(" ");
  goto B;
  
 }
}


void matchkey(int key1)
{
lcd.clear();
  if(key_o==key1||key1==key_u)
  { 
    lock();
  }
  else
  {
  lcd.print("Wrong pin");
    tone(buzzer,1000);
    
  }
}
 
void lock(void)
{
  lcd.print("opening door");
  lcd.setCursor(0,1);
  lcd.print("for 10 secs.");
  while(digitalRead(mp2)==LOW)
  {
  digitalWrite(m1,HIGH);
  digitalWrite(m2,LOW);
  }
  digitalWrite(m1,LOW);
  digitalWrite(m2,LOW);
  Delay(10000);
  while(digitalRead(mp1)==LOW)
  {
  digitalWrite(m1,LOW);
  digitalWrite(m2,HIGH);
  }
  digitalWrite(m1,LOW);
  digitalWrite(m2,LOW);
 lcd.clear();
  lcd.print("gate closed");
}


int entrkey()
{
  A:int pin=0;
  lcd.clear();
  delay(50);
  lcd.print("Enter 4d pin:");
  for(int i=0;i<4;)
  {
  while(keypadval()=='n'||keypadval()=='B'||keypadval()=='C'||keypadval()=='D'||keypadval()=='#'||keypadval()=='*')
  ;
  if((i==0&&keypadval()!='A'&&keypadval()!='n')||(keypadval()!='A'&&keypadval()!='n'))
  {i++;
    
  pin=((keypadval()-48)*pow(10,4-i))+pin;
    lcd.setCursor(i-1,1);
    lcd.print(keypadval());
   tone(buzzer,1200);
   delay(150);
  noTone(buzzer);
  delay(500);
  }
   else
   if(keypadval()=='A')
    goto A;
    
  }
  
  pin++;
  lcd.clear();
  lcd.print("your pin:");
  lcd.setCursor(0,1);
  lcd.print(pin);
  delay(500);
  return(pin);
  
    
}

char keypadval()   
{
 float a;
  a=analogRead(keypad);
 a=a/1023*16;       //converting 1023 into 16
 if(a>=0&&a<0.5)    
 {return('n');}
 else
 if(a>=0.5&&a<1.5)
 {return('1');}
 else
 if(a>=1.5&&a<2.5)
 {return('2');}
 else
 if(a>=2.5&&a<3.5)
 {return('3');}
 else
 if(a>=3.5&&a<4.5)
 {return('A');}
 else
 if(a>=4.5&&a<5.5)
 {return('B');}
 else
 if(a>=5.5&&a<6.5)
 {return('6');}
else
 if(a>=6.5&&a<7.5)
 {return('5');}
 else
 if(a>=7.5&&a<8.5)
 {return('4');}
 else
 if(a>=8.5&&a<9.5)
 {return('7');}
 else
 if(a>=9.5&&a<10.5)
 {return('8');}
 else
 if(a>=10.5&&a<11.5)
 {return('9');}
 else
 if(a>=11.5&&a<12.5)
 {return('C');}
 else
 if(a>=12.5&&a<13.5)
 {return('D');}
 else
 if(a>=13.5&&a<14.5)
 {return('#');}
 else
 if(a>=14.5&&a<15.5)
 {return('0');}
 else
 if(a>=15.5&&a<=16)
 {return('*');}
}


void welcome(int a)
{
  char message[]="you are warmly welcomed to our house";   //this is the message displays on welcome function,
                                                                            //you can change message string for a diffrent welcome message 
  if(a==0)
  {
  
 int i,m;

 int k=strlen(message); 
 for(i=0;i<k;i++)
 {lcd.clear();
  
  
  lcd.setCursor(0, 0);
    lcd.print("    WELCOME  ");
  if(i<=15){
     int j=0;
   lcd.setCursor(15-i, 1);
   for(j;j<=i;j++)
   {
  lcd.print(message[j]);
}
    delay(1200);}
  else{lcd.setCursor(0, 1);
      int j=i-15; 
   for(j;j<=i;j++)
   {
  lcd.print(message[j]);
}
    delay(1200);}
 }
  lcd.clear();
}
  if(a!=0)
  {
  
 int i,m;

 int k=strlen(message); 
 for(i=0;i<k;i++)
 {lcd.clear();
  
  
  lcd.setCursor(0, 0);
    lcd.print("  WELCOME ");
  if(i<=15){
     int j=0;
   lcd.setCursor(15-i, 1);
   for(j;j<=i;j++)
   {
  lcd.print(message[j]);
     if(keypadval()!='n')
     {
     lcd.clear();
       break;
     }
}
if(keypadval()!='n')
     {
     lcd.clear();
       break;
     }
    delay(200);}
  else{lcd.setCursor(0, 1);
      int j=i-15; 
   for(j;j<=i;j++)
   {
  lcd.print(message[j]);
     if(keypadval()!='n')
     {
     lcd.clear();
       break;
     }
}
if(keypadval()!='n')
     {
     lcd.clear();
       break;
     }
    delay(200);}
 }
  lcd.clear();
  }
  
}
