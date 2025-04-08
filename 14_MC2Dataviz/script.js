/*****************************************************************************************************************
 *  Kap. 14: Microcontroller -> Dataviz
 *  website.js 
 *  Daten als JSON-String vom Server holen (unload.php), sobald website.js dieses Script per HTTP aufruft (fetch())
 *  Beispiel: https://fiessling.ch/im4/14_MC2Dataviz/website.html
 *  GitHub: https://github.com/Interaktive-Medien/im_physical_computing/tree/main/14MC2Dataviz/index.html
******************************************************************************************************************/

async function fetchData() {
    try{
        const response = await fetch('unload.php');
        const data = await response.json();   // wenn's nicht klappt, zeig den ganzen Text erst mit .text() an
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