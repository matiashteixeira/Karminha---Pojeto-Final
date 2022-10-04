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

void curvaesq();