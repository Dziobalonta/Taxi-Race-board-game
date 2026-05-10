// map coordinates
const coordinates = {
    1: {x: 80, y: 920},
    2: {x: 130, y: 880},
    3: {x: 180, y: 830},
    4: {x: 230, y: 780},
    5: {x: 280, y: 730}
    
};

const canvas = document.getElementById('overlay');
const ctx = canvas.getContext('2d');
const img = document.getElementById('map-img');

let simulationData = []; 
let currentRoundIndex = 0;

function adjustSize() {
    canvas.width = img.width;
    canvas.height = img.height;

    loadCSV();
}

  // Auto loading the CSV file when the page is ready
function loadCSV() {
    fetch('./../simulation.csv')
        .then(response => {
            if (!response.ok) {
                throw new Error("Cannot find simulation.csv!");
            }
            return response.text();
        })
        .then(text => {
            const lines = text.split('\n').filter(line => line.trim() !== '');
            simulationData = lines.map(line => line.split(',').map(Number));
            
            currentRoundIndex = 0;
            updateUI();
            drawState();
        })
        .catch(error => {
            console.error(error);
            alert("Error loading simulation.csv! Are you running a local server?");
        });
}

// ROUND CONTROLS
function changeRound(step) {
    currentRoundIndex += step;
    updateUI();
    drawState();
}

function updateUI() {
    const total = simulationData.length;
    document.getElementById('roundCounter').innerText = `Round: ${total > 0 ? currentRoundIndex + 1 : 0} / ${total}`;
    
    document.getElementById('btnPrev').disabled = (currentRoundIndex <= 0);
    document.getElementById('btnNext').disabled = (currentRoundIndex >= total - 1 || total === 0);
}

// DRAWING
function drawState() {
    ctx.clearRect(0, 0, canvas.width, canvas.height);

    if (simulationData.length === 0) return;

    const roadState = simulationData[currentRoundIndex];

    for (let number in coordinates) {
        let point = coordinates[number];
        let state = roadState[number - 1]; 

        ctx.beginPath();
        ctx.arc(point.x, point.y, 15, 0, 2 * Math.PI);

        if (state === 1) {
            ctx.fillStyle = "rgba(255, 255, 0, 0.6)"; // Yellow
            ctx.fill();
            ctx.strokeStyle = "black";
            ctx.stroke();
        } else if (state === 2) {
            ctx.fillStyle = "rgba(255, 0, 0, 0.6)"; // Red
            ctx.fill();
            ctx.strokeStyle = "white";
            ctx.stroke();
        } else {
            ctx.fillStyle = "rgba(0, 255, 0, 0.4)";  // Green
            ctx.fill();
            ctx.strokeStyle = "black";
            ctx.stroke();
        }
    }
}