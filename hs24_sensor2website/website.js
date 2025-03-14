/*****************************************************************************************************************
 *  website.js 
 *  https://fiessling.ch/iot1/hs24_sensor2website/website.js
 *  Daten als JSON-String vom Server holen (unload.php), sobald website.js dieses Script per HTTP aufruft (fetch())
******************************************************************************************************************/


async function fetchData() {
    try{
        // const response = await fetch('https://localhost/07_sensor2website_improved/db2website.php');
        // const response = await fetch('https://650665-4.web.fhgr.education/07_sensor2website_improved/db2website.php');
        const response = await fetch('https://fiessling.ch/iot1/hs24_sensor2website/unload.php');
        const data = await response.json();
        console.log(data);
        return data;
    } catch (error) {
        console.error(error);
    }
}

async function main() {
    let data = await fetchData();
    console.log(data);

    let wert = data.data.wert;
    let zeit = data.data.zeit;

    // Hier legst du die Art des Diagramms (Line Chart) fest und fütterst die beiden Datenarrays rein
    const config = {
        type: 'line',
        data:{
            labels: zeit,
            datasets: 
            [{
                label: 'Wert',
                data: wert,
                backgroundColor: 'red',
                borderColor: 'red',
                borderWidth: 1
            }]
        }
    };

    // Erstellen des Charts
    const ctx = document.querySelector('#myChart').getContext('2d');
    new Chart(ctx, config);
}

main();