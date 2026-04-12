/************************************************
Kapitel 12: Website2DB > Schritt 2: Website -> DB
sender.js
Hier Werten die Formulardaten aus sender.html extrahiert, als JSON string formattiert und per HTTP POST Request an load.php geschickt.
Später werden die HTTP Nachrichten nicht mehr von einer Website geschickt, sondern von einem ESP
******************************************************/

const form = document.getElementById("dataForm");

form.addEventListener("submit", async (event) => {
  event.preventDefault(); // Neuladen der Seite verhindern

  // Daten aus dem Formular holen
  const formData = new FormData(event.target);
  const dataObject = {
    wert: formData.get("wert"),
  };

  // Daten als JSON string formattieren
  const jsonstring = JSON.stringify(dataObject);

  // debug
  console.log("JSON Output:", jsonstring);
  document.querySelector("#message").innerText =
    "Daten gesendet: " + jsonstring;

  // HTTP POST Request an load.php schicken
  try {
    const response = await fetch("api/load.php", {
      method: "POST",
      headers: {
        "Content-Type": "application/json",
      },
      body: jsonstring,
    });
  } catch (error) {
    console.error("Fehler beim Senden der Daten:", error);
    document.querySelector("#message").innerText =
      "Fehler beim Senden der Daten: " + error.message;
  }
});
