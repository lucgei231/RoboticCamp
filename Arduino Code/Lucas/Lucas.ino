#include <WiFi.h>
#include <WebServer.h>
#include <ESPmDNS.h>
#include <WebSocketsServer.h>
#include <WiFiManager.h> // Ensure you have the WiFiManager library installed

WebServer server(80);
WebSocketsServer webSocket = WebSocketsServer(81);

bool forwardMode = false; // Global flag to trigger infinite forward movement

void stopMotors(){
  analogWrite(13, 0);
  analogWrite(12, 0);
  analogWrite(14, 0);
  analogWrite(27, 0);
}

void moveForward(int fspeed){
  analogWrite(13, 0);
  analogWrite(12, fspeed * 2);
  analogWrite(14, 0);
  analogWrite(27, fspeed * 2);
  Serial.println("Moving forward with speed: " + String(fspeed));
  // Additional logic for moving forward
}

void moveBackward(int bspeed){
  analogWrite(13, bspeed * 2);
  analogWrite(12, 0);
  analogWrite(14, bspeed * 2);
  analogWrite(27, 0);
}

void turnRight(int lspeed){
  analogWrite(13, 0);
  analogWrite(12, lspeed * 2.5);
  analogWrite(14, lspeed * 2.5);
  analogWrite(27, 0);
}

void turnLeft(int rspeed){
  analogWrite(13, rspeed * 2.5);
  analogWrite(12, 0);
  analogWrite(14, 0);
  analogWrite(27, rspeed * 2.5);
}

void setHue(int hue) {
  Serial.print("Set hue to: ");
  Serial.println(hue);
}

void stopAction() {
  stopMotors();
}

void moveForwardAction() {
  moveForward(255);
}

void forwardBackward(int FBValue) {
  if(FBValue >= 0)
    moveForward(FBValue);
  else
    moveBackward(-FBValue);
}

void leftRightAction(int LRValue) {
  if(LRValue >= 0)
    turnRight(LRValue);
  else
    turnLeft(-LRValue);
}

void webSocketEvent(uint8_t num, WStype_t type, uint8_t * payload, size_t length) {
  if (type == WStype_TEXT) {
    String msg = String((char*)payload);
    int commaIndex = msg.indexOf(',');
    String cmd, valStr;
    int value = 0;
    bool hasValue = false;
    if (commaIndex > 0) {
      cmd = msg.substring(0, commaIndex);
      valStr = msg.substring(commaIndex + 1);
      valStr.trim();
      if (valStr.length() > 0) {
        value = valStr.toInt();
        hasValue = true;
      }
    } else {
      cmd = msg;
      cmd.trim();
    }

    if (cmd == "hue" && hasValue) {
      setHue(value);
    } else if (cmd == "fb" && hasValue) {
      forwardBackward(value);
    } else if (cmd == "lr" && hasValue) {
      leftRightAction(value);
    } else if (cmd == "stop") {
      stopAction();
    } else if (cmd == "start") {
      moveForwardAction();
    } else {
      Serial.println("Unknown command received");
    }
    webSocket.sendTXT(num, "ESP32 got: " + msg);
  }
}

void handleRoot() {
  Serial.println("Request for root path received");
  server.send(200, "text/html", R"rawliteral(
<!DOCTYPE html>
<html lang="en">
  <head>
    <meta charset="UTF-8">
    <title>Robium Controller</title>
  </head>
  <body>
    <div class="svg-container">
      <svg
         width="136.90706mm"
         height="82.806686mm"
         viewBox="0 0 136.90706 82.806686"
         version="1.1"
         id="svg1"
         inkscape:version="1.4 (86a8ad7, 2024-10-11)"
         sodipodi:docname="Joystic.svg"
         xmlns:inkscape="http://www.inkscape.org/namespaces/inkscape"
         xmlns:sodipodi="http://sodipodi.sourceforge.net/DTD/sodipodi-0.dtd"
         xmlns="http://www.w3.org/2000/svg"
         xmlns:svg="http://www.w3.org/2000/svg">
        <!-- ... existing SVG elements ... -->
      </svg>
    </div>
    <script>
      let ws = new WebSocket('ws://esp32.local:81/');
      ws.onopen = () => {
        console.log('WebSocket opened');
        const tspan2 = document.getElementById('tspan2');
        if(tspan2) { tspan2.textContent = "Ready"; }
      }
      ws.onmessage = evt => console.log('From ESP32: ' + evt.data);
  
      const stopEl = document.getElementById('g57');
      stopEl && stopEl.addEventListener('click', function(evt) {
          console.log('stop clicked');
          ws.send('stop,');
      });
  
      const startEl = document.getElementById('g58');
      startEl && startEl.addEventListener('click', function(evt) {
          ws.send('start,');
      });
  
      const topY = 30;
      const bottomY = 72;
      const leftCircle = document.getElementById('g59');
      const tspan37 = document.getElementById('tspan37');
      leftCircle && leftCircle.addEventListener('click', function(evt) {
          const svg = document.getElementById('svg1');
          const pt = svg.createSVGPoint();
          pt.x = evt.clientX;
          pt.y = evt.clientY;
          const cursorpt = pt.matrixTransform(svg.getScreenCTM().inverse());
          let v = 100 - ((cursorpt.y - topY) / (bottomY - topY)) * 200;
          v = Math.max(-100, Math.min(100, v));
          v = Math.round(v);
          tspan37.textContent = v;
          ws.send('fb,'+v);
      });
  
      const leftX = 81;
      const rightX = 123;
      const rightCircle = document.getElementById('g60');
      const tspan39 = document.getElementById('tspan39');
      rightCircle && rightCircle.addEventListener('click', function(evt) {
          const svg = document.getElementById('svg1');
          const pt = svg.createSVGPoint();
          pt.x = evt.clientX;
          pt.y = evt.clientY;
          const cursorpt = pt.matrixTransform(svg.getScreenCTM().inverse());
          let v = 100 - ((cursorpt.x - leftX) / (rightX - leftX)) * 200;
          v = Math.max(-100, Math.min(100, v));
          v = Math.round(v)*(-1);
          tspan39.textContent = v;
          ws.send('lr,'+v);
      });
  
      // WASD and arrow keys
      document.addEventListener('keydown', function(event) {
          switch (event.key) {
            case 'ArrowUp':
            case 'w':
              tspan37.textContent = 100;
              ws.send('fb,100');
              break;
            case 'ArrowDown':
            case 's':
              tspan37.textContent = -100;
              ws.send('fb,-100');
              break;
            case 'ArrowLeft':
            case 'a':
              tspan39.textContent = -100;
              ws.send('lr,-100');
              break;
            case 'ArrowRight':
            case 'd':
              tspan39.textContent = 100;
              ws.send('lr,100');
              break;
            default:
              break;
          }
      });
  
      // For desktop, always apply the SVG styling
      const svg = document.getElementById('svg1');
      svg.removeAttribute('width');
      svg.removeAttribute('height');
      svg.style.width = '100vw';
      svg.style.height = '100vh';
      svg.style.maxWidth = '100vw';
      svg.style.maxHeight = '100vh';
      svg.style.display = 'block';
    </script>
  
    <style>
      body, html {
        margin: 0;
        padding: 0;
        width: 100vw;
        height: 100vh;
        overflow: hidden;
      }
      .svg-container {
        width: 100vw;
        height: 100vh;
        display: flex;
        align-items: center;
        justify-content: center;
      }
      svg {
        max-width: 100vw;
        max-height: 100vh;
        width: 136.90706mm;
        height: 82.806686mm;
        display: block;
      }
    </style>
  </body>
</html>
  )rawliteral");
}

void setup(){
  stopMotors();
  Serial.begin(9600);
  
  // Set GPIO 0 as input with pull-up so button press is detected (LOW when pressed)
  pinMode(0, INPUT_PULLUP);
  
  // Set ESP32 to dual mode (AP+STA)
  WiFi.mode(WIFI_AP_STA);
  
  // Start the Access Point open (no password)
  WiFi.softAP("LucasPotato");
  Serial.print("AP IP Address: ");
  Serial.println(WiFi.softAPIP());
  
  // Attempt to connect to STA network
  WiFi.begin("mypotato", "coolpotato");
  Serial.print("Connecting to STA network");
  unsigned long startTime = millis();
  while (WiFi.status() != WL_CONNECTED && (millis() - startTime) < 10000) {
    delay(500);
    Serial.print(".");
  }
  
  if(WiFi.status() == WL_CONNECTED) {
    Serial.println("\nConnected to STA network");
    Serial.print("STA IP Address: ");
    Serial.println(WiFi.localIP());
  } else {
    Serial.println("\nSTA connection failed, starting WiFiManager configuration portal");
    WiFiManager wm;
    wm.resetSettings(); // Optional: clear old settings
    if(!wm.startConfigPortal("LucasPotato_Config")) {
      Serial.println("Failed to connect via WiFiManager");
    }
    else {
      Serial.print("STA IP Address (after config): ");
      Serial.println(WiFi.localIP());
    }
  }
  
  // Register mDNS with hostname "esp32" for both interfaces
  if (MDNS.begin("esp32")) {
    Serial.println("mDNS responder started: http://esp32.local/");
  }
  else {
    Serial.println("Error setting up mDNS responder");
  }
  
  server.on("/", handleRoot);
  server.begin();

  webSocket.begin();
  webSocket.onEvent(webSocketEvent);

  pinMode(12, OUTPUT);
  pinMode(14, OUTPUT);
  pinMode(27, OUTPUT);
  pinMode(13, OUTPUT);
  Serial.println("Done starting up");
}

void loop(){
  // Check if a button on GPIO 0 is pressed (active LOW)
  if(digitalRead(0) == LOW) {
    forwardMode = true;
  }
  
  if(forwardMode) {
    // Continually move forward at full speed
    moveForward(255);
    delay(100); // small delay to prevent spamming
  }
  else {
    // Normal operations remain active when not in infinite forward mode
    server.handleClient();
    webSocket.loop();
  }
}
