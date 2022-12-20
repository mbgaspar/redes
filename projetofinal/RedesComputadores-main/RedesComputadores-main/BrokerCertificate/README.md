# Sumário

1. [Conexão com a RaspberryPi](#conexão-com-a-raspberrypi)
2. [Configurando UFW firewall da RaspberryPi](#configurando-ufw-firewall-da-raspberrypi)
3. [Configurando o Mosquitto](#configurando-o-mosquitto)
4. [Configurando o Fail2Ban](#configurando-o-fail2ban)
5. [Gerando certificados](#gerando-certificados)

# Documentação



# Conexão com a RaspberryPi

A conexão é feita via Linux fazendo a conexão SSH na placa. A porta que está sendo utilizada é a 22109, portanto, o comando pra conectar é o seguinte:

`
ssh -p 22109 pi@172.16.158.155
`

Importante notar que o SSH não funciona via Windows, somente via Linux, portanto é necessário utilizar sistema Linux pra fazer a conexão.
Tomar cuidado na hora de digitar a senha, pois 3 tentativas erradas bane o IP que está fazendo a conexão.

# Configurando UFW firewall da RaspberryPi

É feito a instalação na Raspberry do módulo UFW utilizando:

`
sudo apt-get install ufw
`

Após instalar, deve-se habilitar o UFW rodando o comando:

`
sudo ufw enable
`

Após isso deve-se verificar o funcionamento do UFW rodando o comando de status:

`
sudo ufw status
`

Se retornou algo além de erro é porque está funcionando. Após isso é feita a configuração das portas, quais são habilitadas e quais são bloqueadas pelo firewall. 
Por padrão como comunicação iremos utilizar a 8883, então ela deve ser habilitada rodando o comando:

`
sudo ufw allow 8883
`

Além disso vamos ter que bloquear a porta 22, a 1883 (já que não tem comunicação sem criptografia), e vamos ter que habilitar também a 22109 para a conexão com a raspberry. Para bloquear a porta o comando é o mesmo exceto pelo allow que vira deny. Feito isso o servidor já está configurado.

O tutorial seguido foi [esse](https://linuxhint.com/set-up-ufw-firewall-raspberry-pi/).


# Configurando o Mosquitto

### MQTT (Message Queuing Telemetry Transport) 
Protocolo de mensagem otimizado para redes TCP/IP. É simples, leve e seguro. 

Atualmente usado em diversas indústrias como: telecomunicações, automotiva, gás, petróleo...
Fundamento: Publicador-Subscritor

### Eclipse Mosquitto - Open Source MQTT 
Instalando o pacote mosquitto: 

	sudo apt install -y mosquitto

O comando mosquitto inicia o serviço, já o comando 'pidof' é recomendado para verificar o status, o retorno de um pid significa que o serviço está em execução.

	mosquitto
	pidof mosquitto
As configurações são encontradas em /etc/mosquitto, onde deve haver um arquivo de configuração mosquitto.conf e um diretório conf.d. Como padrão, a inicialização do mosquitto chama o arquivo mosquitto.conf.

É recomendado fazer as configurações em um arquivo como 'custom.conf' em conf.d, e então adicionar o caminho no mosquitto.conf: 

	include_dir /etc/mosquitto/conf.d. 

Essa inclusão garante que os arquivos no diretório conf.d sejam elegíveis para execução. Para iniciar o servidor com o arquivo personalizado 'custom.conf' é preciso executar

	mosquitto -c 'filepath'
	
Nesse caso:

	mosquitto -c /etc/mosquitto/conf.d/custom.conf

As configurações utilizadas no projeto podem ser encontradas em custom.conf na pasta atual desse git. 

Pontos importantes:

- Declarar as portas utilizadas e seus protocolos
- Declarar a versão TLS utilizada: 'tls_version tlsv...'


# Configurando o Fail2Ban

Primeiro instalamos o fail2ban na Raspberry Pi usando:

`
sudo apt-get install fail2ban
`

Depois de instalar o próprio fail2ban cria um arquivo "jail.conf", o qual fazemos uma cópia e nomeamos de "jail.local", e com isso o fail2ban já detecta esse arquivo e puxa a configuração para o servidor. No jail.local fazemos a configuração para funcionar em ssh e damos um enable para ele efetivamente funcionar. Podemos também mexer nas configurações de banimento, como por exemplo a quantidade máxima de tentativas, o que acontece com quem for banido, o tempo de banimento. Na Raspberry utilizamos:

`
ação de banimento: banir;
tempo de banimento: indefinido;
número de tentativas: 3;
`

Depois de configurado, reiniciamos o serviço e está pronto para utilizar.

O tutorial dessa configuração foi encontrado [aqui](https://pimylifeup.com/raspberry-pi-fail2ban/).

# Gerando certificados

Os comandos encontrados em criando_chavescertificado_broker.txt não geram separadamente os arquivos client, é usado apenas a chave pública (ca-root-cert.crt) para a conexão TLS.

Abaixo está o link do tutorial que gera os certificados para autenticação do cliente:

https://openest.io/en/services/mqtts-how-to-use-mqtt-with-tls/
Common name utilizado foi o IP da Raspberry Pi: 172.16.158.155

Uma vez gerados, o caminho dos certificados deve ser adicionado ao arquivo de configuração do servidor.

	cafile 		'path_to_cafile'
	
	certfile 	'path_to_crtfile'
	
	keyfile 	'path_to_keyfile'
