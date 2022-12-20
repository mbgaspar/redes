A thread abaixo explica que utilizar IP no campo de common name causa problema devido à biblioteca python. Esse recurso deixou de funcionar na conexão TLS.
https://stackoverflow.com/questions/52855924/problems-using-paho-mqtt-client-with-python-3-7

Uma das soluções é baixar a versão do Python (o recurso funciona em versões antigas). Outra solução é gerar os certificados com SAN field (Subject Alternative Name) como na resposta da thread.
Esse diretório foi criado para esses testes.
