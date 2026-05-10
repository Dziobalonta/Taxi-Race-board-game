// map coordinates
const coordinates = {
    1: {x: 804, y: 6841},
    2: {x: 1007, y: 6500},
    3: {x: 1228, y: 6208},
    4: {x: 1467, y: 5979},
    5: {x: 1679, y: 5795},
    
};

const canvas = document.getElementById('overlay');
const ctx = canvas.getContext('2d');
const img = document.getElementById('map-img');

let simulationData = []; 
let currentRoundIndex = 0;

window.addEventListener('resize', adjustSize);

function adjustSize() {
    // Ustawiamy wewnętrzną rozdzielczość canvas na oryginalny rozmiar pliku JPG
    canvas.width = img.naturalWidth;
    canvas.height = img.naturalHeight;

    // Po zmianie rozmiaru trzeba odświeżyć rysowanie kropek
    drawState(); 
}

loadCSV();

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
        ctx.arc(point.x, point.y, 150, 0, 2 * Math.PI); // circle size
        //  Stroke thickness
        ctx.lineWidth = 15;

        if (state === 1) {
            ctx.fillStyle = "rgba(255, 255, 0, 0.6)"; // Yellow
            ctx.strokeStyle = "black";
        } else if (state === 2) {
            ctx.fillStyle = "rgba(255, 0, 0, 0.6)"; // Red
            ctx.strokeStyle = "white";
        } else {
            ctx.fillStyle = "rgba(0, 255, 0, 0.4)";  // Green
            ctx.fill();
            ctx.strokeStyle = "black";
            ctx.stroke();
        }

            ctx.fill();
            ctx.stroke();
    }
}

// Prints pixel image coordiantes of mouse click
canvas.addEventListener('mousedown', function(e) {
    const rect = canvas.getBoundingClientRect();
    
    // Calculate the scale between the displayed size and the original (natural) size
    const scaleX = canvas.width / rect.width;
    const scaleY = canvas.height / rect.height;
    
    // Get real pixel coordinates regardless of browser scaling
    const x = Math.round((e.clientX - rect.left) * scaleX);
    const y = Math.round((e.clientY - rect.top) * scaleY);
    
    // Print ready-to-copy line to the console
    console.log(`{x: ${x}, y: ${y}},`);
});