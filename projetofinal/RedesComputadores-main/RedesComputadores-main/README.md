# RedesComputadores

Projeto Final da disciplina de Redes de Computadores

O projeto final da disciplina de Redes de Computadores será a implementação de um sistema de monitoramento e controle da iluminação de ambientes. Será utilizado o protocolo MQTT para a comunicação entre o sensor, servidor e cliente.

O módulo ESP8266 será utilizado para fazer a conexão com a  internet e enviar/receber os dados MQTT para/de um servidor (broker). Este também controlará o relé que ficará conectado no circuito que energiza as lâmpadas. 

O broker MQTT será instalado em uma placa Raspberry Pi (RPI), que deverá ser responsável pela conexão com o hardware de controle e monitoração.

Uma interface WEB deverá ser desenvolvida para a monitoração e acionamento do relé. O serviço web pode ser executado diretamente na RPI ou em alguma plataforma de serviço (PaaS). 
