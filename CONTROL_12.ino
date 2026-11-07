#include <SoftwareSerial.h>
SoftwareSerial myserial(A0,A1);

//PINES
int PIR1Pin = 3;
int PIR2Pin = 4;
int PIR3Pin = 5;
int PIR4Pin = 6;
int entrada = 9;
int entrada2 = 11;
int entrada3 = 13;
int control = 8;
int control2 = 10;
int control3 = 12;
int buzzer = 2;
int Antena1 = A2;
int Antena2 = A3; 

//CONSTANTES 
#define TIEMPO_TX 1000
#define TIEMPO_RX 1000

//VARIABLES DE CONTROL
char mat[5];
int t_an = 0;
int t_al = 0;
int t_RX = 0;
int t_buzzON = 2000;
int t_buzzOFF = 3000;
int t_TX = TIEMPO_TX;
int t = 0;
int atento = 0;
int dif_t = 0;
int acum = 0;
int tipo = 0;
int num_sen = 1;
bool visto = false;
bool TX_ON = true;
bool buzzerON = true;
float rango1 = 0.0;
float rango2 = 0.0;
float rango3 = 0.0;
String dato = "";

//VARIABLES DE DISTANCIA
float ultra = 10.7;
float distancia = 10.7;
float distancia2 = 10.7;
float distancia3  = 10.7;
float distreal = 10.7;
float distreal2 = 10.7;
float distreal3  = 10.7;
float minimo = 10.7;



#define NUM_MEDICIONES 5
#define NUM_CASOS 3

float ultimasMediciones[NUM_CASOS][NUM_MEDICIONES];
int indiceUltimaMedicion[NUM_CASOS] = {0};

float antirrebote(float valor, int caso) {
  ultimasMediciones[caso][indiceUltimaMedicion[caso]] = valor;
  indiceUltimaMedicion[caso] = (indiceUltimaMedicion[caso] + 1) % NUM_MEDICIONES; // Circular buffer
 
  float minimo = ultimasMediciones[caso][0];
  for (int i = 1; i < NUM_MEDICIONES; i++) {
    minimo = min(minimo, ultimasMediciones[caso][i]);
  }
  return minimo;
}

void Clasificacion(String rango){
  unsigned long period=20; //En Milisegundos

    static unsigned long previousMillis=0;

    if((millis()-previousMillis)>period){
        int num = rango.toInt();
        /*if(num!=0){
          Serial.print(num);
        }*/
        switch(num) 
        {
        case 43:
          rango1 = 0;
          Serial.println("Rango1= 0");
          break;
        case 42:
          Serial.println("Rango1= 10");
          rango1 = 10.0;
          break;
        case 45:
          Serial.println("Rango2= 0");
          rango2 = 0;
          break;
        case 52:
          Serial.println("Rango2= 4");
          rango2 = 4.0;
          break;
        case 53:
          Serial.println("Rango2= 5");
          rango2 = 5.0;
          break;
        case 54:
          Serial.println("Rango2= 6");
          rango2 = 6.0;
          break;
        case 47:
          Serial.println("Rango1= 0");
          rango3 = 0;
          break;
        case 49:
          Serial.println("Rango1= 1");
          rango3 = 1.0;
          break;
        case 50:
          Serial.println("Rango1= 2");
          rango3 = 2.0;
          break;
        case 51:
          Serial.println("Rango1= 3");
          rango3 = 3.0;
          break;
        case 84:
          Serial.println("OK");
          TX_ON=true;
          t_TX=TIEMPO_TX;
          t_RX=0;
          break;
        default:
          break;
        } 
        previousMillis += period;
     }
}

void setup() {
  myserial.begin(9600);
  Serial.begin(9600);
  pinMode(entrada, INPUT_PULLDOWN);
  pinMode(entrada2, INPUT_PULLDOWN);
  pinMode(entrada3, INPUT_PULLDOWN);
  pinMode(PIR1Pin, INPUT_PULLDOWN);
  pinMode(PIR2Pin, INPUT_PULLDOWN);
  pinMode(PIR3Pin, INPUT_PULLDOWN);
  pinMode(PIR4Pin, INPUT_PULLDOWN);
  pinMode(control,OUTPUT);
  pinMode(control2,OUTPUT);
  pinMode(control3,OUTPUT);  
  pinMode(Antena1,OUTPUT);
  pinMode(Antena2,OUTPUT); 
  pinMode(buzzer, OUTPUT);
  TX_ON= true;
  delay(10000);
}

void salida_buzzer(int zona){
  unsigned long period=100; //En Milisegundos

    static unsigned long previousMillis=0;

    if((millis()-previousMillis)>period){
        switch (zona) {
        case 0:
          analogWrite(buzzer, 0);  
          //Serial.println(t_buzzON);
          t_buzzON=0;
          t_buzzOFF=0;
          break;
        case 3:
          if(buzzerON==false){
            analogWrite(buzzer, 0);
            Serial.print("Buzzer apagado: ");
            //Serial.println(t_buzzOFF);
            t_buzzON=2000;
          }
          else if(buzzerON){
            analogWrite(buzzer, 255);
            Serial.print("Buzzer encendido: ");
            //Serial.println(t_buzzON);
            t_buzzOFF=3000;
          }
          break;
        case 2:
          if(buzzerON==false){
            analogWrite(buzzer, 0);
            Serial.print("Buzzer apagado: ");
            //Serial.println(t_buzzOFF);
            t_buzzON=1000;
          }
          else if(buzzerON){
            analogWrite(buzzer, 255);
            Serial.print("Buzzer encendido: ");
            //Serial.println(t_buzzON);
            t_buzzOFF=1000;
          }
          break;
        case 1:
          analogWrite(buzzer, 255);
          break;
        default:
          break;
        } 
        previousMillis += period;
    }
}

float readDistances() {
  unsigned long period=180; //En Milisegundos

    static unsigned long previousMillis=0;

    if((millis()-previousMillis)>period){
        digitalWrite(control,LOW);
        digitalWrite(control2,LOW);
        digitalWrite(control3,LOW);
        switch (num_sen) {
          case 1:
            digitalWrite(control, HIGH);
            distancia = (pulseIn(entrada,HIGH))/5800.0;
            distreal = antirrebote(distancia, 0);
            break;
          case 2:
            digitalWrite(control2, HIGH);
            distancia2 = (pulseIn(entrada2,HIGH))/5800.0;
            distreal2 = antirrebote(distancia2, 1);
            break;
          case 3:
            digitalWrite(control3, HIGH);
            distancia3 = (pulseIn(entrada3,HIGH))/5800.0;
            distreal3 = antirrebote(distancia3, 2);
            break;
        }
        num_sen++;
        if (num_sen > 3) {
          num_sen = 1;
        }
      
        if(distreal*10==0.00){
          distreal=10.7;
          //float minimo = min(distreal2, distreal3);
          //return minimo;
        }else if(distreal2*10==0.00){
          distreal2=10.7;
          //float minimo = min(distreal, distreal3);
          //return minimo;
        }else if(distreal3*10==0.00){
          distreal3=10.7;
          //float minimo = min(distreal, distreal2);
          //return minimo;
        }
        
          float minimo = min(distreal, min(distreal2, distreal3));
          return minimo;
          
        previousMillis += period;
     }

}

void ConfigAntena(String pantalla, char mat[]) {
  unsigned long period=(t_RX+t_TX)+100; //En Milisegundos

    static unsigned long previousMillis=0;

    if((millis()-previousMillis)>period){
        if(TX_ON==false){
        //MODO RX
        //Serial.println("Modo RX");
        digitalWrite(Antena1, LOW);
        digitalWrite(Antena2, HIGH);
        t_TX=TIEMPO_TX;

        //Serial.print("Hay algo en el buffer ");
        //Serial.println(myserial.available());
        if( myserial.available() > 0){
          t_RX=TIEMPO_RX;
          int b = myserial.read();
          dato = String(b);
          Clasificacion(dato);
        }else{
          dato="";
        }
      }
      else if(TX_ON){
        //MODO TX
        //Serial.println("Modo TX");
        digitalWrite(Antena1, HIGH);
        digitalWrite(Antena2, LOW);
        //Serial.println(pantalla);
        if(ultra>0){
          pantalla.toCharArray(mat,5);
          myserial.write(mat);
        }
        t_RX=TIEMPO_RX;
      }
        previousMillis += period;
     }
}



void Control(int value1, int value2, int value3, int value4, float rango1, float rango2, float rango3){
  unsigned long period=50; //En Milisegundos

    static unsigned long previousMillis=0;

    if((millis()-previousMillis)>period){
        if(rango1>0 || rango2>0 || rango3>0){
          if ((value1==HIGH || value2==HIGH || value3==HIGH || value4==HIGH) && rango1!=0)
          {
            if(tipo!=1 && tipo!=2){
              tipo=3;
              t_al=2000; 
            }
          } 
          
          if (value1 == HIGH || value2==HIGH || value3==HIGH)
          {
            atento=2000;
            if(atento>0)
            {
                if(ultra<rango2)
                {
                  visto = true;
                  if(tipo!=1)
                  {
                    t_al = 2000;
                    tipo = 2;
                  }
                }
                
                if(ultra<=rango3)
                {
                  visto = true;
                  tipo = 1;
                  t_al = 2000;
                }
            }
            if(atento<0){
              atento=0;
            }
          }
          
          if (visto == true)
          {
            if(ultra<rango2)
            {
               tipo = 2;
               t_al=2000;
            }
            if(ultra<=rango3)
            {
              tipo = 1;
              t_al=2000;
            }
            else{visto=false;}
          }
        
           if(value1==LOW && value2==LOW && value3==LOW && value4==LOW && visto==false)
            {
              tipo = 0;
            }
        }
        previousMillis += period;
    }
}

void Tiempos(){
  unsigned long period=50; //En Milisegundos

    static unsigned long previousMillis=0;

    if((millis()-previousMillis)>period){
        unsigned long t=millis();
        dif_t=(t-t_an);
        t_RX=t_RX-dif_t;
        t_TX= t_TX-dif_t;
        t_al=t_al-dif_t;
        t_buzzON = t_buzzON - dif_t;
        t_buzzOFF = t_buzzOFF - dif_t;
        atento=atento-dif_t;
        t_an=t;
        if (t_al<0){
          t_al=0;
          tipo=0;
        }
         if (t_RX<0){
          t_RX=0;
        }
        if(t_RX==0){
          TX_ON= true;
        }
      
        if (t_TX<0){
          t_TX=0;
        }
        if(t_TX==0){
          TX_ON= false;
        }
        if(t_buzzON<0){
          t_buzzON=0;
        }
        if(t_buzzON==0){
          buzzerON=false;
        }
      
        if(t_buzzOFF<0){
          t_buzzOFF=0;
        }
        if(t_buzzOFF==0){
          buzzerON=true;
        }
        
        previousMillis += period;
     }
  
}

std::array<int, 4> LeerPIR(int PIR1Pin, int PIR2Pin, int PIR3Pin, int PIR4Pin)
{
  int value1;
  int value2;
  int value3; 
  int value4;
  unsigned long period=30; //En Milisegundos

    static unsigned long previousMillis=0;

    if((millis()-previousMillis)>period){
        value1= digitalRead(PIR1Pin);
        value2= digitalRead(PIR2Pin);
        value3= digitalRead(PIR3Pin);
        value4= digitalRead(PIR4Pin); 
        previousMillis += period;
     }
  
  return {value1, value2, value3, value4};
}

void Imprimir(){
  unsigned long period=20; //En Milisegundos

    static unsigned long previousMillis=0;

    if((millis()-previousMillis)>period){
        Serial.print("Tipo: ");
        Serial.print(tipo);
        Serial.print(",");
        Serial.print("Rango1: ");
        Serial.print(rango1);
        Serial.print(",");
        Serial.print("Rango2: ");
        Serial.print(rango2);
        Serial.print(",");
        Serial.print("Rango3: ");
        Serial.print(rango3);
        Serial.print(",");
        Serial.print("Ultra: ");
        Serial.print(ultra);
        Serial.println(",");
        /*
        Serial.print("Sensor1: ");
        Serial.print(distreal);
        Serial.print(",");
        Serial.print("Sensor2: ");
        Serial.print(distreal2);
        Serial.print(",");
        Serial.print("Sensor3: ");
        Serial.print(distreal3);
        Serial.println(",");*/
        previousMillis += period;
     }
  
}

void loop() {
   
  std::array<int,4> value=LeerPIR(PIR1Pin, PIR2Pin, PIR3Pin, PIR4Pin);

  ultra = readDistances();

  Control(value[0], value[1], value[2], value[3], rango1, rango2, rango3);

  Tiempos();

  Imprimir();
  
  /*Serial.print("PIR1:");
  Serial.print(value1);
  Serial.print(",");
  Serial.print("PIR2:");
  Serial.print(value2);
  Serial.print(",");
  Serial.print("PIR3:");
  Serial.print(value3);
  Serial.print(",");
  Serial.print("PIR4:");
  Serial.print(value4);
  Serial.println(",");*/
  /**/
  /**/
  
  salida_buzzer(tipo);
  
  

  //Serial.println(ultra);
  String pantalla = String(ultra);
  ConfigAntena(pantalla, mat);
  /*Serial.print("Tipo:");
  Serial.println(tipo);
  
  /*t=millis();
  dif_t=t-t_an;
  acum=acum+dif_t;
  if (acum >= 100){
    acum = 0;
    num_sen=num_sen+1;
  }
  if(num_sen > 3){
    num_sen = 1;
  }
  t_an=t;
  */
  //Serial.println(minimo);
}
