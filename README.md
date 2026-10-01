# 🏎️ Carrinho Robótico Multimodo (ESP32 / IoT)

> **Projeto prático de automação, prototipagem eletrônica e sistemas embarcados** desenvolvido durante o curso Técnico em Informática no Senac.

---

### 🎯 Sobre o Projeto
Este projeto consiste no desenvolvimento e montagem de um carrinho robótico baseado na plataforma **ESP32**, capaz de operar em múltiplos modos de navegação e controle remoto. 

O sistema foi projetado para integrar hardware, comunicação sem fio e lógica de programação em C++, permitindo versatilidade de uso tanto em modos autônomos quanto sob comando do usuário.

---

### 🎮 Modos de Operação
- 🖐️ **Controle por Gestos (Luva Sensorial):** Leitura de orientação e movimento via sensor na luva, enviando comandos em tempo real para o carrinho via comunicação sem fio.
- 📱 **Controle Wi-Fi / Web (Celular ou PC):** Interface de controle acionada via navegador ou aplicação conectada à rede.
- 🤖 **Modo Autônomo:** Navegação e desvio de obstáculos utilizando sensores de distância/presença.

---

### 🛠️ Hardware & Componentes
- **Placa Principal:** ESP32 (Wi-Fi / Bluetooth integrado)
- **Sensores:** Sensor de movimento/orientação (Luva) e sensores de distância (Carrinho)
- **Atuadores:** Motores DC com Driver de ponte H
- **Alimentação:** Baterias recarregáveis e circuito de distribuição de energia

---

### 💻 Tecnologias e Conceitos Aplicados
- **Linguagem:** C / C++ (Ambiente Arduino IDE)
- **Comunicação Sem Fio:** Protocolos de rede Wi-Fi / ESP-NOW para controle de baixa latência
- **Sistemas Embarcados:** Controle de sinais PWM para velocidade dos motores, leitura analógica e digital de sensores e depuração via porta serial (*Serial Debugging*).
- **Inclusão de Bibliotecas:** Uso de bibliotecas de ecossistema aberto para integração de sensores e controle de rede.

---

### 👤 Autor
Desenvolvido por **Diego Ferreira**  
[LinkedIn](https://www.linkedin.com/in/diego-ferreira-4b6272144)
