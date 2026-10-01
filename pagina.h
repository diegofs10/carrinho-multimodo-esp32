String paginaHTML = R"=====(
<!DOCTYPE html>
<html>
<head>
  <meta charset="UTF-8">
  <meta name="viewport" content="width=device-width, initial-scale=1, maximum-scale=1, user-scalable=no">
  <style>
    body { background-color: #1a1a1a; color: white; font-family: Arial; margin: 0; text-align: center; user-select: none; }
    .tela { display: none; flex-direction: column; height: 100vh; justify-content: center; align-items: center; }
    .ativa { display: flex; }
    
    .btn { padding: 15px 20px; margin: 10px; font-size: 18px; border: none; border-radius: 8px; color: white; cursor: pointer; font-weight: bold; width: 80%; max-width: 300px; }
    .btn-livre { background: #4CAF50; }
    .btn-pet { background: #2196F3; }
    .btn-pulso { background: #FF9800; }
    .btn-controle { background: #9C27B0; }
    .btn-voltar { background: #f44336; margin-top: 15px; }
    
    .vel-titulo { color: #aaa; font-size: 14px; margin-top: 15px; }
    .vel-container { display: flex; justify-content: center; margin: 10px 0 20px 0; width: 90%; max-width: 350px; }
    .vel-btn { background: #444; color: white; padding: 12px 0; flex: 1; margin: 0 2px; border-radius: 5px; cursor: pointer; font-weight: bold; border: 1px solid #333;}
    .vel-ativa { background: #f1c40f; color: black; border-color: #f1c40f; }
    
    #joystick { width: 160px; height: 160px; background: #444; border-radius: 50%; position: relative; touch-action: none; margin: 10px auto; }
    #stick { width: 60px; height: 60px; background: #9C27B0; border-radius: 50%; position: absolute; top: 50px; left: 50px; box-shadow: 0 4px 8px rgba(0,0,0,0.5); }
    #telemetria { color: #aaa; margin-bottom: 5px;}

    .dpad { display: grid; grid-template-columns: 70px 70px 70px; gap: 10px; justify-content: center; margin: 15px 0; }
    .dpad .btn-dir { background: #555; height: 70px; border-radius: 10px; font-size: 28px; display: flex; align-items: center; justify-content: center; color: white;}
    .dpad .vazio { background: transparent; }
    .dpad .btn-dir:active { background: #777; }
  </style>
</head>
<body>

  <!-- TELA 1: MENU PRINCIPAL -->
  <div id="tela-menu" class="tela ativa">
    <h2>Painel do Robô</h2>
    <button class="btn btn-livre" onclick="abrirTela('tela-status', 'L', 'Modo Livre em Operação')">Modo Livre</button>
    <button class="btn btn-pet" onclick="abrirTela('tela-status', 'A', 'Modo Pet Ativado')">Modo Pet</button>
    <button class="btn btn-pulso" onclick="abrirTela('tela-pulso', 'P')">Modo Pulso</button>
    <button class="btn btn-controle" onclick="abrirTela('tela-controle', 'C')">Modo Controle</button>
  </div>

  <!-- TELA 2: STATUS (Para Livre e Pet) -->
  <div id="tela-status" class="tela">
    <h1 id="status-texto">Modo Ativado</h1>
    <p style="color: #aaa;">Os sensores estão operando...</p>
    
    <div class="vel-titulo">Ajuste de Velocidade (%)</div>
    <div class="vel-container">
      <div class="vel-btn vel-ativa" onclick="setVel(60)">60</div>
      <div class="vel-btn" onclick="setVel(70)">70</div>
      <div class="vel-btn" onclick="setVel(80)">80</div>
      <div class="vel-btn" onclick="setVel(90)">90</div>
      <div class="vel-btn" onclick="setVel(100)">100</div>
    </div>

    <button class="btn btn-voltar" onclick="abrirTela('tela-menu', 'P')">Parar Robô e Voltar</button>
  </div>

  <!-- TELA 3: CONTROLE (Joystick) -->
  <div id="tela-controle" class="tela">
    <h2>Controle Analógico</h2>
    <div id="telemetria">X: <span id="valX">0</span> | Y: <span id="valY">0</span></div>
    <div id="joystick"><div id="stick"></div></div>
    
    <div class="vel-titulo">Velocidade Máxima (%)</div>
    <div class="vel-container">
      <div class="vel-btn vel-ativa" onclick="setVel(60)">60</div>
      <div class="vel-btn" onclick="setVel(70)">70</div>
      <div class="vel-btn" onclick="setVel(80)">80</div>
      <div class="vel-btn" onclick="setVel(90)">90</div>
      <div class="vel-btn" onclick="setVel(100)">100</div>
    </div>

    <button class="btn btn-voltar" onclick="abrirTela('tela-menu', 'P')">Parar Robô e Voltar</button>
  </div>

  <!-- TELA 4: PULSO (D-Pad Setas) -->
  <div id="tela-pulso" class="tela">
    <h2>Comandos de Pulso</h2>
    <div class="dpad">
      <div class="vazio"></div>
      <div class="btn-dir" ontouchstart="enviarCmd('F'); event.preventDefault();">▲</div>
      <div class="vazio"></div>
      
      <div class="btn-dir" ontouchstart="enviarCmd('E'); event.preventDefault();">◄</div>
      <div class="btn-dir" style="background:#f44336;" ontouchstart="enviarCmd('S'); event.preventDefault();">■</div>
      <div class="btn-dir" ontouchstart="enviarCmd('D'); event.preventDefault();">►</div>
      
      <div class="vazio"></div>
      <div class="btn-dir" ontouchstart="enviarCmd('T'); event.preventDefault();">▼</div>
      <div class="vazio"></div>
    </div>
    
    <div class="vel-titulo">Ajuste de Velocidade (%)</div>
    <div class="vel-container">
      <div class="vel-btn vel-ativa" onclick="setVel(60)">60</div>
      <div class="vel-btn" onclick="setVel(70)">70</div>
      <div class="vel-btn" onclick="setVel(80)">80</div>
      <div class="vel-btn" onclick="setVel(90)">90</div>
      <div class="vel-btn" onclick="setVel(100)">100</div>
    </div>

    <button class="btn btn-voltar" onclick="abrirTela('tela-menu', 'S')">Parar Robô e Voltar</button>
  </div>

  <script>
    function abrirTela(idTela, comandoModo, textoStatus = '') {
      document.querySelectorAll('.tela').forEach(t => t.classList.remove('ativa'));
      document.getElementById(idTela).classList.add('ativa');
      
      if(textoStatus !== '') document.getElementById('status-texto').innerText = textoStatus;
      if(comandoModo) enviarCmd(comandoModo);
    }

    // A mágica da sincronização visual das barras
    function setVel(valor) {
      document.querySelectorAll('.vel-btn').forEach(b => {
        if(b.innerText == valor) b.classList.add('vel-ativa');
        else b.classList.remove('vel-ativa');
      });
      enviarCmd('V' + valor);
    }

    function enviarCmd(cmd) { fetch('/comando?cmd=' + cmd); }

    let stick = document.getElementById('stick');
    let joystick = document.getElementById('joystick');
    let ativo = false, ocupado = false;
    let cx = 80, cy = 80, raio = 50; 

    joystick.addEventListener('touchstart', (e) => { ativo = true; e.preventDefault(); });
    joystick.addEventListener('touchend', () => { ativo = false; stick.style.transform = `translate(0px, 0px)`; enviarJoy(0, 0); });
    
    joystick.addEventListener('touchmove', (e) => {
      if (!ativo) return;
      e.preventDefault();
      let rect = joystick.getBoundingClientRect();
      let x = e.touches[0].clientX - rect.left - cx;
      let y = e.touches[0].clientY - rect.top - cy;
      let dist = Math.sqrt(x*x + y*y);
      
      if (dist > raio) { x = (x/dist)*raio; y = (y/dist)*raio; }
      stick.style.transform = `translate(${x}px, ${y}px)`;
      
      enviarJoy(Math.round((x/raio)*100), Math.round((-y/raio)*100));
    });

    function enviarJoy(x, y) {
      document.getElementById('valX').innerText = x;
      document.getElementById('valY').innerText = y;
      if(!ocupado) { ocupado = true; fetch(`/joy?x=${x}&y=${y}`).then(() => ocupado = false).catch(() => ocupado = false); }
    }
  </script>
</body>
</html>
)=====";