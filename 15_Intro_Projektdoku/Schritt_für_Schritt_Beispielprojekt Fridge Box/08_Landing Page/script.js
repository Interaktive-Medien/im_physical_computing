 /*************************************************************
 * Kapitel 15: Demo-Projekt Fridge Box
 * Schritt 8 ("Landing Page")
 * script.js: 
 * Hier werden vom Client aus Daten bei unload.php angefragt
 * In unload.php werden sie aus der Datenbank per SQL abgefragt, sortiert
 * Es wird ein entsprechendes Array als JSON-String per echo bereit gestellt, 
 * das der client (script.js) mit fetch() abholen kann.
 * Datenbank-Verbindung:
 * Ersetze $db_host, $db_name, $db_user, $db_pass durch deine eigenen Daten. 
 * GitHub: 
 **************************************************************/



/////////////////////////  Fetch data from unload.php

async function fetchData() {
    try {
        const response = await fetch('unload.php');
        const all_data = await response.json();
        const count_opens = all_data.count_opens;
        console.log("count_opens", count_opens);
        return count_opens;
    } catch (error) {
        console.error('There was a problem with the fetch operation:', error);
    }
}

async function main(){
    let count_opens = await fetchData();

    
    /////////////////////////  Chartjs zusammenbauen

    let myChart = document.querySelector("#myChart").getContext("2d");
    const fridgeChart = new Chart(myChart, {
        type: "bar",
        data: {
            labels: ["heute", "gestern", "vorgestern", "vor 3 Tagen", "vor 4 Tagen", "vor 6 Tagen", "vor 7 Tagen"],
            datasets: [{
                label: "Wie oft offen",
                // data: [5, 10, 15],  // Fülldaten
                data: count_opens      // echte Daten, siehe oben
            }]
        }
    });
};

main();