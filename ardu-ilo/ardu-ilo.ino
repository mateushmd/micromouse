// Exemplo 18 – Chassi 2 rodas
// Apostila Eletrogate - KIT Robotica
// Classe para facilitar o uso da ponte H L298N na manipulação dos motores na função Setup e Loop.

int abacaxi = 0;
const int led = 12;
const int botao = 13;


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

void direita(DCMotor m1, DCMotor m2){
    m1.Forward();  
    m2.Backward();   // Forward
    m1.Forward();
    m2.Forward();    // Backward
    delay(450);
}

void esquerda(DCMotor m1, DCMotor m2){
    m1.Forward();  
    m2.Backward();  
    m1.Backward();
    m2.Backward();   // Forward
    delay(450);
}

void frente(DCMotor m1, DCMotor m2){
    m1.Forward();  
    m2.Backward();  
    delay(1500);
}

// estão prontos para receber os comandos já configurados acima.
void setup() {
    Motor1.Pinout(5,6); // Seleção dos pinos que cada motor usará, como descrito na classe.
    Motor2.Pinout(9,10);
    pinMode(led, OUTPUT);
    pinMode(botao, INPUT);
}
void loop() {

    if (abacaxi == 1){
        digitalWrite(led, HIGH);
        delay(3000);
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
        delay(3000);
        if (digitalRead(botao) == LOW){
            abacaxi = 1;
        }
    }
}
