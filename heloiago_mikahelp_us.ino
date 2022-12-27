//Definido pelo professor o Hexadecimal necessário.
#define TEMPERATURA 0x54
#define UMIDADE 0x48
#define YELLOW_LED 7
#define GREEN_LED 6

// esses dois define ficaram confusos de entender, mas o mikael sugeriu colocar por conta de um erro para perca de pacotes.
#define SYNC_I 'A' 
#define SYNC_O 'Z'  

#define BUFF_LEN 14 // <- a estrutura tem tamanho de 11 bytes
//ai foi botado mais dois bytes de IN e Out (SYNC_I and SYNC_O), e mais um para ficar par.

// feito pelo mikael, pare retornar com 0 ou 1 caso de certo ou de problema.
#define PKG_ERROR 0
#define PKG_SUCCESS 1


#include <Adafruit_Sensor.h> //biblioteca adicionada para leitura dos sensores de humidade e temperatura

#include <DHT_U.h>  // <-- unificação (veio do exemplo, nem sei pra o que ta servindo)
#include <DHT.h>    // <-- unificação (veio do exemplo, nem sei pra o que ta servindo)

#define DHTPIN A1     // Pino conectado no PDF

// Uncomment the type of sensor in use:
#define DHTTYPE    DHT11  // DHT 11 é o do PDF 

DHT_Unified dht(DHTPIN, DHTTYPE);  // <-- exemplo do github.

//Cabeçalho necessário pedido pelo professor para ser incrementado, totalizando 11 bytes de extensão.
typedef struct Frame {
  uint32_t  N_REQ;      // 4 bytes
  uint8_t   CMD;        // 1 byte
  float     DATA;       // 4 bytes
  int16_t   CHECKSUM;   // 2 bytes  
} frame;

// Respostas do microcontrolador pedido pelo pelo professor para ser incrementado. 
void modify_package(frame* pkg, float data) 
  {
  pkg->CMD      = 0xFF;
  pkg->DATA     = data;
  uint16_t sum = (pkg->N_REQ + pkg->CMD + data);
  pkg->CHECKSUM = ~sum;
  }

int verify_package(frame* received) {
  // Check if the command is valid
  if ((received->CMD != TEMPERATURA) && (received->CMD != UMIDADE)) {
    return PKG_ERROR;
  }

  // Check if the package was corruptedtjtt
  uint16_t sum = (received->N_REQ + received->CMD + received->DATA);
  if (received->CHECKSUM != ~sum) {
    return PKG_ERROR;
  }

  // If all was right the package is OK
  return PKG_SUCCESS;
}

void setup() {
  Serial.begin(9600);
  pinMode(YELLOW_LED, OUTPUT);
  pinMode(GREEN_LED, OUTPUT);
  
  dht.begin();
}

void loop() {
  // Delay between measurements.
  delay(1000); // delay de 1 segundo para visualizar respostas.
  digitalWrite(GREEN_LED, LOW);
  digitalWrite(YELLOW_LED, LOW);
  delay(1000);

  sensors_event_t event; //se acontece algum evento, ou recebe mensagem pelo sensor, ai pega temperatura (sensor ta mandando pra ele).
  // isso de "sensors_event_t event;" ta tudo escondido, a biblioteca que tá fazendo.
  if (Serial.available()) {
    frame package;
    
    receive_package(&package); // ta passando a referencia da memória, onde tá esse frame, vai montar o pacote onde tá
    // ai evita estar passando muita memória pra lá e pra cá.

    if (verify_package(&package) != PKG_ERROR) {
      if (package.CMD == TEMPERATURA) {
        dht.temperature().getEvent(&event); 

        modify_package(&package, event.temperature);

      } else {
        dht.humidity().getEvent(&event);

        modify_package(&package, event.relative_humidity); 
      }
      send_package(&package);
      digitalWrite(GREEN_LED, HIGH);
      delay(2000);
    } else {
      digitalWrite(YELLOW_LED, HIGH);
      send_package(&package);
      delay(2000);
    }
  }

  digitalWrite(YELLOW_LED, HIGH);
  digitalWrite(GREEN_LED, HIGH);
}

void send_package(frame* pkg) {
  uint8_t bytes[BUFF_LEN];
  uint8_t pkg_place;

  memset(bytes, 0, BUFF_LEN);
  pkg_place = ((BUFF_LEN)/2) - (sizeof(frame)/2);
  bytes[pkg_place - 1] = SYNC_I;
  memcpy(&bytes[pkg_place], pkg, sizeof(frame));
  bytes[pkg_place + sizeof(frame)] = SYNC_O;
  
  Serial.write(bytes, BUFF_LEN);
}

void receive_package(frame* pkg) {  // 

  uint8_t bytes[BUFF_LEN]; //
  
  Serial.readBytes(bytes, BUFF_LEN); //ele ta jogando 16 bytes aqui dentro, ta colocando 16 bytes nesse espaço vazio que acabei de criar
  
  memset(pkg, 0, sizeof(frame));  //setou tudo pra zero, para nao ter nenhum lixo
  for (int i = 0; i < BUFF_LEN; ++i) { //nesse for vai varrer o byte para localizar onde esta o byte de inicio e de fim da estrutura.
    if ((bytes[i] == SYNC_I) && (bytes[i + sizeof(frame)+1] == SYNC_O)) {
      memcpy(pkg, &bytes[i+1], sizeof(frame)); //todos os bytes aqui eu jogo pro packet
      break;
    }
  }
}
