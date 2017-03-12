#include <SPI.h>
#include <Ethernet.h>

#include <Servo.h> 
Servo myservo1;  //Definicao do servo motor

int cooler = 1;
int led = 1;
int portao = 1;
byte mac[] = { 0x00, 0xAA, 0xBB, 0xCC, 0xDE, 0xE02 }; //Endereco MAC
byte ip[] = { 192, 168, 1, 99 }; // IP da rede
//Observacao: Usar IP vago. abrir cmd e pingar ip's até achar um livre
byte gateway[] = { 192, 168, 1, 1 }; // Gateway padrão
byte subnet[] = { 255, 255, 255, 0 }; // Mascara de Subrede
EthernetServer server(80); //Porta do servidor

String readString; 

//////////////////////

void setup(){


  Ethernet.begin(mac, ip, gateway, subnet);
  server.begin();
  pinMode(6, OUTPUT);
  pinMode(4, OUTPUT);

  myservo1.write(90); //Posicao inicial do servo motor
  
  myservo1.attach(9);  //Pino onde esta o servo motor
  
  Serial.begin(9600); 
  Serial.println("server servo/pin 9,8 test 1.0");
}

void loop()
{
  //Cria uma conexão
  EthernetClient client = server.available();
  if (client) {
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();

        //Ler todos os caracteres que o HTTP precisar 
        if (readString.length() < 100) {

          //Armazena caracteres na funcao String
          readString += c; 
        } 

        //Se o pedido for encerrado
        if (c == '\n') {

          ///////////////
          Serial.println(readString);

          client.println("HTTP/1.1 200 OK"); //Envia a nova pagina
          client.println("Content-Type: text/html");
          client.println();

          client.println("<HTML>");
          client.println("<HEAD>");
          client.println("<meta name='apple-mobile-web-app-capable' content='yes' />");
          client.println("<meta name='apple-mobile-web-app-status-bar-style' content='black-translucent' />");
          client.println("<link rel='stylesheet' type='text/css' href='http://homeautocss.net84.net/a.css' />"); //Nossa referencia de estilo para a pagina
          client.println("<TITLE>Automacao Residencial</TITLE>");
          client.println("</HEAD>");
          client.println("<BODY>");
          client.println("<H1>Automacao Residencial</H1>");
          client.println("<hr />");
          client.println("<br />");
         
         //Botoes e imagens
          client.println("<a href=\"/portao\"\"><img src='http://i.imgur.com/GJ8KKKa.gif'></a>");          
          client.println("<a href=\"/lampada\"\"><img src='http://i.imgur.com/xDFq8Nk.gif'></a>");
          client.println("<br />");  
          client.println("<br />");   
          client.println("<a href=\"/ventilador\"\"><img src='http://i.imgur.com/8jiaNja.gif'></a>");
       
          client.println("<br />");  
          client.println("<br />");            
          client.println("<hr />");

          client.println("</BODY>");
          client.println("<br />");
          client.println("<body>Thomas Machado</body>");
          client.println("<br />");
          client.println("<body>EEL7802 - Projeto em Eletronica II</body>");
          
          client.println("</HTML>");
 
          delay(100);
          //Encerra a conexao
          client.stop();

          //Ligar e desligar o portao

          if(readString.indexOf("portao") >0)
          {
            if (portao == 1) {
              myservo1.write(0);
              portao = 0;
            }
            else if (portao == 0) {
              myservo1.write(90);
              portao = 1;
            }
           
          }
          
          //Ligar e desligar a lampada
          
          if(readString.indexOf("lampada") >0)
          {
            if (led == 1) {
            digitalWrite(6, HIGH);    //Pino 6 ligado
            Serial.println("Led On");
            led = 0;
          }
          
            else if (led == 0) {
            digitalWrite(6, LOW);    //Pino 6 desligado
            Serial.println("Led Off");
            led = 1;
          }
          }
          
          if(readString.indexOf("ventilador") >0)
          {
            if (cooler == 1) {
            digitalWrite(4, HIGH);    //Pino 4 ligado
            Serial.println("cooler On");
            cooler = 0;
          }
          
            else if (cooler == 0) {
            digitalWrite(4, LOW);    //Pino 4 desligado
            Serial.println("cooler Off");
            cooler = 1;
          }
          }
                    
      
               
          
          readString="";
          

        }
      }
    }
  }
} 



