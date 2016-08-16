#include <SPI.h> //Biblioteca SPI (Serial Peripheral Interface)
#include <Ethernet.h> //Biblioteca EthernetShield 
String readString;

int Rele = 4; // Variável para armazenar a leitura do rele;

//Informacoes de endereco IP, gateway, mascara de rede;
byte mac[] = { 0xA4, 0x28, 0x72, 0xCA, 0x55, 0x2F }; //Define o endereço físico da placa
byte ip[] = { 192, 168, 1, 202 }; //Ip da EthernetShield
byte gateway[] = { 192, 168, 1, 1 }; //Gateway (opcional)
byte subnet[] = { 255, 255, 255, 0 }; //Mascara de subrede
 
EthernetServer server(9999); //Cria o objeto na porta 9999

void setup ()
{
 Serial.begin(9600);
 pinMode(Rele, OUTPUT);      

  //Inicializa Ethernet Shield;
  Ethernet.begin(mac, ip, gateway, subnet); //Inicializando a ethernet
  server.begin(); //Inicializando o servidor web
  
  Serial.println("Codlight"); 
 
  //Desliga o Rele;
  digitalWrite(Rele, HIGH); 
}
            
 void loop () 
{
 EthernetClient client = server.available();
  if (client) { 
    while (client.connected())
    {
      if (client.available())
      {
        char c = client.read();
        if (readString.length() < 100) {
          readString += c;
        }
          if (c == '\n')
        { 
          //Controle Rele;         
            if(readString.indexOf("?reiniciar") > 0)
            {
              digitalWrite(Rele, HIGH);//Desligar Rele;
              delay(5000); 
              digitalWrite(Rele, LOW);//Ligar Rele;
            }
            readString="";
      
          client.println("HTTP/1.1 200 OK");//Inicio do codigo HTML;
          client.println("Content-Type: text/html");
          client.println();
           
          client.println("<html>");
          client.println("<head>");         
          client.println("<title>CODLIGHT</title>");
          client.println("<meta http-equiv='Content-Type' content='text/html; charset=utf-8'>");          
          client.println("</head>");
          client.println("<div style='padding-top: 2px; margin-left: 500px; margin-right: 500px; font-family: Garamond, serif; color: #000000;'><p id='home'>CODLIGHT</p></a></div>");
          client.println("<body>");
          client.println("<div style='padding-top: 50px; margin-left: 500px; margin-right: 500px; font-family: Garamond, serif; color: #000000;'>");
	  client.println("<label>Central Telefônica</label>");
          
          client.println("<a href=\"/?reiniciar\"\"><input type='button' value='Reiniciar'style='display: inline-block; background-color: #ccc; color: #444; padding: 10px 20px; text-decoration: none; box-sizing: border-box; font-family: Helvetica, Arial, sans-serif; font-size: 14px;border: 0px;color: #fff; background-color: #CC0000; box-shadow: 1px 1px 1px #999; border-radius: 4px;'></a>");//Botao Reiniciar  
           
          client.println("</div>");          
          client.println("</body>");
          client.println("</html>");//Fim do codigo HTML;
           
          delay(1); // Tempo para o navegador receber os dados
          client.stop(); // Fecha a conexão    
        }      
      }
    }
  }
}

