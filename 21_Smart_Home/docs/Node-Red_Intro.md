# Node-Red Intro

In den folgenden ausklappbaren Bereichen befindet sich die Node-Red-Flows im JSON-Format. Die können kopiert werden und in Node-Red importiert werden.

- Öffne Node-Red: `http://[hostname]:1880`, z.B. `http://jan:1880`
- Melde dich an: Username: `admin`, Passwort: `strenggeheim`
- Importiere flows per `Hamburger-Icon rechts oben / Import ..."

![node-red_flow_importieren](img/node-red_flow_importieren.jpg)

In das grosse Textfeld in der Mitte wird der jeweilige kopierte JSON-formatierte Text aus den Beispielen unten eingefügt und importiert.

## 01: Basics

Trigger, console.log, Funktionen, Bedingungen,

<details>
<summary>Node-Red Flow anzeigen (JSON)</summary>

```json
[
  {
    "id": "27280ba0b9fc0902",
    "type": "tab",
    "label": "01_Basics",
    "disabled": false,
    "info": "zu übungszwecken",
    "env": []
  },
  {
    "id": "5a5c4c5d0e767af0",
    "type": "group",
    "z": "27280ba0b9fc0902",
    "name": "Inject + Debug",
    "style": {
      "fill": "#6f2fa0",
      "label": true
    },
    "nodes": ["f0bcdfed47850ddd", "e27c0f25efee8358"],
    "x": 134,
    "y": 159,
    "w": 732,
    "h": 82
  },
  {
    "id": "39d6db260249fc17",
    "type": "group",
    "z": "27280ba0b9fc0902",
    "name": "Input wird transformiert",
    "style": {
      "fill": "#6f2fa0",
      "label": true
    },
    "nodes": ["d2a02a7327061167", "07b5e9e03d7a3504", "834b516de1e5c3ad"],
    "x": 134,
    "y": 319,
    "w": 652,
    "h": 82
  },
  {
    "id": "f89ea8f75e637c5d",
    "type": "group",
    "z": "27280ba0b9fc0902",
    "name": "Bedingung + Funktion",
    "style": {
      "fill": "#6f2fa0",
      "label": true
    },
    "nodes": [
      "1dabdd11f150d280",
      "a889c74020abf82d",
      "f6e570014482774c",
      "b0860a43a7b08f2d",
      "137ffceb2a10b1d7",
      "47d880a385536bd6"
    ],
    "x": 134,
    "y": 479,
    "w": 732,
    "h": 162
  },
  {
    "id": "63d37fcda41326e9",
    "type": "group",
    "z": "27280ba0b9fc0902",
    "name": "mehr Logik",
    "style": {
      "fill": "#6f2fa0",
      "label": true
    },
    "nodes": [
      "d1311c816e06807c",
      "33e4c655b67e4d4d",
      "309ffda10da5082f",
      "f296e23fcab6609d",
      "b4d18b184a1a35b9",
      "17e6a417d69dff18",
      "8d4b36518907496f"
    ],
    "x": 134,
    "y": 759,
    "w": 952,
    "h": 182
  },
  {
    "id": "f0bcdfed47850ddd",
    "type": "inject",
    "z": "27280ba0b9fc0902",
    "g": "5a5c4c5d0e767af0",
    "name": "",
    "props": [
      {
        "p": "payload"
      },
      {
        "p": "topic",
        "vt": "str"
      }
    ],
    "repeat": "",
    "crontab": "",
    "once": false,
    "onceDelay": 0.1,
    "topic": "",
    "payload": "huhu",
    "payloadType": "str",
    "x": 230,
    "y": 200,
    "wires": [["e27c0f25efee8358"]]
  },
  {
    "id": "e27c0f25efee8358",
    "type": "debug",
    "z": "27280ba0b9fc0902",
    "g": "5a5c4c5d0e767af0",
    "name": "hier kommt ein String",
    "active": true,
    "tosidebar": true,
    "console": false,
    "tostatus": true,
    "complete": "payload",
    "targetType": "msg",
    "statusVal": "payload",
    "statusType": "auto",
    "x": 720,
    "y": 200,
    "wires": []
  },
  {
    "id": "d2a02a7327061167",
    "type": "inject",
    "z": "27280ba0b9fc0902",
    "g": "39d6db260249fc17",
    "name": "",
    "props": [
      {
        "p": "payload"
      },
      {
        "p": "topic",
        "vt": "str"
      }
    ],
    "repeat": "",
    "crontab": "",
    "once": false,
    "onceDelay": 0.1,
    "topic": "",
    "payload": "",
    "payloadType": "date",
    "x": 240,
    "y": 360,
    "wires": [["07b5e9e03d7a3504"]]
  },
  {
    "id": "07b5e9e03d7a3504",
    "type": "function",
    "z": "27280ba0b9fc0902",
    "g": "39d6db260249fc17",
    "name": "function 1",
    "func": "\n\nlet sensorvalues = {\n    \"temperatur\": 20,\n    \"lichtstarke\": 300\n}\n\nmsg.payload = sensorvalues;\n\nreturn msg; ",
    "outputs": 1,
    "timeout": 0,
    "noerr": 0,
    "initialize": "",
    "finalize": "",
    "libs": [],
    "x": 460,
    "y": 360,
    "wires": [["834b516de1e5c3ad"]]
  },
  {
    "id": "834b516de1e5c3ad",
    "type": "debug",
    "z": "27280ba0b9fc0902",
    "g": "39d6db260249fc17",
    "name": "debug 1",
    "active": true,
    "tosidebar": true,
    "console": false,
    "tostatus": true,
    "complete": "payload",
    "targetType": "msg",
    "statusVal": "payload",
    "statusType": "auto",
    "x": 680,
    "y": 360,
    "wires": []
  },
  {
    "id": "1dabdd11f150d280",
    "type": "inject",
    "z": "27280ba0b9fc0902",
    "g": "f89ea8f75e637c5d",
    "name": "",
    "props": [
      {
        "p": "payload"
      },
      {
        "p": "topic",
        "vt": "str"
      }
    ],
    "repeat": "",
    "crontab": "",
    "once": false,
    "onceDelay": 0.1,
    "topic": "",
    "payload": "oben",
    "payloadType": "str",
    "x": 230,
    "y": 520,
    "wires": [["b0860a43a7b08f2d"]]
  },
  {
    "id": "a889c74020abf82d",
    "type": "inject",
    "z": "27280ba0b9fc0902",
    "g": "f89ea8f75e637c5d",
    "name": "",
    "props": [
      {
        "p": "payload"
      },
      {
        "p": "topic",
        "vt": "str"
      }
    ],
    "repeat": "",
    "crontab": "",
    "once": false,
    "onceDelay": 0.1,
    "topic": "",
    "payload": "mitte",
    "payloadType": "str",
    "x": 230,
    "y": 560,
    "wires": [["b0860a43a7b08f2d"]]
  },
  {
    "id": "f6e570014482774c",
    "type": "inject",
    "z": "27280ba0b9fc0902",
    "g": "f89ea8f75e637c5d",
    "name": "",
    "props": [
      {
        "p": "payload"
      },
      {
        "p": "topic",
        "vt": "str"
      }
    ],
    "repeat": "",
    "crontab": "",
    "once": false,
    "onceDelay": 0.1,
    "topic": "",
    "payload": "unten",
    "payloadType": "str",
    "x": 230,
    "y": 600,
    "wires": [["b0860a43a7b08f2d"]]
  },
  {
    "id": "b0860a43a7b08f2d",
    "type": "function",
    "z": "27280ba0b9fc0902",
    "g": "f89ea8f75e637c5d",
    "name": "Abfrage",
    "func": "if(msg.payload==\"oben\"){\n    return [msg, null]\n}\nelse if(msg.payload==\"unten\"){\n    return [null, msg]\n}\n",
    "outputs": 2,
    "timeout": 0,
    "noerr": 0,
    "initialize": "",
    "finalize": "",
    "libs": [],
    "x": 460,
    "y": 560,
    "wires": [["137ffceb2a10b1d7"], ["47d880a385536bd6"]]
  },
  {
    "id": "137ffceb2a10b1d7",
    "type": "debug",
    "z": "27280ba0b9fc0902",
    "g": "f89ea8f75e637c5d",
    "name": "Auswertung oben",
    "active": true,
    "tosidebar": true,
    "console": false,
    "tostatus": false,
    "complete": "payload",
    "targetType": "msg",
    "statusVal": "",
    "statusType": "auto",
    "x": 710,
    "y": 520,
    "wires": []
  },
  {
    "id": "47d880a385536bd6",
    "type": "debug",
    "z": "27280ba0b9fc0902",
    "g": "f89ea8f75e637c5d",
    "name": "if Ausweertung unten",
    "active": true,
    "tosidebar": true,
    "console": false,
    "tostatus": false,
    "complete": "payload",
    "targetType": "msg",
    "statusVal": "",
    "statusType": "auto",
    "x": 720,
    "y": 600,
    "wires": []
  },
  {
    "id": "d1311c816e06807c",
    "type": "inject",
    "z": "27280ba0b9fc0902",
    "g": "63d37fcda41326e9",
    "name": "",
    "props": [
      {
        "p": "payload"
      },
      {
        "p": "topic",
        "vt": "str"
      }
    ],
    "repeat": "",
    "crontab": "",
    "once": false,
    "onceDelay": 0.1,
    "topic": "",
    "payload": "",
    "payloadType": "date",
    "x": 240,
    "y": 800,
    "wires": [["33e4c655b67e4d4d"]]
  },
  {
    "id": "33e4c655b67e4d4d",
    "type": "function",
    "z": "27280ba0b9fc0902",
    "g": "63d37fcda41326e9",
    "name": "Objekt",
    "func": "let myjson={\n    \"noderedkurs\":{\n        \"mosquitto\":true,\n        \"mosquitto-version\":5.0,\n        \"mosquitto-user\":\"jan\"\n    }\n}\n\nmsg.payload = myjson;\nreturn msg;",
    "outputs": 1,
    "timeout": 0,
    "noerr": 0,
    "initialize": "",
    "finalize": "",
    "libs": [],
    "x": 390,
    "y": 800,
    "wires": [["309ffda10da5082f", "b4d18b184a1a35b9", "8d4b36518907496f"]]
  },
  {
    "id": "309ffda10da5082f",
    "type": "debug",
    "z": "27280ba0b9fc0902",
    "g": "63d37fcda41326e9",
    "name": "debug  als Objekt",
    "active": false,
    "tosidebar": true,
    "console": false,
    "tostatus": false,
    "complete": "payload",
    "targetType": "msg",
    "statusVal": "",
    "statusType": "auto",
    "x": 870,
    "y": 800,
    "wires": []
  },
  {
    "id": "f296e23fcab6609d",
    "type": "debug",
    "z": "27280ba0b9fc0902",
    "g": "63d37fcda41326e9",
    "name": "debug als String",
    "active": false,
    "tosidebar": true,
    "console": false,
    "tostatus": false,
    "complete": "payload",
    "targetType": "msg",
    "statusVal": "",
    "statusType": "auto",
    "x": 860,
    "y": 840,
    "wires": []
  },
  {
    "id": "b4d18b184a1a35b9",
    "type": "json",
    "z": "27280ba0b9fc0902",
    "g": "63d37fcda41326e9",
    "name": "",
    "property": "payload",
    "action": "",
    "pretty": false,
    "x": 690,
    "y": 840,
    "wires": [["f296e23fcab6609d"]]
  },
  {
    "id": "17e6a417d69dff18",
    "type": "debug",
    "z": "27280ba0b9fc0902",
    "g": "63d37fcda41326e9",
    "name": "ein Element von einem Objekt",
    "active": true,
    "tosidebar": true,
    "console": false,
    "tostatus": false,
    "complete": "payload",
    "targetType": "msg",
    "statusVal": "",
    "statusType": "auto",
    "x": 910,
    "y": 900,
    "wires": []
  },
  {
    "id": "8d4b36518907496f",
    "type": "function",
    "z": "27280ba0b9fc0902",
    "g": "63d37fcda41326e9",
    "name": "ein JSON Element",
    "func": "let user = msg.payload[\"noderedkurs\"][\"mosquitto-user\"];\nif(user == \"jan\"){\n    msg.payload = \"Access granted\";\n}\nelse{\n    msg.payload = \"Access denied\";\n}\nreturn msg;",
    "outputs": 1,
    "timeout": 0,
    "noerr": 0,
    "initialize": "",
    "finalize": "",
    "libs": [],
    "x": 650,
    "y": 900,
    "wires": [["17e6a417d69dff18"]]
  }
]
```

</details>

## 02: HTTP-Basics

HTTP GET Request und POST Request

![nodered02_http](img/nodered02_http.jpg)

<details>
<summary>Node-RED Flow anzeigen (JSON)</summary>

```json
[
  {
    "id": "7d5555f0f0b59880",
    "type": "tab",
    "label": "02_HTTP_Basics",
    "disabled": false,
    "info": "",
    "env": []
  },
  {
    "id": "a65b274c935ee24d",
    "type": "group",
    "z": "7d5555f0f0b59880",
    "name": "GET Request -> json bzw html Antwort",
    "style": { "fill": "#6f2fa0", "label": true },
    "nodes": [
      "http_in",
      "func_hello",
      "http_resp",
      "80c48456d7b47fd1",
      "c382bd289f450ecd",
      "845aba545d5157e1",
      "0c83833ef2930587",
      "3d03f3b9d46515a1",
      "d49fdfd1274e0a68",
      "a531e39ba8a9977b"
    ],
    "x": 54,
    "y": 139,
    "w": 1012,
    "h": 262
  },
  {
    "id": "faea61bfa321385c",
    "type": "group",
    "z": "7d5555f0f0b59880",
    "name": "GET und POST Request",
    "style": { "fill": "#6f2fa0", "label": true },
    "nodes": [
      "debug_msg",
      "http_response",
      "http_in_form_html",
      "template_html_form",
      "http_in_form",
      "function_build_json",
      "5acc2d06f524cfb9",
      "69a2bf222e7cae43",
      "5d3394242cfee3b0",
      "63eddf32ff002bc6"
    ],
    "x": 54,
    "y": 519,
    "w": 1112,
    "h": 322
  },
  {
    "id": "http_in",
    "type": "http in",
    "z": "7d5555f0f0b59880",
    "g": "a65b274c935ee24d",
    "name": "GET /hellooo",
    "url": "/hello",
    "method": "get",
    "upload": false,
    "skipBodyParsing": false,
    "swaggerDoc": "",
    "x": 550,
    "y": 220,
    "wires": [["func_hello"]]
  },
  {
    "id": "func_hello",
    "type": "function",
    "z": "7d5555f0f0b59880",
    "g": "a65b274c935ee24d",
    "name": "Build JSON",
    "func": "let name = msg.req.query.name || 'World';\nmsg.payload = { msg: 'Hello ' + name };\nreturn msg;",
    "outputs": 1,
    "timeout": "",
    "noerr": 0,
    "initialize": "",
    "finalize": "",
    "libs": [],
    "x": 740,
    "y": 220,
    "wires": [["http_resp"]]
  },
  {
    "id": "http_resp",
    "type": "http response",
    "z": "7d5555f0f0b59880",
    "g": "a65b274c935ee24d",
    "name": "",
    "statusCode": "",
    "headers": {},
    "x": 940,
    "y": 220,
    "wires": []
  },
  {
    "id": "80c48456d7b47fd1",
    "type": "comment",
    "z": "7d5555f0f0b59880",
    "g": "a65b274c935ee24d",
    "name": "http://[your_hostname]:1880/hello?name=Jan",
    "info": "",
    "x": 650,
    "y": 180,
    "wires": []
  },
  {
    "id": "debug_msg",
    "type": "debug",
    "z": "7d5555f0f0b59880",
    "g": "faea61bfa321385c",
    "name": "Debug Output",
    "active": true,
    "tosidebar": true,
    "complete": "payload",
    "targetType": "msg",
    "x": 1040,
    "y": 800,
    "wires": []
  },
  {
    "id": "http_response",
    "type": "http response",
    "z": "7d5555f0f0b59880",
    "g": "faea61bfa321385c",
    "name": "Send Response",
    "statusCode": "",
    "headers": {},
    "x": 1040,
    "y": 620,
    "wires": []
  },
  {
    "id": "http_in_form_html",
    "type": "http in",
    "z": "7d5555f0f0b59880",
    "g": "faea61bfa321385c",
    "name": "GET /form",
    "url": "/form",
    "method": "get",
    "swaggerDoc": "",
    "x": 540,
    "y": 600,
    "wires": [["template_html_form"]]
  },
  {
    "id": "template_html_form",
    "type": "template",
    "z": "7d5555f0f0b59880",
    "g": "faea61bfa321385c",
    "name": "HTML Form",
    "field": "payload",
    "fieldType": "msg",
    "format": "handlebars",
    "syntax": "mustache",
    "template": "<!DOCTYPE html>\n<html>\n  <body style=\"font-family: sans-serif;\">\n    <h3>Say Hello 👋</h3>\n    <form action=\"/submitted\" method=\"POST\">\n      <input type=\"text\" name=\"name\" placeholder=\"Enter your name\" required>\n      <button type=\"submit\">Send</button>\n    </form>\n  </body>\n</html>",
    "x": 780,
    "y": 600,
    "wires": [["http_response"]]
  },
  {
    "id": "http_in_form",
    "type": "http in",
    "z": "7d5555f0f0b59880",
    "g": "faea61bfa321385c",
    "name": "POST /submitted",
    "url": "/submitted",
    "method": "post",
    "upload": false,
    "skipBodyParsing": false,
    "swaggerDoc": "",
    "x": 560,
    "y": 780,
    "wires": [["function_build_json"]]
  },
  {
    "id": "function_build_json",
    "type": "function",
    "z": "7d5555f0f0b59880",
    "g": "faea61bfa321385c",
    "name": "Build JSON Response",
    "func": "const name = msg.req.body.name || 'World';\nmsg.payload = { msg: `Hello ${name}` };\nreturn msg;",
    "outputs": 1,
    "timeout": "",
    "noerr": 0,
    "initialize": "",
    "finalize": "",
    "libs": [],
    "x": 780,
    "y": 780,
    "wires": [["debug_msg", "http_response"]]
  },
  {
    "id": "5acc2d06f524cfb9",
    "type": "comment",
    "z": "7d5555f0f0b59880",
    "g": "faea61bfa321385c",
    "name": "http://[your_hostname]:1880/form",
    "info": "",
    "x": 610,
    "y": 560,
    "wires": []
  },
  {
    "id": "c382bd289f450ecd",
    "type": "comment",
    "z": "7d5555f0f0b59880",
    "g": "a65b274c935ee24d",
    "name": "Sende JSON Response bei GET Request",
    "info": "",
    "x": 240,
    "y": 220,
    "wires": []
  },
  {
    "id": "69a2bf222e7cae43",
    "type": "comment",
    "z": "7d5555f0f0b59880",
    "g": "faea61bfa321385c",
    "name": "Sende HTML Response bei GET Request",
    "info": "",
    "x": 240,
    "y": 600,
    "wires": []
  },
  {
    "id": "845aba545d5157e1",
    "type": "comment",
    "z": "7d5555f0f0b59880",
    "g": "a65b274c935ee24d",
    "name": "Sende HTML Response bei GET Request",
    "info": "",
    "x": 240,
    "y": 360,
    "wires": []
  },
  {
    "id": "0c83833ef2930587",
    "type": "http in",
    "z": "7d5555f0f0b59880",
    "g": "a65b274c935ee24d",
    "name": "GET /html",
    "url": "/html",
    "method": "get",
    "upload": false,
    "skipBodyParsing": false,
    "swaggerDoc": "",
    "x": 540,
    "y": 360,
    "wires": [["3d03f3b9d46515a1"]]
  },
  {
    "id": "3d03f3b9d46515a1",
    "type": "template",
    "z": "7d5555f0f0b59880",
    "g": "a65b274c935ee24d",
    "name": "HTML Form",
    "field": "payload",
    "fieldType": "msg",
    "format": "handlebars",
    "syntax": "mustache",
    "template": "<!DOCTYPE html>\n<html>\n  <body style=\"font-family: sans-serif;\">\n    <h3>Say Hello 👋</h3>\n    <form action=\"/submitted\" method=\"POST\">\n      <input type=\"text\" name=\"name\" placeholder=\"Enter your name\" required>\n      <button type=\"submit\">Send</button>\n    </form>\n  </body>\n</html>",
    "x": 750,
    "y": 360,
    "wires": [["d49fdfd1274e0a68"]]
  },
  {
    "id": "d49fdfd1274e0a68",
    "type": "http response",
    "z": "7d5555f0f0b59880",
    "g": "a65b274c935ee24d",
    "name": "Send Response",
    "statusCode": "",
    "headers": {},
    "x": 960,
    "y": 360,
    "wires": []
  },
  {
    "id": "a531e39ba8a9977b",
    "type": "comment",
    "z": "7d5555f0f0b59880",
    "g": "a65b274c935ee24d",
    "name": "http://[your_hostname]:1880/html",
    "info": "",
    "x": 610,
    "y": 320,
    "wires": []
  },
  {
    "id": "5d3394242cfee3b0",
    "type": "comment",
    "z": "7d5555f0f0b59880",
    "g": "faea61bfa321385c",
    "name": "Sende JSON Response bei POST Request",
    "info": "",
    "x": 240,
    "y": 780,
    "wires": []
  },
  {
    "id": "63eddf32ff002bc6",
    "type": "comment",
    "z": "7d5555f0f0b59880",
    "g": "faea61bfa321385c",
    "name": "http://[your_hostname]:1880/submitted",
    "info": "",
    "x": 630,
    "y": 740,
    "wires": []
  }
]
```

</details>

#### Beispiele

- [HTTP-Beispiel mit Node-Red und TouchDesigner](HTTP_TouchDesigner_Node-Red/node-red_touchdesigner_http)
  ![TD HTTP](HTTP_TouchDesigner_Node-Red/node-red_touchdesigner_http.gif)

## 03: Interaktion mit Maria DB

Datenbankoperationen(SQL) von Node-Red aus (ohne PHP!)
To do:

![Node-Red DB](img/nodered03_db.jpg)

- Datenbank muss angelegt werden. Am einfchsten per UI (Adminer): `http://[dein_hostname]:8080`(bei mir `http://jan:8080`)
  **Server**: `mariadb`
  **Benutzer**: `root`
  **Passwort**: `strenggeheim`
  Bei erfolgreicher Anmeldung: Erstelle die Datenbank namens `nodered`

- In Node-Red: `mysql`-Node (hier "MariaDB local"): Doppelklick auf die Node, dan Stift-Button klicken,dann eingeben:  
  **Host**: `[dein_hostname]`(bei mir `jan`),
  **User**: `root`,
  **Passwort**: `strenggeheim`,
  **Database**: `nodered`

![nodered03_db_settings](img/nodered03_db_settings.jpg)

<details>
<summary>Node-RED Flow anzeigen (JSON)</summary>

```json
[
  {
    "id": "test-db-flow",
    "type": "tab",
    "label": "03_MariaDB_Test",
    "disabled": false,
    "info": ""
  },
  {
    "id": "c4b741bacb963638",
    "type": "function",
    "z": "test-db-flow",
    "name": "select",
    "func": "msg.topic = \"SELECT * FROM weather_data LIMIT 5\";\nreturn msg;",
    "outputs": 1,
    "timeout": 0,
    "noerr": 0,
    "initialize": "",
    "finalize": "",
    "libs": [],
    "x": 550,
    "y": 340,
    "wires": [["356a20e1ca0f6092"]]
  },
  {
    "id": "c0d54209f4c9f811",
    "type": "inject",
    "z": "test-db-flow",
    "name": "",
    "props": [{ "p": "payload" }, { "p": "topic", "vt": "str" }],
    "repeat": "",
    "crontab": "",
    "once": false,
    "onceDelay": 0.1,
    "topic": "",
    "payload": "iso",
    "payloadType": "date",
    "x": 400,
    "y": 340,
    "wires": [["c4b741bacb963638"]]
  },
  {
    "id": "714825a7b3b7ba0c",
    "type": "function",
    "z": "test-db-flow",
    "name": "insert",
    "func": "msg.topic = \"INSERT INTO `weather_data` (`location`, `temperature_celsius`, `rain`, `showers`, `snowfall`, `cloud_cover`, `weather_condition`) VALUES ('Bern', 22, 0, 0, 0, 95, 'bewölkt') \"\nreturn msg;",
    "outputs": 1,
    "timeout": 0,
    "noerr": 0,
    "initialize": "",
    "finalize": "",
    "libs": [],
    "x": 550,
    "y": 420,
    "wires": [["356a20e1ca0f6092"]]
  },
  {
    "id": "733033ab602d4e7e",
    "type": "inject",
    "z": "test-db-flow",
    "name": "insert",
    "props": [{ "p": "payload" }, { "p": "topic", "vt": "str" }],
    "repeat": "",
    "crontab": "",
    "once": false,
    "onceDelay": 0.1,
    "topic": "",
    "payload": "",
    "payloadType": "date",
    "x": 390,
    "y": 420,
    "wires": [["714825a7b3b7ba0c"]]
  },
  {
    "id": "e4e777261e89ad21",
    "type": "function",
    "z": "test-db-flow",
    "name": "delete",
    "func": "msg.topic = \"DELETE FROM `weather_data`\"\nreturn msg;",
    "outputs": 1,
    "timeout": 0,
    "noerr": 0,
    "initialize": "",
    "finalize": "",
    "libs": [],
    "x": 550,
    "y": 500,
    "wires": [["356a20e1ca0f6092"]]
  },
  {
    "id": "07ced6bb20ba6a55",
    "type": "inject",
    "z": "test-db-flow",
    "name": "delete",
    "props": [{ "p": "payload" }, { "p": "topic", "vt": "str" }],
    "repeat": "",
    "crontab": "",
    "once": false,
    "onceDelay": 0.1,
    "topic": "",
    "payload": "",
    "payloadType": "date",
    "x": 390,
    "y": 500,
    "wires": [["e4e777261e89ad21"]]
  },
  {
    "id": "0562217e52ad1367",
    "type": "debug",
    "z": "test-db-flow",
    "name": "debug",
    "active": true,
    "tosidebar": true,
    "console": false,
    "tostatus": false,
    "complete": "payload",
    "targetType": "msg",
    "statusVal": "",
    "statusType": "auto",
    "x": 1050,
    "y": 420,
    "wires": []
  },
  {
    "id": "356a20e1ca0f6092",
    "type": "mysql",
    "z": "test-db-flow",
    "mydb": "40c709131d86de40",
    "name": "",
    "x": 860,
    "y": 420,
    "wires": [["0562217e52ad1367"]]
  },
  {
    "id": "bf5d02f742cc1e57",
    "type": "inject",
    "z": "test-db-flow",
    "name": "",
    "props": [{ "p": "payload" }, { "p": "topic", "vt": "str" }],
    "repeat": "",
    "crontab": "",
    "once": false,
    "onceDelay": 0.1,
    "topic": "",
    "payload": "iso",
    "payloadType": "date",
    "x": 400,
    "y": 260,
    "wires": [["49a9165eda699ced"]]
  },
  {
    "id": "49a9165eda699ced",
    "type": "function",
    "z": "test-db-flow",
    "name": "create table",
    "func": "msg.topic = \"CREATE TABLE `weather_data` (`id` INT AUTO_INCREMENT PRIMARY KEY,`location` VARCHAR(255) NOT NULL,`temperature_celsius` FLOAT,`rain` FLOAT,`showers` FLOAT,`snowfall` FLOAT,`cloud_cover` INT,`weather_condition` VARCHAR(255),`created_at` TIMESTAMP DEFAULT CURRENT_TIMESTAMP);\";\nreturn msg;",
    "outputs": 1,
    "timeout": 0,
    "noerr": 0,
    "initialize": "",
    "finalize": "",
    "libs": [],
    "x": 570,
    "y": 260,
    "wires": [["356a20e1ca0f6092"]]
  },
  {
    "id": "3928d75451921b18",
    "type": "inject",
    "z": "test-db-flow",
    "name": "",
    "props": [{ "p": "payload" }, { "p": "topic", "vt": "str" }],
    "repeat": "",
    "crontab": "",
    "once": false,
    "onceDelay": 0.1,
    "topic": "",
    "payload": "iso",
    "payloadType": "date",
    "x": 380,
    "y": 580,
    "wires": [["51938b18be6344b6"]]
  },
  {
    "id": "51938b18be6344b6",
    "type": "function",
    "z": "test-db-flow",
    "name": "drop table",
    "func": "msg.topic = \"DROP TABLE IF EXISTS `weather_data`;\";\nreturn msg;",
    "outputs": 1,
    "timeout": 0,
    "noerr": 0,
    "initialize": "",
    "finalize": "",
    "libs": [],
    "x": 570,
    "y": 580,
    "wires": [["356a20e1ca0f6092"]]
  },
  {
    "id": "40c709131d86de40",
    "type": "MySQLdatabase",
    "name": "MariaDB local ",
    "host": "q7",
    "port": "3306",
    "db": "nodered",
    "tz": "",
    "charset": "UTF8"
  },
  {
    "id": "85dfa84a1076e473",
    "type": "global-config",
    "env": [],
    "modules": { "node-red-node-mysql": "2.0.0" }
  }
]
```

</details>

## 04: Extract - Transform - Load - Unload

IM3 ganz anschaulich - so einfach kann's gehen.

![nodered04_etl](img/nodered04_etl.jpg)

<details>
<summary>Node-RED Flow anzeigen (JSON)</summary>

```json
[
  {
    "id": "94b55faa52c8768c",
    "type": "tab",
    "label": "04_ETL",
    "disabled": false,
    "info": "Extrakt -> Transform -> Load in MariaDB und HTTP /unload zum Abruf"
  },
  {
    "id": "b25b83ce268ddac9",
    "type": "group",
    "z": "94b55faa52c8768c",
    "name": "Extract - Transform - Load",
    "style": { "fill": "#6f2fa0", "label": true },
    "nodes": [
      "e42fe5e2cc02847a",
      "5a54c097c7d068d3",
      "024537131b86baf0",
      "ea561fda4842de5d",
      "cd86fd2dcab47b12",
      "4c72943c2792f177",
      "782be00d9de0b573"
    ],
    "x": 374,
    "y": 279,
    "w": 1532,
    "h": 82
  },
  {
    "id": "9ba065b35c0d78cc",
    "type": "group",
    "z": "94b55faa52c8768c",
    "name": "Unload (API) + Frontend",
    "style": { "fill": "#6f2fa0", "label": true },
    "nodes": [
      "88f926efd55d659b",
      "cb09c7a597b21034",
      "67bdd44af2ea57ad",
      "22b94a0d6785024d",
      "bd7b52176d198bc3",
      "eccd478fbe085994",
      "7b9b03ff3c47adba",
      "40afcc05fd2305a6"
    ],
    "x": 374,
    "y": 479,
    "w": 1592,
    "h": 202
  },
  {
    "id": "e42fe5e2cc02847a",
    "type": "http request",
    "z": "94b55faa52c8768c",
    "g": "b25b83ce268ddac9",
    "name": "Extract from API",
    "method": "GET",
    "ret": "txt",
    "paytoqs": "ignore",
    "url": "https://api.open-meteo.com/v1/forecast?latitude=46.9481,46.8499,47.3667&longitude=7.4474,9.5329,8.55&current=temperature_2m,relative_humidity_2m,apparent_temperature,is_day,rain,showers,snowfall,cloud_cover&temperature_unit=fahrenheit&timezone=auto&forecast_days=1",
    "tls": "",
    "persist": false,
    "proxy": "",
    "insecureHTTPParser": false,
    "authType": "",
    "senderr": false,
    "headers": [],
    "x": 700,
    "y": 320,
    "wires": [["5a54c097c7d068d3"]]
  },
  {
    "id": "5a54c097c7d068d3",
    "type": "function",
    "z": "94b55faa52c8768c",
    "g": "b25b83ce268ddac9",
    "name": "Transform",
    "func": "// Erwartet msg.payload als JSON-String oder bereits als Objekt-Array\nlet data = msg.payload;\nif (typeof data === 'string') {\n    try {\n        data = JSON.parse(data);\n    } catch (e) {\n        node.error('JSON parse error from Open-Meteo response', msg);\n        return null;\n    }\n}\n\n// Hilfsfunktionen\nfunction fToC(f) {\n    return Math.round(((f - 32) * (5/9)) * 100) / 100;\n}\nfunction determineLocation(lat, lon) {\n    lat = Math.round(lat * 100) / 100;\n    lon = Math.round(lon * 100) / 100;\n    if (lat === 46.95 && lon === 7.46) return 'Bern';\n    if (lat === 46.85 && lon === 9.52) return 'Chur';\n    if (lat === 47.37 && (lon === 8.56 || Math.round(lon*100)/100 === 8.56)) return 'Zürich';\n    return 'Unbekannt';\n}\nfunction determineWeatherCondition(cloud_cover, rain, showers, snowfall) {\n    if (snowfall > 0.0) return 'schneit';\n    if (rain > 0.0 || showers > 0.0) return 'regnerisch';\n    if (cloud_cover >= 85) return 'bewölkt';\n    if (cloud_cover >= 30) return 'teilweise bewölkt';\n    return 'sonnig';\n}\n\nlet transformed = [];\n\nif (!Array.isArray(data)) {\n    node.warn('Open-Meteo returned unexpected payload (not array)');\n} else {\n    for (let i=0;i<data.length;i++) {\n        let item = data[i];\n        let cur = item.current || {};\n        const temperature_celsius = (cur.temperature_2m !== undefined) ? fToC(cur.temperature_2m) : null;\n        const location = determineLocation(item.latitude, item.longitude);\n        const weather_condition = determineWeatherCondition(\n            parseInt(cur.cloud_cover || 0),\n            parseFloat(cur.rain || 0),\n            parseFloat(cur.showers || 0),\n            parseFloat(cur.snowfall || 0)\n        );\n        transformed.push({\n            location: location,\n            temperature_celsius: temperature_celsius,\n            rain: Math.round(parseFloat(cur.rain || 0)),\n            showers: Math.round(parseFloat(cur.showers || 0)),\n            snowfall: Math.round(parseFloat(cur.snowfall || 0)),\n            cloud_cover: parseInt(cur.cloud_cover || 0),\n            weather_condition: weather_condition,\n            source_time: cur.time || null\n        });\n    }\n}\n\nmsg.transformedData = transformed;\n// Weitergeben des Arrays zum Split (each item einzeln)\nmsg.payload = transformed;\nreturn msg;",
    "outputs": 1,
    "timeout": "",
    "noerr": 0,
    "initialize": "",
    "finalize": "",
    "libs": [],
    "x": 880,
    "y": 320,
    "wires": [["024537131b86baf0"]]
  },
  {
    "id": "024537131b86baf0",
    "type": "split",
    "z": "94b55faa52c8768c",
    "g": "b25b83ce268ddac9",
    "name": "split transformedData → each item",
    "splt": "\\n",
    "spltType": "str",
    "arraySplt": 1,
    "arraySpltType": "len",
    "stream": false,
    "addname": "",
    "property": "payload",
    "x": 1100,
    "y": 320,
    "wires": [["ea561fda4842de5d"]]
  },
  {
    "id": "ea561fda4842de5d",
    "type": "function",
    "z": "94b55faa52c8768c",
    "g": "b25b83ce268ddac9",
    "name": "Load",
    "func": "// msg.payload is one transformed item\nlet it = msg.payload;\nif (!it) return null;\n\n// Prepare values, ensure numeric fields\nlet loc = it.location || 'Unbekannt';\nlet temp = (it.temperature_celsius === null || it.temperature_celsius === undefined) ? 'NULL' : Number(it.temperature_celsius);\nlet rain = (it.rain === undefined) ? 0 : Number(it.rain);\nlet showers = (it.showers === undefined) ? 0 : Number(it.showers);\nlet snowfall = (it.snowfall === undefined) ? 0 : Number(it.snowfall);\nlet cloud = (it.cloud_cover === undefined) ? 0 : Number(it.cloud_cover);\nlet cond = it.weather_condition || '';\n\n// Escape single quotes in strings\nfunction esc(s) { return String(s).replace(/'/g, \"''\"); }\n\n// Build SQL (note: values come from API -> considered safe here; replace with prepared statements if desired)\nlet sql = `INSERT INTO weather_data (location, temperature_celsius, rain, showers, snowfall, cloud_cover, weather_condition) VALUES ('${esc(loc)}', ${temp}, ${rain}, ${showers}, ${snowfall}, ${cloud}, '${esc(cond)}')`;\n\nmsg.topic = sql;\nreturn msg;\n\n\n\n/*\nSQL Command zur Erstellung der Tabelle:\n\nCREATE DATABASE `etl`;\n\n-- Zuerst die Datenbank auswählen, zu der die Tabelle gehören soll\nUSE etl;\n\n-- Nun die Tabelle in der ausgewählten Datenbank erstellen\nCREATE TABLE `weather_data` (\n  `id` int(11) UNSIGNED NOT NULL AUTO_INCREMENT,\n  `location` varchar(64) NOT NULL,\n  `temperature_celsius` float NOT NULL,\n  `rain` int(5) NOT NULL,\n  `showers` int(5) NOT NULL,\n  `snowfall` int(5) NOT NULL,\n  `cloud_cover` int(5) NOT NULL,\n  `weather_condition` varchar(64) NOT NULL,\n  `created_at` timestamp NOT NULL DEFAULT current_timestamp(),\n  PRIMARY KEY (`id`)\n) ENGINE=InnoDB DEFAULT CHARSET=utf8mb3 COLLATE=utf8mb3_general_ci;\n\n*/",
    "outputs": 1,
    "timeout": "",
    "noerr": 0,
    "initialize": "",
    "finalize": "",
    "libs": [],
    "x": 1330,
    "y": 320,
    "wires": [["782be00d9de0b573"]]
  },
  {
    "id": "cd86fd2dcab47b12",
    "type": "debug",
    "z": "94b55faa52c8768c",
    "g": "b25b83ce268ddac9",
    "name": "Insert result",
    "active": true,
    "tosidebar": true,
    "console": false,
    "tostatus": false,
    "complete": "true",
    "targetType": "full",
    "x": 1790,
    "y": 320,
    "wires": []
  },
  {
    "id": "88f926efd55d659b",
    "type": "http in",
    "z": "94b55faa52c8768c",
    "g": "9ba065b35c0d78cc",
    "name": "HTTP GET /unload",
    "url": "/unload",
    "method": "get",
    "upload": false,
    "skipBodyParsing": false,
    "swaggerDoc": "",
    "x": 490,
    "y": 560,
    "wires": [["cb09c7a597b21034"]]
  },
  {
    "id": "cb09c7a597b21034",
    "type": "function",
    "z": "94b55faa52c8768c",
    "g": "9ba065b35c0d78cc",
    "name": "Unload",
    "func": "// Optional: support ?location=Bern – wenn vorhanden, filter anwenden\nlet location = (msg.req && msg.req.query && msg.req.query.location) ? msg.req.query.location : null;\nlet sql = '';\nif (location) {\n    // Escape single quotes\n    location = String(location).replace(/'/g, \"''\");\n    sql = `SELECT * FROM weather_data WHERE location='${location}' ORDER BY created_at DESC LIMIT 216`;\n} else {\n    sql = `SELECT * FROM weather_data ORDER BY created_at DESC LIMIT 216`;\n}\nmsg.topic = sql;\nreturn msg;",
    "outputs": 1,
    "timeout": "",
    "noerr": 0,
    "initialize": "",
    "finalize": "",
    "libs": [],
    "x": 740,
    "y": 560,
    "wires": [["22b94a0d6785024d"]]
  },
  {
    "id": "67bdd44af2ea57ad",
    "type": "function",
    "z": "94b55faa52c8768c",
    "g": "9ba065b35c0d78cc",
    "name": "Format as JSON + set headers",
    "func": "// mysql node returns rows in msg.payload\nmsg.headers = msg.headers || {};\nmsg.headers['Content-Type'] = 'application/json; charset=utf-8';\n// Pass through payload (array of rows)\n// In case of empty result, return 404 with JSON error (like PHP did)\nif (!msg.payload || (Array.isArray(msg.payload) && msg.payload.length === 0)) {\n    msg.statusCode = 404;\n    msg.payload = { error: 'No records found' };\n} else {\n    msg.statusCode = 200;\n}\nreturn msg;",
    "outputs": 1,
    "timeout": "",
    "noerr": 0,
    "initialize": "",
    "finalize": "",
    "libs": [],
    "x": 1430,
    "y": 560,
    "wires": [["40afcc05fd2305a6"]]
  },
  {
    "id": "4c72943c2792f177",
    "type": "inject",
    "z": "94b55faa52c8768c",
    "g": "b25b83ce268ddac9",
    "name": "alle 15 Minuten",
    "props": [{ "p": "payload" }, { "p": "topic", "vt": "str" }],
    "repeat": "900",
    "crontab": "",
    "once": true,
    "onceDelay": 5,
    "topic": "",
    "payload": "",
    "payloadType": "date",
    "x": 500,
    "y": 320,
    "wires": [["e42fe5e2cc02847a"]]
  },
  {
    "id": "bd7b52176d198bc3",
    "type": "comment",
    "z": "94b55faa52c8768c",
    "g": "9ba065b35c0d78cc",
    "name": "http://[dein_hostname]:1880/unload",
    "info": "",
    "x": 540,
    "y": 520,
    "wires": []
  },
  {
    "id": "eccd478fbe085994",
    "type": "function",
    "z": "94b55faa52c8768c",
    "g": "9ba065b35c0d78cc",
    "name": "Build HTML with Chart.js",
    "func": "const rawData = msg.payload || [];\n\n// 1. Daten filtern und extrahieren (Logik von script.js übernommen)\nconst temperature_bern = rawData\n  .filter(item => item.location === \"Bern\")\n  .map(item => item.temperature_celsius);\nconst temperature_chur = rawData\n  .filter(item => item.location === \"Chur\")\n  .map(item => item.temperature_celsius);\nconst temperature_zuerich = rawData\n  .filter(item => item.location === \"Zürich\")\n  .map(item => item.temperature_celsius);\nconst time = rawData\n  .filter(item => item.location === \"Bern\")\n  .map(item => item.created_at);\n\n// 2. Erzeuge den JavaScript-Code mit den eingebetteten Daten\nconst scriptCode = `\ndocument.addEventListener(\"DOMContentLoaded\", () => {\n  const temperature_bern = ${JSON.stringify(temperature_bern)};\n  const temperature_chur = ${JSON.stringify(temperature_chur)};\n  const temperature_zuerich = ${JSON.stringify(temperature_zuerich)};\n  const time = ${JSON.stringify(time)};\n\n  let myChart = document.querySelector('#temperatureChart').getContext(\"2d\");\n  const fridgeChart = new Chart(myChart, {\n    type: \"line\", \n    data: {\n      labels: time,\n      datasets:[\n        {\n            data: temperature_bern,\n            label: \"Bern\",\n            borderColor: 'rgb(255, 99, 132)',\n            tension: 0.1\n        },          \n        {\n            data: temperature_chur,\n            label: \"Chur\",\n            borderColor: 'rgb(54, 162, 235)',\n            tension: 0.1\n        },          \n        {\n            data: temperature_zuerich,\n            label: \"Zürich\",\n            borderColor: 'rgb(75, 192, 192)',\n            tension: 0.1\n        }          \n      ]\n    }\n  });\n});\n`;\n\n// 3. Erzeuge das vollständige HTML-Dokument\nconst html = `\n<!DOCTYPE html>\n<html lang=\"de\">\n  <head>\n    <meta charset=\"UTF-8\" />\n    <title>Stadttemperaturen</title>\n    <script src=\"https://cdn.jsdelivr.net/npm/chart.js\"></script>\n  </head>\n  <body>\n    <h1>Temperaturen aller 3 Städte</h1>\n    <p>Daten geladen von Node-RED Flow.</p>\n    <canvas id=\"temperatureChart\"></canvas>\n    <script>${scriptCode}</script>\n  </body>\n</html>\n`;\n\n// 4. Setze Header und Payload für die HTTP-Antwort\nmsg.headers = msg.headers || {};\nmsg.headers['Content-Type'] = 'text/html; charset=utf-8';\nmsg.statusCode = (rawData.length > 0) ? 200 : 404;\nmsg.payload = html;\n\nreturn msg;",
    "outputs": 1,
    "timeout": "",
    "noerr": 0,
    "initialize": "",
    "finalize": "",
    "libs": [],
    "x": 1410,
    "y": 640,
    "wires": [["7b9b03ff3c47adba"]]
  },
  {
    "id": "7b9b03ff3c47adba",
    "type": "http response",
    "z": "94b55faa52c8768c",
    "g": "9ba065b35c0d78cc",
    "name": "HTTP Response (HTML)",
    "statusCode": "",
    "headers": {},
    "x": 1830,
    "y": 640,
    "wires": []
  },
  {
    "id": "40afcc05fd2305a6",
    "type": "debug",
    "z": "94b55faa52c8768c",
    "g": "9ba065b35c0d78cc",
    "name": "debug 2",
    "active": true,
    "tosidebar": true,
    "console": false,
    "tostatus": false,
    "complete": "false",
    "statusVal": "",
    "statusType": "auto",
    "x": 1780,
    "y": 560,
    "wires": []
  },
  {
    "id": "782be00d9de0b573",
    "type": "mysql",
    "z": "94b55faa52c8768c",
    "g": "b25b83ce268ddac9",
    "mydb": "40c709131d86de40",
    "name": "",
    "x": 1500,
    "y": 320,
    "wires": [["cd86fd2dcab47b12"]]
  },
  {
    "id": "22b94a0d6785024d",
    "type": "mysql",
    "z": "94b55faa52c8768c",
    "g": "9ba065b35c0d78cc",
    "mydb": "40c709131d86de40",
    "name": "",
    "x": 940,
    "y": 560,
    "wires": [["67bdd44af2ea57ad", "eccd478fbe085994"]]
  },
  {
    "id": "40c709131d86de40",
    "type": "MySQLdatabase",
    "name": "MariaDB local ",
    "host": "q7",
    "port": "3306",
    "db": "nodered",
    "tz": "",
    "charset": "UTF8"
  },
  {
    "id": "b50f637891809bd0",
    "type": "global-config",
    "env": [],
    "modules": { "node-red-node-mysql": "2.0.0" }
  }
]
```

</details>

## 05: MQTT Test

MQTT ist ein leichtgewichtiges Kommunikationsprotokoll für Standard-Kommunikation im Internet der Dinge.
Wir können als Kommunikationsserver (="Broker") den von uns per docker-compose installierten MQTT-Broker `mosquitto`verwenden.
Da der Service `mosquitto`heisst und über das docker-interne Netzwerk über diese Adresse erreichbar ist, braucht es in der internen Kommunikation zwischen Node-Red und Mosquitto keinen Hostnamen oder IP-Adresse.
Stattdessen kann auch ein Online-Broker verwendet werden. Zum Beispiel:

- [broker.emqx.io](https://broker.emqx.io)
- [test.mosquitto.org](http://test.mosquitto.org)
- [broker.hivemq.com](http://broker.hivemq.com)
- [public.mqttserver.eu](http://public.mqttserver.eu)
- [broker.mqtt-dashboard.com](http://broker.mqtt-dashboard.com)

Der Port `1883` ist bei den gängigen Brokern Standard.

![nodered05_mqtt_01](img/nodered05_mqtt_01.jpg)

<details>
<summary><b>Node-RED Flow anzeigen (JSON)</b></summary>

```json
[
  {
    "id": "1c1ccda937ce7294",
    "type": "tab",
    "label": "05_MQTT Tests",
    "disabled": false,
    "info": "",
    "env": []
  },
  {
    "id": "f14814695f431557",
    "type": "group",
    "z": "1c1ccda937ce7294",
    "name": "",
    "style": {
      "fill": "#0070c0",
      "label": true,
      "color": "#0070c0"
    },
    "nodes": [
      "e760fc5827336020",
      "3b6a9b3f7c7cea15",
      "8098f4baf22bade8",
      "74a84814.4ecc6",
      "d9c64d70.401e88"
    ],
    "x": 194,
    "y": 619,
    "w": 1112,
    "h": 82
  },
  {
    "id": "92eac68a3a0a4e68",
    "type": "group",
    "z": "1c1ccda937ce7294",
    "g": "5935321d3ff7c72b",
    "name": "Büro",
    "style": {
      "fill": "#6f2fa0",
      "label": true
    },
    "nodes": [
      "66305ad1d86ead7e",
      "aa1cd739b6cbbeba",
      "f7f9729f0b020753",
      "2a648c468fbba2c3"
    ],
    "x": 134,
    "y": 1319,
    "w": 432,
    "h": 142
  },
  {
    "id": "8845791c3273f8a4",
    "type": "group",
    "z": "1c1ccda937ce7294",
    "g": "5935321d3ff7c72b",
    "name": "Schlafzimmer",
    "style": {
      "fill": "#6f2fa0",
      "label": true
    },
    "nodes": [
      "b20891d5ccd0a3bf",
      "2e0617c03d2c304c",
      "ca4b343d8c21f419",
      "baa5e24b1be570a4"
    ],
    "x": 694,
    "y": 1319,
    "w": 472,
    "h": 142
  },
  {
    "id": "8083fbfd5c2c3198",
    "type": "group",
    "z": "1c1ccda937ce7294",
    "g": "be9103b31f26e902",
    "name": "WC",
    "style": {
      "fill": "#6f2fa0",
      "label": true
    },
    "nodes": [
      "cb26d19cdf7154f5",
      "6de1325ee73ae4f4",
      "679836f421d2815e",
      "4d37d70d76310e50"
    ],
    "x": 134,
    "y": 1559,
    "w": 432,
    "h": 142
  },
  {
    "id": "fcc5e797fb12b637",
    "type": "group",
    "z": "1c1ccda937ce7294",
    "g": "be9103b31f26e902",
    "name": "Küche",
    "style": {
      "fill": "#6f2fa0",
      "label": true
    },
    "nodes": [
      "94364a3ff0967d31",
      "f8f39087a9405d59",
      "9dbbcb4b47b39881",
      "e6730d5e60567f7b"
    ],
    "x": 694,
    "y": 1559,
    "w": 472,
    "h": 142
  },
  {
    "id": "be9103b31f26e902",
    "type": "group",
    "z": "1c1ccda937ce7294",
    "g": "ac27501245a33d3a",
    "name": "Erdgeschoss",
    "style": {
      "fill": "#001f60",
      "label": true
    },
    "nodes": ["8083fbfd5c2c3198", "fcc5e797fb12b637"],
    "x": 108,
    "y": 1533,
    "w": 1084,
    "h": 194
  },
  {
    "id": "5935321d3ff7c72b",
    "type": "group",
    "z": "1c1ccda937ce7294",
    "g": "ac27501245a33d3a",
    "name": "Obergeschoss",
    "style": {
      "fill": "#001f60",
      "label": true
    },
    "nodes": ["92eac68a3a0a4e68", "8845791c3273f8a4"],
    "x": 108,
    "y": 1293,
    "w": 1084,
    "h": 194
  },
  {
    "id": "ac27501245a33d3a",
    "type": "group",
    "z": "1c1ccda937ce7294",
    "name": "Haus",
    "style": {
      "fill": "#3f3f3f",
      "label": true
    },
    "nodes": ["be9103b31f26e902", "5935321d3ff7c72b"],
    "x": 82,
    "y": 1267,
    "w": 1136,
    "h": 486
  },
  {
    "id": "f8db239ea2210a9b",
    "type": "group",
    "z": "1c1ccda937ce7294",
    "name": "Kommunikation mit ESP und TouchDesigner",
    "style": {
      "label": true
    },
    "nodes": [
      "9706f9fce318f43f",
      "bed761109a90c47a",
      "a3b49ee52f817a53",
      "dd91693eacb3a754",
      "2f53e4edd910e895"
    ],
    "x": 194,
    "y": 939,
    "w": 552,
    "h": 142
  },
  {
    "id": "6566a3379a01c108",
    "type": "group",
    "z": "1c1ccda937ce7294",
    "name": "Alle Lichter",
    "style": {
      "label": true,
      "fill": "#3f3f3f"
    },
    "nodes": [
      "c3d2e1f0a9b87654",
      "e5f4d3c2b1a09876",
      "a1b2c3d4e5f67890",
      "9876543210fedcba",
      "bea869deba67b71a",
      "fe172ed00177f040"
    ],
    "x": 94,
    "y": 1859,
    "w": 692,
    "h": 262
  },
  {
    "id": "e0e84b770a360eef",
    "type": "group",
    "z": "1c1ccda937ce7294",
    "name": "Alle Geräte im Erdgeschoss",
    "style": {
      "label": true,
      "fill": "#3f3f3f"
    },
    "nodes": [
      "32c0bcb2baf02134",
      "a63b7adf0c1f5bf4",
      "53e972bd33725622",
      "f994ef2d90a09d4b",
      "3e3d563ffeb93911",
      "45e61afe5ee74697"
    ],
    "x": 834,
    "y": 1859,
    "w": 652,
    "h": 262
  },
  {
    "id": "e760fc5827336020",
    "type": "mqtt out",
    "z": "1c1ccda937ce7294",
    "g": "f14814695f431557",
    "name": "",
    "topic": "erdgeschoss/wc/heizung",
    "qos": "1",
    "retain": "",
    "respTopic": "",
    "contentType": "",
    "userProps": "",
    "correl": "",
    "expiry": "",
    "broker": "40d0272a08d772d1",
    "x": 670,
    "y": 660,
    "wires": []
  },
  {
    "id": "3b6a9b3f7c7cea15",
    "type": "mqtt in",
    "z": "1c1ccda937ce7294",
    "g": "f14814695f431557",
    "name": "",
    "topic": "erdgeschoss/wc/heizung",
    "qos": "1",
    "datatype": "auto-detect",
    "broker": "40d0272a08d772d1",
    "nl": false,
    "rap": true,
    "rh": 0,
    "inputs": 0,
    "x": 950,
    "y": 660,
    "wires": [["8098f4baf22bade8"]]
  },
  {
    "id": "8098f4baf22bade8",
    "type": "debug",
    "z": "1c1ccda937ce7294",
    "g": "f14814695f431557",
    "name": "Aktueller Zustand",
    "active": true,
    "tosidebar": true,
    "console": false,
    "tostatus": true,
    "complete": "payload",
    "targetType": "msg",
    "statusVal": "payload",
    "statusType": "auto",
    "x": 1170,
    "y": 660,
    "wires": []
  },
  {
    "id": "74a84814.4ecc6",
    "type": "inject",
    "z": "1c1ccda937ce7294",
    "g": "f14814695f431557",
    "name": "Heizung EG WG",
    "props": [
      {
        "p": "payload"
      },
      {
        "p": "topic",
        "vt": "str"
      }
    ],
    "repeat": "",
    "crontab": "",
    "once": false,
    "onceDelay": 0.1,
    "topic": "-",
    "payload": "-",
    "payloadType": "str",
    "x": 320,
    "y": 660,
    "wires": [["d9c64d70.401e88"]]
  },
  {
    "id": "d9c64d70.401e88",
    "type": "toggle",
    "z": "1c1ccda937ce7294",
    "g": "f14814695f431557",
    "name": "",
    "onOffTopic": "",
    "onValue": "1",
    "onType": "num",
    "offValue": "0",
    "offType": "num",
    "toggleTopic": "",
    "toggleValue": "",
    "toggleType": "any",
    "passOnOff": "ifChanged",
    "x": 490,
    "y": 660,
    "wires": [["e760fc5827336020"]]
  },
  {
    "id": "cb26d19cdf7154f5",
    "type": "mqtt in",
    "z": "1c1ccda937ce7294",
    "g": "8083fbfd5c2c3198",
    "name": "",
    "topic": "erdgeschoss/wc/heizung",
    "qos": "1",
    "datatype": "auto-detect",
    "broker": "40d0272a08d772d1",
    "nl": false,
    "rap": true,
    "rh": 0,
    "inputs": 0,
    "x": 270,
    "y": 1600,
    "wires": [["6de1325ee73ae4f4"]]
  },
  {
    "id": "6de1325ee73ae4f4",
    "type": "debug",
    "z": "1c1ccda937ce7294",
    "g": "8083fbfd5c2c3198",
    "name": "state",
    "active": true,
    "tosidebar": true,
    "console": false,
    "tostatus": true,
    "complete": "payload",
    "targetType": "msg",
    "statusVal": "payload",
    "statusType": "auto",
    "x": 470,
    "y": 1600,
    "wires": []
  },
  {
    "id": "679836f421d2815e",
    "type": "mqtt in",
    "z": "1c1ccda937ce7294",
    "g": "8083fbfd5c2c3198",
    "name": "",
    "topic": "erdgeschoss/wc/licht",
    "qos": "1",
    "datatype": "auto-detect",
    "broker": "40d0272a08d772d1",
    "nl": false,
    "rap": true,
    "rh": 0,
    "inputs": 0,
    "x": 260,
    "y": 1660,
    "wires": [["4d37d70d76310e50"]]
  },
  {
    "id": "4d37d70d76310e50",
    "type": "debug",
    "z": "1c1ccda937ce7294",
    "g": "8083fbfd5c2c3198",
    "name": "state",
    "active": true,
    "tosidebar": true,
    "console": false,
    "tostatus": true,
    "complete": "payload",
    "targetType": "msg",
    "statusVal": "payload",
    "statusType": "auto",
    "x": 470,
    "y": 1660,
    "wires": []
  },
  {
    "id": "66305ad1d86ead7e",
    "type": "mqtt in",
    "z": "1c1ccda937ce7294",
    "g": "92eac68a3a0a4e68",
    "name": "",
    "topic": "obergeschoss/buero/heizung",
    "qos": "1",
    "datatype": "auto-detect",
    "broker": "40d0272a08d772d1",
    "nl": false,
    "rap": true,
    "rh": 0,
    "inputs": 0,
    "x": 280,
    "y": 1360,
    "wires": [["aa1cd739b6cbbeba"]]
  },
  {
    "id": "aa1cd739b6cbbeba",
    "type": "debug",
    "z": "1c1ccda937ce7294",
    "g": "92eac68a3a0a4e68",
    "name": "state",
    "active": true,
    "tosidebar": true,
    "console": false,
    "tostatus": true,
    "complete": "payload",
    "targetType": "msg",
    "statusVal": "payload",
    "statusType": "auto",
    "x": 470,
    "y": 1360,
    "wires": []
  },
  {
    "id": "f7f9729f0b020753",
    "type": "mqtt in",
    "z": "1c1ccda937ce7294",
    "g": "92eac68a3a0a4e68",
    "name": "",
    "topic": "obergeschoss/buero/licht",
    "qos": "1",
    "datatype": "auto-detect",
    "broker": "40d0272a08d772d1",
    "nl": false,
    "rap": true,
    "rh": 0,
    "inputs": 0,
    "x": 270,
    "y": 1420,
    "wires": [["2a648c468fbba2c3"]]
  },
  {
    "id": "2a648c468fbba2c3",
    "type": "debug",
    "z": "1c1ccda937ce7294",
    "g": "92eac68a3a0a4e68",
    "name": "state",
    "active": true,
    "tosidebar": true,
    "console": false,
    "tostatus": true,
    "complete": "payload",
    "targetType": "msg",
    "statusVal": "payload",
    "statusType": "auto",
    "x": 470,
    "y": 1420,
    "wires": []
  },
  {
    "id": "94364a3ff0967d31",
    "type": "mqtt in",
    "z": "1c1ccda937ce7294",
    "g": "fcc5e797fb12b637",
    "name": "",
    "topic": "erdgeschoss/kueche/heizung",
    "qos": "1",
    "datatype": "auto-detect",
    "broker": "40d0272a08d772d1",
    "nl": false,
    "rap": true,
    "rh": 0,
    "inputs": 0,
    "x": 840,
    "y": 1600,
    "wires": [["f8f39087a9405d59"]]
  },
  {
    "id": "f8f39087a9405d59",
    "type": "debug",
    "z": "1c1ccda937ce7294",
    "g": "fcc5e797fb12b637",
    "name": "state",
    "active": true,
    "tosidebar": true,
    "console": false,
    "tostatus": true,
    "complete": "payload",
    "targetType": "msg",
    "statusVal": "payload",
    "statusType": "auto",
    "x": 1070,
    "y": 1600,
    "wires": []
  },
  {
    "id": "9dbbcb4b47b39881",
    "type": "mqtt in",
    "z": "1c1ccda937ce7294",
    "g": "fcc5e797fb12b637",
    "name": "",
    "topic": "erdgeschoss/kueche/licht",
    "qos": "1",
    "datatype": "auto-detect",
    "broker": "40d0272a08d772d1",
    "nl": false,
    "rap": true,
    "rh": 0,
    "inputs": 0,
    "x": 830,
    "y": 1660,
    "wires": [["e6730d5e60567f7b"]]
  },
  {
    "id": "e6730d5e60567f7b",
    "type": "debug",
    "z": "1c1ccda937ce7294",
    "g": "fcc5e797fb12b637",
    "name": "state",
    "active": true,
    "tosidebar": true,
    "console": false,
    "tostatus": true,
    "complete": "payload",
    "targetType": "msg",
    "statusVal": "payload",
    "statusType": "auto",
    "x": 1070,
    "y": 1660,
    "wires": []
  },
  {
    "id": "b20891d5ccd0a3bf",
    "type": "mqtt in",
    "z": "1c1ccda937ce7294",
    "g": "8845791c3273f8a4",
    "name": "",
    "topic": "obergeschoss/schlafzimmer/heizung",
    "qos": "1",
    "datatype": "auto-detect",
    "broker": "40d0272a08d772d1",
    "nl": false,
    "rap": true,
    "rh": 0,
    "inputs": 0,
    "x": 860,
    "y": 1360,
    "wires": [["2e0617c03d2c304c"]]
  },
  {
    "id": "2e0617c03d2c304c",
    "type": "debug",
    "z": "1c1ccda937ce7294",
    "g": "8845791c3273f8a4",
    "name": "state",
    "active": true,
    "tosidebar": true,
    "console": false,
    "tostatus": true,
    "complete": "payload",
    "targetType": "msg",
    "statusVal": "payload",
    "statusType": "auto",
    "x": 1070,
    "y": 1360,
    "wires": []
  },
  {
    "id": "ca4b343d8c21f419",
    "type": "mqtt in",
    "z": "1c1ccda937ce7294",
    "g": "8845791c3273f8a4",
    "name": "",
    "topic": "obergeschoss/schlafzimmer/licht",
    "qos": "1",
    "datatype": "auto-detect",
    "broker": "40d0272a08d772d1",
    "nl": false,
    "rap": true,
    "rh": 0,
    "inputs": 0,
    "x": 850,
    "y": 1420,
    "wires": [["baa5e24b1be570a4"]]
  },
  {
    "id": "baa5e24b1be570a4",
    "type": "debug",
    "z": "1c1ccda937ce7294",
    "g": "8845791c3273f8a4",
    "name": "state",
    "active": true,
    "tosidebar": true,
    "console": false,
    "tostatus": true,
    "complete": "payload",
    "targetType": "msg",
    "statusVal": "payload",
    "statusType": "auto",
    "x": 1070,
    "y": 1420,
    "wires": []
  },
  {
    "id": "9706f9fce318f43f",
    "type": "mqtt out",
    "z": "1c1ccda937ce7294",
    "g": "f8db239ea2210a9b",
    "name": "",
    "topic": "erdgeschoss/wc/licht",
    "qos": "0",
    "retain": "",
    "respTopic": "",
    "contentType": "",
    "userProps": "",
    "correl": "",
    "expiry": "",
    "broker": "40d0272a08d772d1",
    "x": 620,
    "y": 980,
    "wires": []
  },
  {
    "id": "bed761109a90c47a",
    "type": "inject",
    "z": "1c1ccda937ce7294",
    "g": "f8db239ea2210a9b",
    "name": "Licht im WC",
    "props": [
      {
        "p": "payload"
      },
      {
        "p": "topic",
        "vt": "str"
      }
    ],
    "repeat": "",
    "crontab": "",
    "once": false,
    "onceDelay": 0.1,
    "topic": "-",
    "payload": "-",
    "payloadType": "str",
    "x": 310,
    "y": 980,
    "wires": [["a3b49ee52f817a53"]]
  },
  {
    "id": "a3b49ee52f817a53",
    "type": "toggle",
    "z": "1c1ccda937ce7294",
    "g": "f8db239ea2210a9b",
    "name": "",
    "onOffTopic": "",
    "onValue": "1",
    "onType": "num",
    "offValue": "0",
    "offType": "num",
    "toggleTopic": "",
    "toggleValue": "",
    "toggleType": "any",
    "passOnOff": "ifChanged",
    "x": 450,
    "y": 980,
    "wires": [["9706f9fce318f43f"]]
  },
  {
    "id": "dd91693eacb3a754",
    "type": "mqtt in",
    "z": "1c1ccda937ce7294",
    "g": "f8db239ea2210a9b",
    "name": "",
    "topic": "erdgeschoss/wc/sensor",
    "qos": "1",
    "datatype": "auto-detect",
    "broker": "40d0272a08d772d1",
    "nl": false,
    "rap": true,
    "rh": 0,
    "inputs": 0,
    "x": 320,
    "y": 1040,
    "wires": [["2f53e4edd910e895"]]
  },
  {
    "id": "2f53e4edd910e895",
    "type": "debug",
    "z": "1c1ccda937ce7294",
    "g": "f8db239ea2210a9b",
    "name": "sensor status",
    "active": true,
    "tosidebar": false,
    "console": false,
    "tostatus": true,
    "complete": "payload",
    "targetType": "msg",
    "statusVal": "payload",
    "statusType": "auto",
    "x": 520,
    "y": 1040,
    "wires": []
  },
  {
    "id": "c3d2e1f0a9b87654",
    "type": "mqtt out",
    "z": "1c1ccda937ce7294",
    "g": "6566a3379a01c108",
    "name": "EG WC Licht",
    "topic": "erdgeschoss/wc/licht",
    "qos": "0",
    "retain": "",
    "respTopic": "",
    "contentType": "",
    "userProps": "",
    "correl": "",
    "expiry": "",
    "broker": "40d0272a08d772d1",
    "x": 610,
    "y": 1900,
    "wires": []
  },
  {
    "id": "e5f4d3c2b1a09876",
    "type": "mqtt out",
    "z": "1c1ccda937ce7294",
    "g": "6566a3379a01c108",
    "name": "EG Küche Licht",
    "topic": "erdgeschoss/kueche/licht",
    "qos": "0",
    "retain": "",
    "respTopic": "",
    "contentType": "",
    "userProps": "",
    "correl": "",
    "expiry": "",
    "broker": "40d0272a08d772d1",
    "x": 620,
    "y": 1960,
    "wires": []
  },
  {
    "id": "a1b2c3d4e5f67890",
    "type": "mqtt out",
    "z": "1c1ccda937ce7294",
    "g": "6566a3379a01c108",
    "name": "OG Schlafzimmer Licht",
    "topic": "obergeschoss/schlafzimmer/licht",
    "qos": "0",
    "retain": "",
    "respTopic": "",
    "contentType": "",
    "userProps": "",
    "correl": "",
    "expiry": "",
    "broker": "40d0272a08d772d1",
    "x": 650,
    "y": 2020,
    "wires": []
  },
  {
    "id": "9876543210fedcba",
    "type": "mqtt out",
    "z": "1c1ccda937ce7294",
    "g": "6566a3379a01c108",
    "name": "OG Büro Licht",
    "topic": "obergeschoss/buero/licht",
    "qos": "0",
    "retain": "",
    "respTopic": "",
    "contentType": "",
    "userProps": "",
    "correl": "",
    "expiry": "",
    "broker": "40d0272a08d772d1",
    "x": 620,
    "y": 2080,
    "wires": []
  },
  {
    "id": "bea869deba67b71a",
    "type": "inject",
    "z": "1c1ccda937ce7294",
    "g": "6566a3379a01c108",
    "name": "Alle Lichter",
    "props": [
      {
        "p": "payload"
      },
      {
        "p": "topic",
        "vt": "str"
      }
    ],
    "repeat": "",
    "crontab": "",
    "once": false,
    "onceDelay": 0.1,
    "topic": "-",
    "payload": "-",
    "payloadType": "str",
    "x": 200,
    "y": 1960,
    "wires": [["fe172ed00177f040"]]
  },
  {
    "id": "fe172ed00177f040",
    "type": "toggle",
    "z": "1c1ccda937ce7294",
    "g": "6566a3379a01c108",
    "name": "",
    "onOffTopic": "",
    "onValue": "1",
    "onType": "num",
    "offValue": "0",
    "offType": "num",
    "toggleTopic": "",
    "toggleValue": "",
    "toggleType": "any",
    "passOnOff": "ifChanged",
    "x": 390,
    "y": 1960,
    "wires": [
      [
        "c3d2e1f0a9b87654",
        "e5f4d3c2b1a09876",
        "a1b2c3d4e5f67890",
        "9876543210fedcba"
      ]
    ]
  },
  {
    "id": "32c0bcb2baf02134",
    "type": "mqtt out",
    "z": "1c1ccda937ce7294",
    "g": "e0e84b770a360eef",
    "name": "EG WC Heizung",
    "topic": "erdgeschoss/wc/heizung",
    "qos": "0",
    "retain": "",
    "respTopic": "",
    "contentType": "",
    "userProps": "",
    "correl": "",
    "expiry": "",
    "broker": "40d0272a08d772d1",
    "x": 1360,
    "y": 1900,
    "wires": []
  },
  {
    "id": "a63b7adf0c1f5bf4",
    "type": "mqtt out",
    "z": "1c1ccda937ce7294",
    "g": "e0e84b770a360eef",
    "name": "EG WC Licht",
    "topic": "erdgeschoss/wc/licht",
    "qos": "0",
    "retain": "",
    "respTopic": "",
    "contentType": "",
    "userProps": "",
    "correl": "",
    "expiry": "",
    "broker": "40d0272a08d772d1",
    "x": 1350,
    "y": 1960,
    "wires": []
  },
  {
    "id": "53e972bd33725622",
    "type": "mqtt out",
    "z": "1c1ccda937ce7294",
    "g": "e0e84b770a360eef",
    "name": "EG Küche Heizung",
    "topic": "erdgeschoss/kueche/heizung",
    "qos": "0",
    "retain": "",
    "respTopic": "",
    "contentType": "",
    "userProps": "",
    "correl": "",
    "expiry": "",
    "broker": "40d0272a08d772d1",
    "x": 1370,
    "y": 2020,
    "wires": []
  },
  {
    "id": "f994ef2d90a09d4b",
    "type": "mqtt out",
    "z": "1c1ccda937ce7294",
    "g": "e0e84b770a360eef",
    "name": "EG Küche Licht",
    "topic": "erdgeschoss/kueche/licht",
    "qos": "0",
    "retain": "",
    "respTopic": "",
    "contentType": "",
    "userProps": "",
    "correl": "",
    "expiry": "",
    "broker": "40d0272a08d772d1",
    "x": 1360,
    "y": 2080,
    "wires": []
  },
  {
    "id": "3e3d563ffeb93911",
    "type": "inject",
    "z": "1c1ccda937ce7294",
    "g": "e0e84b770a360eef",
    "name": "Alle Geräte im EG",
    "props": [
      {
        "p": "payload"
      },
      {
        "p": "topic",
        "vt": "str"
      }
    ],
    "repeat": "",
    "crontab": "",
    "once": false,
    "onceDelay": 0.1,
    "topic": "-",
    "payload": "-",
    "payloadType": "str",
    "x": 970,
    "y": 1960,
    "wires": [["45e61afe5ee74697"]]
  },
  {
    "id": "45e61afe5ee74697",
    "type": "toggle",
    "z": "1c1ccda937ce7294",
    "g": "e0e84b770a360eef",
    "name": "",
    "onOffTopic": "",
    "onValue": "1",
    "onType": "num",
    "offValue": "0",
    "offType": "num",
    "toggleTopic": "",
    "toggleValue": "",
    "toggleType": "any",
    "passOnOff": "ifChanged",
    "x": 1130,
    "y": 1960,
    "wires": [
      [
        "32c0bcb2baf02134",
        "a63b7adf0c1f5bf4",
        "53e972bd33725622",
        "f994ef2d90a09d4b"
      ]
    ]
  },
  {
    "id": "40d0272a08d772d1",
    "type": "mqtt-broker",
    "name": "",
    "broker": "broker.emqx.io",
    "port": 1883,
    "clientid": "",
    "autoConnect": true,
    "usetls": false,
    "protocolVersion": 4,
    "keepalive": 60,
    "cleansession": true,
    "autoUnsubscribe": true,
    "birthTopic": "",
    "birthQos": "0",
    "birthRetain": "false",
    "birthPayload": "",
    "birthMsg": {},
    "closeTopic": "",
    "closeQos": "0",
    "closeRetain": "false",
    "closePayload": "",
    "closeMsg": {},
    "willTopic": "",
    "willQos": "0",
    "willRetain": "false",
    "willPayload": "",
    "willMsg": {},
    "userProps": "",
    "sessionExpiry": ""
  },
  {
    "id": "12c8d35a7147a008",
    "type": "global-config",
    "env": [],
    "modules": {
      "node-red-contrib-toggle": "0.1.1"
    }
  }
]
```

</details></b>

### Beispiele

- [Beispiel Node-Red <-> TouchDesigner via MQTT](MQTT_TouchDesigner_Node-Red)
- [Beispiel Node-Red <-> ESP via MQTT](MQTT_ESP_Node-Red)

## 06: ESP32-Cam

Wir bauen ein verteiltes System mit einem ESP32 und Raspberry Pi.
Die Kamera am ESP32 liefert Bilder. Das Bild kann am Raspberry Pi weiterverarbeitet werden.

<details>
<summary>Node-RED Flow anzeigen (JSON)</summary>

```json
[
  {
    "id": "3cc6ed3b997b8926",
    "type": "tab",
    "label": "ESP32 Cam",
    "disabled": false,
    "info": "",
    "env": []
  },
  {
    "id": "23b03036d349fa2c",
    "type": "inject",
    "z": "3cc6ed3b997b8926",
    "name": "setFlash",
    "props": [{ "p": "payload" }, { "p": "topic", "vt": "str" }],
    "repeat": "",
    "crontab": "",
    "once": false,
    "onceDelay": 0.1,
    "topic": "",
    "payload": "",
    "payloadType": "date",
    "x": 200,
    "y": 200,
    "wires": [["e60a5ffb02f1fc45"]]
  },
  {
    "id": "4778bc78796355bd",
    "type": "inject",
    "z": "3cc6ed3b997b8926",
    "name": "takePicture",
    "props": [{ "p": "payload" }, { "p": "topic", "vt": "str" }],
    "repeat": "",
    "crontab": "",
    "once": false,
    "onceDelay": 0.1,
    "topic": "",
    "payload": "",
    "payloadType": "date",
    "x": 210,
    "y": 260,
    "wires": [["5d0fd4c5c6351fbe"]]
  },
  {
    "id": "e60a5ffb02f1fc45",
    "type": "mqtt out",
    "z": "3cc6ed3b997b8926",
    "name": "",
    "topic": "setFlash",
    "qos": "0",
    "retain": "false",
    "respTopic": "",
    "contentType": "",
    "userProps": "",
    "correl": "",
    "expiry": "",
    "broker": "6fc7cd40ef96dc87",
    "x": 440,
    "y": 200,
    "wires": []
  },
  {
    "id": "5d0fd4c5c6351fbe",
    "type": "mqtt out",
    "z": "3cc6ed3b997b8926",
    "name": "",
    "topic": "takePicture",
    "qos": "0",
    "retain": "false",
    "respTopic": "",
    "contentType": "",
    "userProps": "",
    "correl": "",
    "expiry": "",
    "broker": "6fc7cd40ef96dc87",
    "x": 450,
    "y": 260,
    "wires": []
  },
  {
    "id": "ddd09558ab253f2e",
    "type": "mqtt in",
    "z": "3cc6ed3b997b8926",
    "name": "",
    "topic": "sendPicture",
    "qos": "0",
    "datatype": "auto-detect",
    "broker": "6fc7cd40ef96dc87",
    "nl": false,
    "rap": true,
    "rh": 0,
    "inputs": 0,
    "x": 190,
    "y": 420,
    "wires": [["68f0e14ec5069cad", "fbce49931b5c08b8"]]
  },
  {
    "id": "68f0e14ec5069cad",
    "type": "debug",
    "z": "3cc6ed3b997b8926",
    "name": "debug 2",
    "active": true,
    "tosidebar": true,
    "console": false,
    "tostatus": false,
    "complete": "false",
    "statusVal": "",
    "statusType": "auto",
    "x": 460,
    "y": 420,
    "wires": []
  },
  {
    "id": "fbce49931b5c08b8",
    "type": "image",
    "z": "3cc6ed3b997b8926",
    "name": "",
    "width": 160,
    "data": "payload",
    "dataType": "msg",
    "thumbnail": false,
    "active": true,
    "pass": false,
    "outputs": 0,
    "x": 480,
    "y": 480,
    "wires": []
  },
  {
    "id": "9d2c7326244b4eed",
    "type": "inject",
    "z": "3cc6ed3b997b8926",
    "name": "",
    "props": [{ "p": "payload" }, { "p": "topic", "vt": "str" }],
    "repeat": "",
    "crontab": "",
    "once": false,
    "onceDelay": 0.1,
    "topic": "",
    "payload": "data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAALQAAACyCAIAAABk9nJ4AAAACXBIWXMAAAsTAAALEwEAmpwYAAAGr2lUWHRYTUw6Y29tLmFkb2JlLnhtcAAAAAAAPD94cGFja2V0IGJlZ2luPSLvu78iIGlkPSJXNU0wTXBDZWhpSHpyZVN6TlRjemtjOWQiPz4gPHg6eG1wbWV0YSB4bWxuczp4PSJhZG9iZTpuczptZXRhLyIgeDp4bXB0az0iQWRvYmUgWE1QIENvcmUgOS4xLWMwMDMgNzkuOTY5MGE4N2ZjLCAyMDI1LzAzLzA2LTIwOjUwOjE2ICAgICAgICAiPiA8cmRmOlJERiB4bWxuczpyZGY9Imh0dHA6Ly93d3cudzMub3JnLzE5OTkvMDIvMjItcmRmLXN5bnRheC1ucyMiPiA8cmRmOkRlc2NyaXB0aW9uIHJkZjphYm91dD0iIiB4bWxuczpJcHRjNHhtcEV4dD0iaHR0cDovL2lwdGMub3JnL3N0ZC9JcHRjNHhtcEV4dC8yMDA4LTAyLTI5LyIgeG1sbnM6cGhvdG9zaG9wPSJodHRwOi8vbnMuYWRvYmUuY29tL3Bob3Rvc2hvcC8xLjAvIiB4bWxuczpleGlmPSJodHRwOi8vbnMuYWRvYmUuY29tL2V4aWYvMS4wLyIgeG1sbnM6eG1wPSJodHRwOi8vbnMuYWRvYmUuY29tL3hhcC8xLjAvIiB4bWxuczpkYz0iaHR0cDovL3B1cmwub3JnL2RjL2VsZW1lbnRzLzEuMS8iIHhtbG5zOnhtcE1NPSJodHRwOi8vbnMuYWRvYmUuY29tL3hhcC8xLjAvbW0vIiB4bWxuczpzdEV2dD0iaHR0cDovL25zLmFkb2JlLmNvbS94YXAvMS4wL3NUeXBlL1Jlc291cmNlRXZlbnQjIiBJcHRjNHhtcEV4dDpEaWdpdGFsU291cmNlRmlsZVR5cGU9Imh0dHA6Ly9jdi5pcHRjLm9yZy9uZXdzY29kZXMvZGlnaXRhbHNvdXJjZXR5cGUvY29tcG9zaXRlV2l0aFRyYWluZWRBbGdvcml0aG1pY01lZGlhIiBJcHRjNHhtcEV4dDpEaWdpdGFsU291cmNlVHlwZT0iaHR0cDovL2N2LmlwdGMub3JnL25ld3Njb2Rlcy9kaWdpdGFsc291cmNldHlwZS9jb21wb3NpdGVXaXRoVHJhaW5lZEFsZ29yaXRobWljTWVkaWEiIHBob3Rvc2hvcDpDcmVkaXQ9IkVkaXRlZCB3aXRoIEdvb2dsZSBBSSIgcGhvdG9zaG9wOkRhdGVDcmVhdGVkPSIyMDI1LTA5LTE5VDIxOjIyOjA0IiBwaG90b3Nob3A6Q29sb3JNb2RlPSIzIiBleGlmOkRhdGVUaW1lT3JpZ2luYWw9IjIwMjU6MDk6MTkgMjE6MjI6MDQrMDA6MDAiIHhtcDpDcmVhdGVEYXRlPSIyMDI1LTA5LTE5VDIzOjIzOjA1KzAyOjAwIiB4bXA6TW9kaWZ5RGF0ZT0iMjAyNS0wOS0xOVQyMzoyNTo0NiswMjowMCIgeG1wOk1ldGFkYXRhRGF0ZT0iMjAyNS0wOS0xOVQyMzoyNTo0NiswMjowMCIgZGM6Zm9ybWF0PSJpbWFnZS9wbmciIHhtcE1NOkluc3RhbmNlSUQ9InhtcC5paWQ6ZmE1NWZjZjEtZGNjMi00ZTA5LTg0ODAtOTkzNzE5OWJkNDIyIiB4bXBNTTpEb2N1bWVudElEPSJ4bXAuZGlkOmZhNTVmY2YxLWRjYzItNGUwOS04NDgwLTk5MzcxOTliZDQyMiIgeG1wTU06T3JpZ2luYWxEb2N1bWVudElEPSJ4bXAuZGlkOmZhNTVmY2YxLWRjYzItNGUwOS04NDgwLTk5MzcxOTliZDQyMiI+IDx4bXBNTTpIaXN0b3J5PiA8cmRmOlNlcT4gPHJkZjpsaSBzdEV2dDphY3Rpb249InNhdmVkIiBzdEV2dDppbnN0YW5jZUlEPSJ4bXAuaWlkOmZhNTVmY2YxLWRjYzItNGUwOS04NDgwLTk5MzcxOTliZDQyMiIgc3RFdnQ6d2hlbj0iMjAyNS0wOS0xOVQyMzoyNTo0NiswMjowMCIgc3RFdnQ6c29mdHdhcmVBZ2VudD0iQWRvYmUgUGhvdG9zaG9wIDI2LjggKE1hY2ludG9zaCkiIHN0RXZ0OmNoYW5nZWQ9Ii8iLz4gPC9yZGY6U2VxPiA8L3htcE1NOkhpc3Rvcnk+IDwvcmRmOkRlc2NyaXB0aW9uPiA8L3JkZjpSREY+IDwveDp4bXBtZXRhPiA8P3hwYWNrZXQgZW5kPSJyIj8+5XszUAAAeI9JREFUeJyN/Ve0ddlxHoZ+VXOt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",
    "payloadType": "str",
    "x": 190,
    "y": 520,
    "wires": [["fbce49931b5c08b8"]]
  },
  {
    "id": "6fc7cd40ef96dc87",
    "type": "mqtt-broker",
    "name": "",
    "broker": "mosquitto-server",
    "port": 1883,
    "clientid": "",
    "autoConnect": true,
    "usetls": false,
    "protocolVersion": 4,
    "keepalive": 60,
    "cleansession": true,
    "autoUnsubscribe": true,
    "birthTopic": "",
    "birthQos": "0",
    "birthRetain": "false",
    "birthPayload": "",
    "birthMsg": {},
    "closeTopic": "",
    "closeQos": "0",
    "closeRetain": "false",
    "closePayload": "",
    "closeMsg": {},
    "willTopic": "",
    "willQos": "0",
    "willRetain": "false",
    "willPayload": "",
    "willMsg": {},
    "userProps": "",
    "sessionExpiry": ""
  },
  {
    "id": "2029abcd2f5d17de",
    "type": "global-config",
    "env": [],
    "modules": { "node-red-contrib-image-output": "0.6.4" }
  }
]
```

</details>

## 07: Bildauswertung mit Deepstack (Test)

ESP32 liefert Bilder, der Raspberry Pi wertet sie aus. Der ESP32 selbst ist zu schwach für Bilderkennung. Deshalb wird das Bild in Textform an den Raspberry Pi geschickt, der ein Bilderkennungsmodell betreibt. Node-Red dient als Kommunikationsschnittstelle.

<details>
<summary>Node-RED Flow anzeigen (JSON)</summary>

```json
[
  {
    "id": "f6f2187d.f17ca8",
    "type": "tab",
    "label": "07_Deepstack Test",
    "disabled": false,
    "info": ""
  },
  {
    "id": "d1e2f3a4.5b6c7d",
    "type": "inject",
    "z": "f6f2187d.f17ca8",
    "name": "Start Test",
    "props": [{ "p": "payload" }],
    "repeat": "",
    "crontab": "",
    "once": false,
    "onceDelay": 0.1,
    "topic": "",
    "x": 140,
    "y": 120,
    "wires": [["a1b2c3d4.e5f6a7"]]
  },
  {
    "id": "a1b2c3d4.e5f6a7",
    "type": "http request",
    "z": "f6f2187d.f17ca8",
    "name": "Lade Testbild (Hund)",
    "method": "GET",
    "ret": "bin",
    "paytoqs": "ignore",
    "url": "https://cdn.royalcanin-weshare-online.io/mOgVlYgBaPOZra8q1ABE/v3/pubertat-beim-hund-2?w=1440&fm=jpg&auto=format%2Ccompress",
    "tls": "",
    "persist": false,
    "proxy": "",
    "insecureHTTPParser": false,
    "authType": "",
    "senderr": false,
    "headers": [],
    "x": 340,
    "y": 120,
    "wires": [["7fdc7e58508613fe", "56cd478a5bfff0e2"]]
  },
  {
    "id": "7fdc7e58508613fe",
    "type": "image",
    "z": "f6f2187d.f17ca8",
    "name": "",
    "width": 160,
    "data": "payload",
    "dataType": "msg",
    "thumbnail": false,
    "active": true,
    "pass": false,
    "outputs": 0,
    "x": 630,
    "y": 340,
    "wires": []
  },
  {
    "id": "56cd478a5bfff0e2",
    "type": "deepstack-object-detection",
    "z": "f6f2187d.f17ca8",
    "name": "",
    "server": "e2e35f99e3052fdb",
    "confidence": "40",
    "drawPredictions": false,
    "printLabel": false,
    "outlineColor": "pink",
    "custom": "",
    "filters": [],
    "outputs": 1,
    "x": 610,
    "y": 120,
    "wires": [["5f43da572e62451d"]]
  },
  {
    "id": "5f43da572e62451d",
    "type": "debug",
    "z": "f6f2187d.f17ca8",
    "name": "debug 4",
    "active": true,
    "tosidebar": true,
    "console": false,
    "tostatus": false,
    "complete": "false",
    "statusVal": "",
    "statusType": "auto",
    "x": 870,
    "y": 120,
    "wires": []
  },
  {
    "id": "e2e35f99e3052fdb",
    "type": "deepstack-server",
    "proto": "http",
    "host": "jan5",
    "port": "85",
    "version": "v1",
    "rejectUnauthorized": false
  },
  {
    "id": "b4508116881a2f20",
    "type": "global-config",
    "env": [],
    "modules": {
      "node-red-contrib-image-output": "0.6.4",
      "node-red-contrib-deepstack": "0.3.0"
    }
  }
]
```

</details>

## 08: Bildauswertung mit Deepstack

Ausgereiftere Version des vorherigen Beispiels

<details>
<summary>Node-RED Flow anzeigen (JSON)</summary>

```json
[
  {
    "id": "7672c2f0dd60ead7",
    "type": "tab",
    "label": "08_ESP32 Cam + Deepstack",
    "disabled": false,
    "info": "",
    "env": []
  },
  {
    "id": "99533bda139feb54",
    "type": "group",
    "z": "7672c2f0dd60ead7",
    "name": "Trigger ESP Cam",
    "style": { "label": true, "color": "#ffff00", "fill": "#ffff00" },
    "nodes": [
      "a78fa74af7db72fa",
      "fa76cacbefc5c9a3",
      "e99a655ba3a8208b",
      "7c5667c21fde1f33"
    ],
    "x": 454,
    "y": 139,
    "w": 452,
    "h": 142
  },
  {
    "id": "bafab6c127fbe920",
    "type": "group",
    "z": "7672c2f0dd60ead7",
    "name": "Receive Picture and detect",
    "style": { "fill": "#ffff00", "label": true, "color": "#ffff00" },
    "nodes": [
      "a664814a27a204a3",
      "84fe4402dfcd648d",
      "80707f88ea12fa6d",
      "d8d8ef12b5a07204",
      "3ce712864110bff3",
      "4f840db5362eee36",
      "94c9ddeeb0b403cd",
      "b62e7e891db0fff5",
      "4acdce29aa74167a",
      "52e76609449b3c68",
      "80606c022ad27845"
    ],
    "x": 454,
    "y": 359,
    "w": 812,
    "h": 442
  },
  {
    "id": "7048ae2158b68615",
    "type": "group",
    "z": "7672c2f0dd60ead7",
    "name": "Test Receive (without ESP32 Cam)",
    "style": { "label": true },
    "nodes": ["801a6059c39b5e13", "06d0f17e61c53720"],
    "x": 94,
    "y": 339,
    "w": 332,
    "h": 82
  },
  {
    "id": "a78fa74af7db72fa",
    "type": "inject",
    "z": "7672c2f0dd60ead7",
    "g": "99533bda139feb54",
    "name": "setFlash",
    "props": [{ "p": "payload" }, { "p": "topic", "vt": "str" }],
    "repeat": "",
    "crontab": "",
    "once": false,
    "onceDelay": 0.1,
    "topic": "",
    "payload": "",
    "payloadType": "date",
    "x": 560,
    "y": 180,
    "wires": [["e99a655ba3a8208b"]]
  },
  {
    "id": "fa76cacbefc5c9a3",
    "type": "inject",
    "z": "7672c2f0dd60ead7",
    "g": "99533bda139feb54",
    "name": "takePicture",
    "props": [{ "p": "payload" }, { "p": "topic", "vt": "str" }],
    "repeat": "",
    "crontab": "",
    "once": false,
    "onceDelay": 0.1,
    "topic": "",
    "payload": "",
    "payloadType": "date",
    "x": 570,
    "y": 240,
    "wires": [["7c5667c21fde1f33"]]
  },
  {
    "id": "e99a655ba3a8208b",
    "type": "mqtt out",
    "z": "7672c2f0dd60ead7",
    "g": "99533bda139feb54",
    "name": "",
    "topic": "setFlash",
    "qos": "0",
    "retain": "false",
    "respTopic": "",
    "contentType": "",
    "userProps": "",
    "correl": "",
    "expiry": "",
    "broker": "6fc7cd40ef96dc87",
    "x": 800,
    "y": 180,
    "wires": []
  },
  {
    "id": "7c5667c21fde1f33",
    "type": "mqtt out",
    "z": "7672c2f0dd60ead7",
    "g": "99533bda139feb54",
    "name": "",
    "topic": "takePicture",
    "qos": "0",
    "retain": "false",
    "respTopic": "",
    "contentType": "",
    "userProps": "",
    "correl": "",
    "expiry": "",
    "broker": "6fc7cd40ef96dc87",
    "x": 810,
    "y": 240,
    "wires": []
  },
  {
    "id": "a664814a27a204a3",
    "type": "mqtt in",
    "z": "7672c2f0dd60ead7",
    "g": "bafab6c127fbe920",
    "name": "",
    "topic": "sendPicture",
    "qos": "0",
    "datatype": "auto-detect",
    "broker": "6fc7cd40ef96dc87",
    "nl": false,
    "rap": true,
    "rh": 0,
    "inputs": 0,
    "x": 550,
    "y": 400,
    "wires": [["84fe4402dfcd648d", "d8d8ef12b5a07204", "3ce712864110bff3"]]
  },
  {
    "id": "84fe4402dfcd648d",
    "type": "debug",
    "z": "7672c2f0dd60ead7",
    "g": "bafab6c127fbe920",
    "name": "debug 3",
    "active": false,
    "tosidebar": true,
    "console": false,
    "tostatus": false,
    "complete": "false",
    "statusVal": "",
    "statusType": "auto",
    "x": 820,
    "y": 400,
    "wires": []
  },
  {
    "id": "80707f88ea12fa6d",
    "type": "inject",
    "z": "7672c2f0dd60ead7",
    "g": "bafab6c127fbe920",
    "name": "",
    "props": [{ "p": "payload" }, { "p": "topic", "vt": "str" }],
    "repeat": "",
    "crontab": "",
    "once": false,
    "onceDelay": 0.1,
    "topic": "",
    "payload": "data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAALQAAACyCAIAAABk9nJ4AAAACXBIWXMAAAsTAAALEwEAmpwYAAAGr2lUWHRYTUw6Y29tLmFkb2JlLnhtcAAAAAAAPD94cGFja2V0IGJlZ2luPSLvu78iIGlkPSJXNU0wTXBDZWhpSHpyZVN6TlRjemtjOWQiPz4gPHg6eG1wbWV0YSB4bWxuczp4PSJhZG9iZTpuczptZXRhLyIgeDp4bXB0az0iQWRvYmUgWE1QIENvcmUgOS4xLWMwMDMgNzkuOTY5MGE4N2ZjLCAyMDI1LzAzLzA2LTIwOjUwOjE2ICAgICAgICAiPiA8cmRmOlJERiB4bWxuczpyZGY9Imh0dHA6Ly93d3cudzMub3JnLzE5OTkvMDIvMjItcmRmLXN5bnRheC1ucyMiPiA8cmRmOkRlc2NyaXB0aW9uIHJkZjphYm91dD0iIiB4bWxuczpJcHRjNHhtcEV4dD0iaHR0cDovL2lwdGMub3JnL3N0ZC9JcHRjNHhtcEV4dC8yMDA4LTAyLTI5LyIgeG1sbnM6cGhvdG9zaG9wPSJodHRwOi8vbnMuYWRvYmUuY29tL3Bob3Rvc2hvcC8xLjAvIiB4bWxuczpleGlmPSJodHRwOi8vbnMuYWRvYmUuY29tL2V4aWYvMS4wLyIgeG1sbnM6eG1wPSJodHRwOi8vbnMuYWRvYmUuY29tL3hhcC8xLjAvIiB4bWxuczpkYz0iaHR0cDovL3B1cmwub3JnL2RjL2VsZW1lbnRzLzEuMS8iIHhtbG5zOnhtcE1NPSJodHRwOi8vbnMuYWRvYmUuY29tL3hhcC8xLjAvbW0vIiB4bWxuczpzdEV2dD0iaHR0cDovL25zLmFkb2JlLmNvbS94YXAvMS4wL3NUeXBlL1Jlc291cmNlRXZlbnQjIiBJcHRjNHhtcEV4dDpEaWdpdGFsU291cmNlRmlsZVR5cGU9Imh0dHA6Ly9jdi5pcHRjLm9yZy9uZXdzY29kZXMvZGlnaXRhbHNvdXJjZXR5cGUvY29tcG9zaXRlV2l0aFRyYWluZWRBbGdvcml0aG1pY01lZGlhIiBJcHRjNHhtcEV4dDpEaWdpdGFsU291cmNlVHlwZT0iaHR0cDovL2N2LmlwdGMub3JnL25ld3Njb2Rlcy9kaWdpdGFsc291cmNldHlwZS9jb21wb3NpdGVXaXRoVHJhaW5lZEFsZ29yaXRobWljTWVkaWEiIHBob3Rvc2hvcDpDcmVkaXQ9IkVkaXRlZCB3aXRoIEdvb2dsZSBBSSIgcGhvdG9zaG9wOkRhdGVDcmVhdGVkPSIyMDI1LTA5LTE5VDIxOjIyOjA0IiBwaG90b3Nob3A6Q29sb3JNb2RlPSIzIiBleGlmOkRhdGVUaW1lT3JpZ2luYWw9IjIwMjU6MDk6MTkgMjE6MjI6MDQrMDA6MDAiIHhtcDpDcmVhdGVEYXRlPSIyMDI1LTA5LTE5VDIzOjIzOjA1KzAyOjAwIiB4bXA6TW9kaWZ5RGF0ZT0iMjAyNS0wOS0xOVQyMzoyNTo0NiswMjowMCIgeG1wOk1ldGFkYXRhRGF0ZT0iMjAyNS0wOS0xOVQyMzoyNTo0NiswMjowMCIgZGM6Zm9ybWF0PSJpbWFnZS9wbmciIHhtcE1NOkluc3RhbmNlSUQ9InhtcC5paWQ6ZmE1NWZjZjEtZGNjMi00ZTA5LTg0ODAtOTkzNzE5OWJkNDIyIiB4bXBNTTpEb2N1bWVudElEPSJ4bXAuZGlkOmZhNTVmY2YxLWRjYzItNGUwOS04NDgwLTk5MzcxOTliZDQyMiIgeG1wTU06T3JpZ2luYWxEb2N1bWVudElEPSJ4bXAuZGlkOmZhNTVmY2YxLWRjYzItNGUwOS04NDgwLTk5MzcxOTliZDQyMiI+IDx4bXBNTTpIaXN0b3J5PiA8cmRmOlNlcT4gPHJkZjpsaSBzdEV2dDphY3Rpb249InNhdmVkIiBzdEV2dDppbnN0YW5jZUlEPSJ4bXAuaWlkOmZhNTVmY2YxLWRjYzItNGUwOS04NDgwLTk5MzcxOTliZDQyMiIgc3RFdnQ6d2hlbj0iMjAyNS0wOS0xOVQyMzoyNTo0NiswMjowMCIgc3RFdnQ6c29mdHdhcmVBZ2VudD0iQWRvYmUgUGhvdG9zaG9wIDI2LjggKE1hY2ludG9zaCkiIHN0RXZ0OmNoYW5nZWQ9Ii8iLz4gPC9yZGY6U2VxPiA8L3htcE1NOkhpc3Rvcnk+IDwvcmRmOkRlc2NyaXB0aW9uPiA8L3JkZjpSREY+IDwveDp4bXBtZXRhPiA8P3hwYWNrZXQgZW5kPSJyIj8+5XszUAAAeI9JREFUeJyN/Ve0ddlxHoZ+VXOt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",
    "payloadType": "str",
    "x": 550,
    "y": 500,
    "wires": [["d8d8ef12b5a07204"]]
  },
  {
    "id": "d8d8ef12b5a07204",
    "type": "image",
    "z": "7672c2f0dd60ead7",
    "g": "bafab6c127fbe920",
    "name": "",
    "width": 160,
    "data": "payload",
    "dataType": "msg",
    "thumbnail": false,
    "active": true,
    "pass": false,
    "outputs": 0,
    "x": 840,
    "y": 460,
    "wires": []
  },
  {
    "id": "801a6059c39b5e13",
    "type": "inject",
    "z": "7672c2f0dd60ead7",
    "g": "7048ae2158b68615",
    "name": "",
    "props": [{ "p": "payload" }, { "p": "topic", "vt": "str" }],
    "repeat": "",
    "crontab": "",
    "once": false,
    "onceDelay": 0.1,
    "topic": "",
    "payload": "data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAALQAAACyCAIAAABk9nJ4AAAACXBIWXMAAAsTAAALEwEAmpwYAAAGr2lUWHRYTUw6Y29tLmFkb2JlLnhtcAAAAAAAPD94cGFja2V0IGJlZ2luPSLvu78iIGlkPSJXNU0wTXBDZWhpSHpyZVN6TlRjemtjOWQiPz4gPHg6eG1wbWV0YSB4bWxuczp4PSJhZG9iZTpuczptZXRhLyIgeDp4bXB0az0iQWRvYmUgWE1QIENvcmUgOS4xLWMwMDMgNzkuOTY5MGE4N2ZjLCAyMDI1LzAzLzA2LTIwOjUwOjE2ICAgICAgICAiPiA8cmRmOlJERiB4bWxuczpyZGY9Imh0dHA6Ly93d3cudzMub3JnLzE5OTkvMDIvMjItcmRmLXN5bnRheC1ucyMiPiA8cmRmOkRlc2NyaXB0aW9uIHJkZjphYm91dD0iIiB4bWxuczpJcHRjNHhtcEV4dD0iaHR0cDovL2lwdGMub3JnL3N0ZC9JcHRjNHhtcEV4dC8yMDA4LTAyLTI5LyIgeG1sbnM6cGhvdG9zaG9wPSJodHRwOi8vbnMuYWRvYmUuY29tL3Bob3Rvc2hvcC8xLjAvIiB4bWxuczpleGlmPSJodHRwOi8vbnMuYWRvYmUuY29tL2V4aWYvMS4wLyIgeG1sbnM6eG1wPSJodHRwOi8vbnMuYWRvYmUuY29tL3hhcC8xLjAvIiB4bWxuczpkYz0iaHR0cDovL3B1cmwub3JnL2RjL2VsZW1lbnRzLzEuMS8iIHhtbG5zOnhtcE1NPSJodHRwOi8vbnMuYWRvYmUuY29tL3hhcC8xLjAvbW0vIiB4bWxuczpzdEV2dD0iaHR0cDovL25zLmFkb2JlLmNvbS94YXAvMS4wL3NUeXBlL1Jlc291cmNlRXZlbnQjIiBJcHRjNHhtcEV4dDpEaWdpdGFsU291cmNlRmlsZVR5cGU9Imh0dHA6Ly9jdi5pcHRjLm9yZy9uZXdzY29kZXMvZGlnaXRhbHNvdXJjZXR5cGUvY29tcG9zaXRlV2l0aFRyYWluZWRBbGdvcml0aG1pY01lZGlhIiBJcHRjNHhtcEV4dDpEaWdpdGFsU291cmNlVHlwZT0iaHR0cDovL2N2LmlwdGMub3JnL25ld3Njb2Rlcy9kaWdpdGFsc291cmNldHlwZS9jb21wb3NpdGVXaXRoVHJhaW5lZEFsZ29yaXRobWljTWVkaWEiIHBob3Rvc2hvcDpDcmVkaXQ9IkVkaXRlZCB3aXRoIEdvb2dsZSBBSSIgcGhvdG9zaG9wOkRhdGVDcmVhdGVkPSIyMDI1LTA5LTE5VDIxOjIyOjA0IiBwaG90b3Nob3A6Q29sb3JNb2RlPSIzIiBleGlmOkRhdGVUaW1lT3JpZ2luYWw9IjIwMjU6MDk6MTkgMjE6MjI6MDQrMDA6MDAiIHhtcDpDcmVhdGVEYXRlPSIyMDI1LTA5LTE5VDIzOjIzOjA1KzAyOjAwIiB4bXA6TW9kaWZ5RGF0ZT0iMjAyNS0wOS0xOVQyMzoyNTo0NiswMjowMCIgeG1wOk1ldGFkYXRhRGF0ZT0iMjAyNS0wOS0xOVQyMzoyNTo0NiswMjowMCIgZGM6Zm9ybWF0PSJpbWFnZS9wbmciIHhtcE1NOkluc3RhbmNlSUQ9InhtcC5paWQ6ZmE1NWZjZjEtZGNjMi00ZTA5LTg0ODAtOTkzNzE5OWJkNDIyIiB4bXBNTTpEb2N1bWVudElEPSJ4bXAuZGlkOmZhNTVmY2YxLWRjYzItNGUwOS04NDgwLTk5MzcxOTliZDQyMiIgeG1wTU06T3JpZ2luYWxEb2N1bWVudElEPSJ4bXAuZGlkOmZhNTVmY2YxLWRjYzItNGUwOS04NDgwLTk5MzcxOTliZDQyMiI+IDx4bXBNTTpIaXN0b3J5PiA8cmRmOlNlcT4gPHJkZjpsaSBzdEV2dDphY3Rpb249InNhdmVkIiBzdEV2dDppbnN0YW5jZUlEPSJ4bXAuaWlkOmZhNTVmY2YxLWRjYzItNGUwOS04NDgwLTk5MzcxOTliZDQyMiIgc3RFdnQ6d2hlbj0iMjAyNS0wOS0xOVQyMzoyNTo0NiswMjowMCIgc3RFdnQ6c29mdHdhcmVBZ2VudD0iQWRvYmUgUGhvdG9zaG9wIDI2LjggKE1hY2ludG9zaCkiIHN0RXZ0OmNoYW5nZWQ9Ii8iLz4gPC9yZGY6U2VxPiA8L3htcE1NOkhpc3Rvcnk+IDwvcmRmOkRlc2NyaXB0aW9uPiA8L3JkZjpSREY+IDwveDp4bXBtZXRhPiA8P3hwYWNrZXQgZW5kPSJyIj8+5XszUAAAeI9JREFUeJyN/Ve0ddlxHoZ+VXOt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",
    "payloadType": "str",
    "x": 190,
    "y": 380,
    "wires": [["06d0f17e61c53720"]]
  },
  {
    "id": "06d0f17e61c53720",
    "type": "mqtt out",
    "z": "7672c2f0dd60ead7",
    "g": "7048ae2158b68615",
    "name": "",
    "topic": "sendPicture",
    "qos": "0",
    "retain": "",
    "respTopic": "",
    "contentType": "",
    "userProps": "",
    "correl": "",
    "expiry": "",
    "broker": "6fc7cd40ef96dc87",
    "x": 330,
    "y": 380,
    "wires": []
  },
  {
    "id": "3ce712864110bff3",
    "type": "deepstack-object-detection",
    "z": "7672c2f0dd60ead7",
    "g": "bafab6c127fbe920",
    "name": "",
    "server": "e2e35f99e3052fdb",
    "confidence": "40",
    "drawPredictions": false,
    "printLabel": false,
    "outlineColor": "pink",
    "custom": "",
    "filters": [],
    "outputs": 1,
    "x": 850,
    "y": 640,
    "wires": [["4f840db5362eee36"]]
  },
  {
    "id": "4f840db5362eee36",
    "type": "debug",
    "z": "7672c2f0dd60ead7",
    "g": "bafab6c127fbe920",
    "name": "debug 5",
    "active": true,
    "tosidebar": true,
    "console": false,
    "tostatus": false,
    "complete": "false",
    "statusVal": "",
    "statusType": "auto",
    "x": 1160,
    "y": 640,
    "wires": []
  },
  {
    "id": "94c9ddeeb0b403cd",
    "type": "deepstack-face-registration",
    "z": "7672c2f0dd60ead7",
    "g": "bafab6c127fbe920",
    "name": "",
    "server": "e2e35f99e3052fdb",
    "userid": "",
    "outputs": 1,
    "x": 850,
    "y": 700,
    "wires": [["80606c022ad27845"]]
  },
  {
    "id": "b62e7e891db0fff5",
    "type": "deepstack-face-recognition",
    "z": "7672c2f0dd60ead7",
    "g": "bafab6c127fbe920",
    "name": "",
    "server": "e2e35f99e3052fdb",
    "confidence": "50",
    "drawPredictions": false,
    "outlineColor": "pink",
    "filters": [],
    "outputs": 1,
    "x": 850,
    "y": 760,
    "wires": [["4acdce29aa74167a"]]
  },
  {
    "id": "4acdce29aa74167a",
    "type": "function",
    "z": "7672c2f0dd60ead7",
    "g": "bafab6c127fbe920",
    "name": "function 2",
    "func": "let conf = msg.payload[0][\"confidence\"];\n\nif(conf>0.75){\n    node.warn(\"öffnen\")\n}\n\nreturn msg;",
    "outputs": 1,
    "timeout": 0,
    "noerr": 0,
    "initialize": "",
    "finalize": "",
    "libs": [],
    "x": 1020,
    "y": 760,
    "wires": [["52e76609449b3c68"]]
  },
  {
    "id": "80606c022ad27845",
    "type": "debug",
    "z": "7672c2f0dd60ead7",
    "g": "bafab6c127fbe920",
    "name": "debug 6",
    "active": true,
    "tosidebar": true,
    "console": false,
    "tostatus": false,
    "complete": "false",
    "statusVal": "",
    "statusType": "auto",
    "x": 1160,
    "y": 700,
    "wires": []
  },
  {
    "id": "52e76609449b3c68",
    "type": "debug",
    "z": "7672c2f0dd60ead7",
    "g": "bafab6c127fbe920",
    "name": "debug 7",
    "active": true,
    "tosidebar": true,
    "console": false,
    "tostatus": false,
    "complete": "false",
    "statusVal": "",
    "statusType": "auto",
    "x": 1160,
    "y": 760,
    "wires": []
  },
  {
    "id": "6fc7cd40ef96dc87",
    "type": "mqtt-broker",
    "name": "",
    "broker": "mosquitto-server",
    "port": 1883,
    "clientid": "",
    "autoConnect": true,
    "usetls": false,
    "protocolVersion": 4,
    "keepalive": 60,
    "cleansession": true,
    "autoUnsubscribe": true,
    "birthTopic": "",
    "birthQos": "0",
    "birthRetain": "false",
    "birthPayload": "",
    "birthMsg": {},
    "closeTopic": "",
    "closeQos": "0",
    "closeRetain": "false",
    "closePayload": "",
    "closeMsg": {},
    "willTopic": "",
    "willQos": "0",
    "willRetain": "false",
    "willPayload": "",
    "willMsg": {},
    "userProps": "",
    "sessionExpiry": ""
  },
  {
    "id": "e2e35f99e3052fdb",
    "type": "deepstack-server",
    "proto": "http",
    "host": "jan5",
    "port": "85",
    "version": "v1",
    "rejectUnauthorized": false
  },
  {
    "id": "f9e4c1cdaa456059",
    "type": "global-config",
    "env": [],
    "modules": {
      "node-red-contrib-image-output": "0.6.4",
      "node-red-contrib-deepstack": "0.3.0"
    }
  }
]
```

</details>
