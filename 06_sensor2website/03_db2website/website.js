async function fetchData() {
    try{
        const response = await fetch('https://localhost/06_sensor2website/03_db2website/db2website.php');
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