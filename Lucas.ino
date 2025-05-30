#include <WiFi.h>
#include <WebServer.h>
#include <ESPmDNS.h>
#include <WebSocketsServer.h>

WebServer server(80);
WebSocketsServer webSocket = WebSocketsServer(81);

void stopMotors(){
  analogWrite(13, 0);
  analogWrite(12, 0);
  analogWrite(14, 0);
  analogWrite(27, 0);
}

void moveForward(int fspeed){
  analogWrite(13, 0);
  analogWrite(12, fspeed);
  analogWrite(14, 0);
  analogWrite(27, fspeed);
}

void moveBackward(int bspeed){
  analogWrite(13, bspeed);
  analogWrite(12, 0);
  analogWrite(14, bspeed);
  analogWrite(27, 0);
}

void turnLeft(int lspeed){
  analogWrite(13, 0);
  analogWrite(12, lspeed);
  analogWrite(14, lspeed);
  analogWrite(27, 0);
}

void turnRight(int rspeed){
  analogWrite(13, rspeed);
  analogWrite(12, 0);
  analogWrite(14, 0);
  analogWrite(27, 255);
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
        <sodipodi:namedview
           id="namedview1"
           pagecolor="#ffffff"
           bordercolor="#000000"
           borderopacity="0.25"
           inkscape:showpageshadow="2"
           inkscape:pageopacity="0.0"
           inkscape:pagecheckerboard="0"
           inkscape:deskcolor="#d1d1d1"
           inkscape:document-units="mm"
           inkscape:zoom="1.4378367"
           inkscape:cx="387.73527"
           inkscape:cy="217.3404"
           inkscape:window-width="1920"
           inkscape:window-height="1009"
           inkscape:window-x="-8"
           inkscape:window-y="-8"
           inkscape:window-maximized="1"
           inkscape:current-layer="layer1"
           showguides="true" />
        <defs
           id="defs1">
          <meshgradient
             inkscape:collect="always"
             id="meshgradient14"
             gradientUnits="userSpaceOnUse"
             x="26.130112"
             y="73.973984">
            <meshrow
               id="meshrow14">
              <meshpatch
                 id="meshpatch14">
                <stop
                   path="c 45.6357,0  91.2714,0  136.907,0"
                   style="stop-color:#d7d7d7;stop-opacity:1"
                   id="stop15" />
                <stop
                   path="c 0,27.6022  0,55.2045  0,82.8067"
                   style="stop-color:#666666;stop-opacity:1"
                   id="stop16" />
                <stop
                   path="c -45.6357,0  -91.2714,0  -136.907,0"
                   style="stop-color:#e7e7e7;stop-opacity:1"
                   id="stop17" />
                <stop
                   path="c 0,-27.6022  0,-55.2045  0,-82.8067"
                   style="stop-color:#666666;stop-opacity:1"
                   id="stop18" />
              </meshpatch>
            </meshrow>
          </meshgradient>
          <linearGradient id="rainbow-gradient" x1="0%" y1="0%" x2="100%" y2="0%">
              <stop offset="0%"   stop-color="#FF0000"/>
              <stop offset="16%"  stop-color="#FF7F00"/>
              <stop offset="33%"  stop-color="#FFFF00"/>
              <stop offset="50%"  stop-color="#00FF00"/>
              <stop offset="66%"  stop-color="#0000FF"/>
              <stop offset="83%"  stop-color="#4B0082"/>
              <stop offset="100%" stop-color="#8B00FF"/>
          </linearGradient>
        </defs>
        <g inkscape:label="Layer 1"
           inkscape:groupmode="layer"
           id="layer1"
           transform="translate(-26.130112,-73.973984)">
          <rect
             style="fill:url(#meshgradient14);fill-opacity:1;stroke-width:0.303295"
             id="rect1"
             width="136.90706"
             height="82.806686"
             x="26.130112"
             y="73.973984" />
          <rect style="fill:url(#rainbow-gradient);fill-opacity:1;stroke-width:0.272325;cursor:pointer;"
             id="rect36"
             width="125.11385"
             height="8.1891403"
             x="32.026714"
             y="82.04776" />
          <g id="g59" style="cursor: pointer;">
            <path
               style="fill:#e7e7e7;fill-opacity:1;stroke-width:0.3"
               d="m 60.38299,124.80323 -14.846639,14.84664 a 21.161711,21.161711 0 0 0 14.826485,6.13658 21.161711,21.161711 0 0 0 14.846122,-6.15725 z"
               id="path25" />
            <path
               style="fill:#e7e7e7;fill-opacity:1;stroke-width:0.3"
               d="m 60.362836,103.46294 a 21.161711,21.161711 0 0 0 -14.846639,6.15724 l 14.866793,14.8668 14.883329,-14.88333 a 21.161711,21.161711 0 0 0 -14.903483,-6.14071 z"
               id="path18" />
            <circle
               style="fill:#333333;fill-opacity:1;stroke-width:0.21238"
               id="circle18"
               cx="60.362579"
               cy="124.62481"
               r="14.981098" />
            <path
               id="circle19"
               style="fill:#333333;fill-opacity:1;stroke-width:0.295388"
               d="m 60.362574,103.78836 a 20.836416,20.836416 0 0 0 -20.83645,20.83645 20.836416,20.836416 0 0 0 20.83645,20.83645 20.836416,20.836416 0 0 0 20.83646,-20.83645 20.836416,20.836416 0 0 0 -20.83646,-20.83645 z m 0,0.22789 a 20.608709,20.608709 0 0 1 20.60908,20.60856 20.608709,20.608709 0 0 1 -20.60908,20.60856 20.608709,20.608709 0 0 1 -20.608557,-20.60856 20.608709,20.608709 0 0 1 20.608557,-20.60856 z" />
            <path
               id="path20"
               style="fill:#333333;fill-opacity:1;stroke-width:0.31691"
               d="m 60.362573,102.27024 a 22.354536,22.354536 0 0 0 -22.354572,22.35457 22.354536,22.354536 0 0 0 22.354572,22.35457 22.354536,22.354536 0 0 0 22.354583,-22.35457 22.354536,22.354536 0 0 0 -22.354583,-22.35457 z m 0,0.24449 A 22.110238,22.110238 0 0 1 82.47321,124.62481 22.110238,22.110238 0 0 1 60.362573,146.73489 22.110238,22.110238 0 0 1 38.252498,124.62481 22.110238,22.110238 0 0 1 60.362573,102.51473 Z" />
            <rect
               style="fill:#ffffff;fill-opacity:1;stroke-width:0.171085"
               id="rect20"
               width="0.15741065"
               height="29.960497"
               x="130.21555"
               y="31.906748"
               transform="rotate(44.365062)" />
            <rect
               style="fill:#ffffff;fill-opacity:1;stroke-width:0.171085"
               id="rect21"
               width="0.15741065"
               height="29.960497"
               x="43.704025"
               y="-146.35017"
               transform="rotate(135.72446)" />
            <circle
               style="fill:#ffffff;fill-opacity:1;stroke-width:0.3"
               id="path26"
               cx="60.362579"
               cy="124.62481"
               r="4.5083642" />
            <path
               id="path27"
               style="fill:#ffffff;fill-opacity:1;stroke-width:0.0739703"
               d="m 60.362577,119.40698 a 5.2178191,5.2178191 0 0 0 -5.217827,5.21783 5.2178191,5.2178191 0 0 0 5.217827,5.21782 5.2178191,5.2178191 0 0 0 5.21783,-5.21782 5.2178191,5.2178191 0 0 0 -5.21783,-5.21783 z m 0,0.057 a 5.160797,5.160797 0 0 1 5.16089,5.16077 5.160797,5.160797 0 0 1 -5.16089,5.16076 5.160797,5.160797 0 0 1 -5.160758,-5.16076 5.160797,5.160797 0 0 1 5.160758,-5.16077 z" />
          </g>
          <g id="g37"
             transform="translate(2.886857,3.5430299)">
            <text
               xml:space="preserve"
               style="font-size:3.52778px;line-height:1.3;font-family:Ubuntu;-inkscape-font-specification:Ubuntu;text-align:start;writing-mode:lr-tb;direction:ltr;text-anchor:start;fill:#f9f9f9;fill-opacity:1;stroke-width:0.3"
               x="48.027882"
               y="96.42379"
               id="text36">
              <tspan sodipodi:role="line" id="tspan36" style="font-size:3.52778px;stroke-width:0.3" x="48.027882" y="96.42379">Value:</tspan>
            </text>
            <text
               xml:space="preserve"
               style="font-size:3.52778px;line-height:1.3;font-family:Ubuntu;-inkscape-font-specification:Ubuntu;text-align:start;writing-mode:lr-tb;direction:ltr;text-anchor:start;fill:#000000;fill-opacity:1;stroke-width:0.3"
               x="59.669559"
               y="96.42379"
               id="text37">
              <tspan sodipodi:role="line" id="tspan37" style="font-size:3.52778px;fill:#000000;stroke-width:0.3" x="59.669559" y="96.42379">100</tspan>
            </text>
          </g>
          <g id="g60" style="cursor: pointer;">
            <path
               style="fill:#e7e7e7;fill-opacity:1;stroke-width:0.3"
               d="m 128.62629,124.6452 -14.84664,-14.84664 a 21.161711,21.161711 0 0 0 -6.13658,14.82648 21.161711,21.161711 0 0 0 6.15725,14.84612 z"
               id="path28" />
            <path
               style="fill:#e7e7e7;fill-opacity:1;stroke-width:0.3"
               d="m 149.96658,124.62504 a 21.161711,21.161711 0 0 0 -6.15724,-14.84664 l -14.8668,14.8668 14.88333,14.88332 a 21.161711,21.161711 0 0 0 6.14071,-14.90348 z"
               id="path29" />
            <circle
               style="fill:#333333;fill-opacity:1;stroke-width:0.21238"
               id="circle29"
               cx="124.62479"
               cy="-128.80472"
               r="14.981098"
               transform="rotate(90)" />
            <path
               id="path30"
               style="fill:#333333;fill-opacity:1;stroke-width:0.295388"
               d="m 149.64116,124.62478 a 20.836416,20.836416 0 0 0 -20.83645,-20.83645 20.836416,20.836416 0 0 0 -20.83645,20.83645 20.836416,20.836416 0 0 0 20.83645,20.83646 20.836416,20.836416 0 0 0 20.83645,-20.83646 z m -0.22789,0 a 20.608709,20.608709 0 0 1 -20.60856,20.60908 20.608709,20.608709 0 0 1 -20.60856,-20.60908 20.608709,20.608709 0 0 1 20.60856,-20.60856 20.608709,20.608709 0 0 1 20.60856,20.60856 z"
               />
            <path
               id="path31"
               style="fill:#333333;fill-opacity:1;stroke-width:0.31691"
               d="m 151.15928,124.62478 a 22.354536,22.354536 0 0 0 -22.35457,-22.35457 22.354536,22.354536 0 0 0 -22.35457,22.35457 22.354536,22.354536 0 0 0 22.35457,22.35458 22.354536,22.354536 0 0 0 22.35457,-22.35458 z m -0.24448,0 a 22.110238,22.110238 0 0 1 -22.11009,22.11064 22.110238,22.110238 0 0 1 -22.11008,-22.11064 22.110238,22.110238 0 0 1 22.11008,-22.11008 22.110238,22.110238 0 0 1 22.11009,22.11008 z"
               />
            <rect
               style="fill:#ffffff;fill-opacity:1;stroke-width:0.171085"
               id="rect31"
               width="0.15741065"
               height="29.960497"
               x="-1.0483481"
               y="-194.20372"
               transform="rotate(134.36506)" />
            <rect
               style="fill:#ffffff;fill-opacity:1;stroke-width:0.171085"
               id="rect32"
               width="0.15741065"
               height="29.960497"
               x="-179.22873"
               y="-9.7590256"
               transform="rotate(-134.27554)" />
            <circle
               style="fill:#ffffff;fill-opacity:1;stroke-width:0.3"
               id="circle32"
               cx="124.62479"
               cy="-128.80472"
               r="4.5083642"
               transform="rotate(90)" />
            <path
               id="path32"
               style="fill:#ffffff;fill-opacity:1;stroke-width:0.0739703"
               d="m 134.02254,124.62478 a 5.2178191,5.2178191 0 0 0 -5.21783,-5.21782 5.2178191,5.2178191 0 0 0 -5.21782,5.21782 5.2178191,5.2178191 0 0 0 5.21782,5.21783 5.2178191,5.2178191 0 0 0 5.21783,-5.21783 z m -0.057,0 a 5.160797,5.160797 0 0 1 -5.16077,5.16089 5.160797,5.160797 0 0 1 -5.16076,-5.16089 5.160797,5.160797 0 0 1 5.16076,-5.16076 5.160797,5.160797 0 0 1 5.16077,5.16076 z"
               />
          </g>
          <g id="g39"
             transform="translate(71.141387,3.5430299)">
            <text
               xml:space="preserve"
               style="font-size:3.52778px;line-height:1.3;font-family:Ubuntu;-inkscape-font-specification:Ubuntu;text-align:start;writing-mode:lr-tb;direction:ltr;text-anchor:start;fill:#f9f9f9;fill-opacity:1;stroke-width:0.3"
               x="48.027882"
               y="96.42379"
               id="text38">
              <tspan sodipodi:role="line" id="tspan38" style="font-size:3.52778px;stroke-width:0.3" x="48.027882" y="96.42379">Value:</tspan>
            </text>
            <text
               xml:space="preserve"
               style="font-size:3.52778px;line-height:1.3;font-family:Ubuntu;-inkscape-font-specification:Ubuntu;text-align:start;writing-mode:lr-tb;direction:ltr;text-anchor:start;fill:#000000;fill-opacity:1;stroke-width:0.3"
               x="59.669559"
               y="96.42379"
               id="text39">
              <tspan sodipodi:role="line" id="tspan39" style="font-size:3.52778px;fill:#000000;stroke-width:0.3" x="59.669559" y="96.42379">100</tspan>
            </text>
          </g>
          <g id="g41"
             transform="translate(40.323936,-17.02088)">
            <g id="g53">
              <text
                 xml:space="preserve"
                 style="font-size:3.52778px;line-height:1.3;font-family:Ubuntu;-inkscape-font-specification:Ubuntu;text-align:start;writing-mode:lr-tb;direction:ltr;text-anchor:start;fill:#f9f9f9;fill-opacity:1;stroke-width:0.3"
                 x="42.207043"
                 y="96.42379"
                 id="text40">
                <tspan sodipodi:role="line" id="tspan40" style="font-size:3.52778px;stroke-width:0.3" x="42.207043" y="96.42379">Value Hue:</tspan>
              </text>
              <text
                 xml:space="preserve"
                 style="font-size:3.52778px;line-height:1.3;font-family:Ubuntu;-inkscape-font-specification:Ubuntu;text-align:start;writing-mode:lr-tb;direction:ltr;text-anchor:start;fill:#000000;fill-opacity:1;stroke-width:0.3"
                 x="59.669559"
                 y="96.42379"
                 id="text41">
                <tspan sodipodi:role="line" id="tspan41" style="font-size:3.52778px;fill:#000000;stroke-width:0.3" x="59.669559" y="96.42379"> 100</tspan>
              </text>
            </g>
          </g>
          <path
             id="rect41"
             style="fill:#333333;fill-opacity:1;stroke-width:0.283737"
             d="M 36.894906,82.047615 V 90.9528 h -0.134359 v 1.989331 h 0.544669 V 90.9528 h -0.134358 v -8.905185 z" />
          <g id="g57"  style="cursor:pointer;">
            <circle
               style="fill:#333333;fill-opacity:1;stroke-width:0.21"
               id="path42"
               cx="86.958687"
               cy="146.61284"
               r="5.9203753" />
            <path
               id="path47"
               style="fill:#ffffff;fill-opacity:1;stroke-width:0.0739703"
               d="m 86.95869,141.39506 a 5.2178191,5.2178191 0 0 0 -5.217827,5.21783 5.2178191,5.2178191 0 0 0 5.217827,5.21782 5.2178191,5.2178191 0 0 0 5.21783,-5.21782 5.2178191,5.2178191 0 0 0 -5.21783,-5.21783 z m 0,0.057 a 5.160797,5.160797 0 0 1 5.16089,5.16077 5.160797,5.160797 0 0 1 -5.16089,5.16076 5.160797,5.160797 0 0 1 -5.160758,-5.16076 5.160797,5.160797 0 0 1 5.160758,-5.16077 z" />
            <g id="g48"
               transform="translate(29.785713,56.955833)">
              <text
                 xml:space="preserve"
                 style="font-size:3.52778px;line-height:1.3;font-family:Ubuntu;-inkscape-font-specification:Ubuntu;text-align:start;writing-mode:lr-tb;direction:ltr;text-anchor:start;fill:#f9f9f9;fill-opacity:1;stroke-width:0.3"
                 x="53.502323"
                 y="90.581314"
                 id="text47">
                <tspan sodipodi:role="line" id="tspan47" style="font-size:3.52778px;stroke-width:0.3" x="53.502323" y="90.581314">Stop</tspan>
              </text>
            </g>
          </g>
          <g id="g58" style="cursor:pointer;">
            <circle
               style="fill:#333333;fill-opacity:1;stroke-width:0.21"
               id="circle48"
               cx="102.2086"
               cy="146.61284"
               r="5.9203753" />
            <path
               id="path48"
               style="fill:#ffffff;fill-opacity:1;stroke-width:0.0739703"
               d="m 102.20859,141.39506 a 5.2178191,5.2178191 0 0 0 -5.217819,5.21783 5.2178191,5.2178191 0 0 0 5.217819,5.21782 5.2178191,5.2178191 0 0 0 5.21783,-5.21782 5.2178191,5.2178191 0 0 0 -5.21783,-5.21783 z m 0,0.057 a 5.160797,5.160797 0 0 1 5.16089,5.16077 5.160797,5.160797 0 0 1 -5.16089,5.16076 5.160797,5.160797 0 0 1 -5.16075,-5.16076 5.160797,5.160797 0 0 1 5.16075,-5.16077 z" />
            <g id="g49"
               transform="translate(44.72165,56.955833)">
              <text
                 xml:space="preserve"
                 style="font-size:3.52778px;line-height:1.3;font-family:Ubuntu;-inkscape-font-specification:Ubuntu;text-align:start;writing-mode:lr-tb;direction:ltr;text-anchor:start;fill:#f9f9f9;fill-opacity:1;stroke-width:0.3"
                 x="53.502323"
                 y="90.581314"
                 id="text48">
                <tspan sodipodi:role="line" id="tspan48" style="font-size:3.52778px;stroke-width:0.3" x="53.502323" y="90.581314">Start</tspan>
              </text>
            </g>
          </g>
          <path
             id="rect52"
             style="fill:#333333;fill-opacity:1;stroke-width:0.289062"
             d="m 31.53555,81.565092 v 9.15448 h 126.09618 v -9.15448 z m 0.275953,0.29869 H 157.35577 v 8.5571 H 31.811503 Z" />
          <path
             id="path56"
             style="fill:#333333;fill-opacity:1;stroke-width:0.289062"
             d="M 26.553015,74.598907 V 156.15575 H 162.61427 V 74.598907 Z m 0.275953,0.29869 H 162.33831 V 155.85706 H 26.828968 Z"
             sodipodi:nodetypes="cccccccccc" />
          <text
             xml:space="preserve"
             style="font-style:italic;font-size:2.11667px;line-height:0.6;font-family:Ubuntu;-inkscape-font-specification:'Ubuntu Italic';text-align:start;writing-mode:lr-tb;direction:ltr;text-anchor:start;fill:#000000;fill-opacity:1;stroke-width:0.3"
             x="134.54373"
             y="79.408203"
             id="text1">
            <tspan sodipodi:role="line" id="tspan1" style="font-style:normal;font-variant:normal;font-weight:normal;font-stretch:normal;font-size:2.11667px;font-family:Ubuntu;-inkscape-font-specification:Ubuntu;fill:#000000;fill-opacity:1;stroke-width:0.3" x="134.54373" y="79.408203">Status: </tspan>
          </text>
          <text
             xml:space="preserve"
             style="font-style:italic;font-size:2.11667px;line-height:0.6;font-family:Ubuntu;-inkscape-font-specification:'Ubuntu Italic';text-align:start;writing-mode:lr-tb;direction:ltr;text-anchor:start;fill:#000000;fill-opacity:1;stroke-width:0.3"
             x="141.95197"
             y="79.408203"
             id="text2">
            <tspan sodipodi:role="line" id="tspan2" style="font-style:normal;font-variant:normal;font-weight:normal;font-stretch:normal;font-size:2.11667px;font-family:Ubuntu;-inkscape-font-specification:Ubuntu;fill:#000000;fill-opacity:1;stroke-width:0.3" x="141.95197" y="79.408203">Connecting... </tspan>
          </text>
        </g>
        <script
           id="mesh_polyfill"
           type="text/javascript">
          /* [The complete mesh polyfill JS code as in your file] */
          !function(){const t="http://www.w3.org/2000/svg",e="http://www.w3.org/1999/xlink",s="http://www.w3.org/1999/xhtml",r=2;if(document.createElementNS(t,"meshgradient").x)return;const n=(t,e,s,r)=>{let n=new x(.5*(e.x+s.x),.5*(e.y+s.y)),o=new x(.5*(t.x+e.x),.5*(t.y+e.y)),i=new x(.5*(s.x+r.x),.5*(s.y+r.y)),a=new x(.5*(n.x+o.x),.5*(n.y+o.y)),h=new x(.5*(n.x+i.x),.5*(n.y+i.y)),l=new x(.5*(a.x+h.x),.5*(a.y+h.y));return[[t,o,a,l],[l,h,i,r]]},o=t=>{let e=t[0].distSquared(t[1]),s=t[2].distSquared(t[3]),r=.25*t[0].distSquared(t[2]),n=.25*t[1].distSquared(t[3]),o=e>s?e:s,i=r>n?r:n;return 18*(o>i?o:i)},i=(t,e)=>Math.sqrt(t.distSquared(e)),a=(t,e)=>t.scale(2/3).add(e.scale(1/3)),h=t=>{let e,s,r,n,o,i,a,h=new g;return t.match(/(w+(s*[^)]+))+/g).forEach(t=>{let l=t.match(/[w.-]+/g),d=l.shift();switch(d){case"translate":2===l.length?e=new g(1,0,0,1,l[0],l[1]):(console.error("mesh.js: translate does not have 2 arguments!"),e=new g(1,0,0,1,0,0)),h=h.append(e);break;case"scale":1===l.length?s=new g(l[0],0,0,l[0],0,0):2===l.length?s=new g(l[0],0,0,l[1],0,0):(console.error("mesh.js: scale does not have 1 or 2 arguments!"),s=new g(1,0,0,1,0,0)),h=h.append(s);break;case"rotate":if(3===l.length&&(e=new g(1,0,0,1,l[1],l[2]),h=h.append(e)),l[0]){r=l[0]*Math.PI/180;let t=Math.cos(r),e=Math.sin(r);Math.abs(t)<1e-16&&(t=0),Math.abs(e)<1e-16&&(e=0),a=new g(t,e,-e,t,0,0),h=h.append(a)}else console.error("math.js: No argument to rotate transform!");3===l.length&&(e=new g(1,0,0,1,-l[1],-l[2]),h=h.append(e));break;case"skewX":l[0]?(r=l[0]*Math.PI/180,n=Math.tan(r),o=new g(1,0,n,1,0,0),h=h.append(o)):console.error("math.js: No argument to skewX transform!");break;case"skewY":l[0]?(r=l[0]*Math.PI/180,n=Math.tan(r),i=new g(1,n,0,1,0,0),h=h.append(i)):console.error("math.js: No argument to skewY transform!");break;case"matrix":6===l.length?h=h.append(new g(...l)):console.error("math.js: Incorrect number of arguments for matrix!");break;default:console.error("mesh.js: Unhandled transform type: "+d)}}),h},l=t=>{let e=[],s=t.split(/[ ,]+/);for(let t=0,r=s.length-1;t<r;t+=2)e.push(new x(parseFloat(s[t]),parseFloat(s[t+1])));return e},d=(t,e)=>{for(let s in e)t.setAttribute(s,e[s])},c=(t,e,s,r,n)=>{let o,i,a=[0,0,0,0];for(let h=0;h<3;++h)e[h]<t[h]&&e[h]<s[h]||t[h]<e[h]&&s[h]<e[h]?a[h]=0:(a[h]=.5*((e[h]-t[h])/r+(s[h]-e[h])/n),o=Math.abs(3*(e[h]-t[h])/r),i=Math.abs(3*(s[h]-e[h])/n),a[h]>o?a[h]=o:a[h]>i&&(a[h]=i));return a},u=[[1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0],[0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0],[-3,3,0,0,-2,-1,0,0,0,0,0,0,0,0,0,0],[2,-2,0,0,1,1,0,0,0,0,0,0,0,0,0,0],[0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0],[0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0],[0,0,0,0,0,0,0,0,-3,3,0,0,-2,-1,0,0],[0,0,0,0,0,0,0,0,2,-2,0,0,1,1,0,0],[-3,0,3,0,0,0,0,0,-2,0,-1,0,0,0,0,0],[0,0,0,0,-3,0,3,0,0,0,0,0,-2,0,-1,0],[9,-9,-9,9,6,3,-6,-3,6,-6,3,-3,4,2,2,1],[-6,6,6,-6,-3,-3,3,3,-4,4,-2,2,-2,-2,-1,-1],[2,0,-2,0,0,0,0,0,1,0,1,0,0,0,0,0],[0,0,0,0,2,0,-2,0,0,0,0,0,1,0,1,0],[-6,6,6,-6,-4,-2,4,2,-3,3,-3,3,-2,-1,-2,-1],[4,-4,-4,4,2,2,-2,-2,2,-2,2,-2,1,1,1,1]],f=t=>{let e=[];for(let s=0;s<16;++s){e[s]=0;for(let r=0;r<16;++r)e[s]+=u[s][r]*t[r]}return e},p=(t,e,s)=>{const r=e*e,n=s*s,o=e*e*e,i=s*s*s;return t[0]+t[1]*e+t[2]*r+t[3]*o+t[4]*s+t[5]*s*e+t[6]*s*r+t[7]*s*o+t[8]*n+t[9]*n*e+t[10]*n*r+t[11]*n*o+t[12]*i+t[13]*i*e+t[14]*i*r+t[15]*i*o},y=t=>{let e=[],s=[],r=[];for(let s=0;s<4;++s)e[s]=[],e[s][0]=n(t[0][s],t[1][s],t[2][s],t[3][s]),e[s][1]=[],e[s][1].push(...n(...e[s][0][0])),e[s][1].push(...n(...e[s][0][1])),e[s][2]=[],e[s][2].push(...n(...e[s][1][0])),e[s][2].push(...n(...e[s][1][1])),e[s][2].push(...n(...e[s][1][2])),e[s][2].push(...n(...e[s][1][3]));for(let t=0;t<8;++t){s[t]=[];for(let r=0;r<4;++r)s[t][r]=[],s[t][r][0]=n(e[0][2][t][r],e[1][2][t][r],e[2][2][t][r],e[3][2][t][r]),s[t][r][1]=[],s[t][r][1].push(...n(...s[t][r][0][0])),s[t][r][1].push(...n(...s[t][r][0][1])),s[t][r][2]=[],s[t][r][2].push(...n(...s[t][r][1][0])),s[t][r][2].push(...n(...s[t][r][1][1])),s[t][r][2].push(...n(...s[t][r][1][2])),s[t][r][2].push(...n(...s[t][r][1][3]))}for(let t=0;t<8;++t){r[t]=[];for(let e=0;e<8;++e)r[t][e]=[],r[t][e][0]=s[t][0][2][e],r[t][e][1]=s[t][1][2][e],r[t][e][2]=s[t][2][2][e],r[t][e][3]=s[t][3][2][e]}return r};class x{constructor(t,e){this.x=t||0,this.y=e||0}toString(){return`(x=undefined, y=undefined)`}clone(){return new x(this.x,this.y)}add(t){return new x(this.x+t.x,this.y+t.y)}scale(t){return void 0===t.x?new x(this.x*t,this.y*t):new x(this.x*t.x,this.y*t.y)}distSquared(t){let e=this.x-t.x,s=this.y-t.y;return e*e+s*s}transform(t){let e=this.x*t.a+this.y*t.c+t.e,s=this.x*t.b+this.y*t.d+t.f;return new x(e,s)}}class g{constructor(t,e,s,r,n,o){void 0===t?(this.a=1,this.b=0,this.c=0,this.d=1,this.e=0,this.f=0):(this.a=t,this.b=e,this.c=s,this.d=r,this.e=n,this.f=o)}toString(){return`affine: undefined undefined undefined
       undefined undefined undefined`}append(t){t instanceof g||console.error("mesh.js: argument to Affine.append is not affine!");let e=this.a*t.a+this.c*t.b,s=this.b*t.a+this.d*t.b,r=this.a*t.c+this.c*t.d,n=this.b*t.c+this.d*t.d,o=this.a*t.e+this.c*t.f+this.e,i=this.b*t.e+this.d*t.f+this.f;return new g(e,s,r,n,o,i)}}class w{constructor(t,e){this.nodes=t,this.colors=e}paintCurve(t,e){if(o(this.nodes)>r){const s=n(...this.nodes);let r=[[],[]],o=[[],[]];for(let t=0;t<4;++t)r[0][t]=this.colors[0][t],r[1][t]=(this.colors[0][t]+this.colors[1][t])/2,o[0][t]=r[1][t],o[1][t]=this.colors[1][t];let i=new w(s[0],r),a=new w(s[1],o);i.paintCurve(t,e),a.paintCurve(t,e)}else{let s=Math.round(this.nodes[0].x);if(s>=0&&s<e){let r=4*(~~this.nodes[0].y*e+s);t[r]=Math.round(this.colors[0][0]),t[r+1]=Math.round(this.colors[0][1]),t[r+2]=Math.round(this.colors[0][2]),t[r+3]=Math.round(this.colors[0][3])}}}}class m{constructor(t,e){this.nodes=t,this.colors=e}split(){let t=[[],[],[],[]],e=[[],[],[],[]],s=[[[],[]],[[],[]]],r=[[[],[]],[[],[]]];for(let s=0;s<4;++s){const r=n(this.nodes[0][s],this.nodes[1][s],this.nodes[2][s],this.nodes[3][s]);t[0][s]=r[0][0],t[1][s]=r[0][1],t[2][s]=r[0][2],t[3][s]=r[0][3],e[0][s]=r[1][0],e[1][s]=r[1][1],e[2][s]=r[1][2],e[3][s]=r[1][3]}for(let t=0;t<4;++t)s[0][0][t]=this.colors[0][0][t],s[0][1][t]=this.colors[0][1][t],s[1][0][t]=(this.colors[0][0][t]+this.colors[1][0][t])/2,s[1][1][t]=(this.colors[0][1][t]+this.colors[1][1][t])/2,r[0][0][t]=s[1][0][t],r[0][1][t]=s[1][1][t],r[1][0][t]=this.colors[1][0][t],r[1][1][t]=this.colors[1][1][t];return[new m(t,s),new m(e,r)]}paint(t,e){let s,n=!1;for(let t=0;t<4;++t)if((s=o([this.nodes[0][t],this.nodes[1][t],this.nodes[2][t],this.nodes[3][t]]))>r){n=!0;break}if(n){let s=this.split();s[0].paint(t,e),s[1].paint(t,e)}else{new w([...this.nodes[0]],[...this.colors[0]]).paintCurve(t,e)}}}class b{constructor(t){this.readMesh(t),this.type=t.getAttribute("type")||"bilinear"}readMesh(t){let e=[[]],s=[[]],r=Number(t.getAttribute("x")),n=Number(t.getAttribute("y"));e[0][0]=new x(r,n);let o=t.children;for(let t=0,r=o.length;t<r;++t){e[3*t+1]=[],e[3*t+2]=[],e[3*t+3]=[],s[t+1]=[];let r=o[t].children;for(let n=0,o=r.length;n<o;++n){let o=r[n].children;for(let r=0,i=o.length;r<i;++r){let i=r;0!==t&&++i;let h,d=o[r].getAttribute("path"),c="l";null!=d&&(c=(h=d.match(/s*([lLcC])s*(.*)/))[1]);let u=l(h[2]);switch(c){case"l":0===i?(e[3*t][3*n+3]=u[0].add(e[3*t][3*n]),e[3*t][3*n+1]=a(e[3*t][3*n],e[3*t][3*n+3]),e[3*t][3*n+2]=a(e[3*t][3*n+3],e[3*t][3*n])):1===i?(e[3*t+3][3*n+3]=u[0].add(e[3*t][3*n+3]),e[3*t+1][3*n+3]=a(e[3*t][3*n+3],e[3*t+3][3*n+3]),e[3*t+2][3*n+3]=a(e[3*t+3][3*n+3],e[3*t][3*n+3])):2===i?(0===n&&(e[3*t+3][3*n+0]=u[0].add(e[3*t+3][3*n+3])),e[3*t+3][3*n+1]=a(e[3*t+3][3*n],e[3*t+3][3*n+3]),e[3*t+3][3*n+2]=a(e[3*t+3][3*n+3],e[3*t+3][3*n])):(e[3*t+1][3*n]=a(e[3*t][3*n],e[3*t+3][3*n]),e[3*t+2][3*n]=a(e[3*t+3][3*n],e[3*t][3*n]));break;case"L":0===i?(e[3*t][3*n+3]=u[0],e[3*t][3*n+1]=a(e[3*t][3*n],e[3*t][3*n+3]),e[3*t][3*n+2]=a(e[3*t][3*n+3],e[3*t][3*n])):1===i?(e[3*t+3][3*n+3]=u[0],e[3*t+1][3*n+3]=a(e[3*t][3*n+3],e[3*t+3][3*n+3]),e[3*t+2][3*n+3]=a(e[3*t+3][3*n+3],e[3*t][3*n+3])):2===i?(0===n&&(e[3*t+3][3*n+0]=u[0]),e[3*t+3][3*n+1]=a(e[3*t+3][3*n],e[3*t+3][3*n+3]),e[3*t+3][3*n+2]=a(e[3*t+3][3*n+3],e[3*t+3][3*n])):(e[3*t+1][3*n]=a(e[3*t][3*n],e[3*t+3][3*n]),e[3*t+2][3*n]=a(e[3*t+3][3*n],e[3*t][3*n]));break;case"c":0===i?(e[3*t][3*n+1]=u[0].add(e[3*t][3*n]),e[3*t][3*n+2]=u[1].add(e[3*t][3*n]),e[3*t][3*n+3]=u[2].add(e[3*t][3*n])):1===i?(e[3*t+1][3*n+3]=u[0].add(e[3*t][3*n+3]),e[3*t+2][3*n+3]=u[1].add(e[3*t][3*n+3]),e[3*t+3][3*n+3]=u[2].add(e[3*t][3*n+3])):2===i?(e[3*t+3][3*n+2]=u[0].add(e[3*t+3][3*n+3]),e[3*t+3][3*n+1]=u[1].add(e[3*t+3][3*n+3]),0===n&&(e[3*t+3][3*n+0]=u[2].add(e[3*t+3][3*n+3]))):(e[3*t+2][3*n]=u[0].add(e[3*t+3][3*n]),e[3*t+1][3*n]=u[1].add(e[3*t+3][3*n]));break;case"C":0===i?(e[3*t][3*n+1]=u[0],e[3*t][3*n+2]=u[1],e[3*t][3*n+3]=u[2]):1===i?(e[3*t+1][3*n+3]=u[0],e[3*t+2][3*n+3]=u[1],e[3*t+3][3*n+3]=u[2]):2===i?(e[3*t+3][3*n+2]=u[0],e[3*t+3][3*n+1]=u[1],0===n&&(e[3*t+3][3*n+0]=u[2])):(e[3*t+2][3*n]=u[0],e[3*t+1][3*n]=u[1]);break;default:console.error("mesh.js: "+c+" invalid path type.");}if(0===t&&0===n||r>0){let e=window.getComputedStyle(o[r]).stopColor.match(/^rgbs*(s*(d+)s*,s*(d+)s*,s*(d+)s*)$/i),a=window.getComputedStyle(o[r]).stopOpacity,h=255;a&&(h=Math.floor(255*a)),e&&(0===i?(s[t][n]=[],s[t][n][0]=Math.floor(e[1]),s[t][n][1]=Math.floor(e[2]),s[t][n][2]=Math.floor(e[3]),s[t][n][3]=h):1===i?(s[t][n+1]=[],s[t][n+1][0]=Math.floor(e[1]),s[t][n+1][1]=Math.floor(e[2]),s[t][n+1][2]=Math.floor(e[3]),s[t][n+1][3]=h):2===i?(s[t+1][n+1]=[],s[t+1][n+1][0]=Math.floor(e[1]),s[t+1][n+1][1]=Math.floor(e[2]),s[t+1][n+1][2]=Math.floor(e[3]),s[t+1][n+1][3]=h):3===i&&(s[t+1][n]=[],s[t+1][n][0]=Math.floor(e[1]),s[t+1][n][1]=Math.floor(e[2]),s[t+1][n][2]=Math.floor(e[3]),s[t+1][n][3]=h))}}e[3*t+1][3*n+1]=new x,e[3*t+1][3*n+2]=new x,e[3*t+2][3*n+1]=new x,e[3*t+2][3*n+2]=new x,e[3*t+1][3*n+1].x=(-4*e[3*t][3*n].x+6*(e[3*t][3*n+1].x+e[3*t+1][3*n].x)+-2*(e[3*t][3*n+3].x+e[3*t+3][3*n].x)+3*(e[3*t+3][3*n+1].x+e[3*t+1][3*n+3].x)+-1*e[3*t+3][3*n+3].x)/9,e[3*t+1][3*n+2].x=(-4*e[3*t][3*n+3].x+6*(e[3*t][3*n+2].x+e[3*t+1][3*n+3].x)+-2*(e[3*t][3*n].x+e[3*t+3][3*n+3].x)+3*(e[3*t+3][3*n+2].x+e[3*t+1][3*n].x)+-1*e[3*t+3][3*n].x)/9,e[3*t+2][3*n+1].x=(-4*e[3*t+3][3*n].x+6*(e[3*t+3][3*n+1].x+e[3*t+2][3*n].x)+-2*(e[3*t+3][3*n+3].x+e[3*t][3*n].x)+3*(e[3*t][3*n+1].x+e[3*t+2][3*n+3].x)+-1*e[3*t][3*n+3].x)/9,e[3*t+2][3*n+2].x=(-4*e[3*t+3][3*n+3].x+6*(e[3*t+3][3*n+2].x+e[3*t+2][3*n+3].x)+-2*(e[3*t+3][3*n].x+e[3*t][3*n+3].x)+3*(e[3*t][3*n+2].x+e[3*t+2][3*n].x)+-1*e[3*t][3*n].x)/9,e[3*t+1][3*n+1].y=(-4*e[3*t][3*n].y+6*(e[3*t][3*n+1].y+e[3*t+1][3*n].y)+-2*(e[3*t][3*n+3].y+e[3*t+3][3*n].y)+3*(e[3*t+3][3*n+1].y+e[3*t+1][3*n+3].y)+-1*e[3*t+3][3*n+3].y)/9,e[3*t+1][3*n+2].y=(-4*e[3*t][3*n+3].y+6*(e[3*t][3*n+2].y+e[3*t+1][3*n+3].y)+-2*(e[3*t][3*n].y+e[3*t+3][3*n+3].y)+3*(e[3*t+3][3*n+2].y+e[3*t+1][3*n].y)+-1*e[3*t+3][3*n].y)/9,e[3*t+2][3*n+1].y=(-4*e[3*t+3][3*n].y+6*(e[3*t+3][3*n+1].y+e[3*t+2][3*n].y)+-2*(e[3*t+3][3*n+3].y+e[3*t][3*n].y)+3*(e[3*t][3*n+1].y+e[3*t+2][3*n+3].y)+-1*e[3*t][3*n+3].y)/9,e[3*t+2][3*n+2].y=(-4*e[3*t+3][3*n+3].y+6*(e[3*t+3][3*n+2].y+e[3*t+2][3*n+3].y)+-2*(e[3*t+3][3*n].y+e[3*t][3*n+3].y)+3*(e[3*t][3*n+2].y+e[3*t+2][3*n].y)+-1*e[3*t][3*n].y)/9}}this.nodes=e,this.colors=s}paintMesh(t,e){let s=(this.nodes.length-1)/3,r=(this.nodes[0].length-1)/3;if("bilinear"===this.type||s<2||r<2){let n;for(let o=0;o<s;++o)for(let s=0;s<r;++s){let r=[];for(let t=3*o,e=3*o+4;t<e;++t)r.push(this.nodes[t].slice(3*s,3*s+4));let i=[];i.push(this.colors[o].slice(s,s+2)),i.push(this.colors[o+1].slice(s,s+2)),(n=new m(r,i)).paint(t,e)}}else{let n,o,a,h,l,d,u;const x=s,g=r;s++,r++;let w=new Array(s);for(let t=0;t<s;++t){w[t]=new Array(r);for(let e=0;e<r;++e)w[t][e]=[],w[t][e][0]=this.nodes[3*t][3*e],w[t][e][1]=this.colors[t][e]}for(let t=0;t<s;++t)for(let e=0;e<r;++e)0!==t&&t!==x&&(n=i(w[t-1][e][0],w[t][e][0]),o=i(w[t+1][e][0],w[t][e][0]),w[t][e][2]=c(w[t-1][e][1],w[t][e][1],w[t+1][e][1],n,o)),0!==e&&e!==g&&(n=i(w[t][e-1][0],w[t][e][0]),o=i(w[t][e+1][0],w[t][e][0]),w[t][e][3]=c(w[t][e-1][1],w[t][e][1],w[t][e+1][1],n,o));for(let t=0;t<r;++t){w[0][t][2]=[],w[x][t][2]=[];for(let e=0;e<4;++e)n=i(w[1][t][0],w[0][t][0]),o=i(w[x][t][0],w[x-1][t][0]),w[0][t][2][e]=n>0?2*(w[1][t][1][e]-w[0][t][1][e])/n-w[1][t][2][e]:0,w[x][t][2][e]=o>0?2*(w[x][t][1][e]-w[x-1][t][1][e])/o-w[x-1][t][2][e]:0}for(let t=0;t<s;++t){w[t][0][3]=[],w[t][g][3]=[];for(let e=0;e<4;++e)n=i(w[t][1][0],w[t][0][0]),o=i(w[t][g][0],w[t][g-1][0]),w[t][0][3][e]=n>0?2*(w[t][1][1][e]-w[t][0][1][e])/n-w[t][1][3][e]:0,w[t][g][3][e]=o>0?2*(w[t][g][1][e]-w[t][g-1][1][e])/o-w[t][g-1][3][e]:0}for(let s=0;s<x;++s)for(let r=0;r<g;++r){let n=i(w[s][r][0],w[s+1][r][0]),o=i(w[s][r+1][0],w[s+1][r+1][0]),c=i(w[s][r][0],w[s][r+1][0]),x=i(w[s+1][r][0],w[s+1][r+1][0]),g=[[],[],[],[]];for(let t=0;t<4;++t){(d=[])[0]=w[s][r][1][t],d[1]=w[s+1][r][1][t],d[2]=w[s][r+1][1][t],d[3]=w[s+1][r+1][1][t],d[4]=w[s][r][2][t]*n,d[5]=w[s+1][r][2][t]*n,d[6]=w[s][r+1][2][t]*o,d[7]=w[s+1][r+1][2][t]*o,d[8]=w[s][r][3][t]*c,d[9]=w[s+1][r][3][t]*x,d[10]=w[s][r+1][3][t]*c,d[11]=w[s+1][r+1][3][t]*x,d[12]=0,d[13]=0,d[14]=0,d[15]=0,u=f(d);for(let e=0;e<9;++e){g[t][e]=[];for(let s=0;s<9;++s)g[t][e][s]=p(u,e/8,s/8),g[t][e][s]>255?g[t][e][s]=255:g[t][e][s]<0&&(g[t][e][s]=0)}}h=[];for(let t=3*s,e=3*s+4;t<e;++t)h.push(this.nodes[t].slice(3*r,3*r+4));l=y(h);for(let s=0;s<8;++s)for(let r=0;r<8;++r)(a=new m(l[s][r],[[[g[0][s][r],g[1][s][r],g[2][s][r],g[3][s][r]],[g[0][s][r+1],g[1][s][r+1],g[2][s][r+1],g[3][s][r+1]]],[[g[0][s+1][r],g[1][s+1][r],g[2][s+1][r],g[3][s+1][r]],[g[0][s+1][r+1],g[1][s+1][r+1],g[2][s+1][r+1],g[3][s+1][r+1]]]])).paint(t,e)}}}transform(t){if(t instanceof x)for(let e=0,s=this.nodes.length;e<s;++e)for(let s=0,r=this.nodes[0].length;s<r;++s)this.nodes[e][s]=this.nodes[e][s].add(t);else if(t instanceof g)for(let e=0,s=this.nodes.length;e<s;++e)for(let s=0,r=this.nodes[0].length;s<r;++s)this.nodes[e][s]=this.nodes[e][s].transform(t)}scale(t){for(let e=0,s=this.nodes.length;e<s;++e)for(let s=0,r=this.nodes[0].length;s<r;++s)this.nodes[e][s]=this.nodes[e][s].scale(t)}}document.querySelectorAll("rect,circle,ellipse,path,text").forEach((r,n)=>{let o=r.getAttribute("id");o||(o="patchjs_shape"+n,r.setAttribute("id",o));const i=r.style.fill.match(/^url(s*"?\s*#([^s"]+)"?\s*)/),a=r.style.stroke.match(/^url(s*"?\s*#([^s"]+)"?\s*)/);if(i&&i[1]){const a=document.getElementById(i[1]);if(a&&"meshgradient"===a.nodeName){const i=r.getBBox();let l=document.createElementNS(s,"canvas");d(l,{width:i.width,height:i.height});const c=l.getContext("2d");let u=c.createImageData(i.width,i.height);const f=new b(a);"objectBoundingBox"===a.getAttribute("gradientUnits")&&f.scale(new x(i.width,i.height));const p=a.getAttribute("gradientTransform");null!=p&&f.transform(h(p)),"userSpaceOnUse"===a.getAttribute("gradientUnits")&&f.transform(new x(-i.x,-i.y)),f.paintMesh(u.data,l.width),c.putImageData(u,0,0);const y=document.createElementNS(t,"image");d(y,{width:i.width,height:i.height,x:i.x,y:i.y});let g=l.toDataURL();y.setAttributeNS(e,"xlink:href",g),r.parentNode.insertBefore(y,r),r.style.fill="none";const w=document.createElementNS(t,"use");w.setAttributeNS(e,"xlink:href","#"+o);const m="patchjs_clip"+n,M=document.createElementNS(t,"clipPath");M.setAttribute("id",m),M.appendChild(w),r.parentElement.insertBefore(M,r),y.setAttribute("clip-path","url(#"+m+")"),u=null,l=null,g=null}}if(a&&a[1]){const o=document.getElementById(a[1]);if(o&&"meshgradient"===o.nodeName){const i=parseFloat(r.style.strokeWidth.slice(0,-2))*(parseFloat(r.style.strokeMiterlimit)||parseFloat(r.getAttribute("stroke-miterlimit"))||1),a=r.getBBox(),l=Math.trunc(a.width+i),c=Math.trunc(a.height+i),u=Math.trunc(a.x-i/2),f=Math.trunc(a.y-i/2);let p=document.createElementNS(s,"canvas");d(p,{width:l,height:c});const y=p.getContext("2d");let g=y.createImageData(l,c);const w=new b(o);"objectBoundingBox"===o.getAttribute("gradientUnits")&&w.scale(new x(l,c));const m=o.getAttribute("gradientTransform");null!=m&&w.transform(h(m)),"userSpaceOnUse"===o.getAttribute("gradientUnits")&&w.transform(new x(-u,-f)),w.paintMesh(g.data,p.width),y.putImageData(g,0,0);const M=document.createElementNS(t,"image");d(M,{width:l,height:c,x:0,y:0});let S=p.toDataURL();M.setAttributeNS(e,"xlink:href",S);const k="pattern_clip"+n,A=document.createElementNS(t,"pattern");d(A,{id:k,patternUnits:"userSpaceOnUse",width:l,height:c,x:u,y:f}),A.appendChild(M),o.parentNode.appendChild(A),r.style.stroke="url(#"+k+")",g=null,p=null,S=null}}})}();
        </script>
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
  
      const rect = document.getElementById('rect36');
      const marker = document.getElementById('rect41');
      const hueText = document.getElementById('tspan41');
  
      if(hueText) { hueText.textContent = "0°"; }
      if(marker) { marker.setAttribute('transform', 'translate(-5,0)'); }
      if(rect){
        rect.addEventListener('click', function(evt) {
          const svg = rect.ownerSVGElement;
          const pt = svg.createSVGPoint();
          pt.x = evt.clientX;
          pt.y = evt.clientY;
          const cursorpt = pt.matrixTransform(svg.getScreenCTM().inverse());
          const minX = 6;
          const maxX = 131;
          const minT = -5;
          const maxT = 120;
          let cx = Math.max(minX, Math.min(maxX, cursorpt.x));
          let translateX = minT + ((cx - minX) / (maxX - minX)) * (maxT - minT);
          marker.setAttribute('transform', 'translate(' + translateX + ',0)');
          const hue = Math.round(((cx - minX) / (maxX - minX)) * 360);
          hueText.textContent = hue + '°';
          ws.send('hue,' + hue);
        });
      }
  
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
          const svg = rect.ownerSVGElement;
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
          const svg = rect.ownerSVGElement;
          const pt = svg.createSVGPoint();
          pt.x = evt.clientX;
          pt.y = evt.clientY;
          const cursorpt = pt.matrixTransform(svg.getScreenCTM().inverse());
          console.log(cursorpt.x);
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
  Serial.begin(9600);
  
  // Set ESP32 to dual mode (AP+STA)
  WiFi.mode(WIFI_AP_STA);
  
  // Start the Access Point
  WiFi.softAP("LucasPotato", "mypotato");
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
    Serial.println("\nSTA connection failed, continuing with AP only");
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
  server.handleClient();
  webSocket.loop();
}
