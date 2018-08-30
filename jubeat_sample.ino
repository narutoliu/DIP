
const int led1 = 13;
const int led2 = 12;
const int led3 = 11;
const int led4 = 10;
const int playList[4][5]={{0,1,1,0,1},{1,0,0,1,0},{1,0,1,0,1},{1,1,0,1,1}};
int currentList[4]={0,0,0,0};



void setup() {
  // put your setup code here, to run once:
  pinMode(led1,OUTPUT);
  pinMode(led2,OUTPUT);
  pinMode(led3,OUTPUT);
  pinMode(led4,OUTPUT);

  
  
}

void loop() {
  for(int i=0;i<5;i++){
      for(int j=0;j<4;j++){
        currentList[j]=playList[j][i];
        
      }
      if (currentList[0]==1){
        digitalWrite(led1,HIGH);
      }else{
        digitalWrite(led1,LOW);
      }
      if (currentList[1]==1){
        digitalWrite(led2,HIGH);
      }else{
        digitalWrite(led2,LOW);
      }
      if (currentList[2]==1){
        digitalWrite(led3,HIGH);
      }else{
        digitalWrite(led3,LOW);
      }
      if (currentList[3]==1){
        digitalWrite(led4,HIGH);
      }else{
        digitalWrite(led4,LOW);
      }
      delay(300);

      
    }
  
  

}
