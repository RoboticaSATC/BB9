#include <WiFi.h>
#include <WebServer.h>
#include <Servo.h>

// Configurações da rede WiFi
const char* ssid = "Robos_Andreia";
const char* password = "12345678";

// Configurações do IP fixo
IPAddress local_IP(192, 168, 4, 10);   // Altere para o IP desejado
IPAddress gateway(192, 168, 4, 1);      // Gateway da rede
IPAddress subnet(255, 255, 255, 0);     // Máscara de sub-rede

// Cria uma instância do servidor web na porta 80
WebServer server(80);

// Definições dos pinos (ajustados para o Pico W)
#define MOTORA_1 0 //12 
#define MOTORA_2 1 //14  
#define MOTORB_1 2 //27
#define MOTORB_2 3 //26
#define SERVO 4 //16
#define BUZZER 5 //17
#define RED 6 //5
#define GREEN 7 //18
#define BLUE 8 //19

// Configurações iniciais do servo motor
Servo servo;

void handleRoot() {
  // Página principal do servidor com botões para controle
  String html = "<html><body><h1>Controle do Robô</h1>";
  html += "<p><a href=\"/frente\"><button>Frente</button></a></p>";
  html += "<p><a href=\"/direita\"><button>Direita</button></a></p>";
  html += "<p><a href=\"/tras\"><button>Ré</button></a></p>";
  html += "<p><a href=\"/esquerda\"><button>Esquerda</button></a></p>";
  html += "<p><a href=\"/servo?pos=0\"><button>Servo 0</button></a></p>";
  html += "<p><a href=\"/servo?pos=90\"><button>Servo 90</button></a></p>";
  html += "<p><a href=\"/servo?pos=180\"><button>Servo 180</button></a></p>";
  html += "<p><a href=\"/parar\"><button>Parar</button></a></p>";
  html += "</body></html>";
  server.send(200, "text/html", html);
}

void setup(){
  // Inicializa a comunicação serial
  Serial.begin(115200);

  // Configura os pinos dos motores, buzzer e LED RGB como saídas
  pinMode(MOTORA_1, OUTPUT);
  pinMode(MOTORA_2, OUTPUT);
  pinMode(MOTORB_1, OUTPUT);
  pinMode(MOTORB_2, OUTPUT);
  pinMode(BUZZER, OUTPUT);
  pinMode(GREEN, OUTPUT);
  pinMode(BLUE, OUTPUT);
  pinMode(RED, OUTPUT);
  pinMode(22, OUTPUT);
  digitalWrite(22, LOW);

  // Configura o estado inicial de cada um dos pinos 
  digitalWrite(MOTORA_1, LOW);
  digitalWrite(MOTORA_2, LOW);
  digitalWrite(MOTORB_1, LOW);
  digitalWrite(MOTORB_2, LOW);
  digitalWrite(GREEN, HIGH);
  digitalWrite(BLUE, HIGH);
  digitalWrite(RED, HIGH);
  
  // Inicializa o servo motor
  servo.attach(SERVO);
  servo.write(90);
  
  // Conecta à rede WiFi com IP fixo (sem verificação de retorno)
  WiFi.config(local_IP, gateway, subnet);
  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Conectando à rede WiFi...");
  }
  
  Serial.println("Conectado!");
  Serial.print("Endereço IP: ");
  Serial.println(WiFi.localIP());

  // Define as rotas do servidor web
  server.on("/", handleRoot);

  server.on("/frente", [](){
    servo.write(90);
    digitalWrite(GREEN, HIGH);
    digitalWrite(BLUE, LOW);
    digitalWrite(RED, HIGH);
    digitalWrite(MOTORA_1, LOW);
    digitalWrite(MOTORA_2, HIGH);
    digitalWrite(MOTORB_1, HIGH);
    digitalWrite(MOTORB_2, LOW);
    server.sendHeader("Location", "/");
    server.send(303);  // Redireciona de volta à página principal
  });

  server.on("/direita", [](){
    tone(BUZZER, 330);
    delay(100);
    noTone(BUZZER);
    delay(100);
    tone(BUZZER, 330);
    delay(100);
    noTone(BUZZER);
    delay(100);
    tone(BUZZER, 330);
    delay(500);
    noTone(BUZZER);
    servo.write(0);
    digitalWrite(GREEN, HIGH);
    digitalWrite(BLUE, LOW);
    digitalWrite(RED, HIGH);
    digitalWrite(MOTORA_1, LOW);
    digitalWrite(MOTORA_2, HIGH);
    digitalWrite(MOTORB_1, LOW);
    digitalWrite(MOTORB_2, HIGH);
    server.sendHeader("Location", "/");
    server.send(303);  // Redireciona de volta à página principal
  });

  server.on("/tras", [](){
    servo.write(90);
    digitalWrite(GREEN, HIGH);
    digitalWrite(BLUE, LOW);
    digitalWrite(RED, HIGH);
    digitalWrite(MOTORA_1, HIGH);
    digitalWrite(MOTORA_2, LOW);
    digitalWrite(MOTORB_1, LOW);
    digitalWrite(MOTORB_2, HIGH);
    server.sendHeader("Location", "/");
    server.send(303);  // Redireciona de volta à página principal
  });

  server.on("/esquerda", [](){
    tone(BUZZER, 330);
    delay(100);
    noTone(BUZZER);
    delay(100);
    tone(BUZZER, 330);
    delay(100);
    noTone(BUZZER);
    delay(100);
    tone(BUZZER, 330);
    delay(500);
    noTone(BUZZER);
    servo.write(180);
    digitalWrite(GREEN, HIGH);
    digitalWrite(BLUE, LOW);
    digitalWrite(RED, HIGH);
    digitalWrite(MOTORA_1, HIGH);
    digitalWrite(MOTORA_2, LOW);
    digitalWrite(MOTORB_1, HIGH);
    digitalWrite(MOTORB_2, LOW);
    server.sendHeader("Location", "/");
    server.send(303);  // Redireciona de volta à página principal
  });

server.on("/servo", [](){
    String pos = server.arg("pos");
    Serial.print("Movendo servo para posição: ");
    Serial.println(pos);
    servo.write(pos.toInt());
    server.sendHeader("Location", "/");
    server.send(303);  // Redireciona de volta à página principal
});

  server.on("/parar", [](){
    // Desliga todos os motores e buzina para indicar parada
    digitalWrite(MOTORA_1, LOW);
    digitalWrite(MOTORA_2, LOW);
    digitalWrite(MOTORB_1, LOW);
    digitalWrite(MOTORB_2, LOW);
    digitalWrite(GREEN, LOW);
    digitalWrite(BLUE, HIGH);
    digitalWrite(RED, HIGH);
    servo.write(90);
    tone(BUZZER, 1000, 500);
    delay(500);
    noTone(BUZZER);
    server.sendHeader("Location", "/");
    server.send(303);  // Redireciona de volta à página principal
  });

  // Inicia o servidor web
  server.begin();
  Serial.println("Servidor Web iniciado.");
}

void loop(){
  // Mantém o servidor web em execução
  server.handleClient();
}
 
