#include <Arduino.h> 
#define PWM1 3           // Pino que controla o PWM do Motor 1(M1) e ativa a Ponte-H do M1
#define PWM2 9           // Pino que controla o PWM do Motor 2(M2) e ativa a Ponte-H do M2
#define E1 2             // Sentido M1
#define E2 4             // Sentido M1
#define E3 6             // Sentido M2
#define E4 5             // Sentido M2
#define T1 A1            // Emissão de ondas para medição da distânica UE
#define Ec1 A0           // Pino de recepção UE
#define T2 A3            // Emissão de ondas para medição da distânica UC
#define Ec2 A2           // Pino de recepção UC
#define T3 A5            // Emissão de ondas para medição da distânica U3
#define Ec3 A4           // Pino de recepção UD
#define btn 10           // Botão Failsafe
float dist1;             // Variável que identifica a posição do adversário na Esquerda
float dist2;             // Variável que identifica a posição do adversário no Centro
float dist3;             // Variável que identifica a posição do adversário na Direita
int PWME;                // Variável que define o PWM no caso de curva à esquerda
int PWMC;                // Variável que define o PWM no caso de ir para frente
int PWMD;                // Variável que define o PWM no caso de curva à direita
int PWMEC;               // Variável que define o PWM no caso de curva leve à esquerda
int PWMCD;               // Variável que define o PWM no caso de curva leve à direita
unsigned long tempo = 0; // Variável de tempo
int VRF=0;               // Variável responsável por aguardar um tempo para que o robô entre no modo patrulha
bool btnliberado;        // Variável responsável pelo controle do botão quando pressionado
bool btnclicado;         // Variável responsável pelo controle do botão quando solto
int failsafe = 1;        // Variável responsável pelo travamento das funções do robô

void setup()  // Definição das portas de entrada e de saída
{ 
  pinMode(PWM1, OUTPUT);
  pinMode(PWM2, OUTPUT);
  pinMode(E1, OUTPUT);
  pinMode(E2, OUTPUT);
  pinMode(E3, OUTPUT);
  pinMode(E4, OUTPUT);
  pinMode(T1, OUTPUT);
  pinMode(T2, OUTPUT);
  pinMode(T3, OUTPUT);
  pinMode(Ec1, INPUT);
  pinMode(Ec2, INPUT);
  pinMode(Ec3, INPUT);
  pinMode(btn,INPUT);
  digitalWrite(T1, LOW);
  digitalWrite(T2, LOW);
  digitalWrite(T3, LOW);
}
void leitura()  // Leitura dos sensores ultrassônicos
{ 
  digitalWrite(T1, HIGH);
  digitalWrite(T2, HIGH);
  digitalWrite(T3, HIGH);
  delayMicroseconds(10);
  digitalWrite(T1, LOW);
  dist1 = pulseIn(Ec1, HIGH);
  digitalWrite(T2, LOW);
  dist2 = pulseIn(Ec2, HIGH);
  digitalWrite(T3, LOW);
  dist3 = pulseIn(Ec3, HIGH);
}
void distancia() // Calcula a distância do objeto através do tempo fornecido pelos sensores
{ 
  dist1 = dist1/2;   //Como o sensor informa o tempo de ida e volta em microssegundos, divide-se o valor por 2
  dist1 = dist1*34800;  //Multiplicasse pela velocidade do som em cm/s
  dist1 = dist1/1000000;  //Divide-se o valor por um milhão para eliminar o micro(10^-6)
  dist2 = dist2/2;
  dist2 = dist2*34800;
  dist2 = dist2/1000000;
  dist3 = dist3/2;
  dist3 = dist3*34800;
  dist3 = dist3/1000000;
}
void PWM()  // Controla a velocidade dos motores de acordo com a proximidade do objeto
{ 
  PWME = map(dist1, 100, 3, 0, 255);  // Utilizasse a função map para fazer uma regra de 3 entre os valores do PWM e da distância captada pelos sendores
  PWMC = map(dist2, 100, 3, 0, 255);  
  PWMD = map(dist3, 100, 3, 0, 255);

  if (dist1 <= dist2){   // Esse if regula o PWM no caso de dois sensores identificarem o objeto
    PWMEC = PWME;
  } else{
    PWMEC = PWMC;
  }

  if (dist2 <= dist3){
    PWMCD = PWMC;
  } else{
    PWMCD = PWMD;
  }
}
void botao() //Controla o botão de failsafe
{  
if (digitalRead(btn) == HIGH) {  //Se o botão for apertado, btnclicado = true
        btnclicado = true;
    }
    else if (digitalRead(btn) == LOW && btnclicado) {  //Se o botão já tiver sido apertado e for solto, então btnliberado = true
        btnclicado = false;
        btnliberado = true;
    }
    if (btnliberado) {  //Toda que o botão for pressionado e solto, a variável failsafe tem seu valor invertido entre 0 e 1
        failsafe = !failsafe;
        btnliberado = false;
    }
}
void curvaesq()  // Curva a esquerda no próprio eixo
{ 
  if (dist1<=100 && dist2>100 && dist3>100)
  {
    analogWrite(PWM1, PWME);
    analogWrite(PWM2, PWME);
    digitalWrite(E2, LOW);
    digitalWrite(E4, LOW);
    digitalWrite(E1, HIGH);
    digitalWrite(E3, HIGH);
    VRF=1;
  }
}
void esqsuave()  // Curva suave à esquerda (só um motor funciona)
{ 
  if (dist1<=100 && dist2<=100 && dist3>100)
  {
    analogWrite(PWM2, PWMEC);
    digitalWrite(PWM1, LOW);
    digitalWrite(E1, LOW);
    digitalWrite(E3, HIGH);
    digitalWrite(E2, LOW);
    digitalWrite(E4, LOW);
    VRF=1;
  }
}
void curvadir()  // Curva a direita no próprio eixo
{ 
  if (dist1>100 && dist2>100 && dist3<=100)
  {
    analogWrite(PWM1, PWMD);
    analogWrite(PWM2, PWMD);
    digitalWrite(E1, LOW);
    digitalWrite(E3, LOW);
    digitalWrite(E2, HIGH);
    digitalWrite(E4, HIGH);
    VRF=1;
  }
}
void dirsuave()  // Curva suave à direita (só um motor funciona)
{ 
  if (dist1>100 && dist2<=100 && dist3<=100)
  {
    analogWrite(PWM1, PWMCD);
    digitalWrite(PWM2, LOW);
    digitalWrite(E1, LOW);
    digitalWrite(E3, LOW);
    digitalWrite(E2, HIGH);
    digitalWrite(E4, LOW);
    VRF=1;
  }
}
void frente()  // Segue em linha reta para frente
{ 
  if (dist1>100 && dist2<=100 && dist3>100)
  {
    analogWrite(PWM1, PWMC);
    analogWrite(PWM2, PWMC);
    digitalWrite(E1, LOW);
    digitalWrite(E3, HIGH);
    digitalWrite(E2, HIGH);
    digitalWrite(E4, LOW);
    VRF=1;
  }
}
void parado()  // Parado
{ 
  digitalWrite(PWM1, LOW);
  digitalWrite(PWM2, LOW);
  digitalWrite(E1, LOW);
  digitalWrite(E3, LOW);
  digitalWrite(E2, LOW);
  digitalWrite(E4, LOW);
}
void patrulha()  // Gira no próprio eixo, procurando um objeto
{ 
  tempo = millis();
  while (millis()-tempo<=1000 && dist1>100 && dist2>100 && dist3>100 && VRF>0 && failsafe>0) //Toda vez que o robô entrar no modo patrulha pela primeira vez, ele ficará parado esperando algum movimento nos sensores para depois começar a girar caso não encontre nada
  {
    leitura();
    distancia();
    parado();
    botao();
   }
  if(millis()-tempo>1000){
    VRF=-1;
  }
  
  tempo = millis();
   while (millis()-tempo<=1000 && dist1>100 && dist2>100 && dist3>100 && VRF<0 && failsafe>0) // O robô gira por um segundo para a direita
   {
    leitura();
    distancia();
    analogWrite(PWM1, 128);
    analogWrite(PWM2, 128);
    digitalWrite(E1, LOW);
    digitalWrite(E3, LOW);
    digitalWrite(E2, HIGH);
    digitalWrite(E4, HIGH);
    botao();
   }
  tempo = millis();
  while (millis()-tempo<=1000 && dist1>100 && dist2>100 && dist3>100 && VRF<0 && failsafe>0) // O robô gira por um segundo para a esquerda
  {
    leitura();
    distancia();
    analogWrite(PWM1, 128);
    analogWrite(PWM2, 128);
    digitalWrite(E1, HIGH);
    digitalWrite(E3, HIGH);
    digitalWrite(E2, LOW);
    digitalWrite(E4, LOW);
    botao();
  }
}
void loop()
{
  if(failsafe>0){  // Se o botão do failsafe for pressionado ele pode ou não permitir o funcionamento regular dos motores do robô
  leitura();
  distancia();
  PWM();
  curvaesq();
  curvadir();
  frente();
  esqsuave();
  dirsuave();
  patrulha();
  botao();
  }else{
  parado();
  botao();}  
}
