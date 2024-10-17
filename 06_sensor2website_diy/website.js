/*****************************************************************************************************************
 *  website.js 
 *  https://fiessling.ch/06_sensor2website_diy/website.js
 *  Daten als JSON-String vom Server holen (unload.php), sobald website.js dieses Script per HTTP aufruft (fetch())
******************************************************************************************************************/


async function fetchData() {
    try{
        // const response = await fetch('https://localhost/06_sensor2website/unload.php');
        // const response = await fetch('https://650665-4.web.fhgr.education/06_sensor2website/unload.php');
        const response = await fetch('https://fiessling.ch/06_sensor2website_diy/unload.php');
        const data = await response.json();   // wenn's nicht klappt, zeig den ganzen Text erst mit .text() an
        return data;
    } catch (error) {
        console.error(error);
    }
}



async function main() {
    console.log("ahoiiii");
    console.log("hello");
    let data = await fetchData();
    console.log(data);




    const DATA_COUNT = 24;
    // const datapoints = [0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23];

    // Config-Teil
    // Hier legst du die Art des Diagramms (Line Chart) und Optionen wie Achsentitel, Minimal- und Maximalwerte sowie die Interpolationsart fest.
    const config = {
        type: 'bar',   // line, pie, bar, radar, doughnut, polarArea
        data: {
            labels: [0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23],  // labels muss drin sein
            datasets: [      // datasets muss drin sein
                {
                    data: data.data,
                }
            ]
        }
    };
    // Erstellen des Charts
    const ctx = document.querySelector('#myChart').getContext('2d');
    new Chart(ctx, config);


}

main();