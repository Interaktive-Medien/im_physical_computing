# Node-Red Intro

```
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
        "nodes": [
            "f0bcdfed47850ddd",
            "e27c0f25efee8358"
        ],
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
        "nodes": [
            "d2a02a7327061167",
            "07b5e9e03d7a3504",
            "834b516de1e5c3ad"
        ],
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
        "wires": [
            [
                "e27c0f25efee8358"
            ]
        ]
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
        "wires": [
            [
                "07b5e9e03d7a3504"
            ]
        ]
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
        "wires": [
            [
                "834b516de1e5c3ad"
            ]
        ]
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
        "wires": [
            [
                "b0860a43a7b08f2d"
            ]
        ]
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
        "wires": [
            [
                "b0860a43a7b08f2d"
            ]
        ]
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
        "wires": [
            [
                "b0860a43a7b08f2d"
            ]
        ]
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
        "wires": [
            [
                "137ffceb2a10b1d7"
            ],
            [
                "47d880a385536bd6"
            ]
        ]
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
        "wires": [
            [
                "33e4c655b67e4d4d"
            ]
        ]
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
        "wires": [
            [
                "309ffda10da5082f",
                "b4d18b184a1a35b9",
                "8d4b36518907496f"
            ]
        ]
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
        "wires": [
            [
                "f296e23fcab6609d"
            ]
        ]
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
        "wires": [
            [
                "17e6a417d69dff18"
            ]
        ]
    }
]
```

huhu
