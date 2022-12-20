# Sumário

1. [Preparando o ambiente do arduino para ESP-8266](#Preparando-o-ambiente-do-arduino-para-ESP8266)
2. [Configurando a IDE para o modelo utilizado](#Configurando-a-IDE-para-o-modelo-utilizado)
3. [Cuidados com o código](#Cuidados-com-o-código)
4. [Validação do funcionamento da ESP](#Validação-do-funcionamento-da-ESP)

# Documentação



# Preparando o ambiente do arduino para ESP8266
A IDE do arduino não funciona nativamente com a ESP-8266 para ela funcionar com a ESP é primeiro necessário instalar o pacote ESP que será utilizado que no caso foi o seguinte:

`
http://arduino.esp8266.com/stable/package_esp8266com_index.json
`

Para se instalar esse pacote é necessário ir na IDE do arduino na aba `Arquivo/Preferências` e copiar e colar no campo URLs Adicionais para Gerenciadores de Placas para possibilitar a IDE funcionar com a ESP.

# Configurando a IDE para o modelo utilizado

Primeiramente é necessário selecionar qual a placa se está trabalhando como é possível ver na imagem abaixo:

![modelo_placa](https://user-images.githubusercontent.com/53865374/181131338-ecd72340-78ce-44e2-b7ef-1ce56a0c738a.png)

Após selecionar a placa deve-se selecionar qual a porta USB que ela está conectada.

Para concluir a configuração da IDE é necessário instalar as duas bibliotecas abaixo e atenção a sua versão pois versões muito recentes podem ter divergências é recomendado instalar as versões da imagem abaixo.

![libs](https://user-images.githubusercontent.com/53865374/181131354-e0850248-a22d-45a2-a63e-19a694f659b2.png)


# Cuidados com o código

Primeiramente é necessário se prestar atenção na identação do certificado ele não deve possuir nenhum espaço para não ter nenhum erro. A função que trata a interrupção de recebimento de informação no nosso caso `messageReceived` tem como padrão dois parâmetros de String que correspondem respectivamente ao tópico e a mensagem recebida e tem que ser mantido esse padrão. É necessário também sempre se certificar que o certificado está atualizado.

# Validação do funcionamento da ESP

Para se validar o funcionamento da ESP independente do broker do projeto estar pronto ou não é possivel usar o link https://test.mosquitto.org/ para se fazer um teste com o certificado gerados por eles e ter a certeza que o código é 100% funcional para se fazer esse teste só necessário alterar a variável  `MQTT_HOST[]` que corresponde ao IP do servidor do broker pode ser usado o próprio link entre aspas duplas, com isso é possível fazer um teste independente de conexão.
