// Exemplo 18 – Chassi 2 rodas
// Apostila Eletrogate - KIT Robotica
// Classe para facilitar o uso da ponte H L298N na manipulação dos motores na função Setup e Loop.

int abacaxi = 0;
const int led = 12;
const int botao = 13;

const int ultraL_trigger = A0;
const int ultraL_echo = 11;

const int ultraR_trigger = A1;
const int ultraR_echo = 3;

class DCMotor {
    int spd = 255, pin1, pin2;
    public:
    void Pinout(int in1, int in2){ // Pinout é o método para a declaração dos pinos que vão controlar
                                   // o objeto motor
        pin1 = in1;
        pin2 = in2;
        pinMode(pin1, OUTPUT);
        pinMode(pin2, OUTPUT);
    }
    void Speed(int in1){ // Speed é o método que irá ser responsável por regular a velocidade
        spd = in1;
    }
    void Forward(){ // Forward é o método para fazer o motor girar para frente
        analogWrite(pin1, spd);
        digitalWrite(pin2, LOW);
    }
    void Backward(){ // Backward é o método para fazer o motor girar para trás
        digitalWrite(pin1, LOW);
        analogWrite(pin2, spd);
    }
    void Stop(){ // Stop é o metodo para fazer o motor ficar parado.
        digitalWrite(pin1, LOW);
        digitalWrite(pin2, LOW);
    }
};
DCMotor Motor1, Motor2; // Criação de dois objetos motores, já que usaremos dois motores, e eles já

double ultraLeft(void) {
    digitalWrite(ultraL_trigger, HIGH);
    delayMicroseconds(10);
    digitalWrite(ultraL_trigger, LOW);

    double duration = pulseIn(ultraL_echo, HIGH);
    return (0.017f * duration);
}

double ultraRight(void) {
    digitalWrite(ultraR_trigger, HIGH);
    delayMicroseconds(10);
    digitalWrite(ultraR_trigger, LOW);

    double duration = pulseIn(ultraR_echo, HIGH);
    return (0.017f * duration);
}

void direita(DCMotor m1, DCMotor m2, double time=250){
    m1.Forward();  
    m2.Backward();   // Forward
    m1.Forward();
    m2.Forward();    // Backward
    delay(time);
}

void esquerda(DCMotor m1, DCMotor m2, double time=250){
    m1.Forward();  
    m2.Backward();   // Forward
    m1.Backward();
    m2.Backward();   // Forward
    delay(time);
}

void frente(DCMotor m1, DCMotor m2){

    // 1.5 s
    unsigned long start = millis();
    while ( (millis() - start) < 1500) {
        m1.Forward(); 
        m2.Backward();  

        double dL = ultraLeft();
        double dR = ultraRight();

        Serial.print(dL);
        Serial.print(", ");
        Serial.print(dR);
        Serial.println();

        if (dL < 20.0) {
            if (dL < 12) {
                direita(m1, m2, 0.0);
            } else if (dL > 16) {
                esquerda(m1, m2, 0.0);
            }
        }

        if (dR < 20.0) {
            if (dR < 12) {
                esquerda(m1, m2, 0.0);
            } else if (dR > 16) {
                direita(m1, m2, 0.0);
            }
        }
    }
    Serial.println("frente!");
}

// estão prontos para receber os comandos já configurados acima.
void setup() {
    Serial.begin(9600);
    Motor1.Pinout(5,6); // Seleção dos pinos que cada motor usará, como descrito na classe.
    Motor2.Pinout(9,10);
    pinMode(led, OUTPUT);
    pinMode(botao, INPUT);

    pinMode(ultraL_trigger, OUTPUT);
    pinMode(ultraL_echo, INPUT);

    pinMode(ultraR_trigger, OUTPUT);
    pinMode(ultraR_echo, INPUT);
}
void loop() {

    if (abacaxi == 1){
        digitalWrite(led, HIGH);

        delay(3000);

        frente(Motor1, Motor2);
        delay(500);
        esquerda(Motor1, Motor2);
        delay(500);
        frente(Motor1, Motor2);

        Motor1.Stop();
        Motor2.Stop();
        delay(3000);
        if (digitalRead(botao) == LOW){
            abacaxi = 0;
        }
    }
    else{
        digitalWrite(led, LOW);
        delay(500);
        if (digitalRead(botao) == LOW){
            abacaxi = 1;
        }
    }
}
