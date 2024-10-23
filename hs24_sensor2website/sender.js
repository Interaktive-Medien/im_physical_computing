async function submit_form(event){
    const formData = new FormData(event.target);
    const dataObject = {
        wert: formData.get("wert")
    };
    const jsonstring = JSON.stringify(dataObject);
    console.log(jsonstring);

    await fetch("https://fiessling.ch/iot1/hs24_sensor2website/load.php", {
        method: "POST",
        headers: {
            'Content-Type': 'application/json'
        },
        body: jsonstring
    });
}