document.getElementById('btnOn').addEventListener('click', () => {
    setLedState('on');
});

document.getElementById('btnOff').addEventListener('click', () => {
    setLedState('off');
});

function setLedState(state) {
    // Sendet einen Request an http://<IP>/setLED?state=on oder off
    fetch(`/setLED?state=${state}`)
        .then(response => response.text())
        .then(data => {
            document.getElementById('stateText').innerText = state.toUpperCase();
            console.log("Server Antwort:", data);
        })
        .catch(err => console.error("Fehler beim Schalten:", err));
}
