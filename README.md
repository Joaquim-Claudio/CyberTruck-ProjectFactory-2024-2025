# CyberTruck-ProjectFactory-2024-2025
# INTRODUÇÃO
Este projeto multidisciplinar envolve a colaboração entre estudantes de Engenharia Informática e Design na conceção e desenvolvimento de um veículo autônomo ou controlado remotamente, projetado para transportar diferentes tipos de carga através de trajetos com obstáculos. O projeto denominado CyberTruck-v1 será desenvolvido por alunos do curso de engenharia informática no âmbito da cadeira Project Factory com o objetivo de desenvolver o chassis do veículo, bem como todo o engenho que garanta as componentes mecânica e elétrica/eletrónica. 

Além da estrutura física, a conceção e desenvolvimento dos sistemas computacionais que permitirão a operação do veículo. Isso inclui a integração de componentes das disciplinas de cunho técnico-científico (com foco no desenvolvimento e tecnologias) que são Computação Física e IoT, Sistemas Operativos e Compiladores, essenciais para a comunicação remota, controlo do movimento e monitoramento em tempo real. 
Este projeto engloba também as áreas de conhecimento estratégico e ético (com foco em gestão, mercado e valores profissionais), sendo elas as cadeiras de Empreendedorismo – Base Tecnológica e Ética e Deontologia Profissional. Importantes para trabalhar o posicionamento estratégico do projeto, analisar o mercado e criar modelo de negócio para uma possível aplicação comercial. Analisar os impactos sociais, éticos e profissionais do desenvolvimento do projeto.

![Modelo 3D do protótipo](https://github.com/Joaquim-Claudio/CyberTruck-ProjectFactory-2024-2025/blob/main/img/CyberTruck_1.png "Modelo 3D do protótipo")

Fig.1 – Modelo 3D do veículo.


# LEVANTAMENTO DE REQUISITOS
## Requisitos Funcionais
**Desenvolvimento de um veículo inteligente:** O veículo deve ser capaz de transportar cargas de um ponto A (Origem) a um ponto B (Destino) através de um percurso com obstáculos e outros desafios, que ilustrem as características de um cenário do mundo real. O veículo deve ser projetado para suportar diferentes formatos de carga (esfera, cilindro, cubo, cone).
Sensores e Atuação: O sistema deve incluir sensores e atuadores para detetar obstáculos, corrigir a trajetória e garantir a estabilidade do veículo e da carga.
> i.	Sensores para deteção de obstáculos (Ex: ultrassônicos HC-SR04);

> ii.	Sensores de aceleração e inclinação para estabilidade e controlo de velocidade (MPU6050);

> iii.	Sensores de peso para detetar o peso total da carga a transportar;

> iv.	Motores DC ou Servo para movimentação e ajuste de direção.

**Automação e Comunicação via rede de dados:** O sistema deve permitir o controlo absoluto do veículo via rede de dados (internet), disponibilizando um um dashboard, que implemente envio/receção de dados via MQTT. Monitoramento dos dados de sensores e envio de comandos remotos. 

**Servidor MQTT e Base de Dados:** Deve existir um servidor central com ligação a uma base de dados, que permita o armazenamento de dados sobre métricas relevantes do veículo, e para intermediar a comunicação entre o agente remoto (dashboard) e o sistema central do veículo (microcontrolador).

**Interface Web/Dashboard:** Desenvolvimento de um dashboard interativo para controlo remoto e monitoramento. Exibição de dados em tempo real (status dos sensores, carga, bateria, etc). Capacidade de enviar comandos ao veículo via rede de dados.

**Autonomia e Controlo Remoto:** Controlo remoto via interface web. Execução de trajeto de forma autônoma. Algoritmos de navegação e desvio de obstáculos. A plataforma deve integrar componentes IoT, permitindo monitoramento e envio de dados em tempo real.

**Gerador de Relatórios Formatados:** O sistema deve ser capaz de gerar relatórios sobre o estado do veículo, a partir dos dados coletados de um dashboard. Um programa Lex/Yacc deve interpretar os dados e convertê-los em um (ou mais) documentos formatados.


## Requisitos Não Funcionais
**Performance:** O sistema deve garantir tempo de resposta menor que 100ms para mensagens MQTT, para possibilitar um controlo remoto “viável”. O processamento dos dados de sensores também deve ser eficiente para evitar atrasos.

**Escalabilidade:** O sistema deve permitir adição de novos sensores ou módulos sem reconfiguração significativa.

**Confiabilidade:** Implementação de QoS (Quality of Service) no MQTT para garantir entrega das mensagens. Implementação de reconexão automática em caso de falha de comunicação.

**Segurança:** Autenticação no servidor MQTT para evitar acessos não autorizados. Uso de TLS/SSL para comunicação segura.

**Manutenção e Atualização:** O sistema deve permitir fácil atualização e customização do dashboard sem interrupção do serviço. Logs detalhados de erros e eventos devem ser mantidos para diagnóstico.


# ARQUITETURA DO SISTEMA
O sistema CyberTruck-v1 é composto essencialmente pelas seguintes componentes ilustradas na figura abaixo:
> i.	MQTT Broker, responsável por mediar a transmissão de dados via MQTT. Nele se ligam os subscribers (receptores) e os publishers (emissores).

> ii.	Node-Red (agente remoto), o “cérebro” por trás da condução autónoma, recebe dados dos sensores do veículo e envia comandos para controlar os atuadores (através de um MQTT Broker).

> iii.	Base de dados para persistir algumas métricas relevantes sobre o veículo e o meio.

> iv.	O veículo, composto de uma série de componentes mecânicos (motores, chassis, etc.) e eletrónicos (sensores, atuadores, etc) que permitem a operação do veículo.

> v.	A funcionamento mínimo do sistema CyberTruck-v1 depende estritamente da ligação a uma rede dados (LAN ou internet).


![Arquitetura do sistema](https://github.com/Joaquim-Claudio/CyberTruck-ProjectFactory-2024-2025/blob/main/img/system-arquitecture.png "Arquitetura do sistema")

Fig.2 – Arquitetura do sistema CyberTruck-v1.
