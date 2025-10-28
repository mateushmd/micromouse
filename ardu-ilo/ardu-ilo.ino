// Exemplo 18 – Chassi 2 rodas
// Apostila Eletrogate - KIT Robotica
// Classe para facilitar o uso da ponte H L298N na manipulação dos motores na função Setup e Loop.

bool abacaxi = 0;

const int led = 12;
const int botao = 13;

const int ultraL_trigger = A0;
const int ultraL_echo = 11;

const int ultraR_trigger = A1;
const int ultraR_echo = 3;

struct DCMotor {

    int speed = 255;
    int pin1;
    int pin2;
    bool invert = false;

    DCMotor() {}

    DCMotor(bool invert) {
        this->invert = invert;
    }

    ~DCMotor() = default;

    void pinout(int in1, int in2) { // Pinout é o método para a declaração dos pinos que vão controlar
                                   // o objeto motor
        pin1 = in1;
        pin2 = in2;
        pinMode(pin1, OUTPUT);
        pinMode(pin2, OUTPUT);
    }

    void setSpeed(int in1) { // Speed é o método que irá ser responsável por regular a velocidade
        speed = in1;
    }

    void fw() { // Forward é o método para fazer o motor girar para frente
        analogWrite(pin1, speed);
        digitalWrite(pin2, LOW);
    }

    void bw() { // Backward é o método para fazer o motor girar para trás
        digitalWrite(pin1, LOW);
        analogWrite(pin2, speed);
    }

    void forward() {
        if (!invert) {
            fw();
        } else {
            bw();
        }
    }

    void backward() {
        if (!invert) {
            bw();
        } else {
            fw();
        }
    }

    void stop() { // Stop é o metodo para fazer o motor ficar parado.
        digitalWrite(pin1, LOW);
        digitalWrite(pin2, LOW);
    }
};

DCMotor Motor1, Motor2(true); // Criação de dois objetos motores, já que usaremos dois motores, e eles já

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

void direita(double time=450) {

    Motor1.forward();  
    Motor2.forward();

    Motor1.forward();
    Motor2.backward();

    delay(time);
}

void esquerda(double time=450) {

    Motor1.forward();  
    Motor2.forward();

    Motor1.backward();
    Motor2.forward();

    delay(time);
}

void frente(void) {

    unsigned long start = millis();
    while ( (millis() - start) < 1500) {    // 1.5 s

        Motor1.forward(); 
        Motor2.forward();  

        double dL = ultraLeft();
        double dR = ultraRight();

        Serial.print(dL);
        Serial.print(", ");
        Serial.print(dR);
        Serial.println();

        if (dL < 20.0) {
            if (dL < 12) {
                direita(1.0);
            } else if (dL > 16) {
                esquerda(1.0);
            }
        }

        if (dR < 20.0) {
            if (dR < 12) {
                esquerda(1.0);
            } else if (dR > 16) {
                direita(1.0);
            }
        }
    }
    Motor1.stop();
    Motor2.stop();
    Serial.println("frente!");
}

// estão prontos para receber os comandos já configurados acima.
void setup() {

    Serial.begin(9600);

    Motor1.pinout(5, 6); // Seleção dos pinos que cada motor usará, como descrito na classe.
    Motor2.pinout(9, 10);

    pinMode(led, OUTPUT);
    pinMode(botao, INPUT);

    pinMode(ultraL_trigger, OUTPUT);
    pinMode(ultraL_echo, INPUT);

    pinMode(ultraR_trigger, OUTPUT);
    pinMode(ultraR_echo, INPUT);
}

// 500
unsigned long start_loop = 0;

void loop() {

    if (abacaxi == 1) {

        if (digitalRead(botao) == LOW) {
            abacaxi = 0;
            digitalWrite(led, LOW);
            delay(500);

        } else if ( (millis() - start_loop) >= 3000) {

            delay(3000);

            frente();
            delay(1000);

            esquerda();
            delay(1000);

            frente();

            start_loop = millis();
        }

    } else {

        if (digitalRead(botao) == LOW){
            abacaxi = 1;
            start_loop = millis();
            digitalWrite(led, HIGH);
            delay(500);
        }
    }
    delay(50);
}
