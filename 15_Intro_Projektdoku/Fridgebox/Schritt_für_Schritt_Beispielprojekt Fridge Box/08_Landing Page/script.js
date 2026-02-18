async function getCountOpens() {
    const response = await fetch("unload.php");  // zB. {"count_opens":[1,0,0,140,0,0,22]}
    const all_data = await response.json();
    const count_opens = all_data.count_opens; // [1,0,0,140,0,0,22]
    console.log("count_opens", count_opens);
    return count_opens; 
}



async function main(){
    //////////////// hole Daten für Chart.js
    let count_opens = await getCountOpens();

    //////////////// Chart.js zusammenbauen
    let myChart = document.querySelector('#myChart').getContext("2d");
    const fridgeChart = new Chart(myChart, {
        type: "bar", // "line", "pie", "doughnut", "polarArea", "radar"
        data: {
            labels: ["heute", "gestern", "vorgestern", "vor 3 Tagen", "vor 4 Tagen", "vor 5 Tagen","vor 6 Tagen"],
            datasets:[
                {
                    data: count_opens,
                    label: "Wie oft offen"
                }          
            ]
        }
    });
}

main();

