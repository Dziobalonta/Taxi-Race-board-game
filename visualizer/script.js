// map coordinates
const coordinates = {
    1: {x: 3165, y: 6349},
    2: {x: 2883, y: 6530},
    3: {x: 2643, y: 6737},
    4: {x: 2278, y: 6720},
    5: {x: 1988, y: 6712},
    6: {x: 1698, y: 6712},
    7: {x: 1334, y: 6654},
    8: {x: 1077, y: 6530},
    9: {x: 1201, y: 6209},
    10: {x: 1483, y: 6011},
    11: {x: 1756, y: 5797},
    12: {x: 1997, y: 5566},
    13: {x: 2171, y: 5360},
    14: {x: 2527, y: 5302},
    15: {x: 2833, y: 5120},
    16: {x: 3132, y: 4964},
    17: {x: 3388, y: 4947},
    18: {x: 3695, y: 4914},
    19: {x: 4018, y: 4865},
    20: {x: 4333, y: 4815},
    21: {x: 4606, y: 4683},
    22: {x: 4871, y: 4510},
    23: {x: 4805, y: 4255},
    24: {x: 4515, y: 4090},
    25: {x: 4242, y: 3983},
    26: {x: 3960, y: 3859},
    27: {x: 3620, y: 3719},
    28: {x: 3380, y: 3612},
    29: {x: 3049, y: 3587},
    30: {x: 2800, y: 3488},
    31: {x: 2585, y: 3298},
    32: {x: 2543, y: 2993},
    33: {x: 2510, y: 2721},
    34: {x: 2336, y: 2507},
    35: {x: 1930, y: 2391},
    36: {x: 1574, y: 2366},
    37: {x: 1226, y: 2259},
    38: {x: 1011, y: 1913},
    39: {x: 688, y: 1748},
    40: {x: 853, y: 1377},
    41: {x: 1243, y: 1196},
    42: {x: 1574, y: 1154},
    43: {x: 1930, y: 1163},
    44: {x: 2320, y: 1138},
    45: {x: 2618, y: 1080},
    46: {x: 2866, y: 841},
    47: {x: 3090, y: 635},
    48: {x: 3380, y: 429},
    49: {x: 3687, y: 264},
    50: {x: 3977, y: 231},
    51: {x: 4333, y: 297},
    52: {x: 4648, y: 272},
    53: {x: 5037, y: 388},
    54: {x: 5294, y: 660},
    55: {x: 5509, y: 956},
    56: {x: 5741, y: 1245},
    57: {x: 5874, y: 1517},
    58: {x: 6039, y: 1855},
    59: {x: 5791, y: 2185},
    60: {x: 5410, y: 2235},
    61: {x: 5103, y: 2416},
    62: {x: 4846, y: 2713},
    63: {x: 4664, y: 3059},
    64: {x: 4515, y: 3356},
    65: {x: 4349, y: 3694},
    66: {x: 4126, y: 4296},
    67: {x: 4059, y: 4552},
    68: {x: 4126, y: 5145},
    69: {x: 4217, y: 5417},
    70: {x: 4333, y: 5739},
    71: {x: 4440, y: 5937},
    72: {x: 4482, y: 6217},
    73: {x: 4581, y: 6489},
    74: {x: 4623, y: 6737},
    75: {x: 5053, y: 6860},
    76: {x: 5401, y: 6803},
    77: {x: 5783, y: 6712},
    78: {x: 6031, y: 6530},
    79: {x: 6048, y: 6168},
    80: {x: 5973, y: 5896},
    81: {x: 5907, y: 5615},
    82: {x: 5849, y: 5252},
    83: {x: 6205, y: 5170},
    84: {x: 6619, y: 5120},
    85: {x: 6851, y: 5269},
    86: {x: 6859, y: 5648},
    87: {x: 6901, y: 5912},
    88: {x: 6967, y: 6135},
    89: {x: 7257, y: 6209},
    90: {x: 7589, y: 6225},
    91: {x: 7638, y: 5805},
    92: {x: 7613, y: 5442},
    93: {x: 7613, y: 5137},
    94: {x: 7613, y: 4733},
    95: {x: 7613, y: 4354},
    96: {x: 7613, y: 3991},
    97: {x: 7688, y: 3669},
    98: {x: 7787, y: 3290},
    99: {x: 7796, y: 3059},
    100: {x: 7741, y: 2540},
    101: {x: 7531, y: 2160},
    102: {x: 7124, y: 1538},
    103: {x: 6849, y: 1250},
    104: {x: 6527, y: 1001},
    105: {x: 6198, y: 752},
    106: {x: 6054, y: 372},
    107: {x: 6520, y: 136},
    108: {x: 6927, y: 169},
    109: {x: 7190, y: 346},
    110: {x: 7472, y: 627},
    111: {x: 7761, y: 922},
    112: {x: 8037, y: 1256},
    113: {x: 8273, y: 1558},
    114: {x: 8529, y: 1879},
    115: {x: 8713, y: 2200},
    116: {x: 8989, y: 2593},
    117: {x: 9160, y: 3025},
    118: {x: 9219, y: 3372},
    119: {x: 9140, y: 3746},
    120: {x: 9081, y: 4145},
    121: {x: 8930, y: 4453},
    122: {x: 8766, y: 4813},
    123: {x: 8648, y: 5128},
    124: {x: 8365, y: 5285},
    125: {x: 7965, y: 5337},
    126: {x: 7216, y: 5508},
    127: {x: 6599, y: 5698},
    128: {x: 6271, y: 5770},
    129: {x: 5608, y: 6012},
    130: {x: 5181, y: 6110},
    131: {x: 4806, y: 6222},
    132: {x: 4189, y: 6287},
    133: {x: 3874, y: 6294},
    134: {x: 3592, y: 6255},
    135: {x: 3309, y: 6045},
    136: {x: 3296, y: 5652},
    137: {x: 3171, y: 5272},
    138: {x: 3119, y: 4637},
    139: {x: 3080, y: 4302},
    140: {x: 3309, y: 3949},
    141: {x: 3835, y: 3444},
    142: {x: 3992, y: 3032},
    143: {x: 4032, y: 2698},
    144: {x: 4091, y: 2344},
    145: {x: 4294, y: 2134},
    146: {x: 4518, y: 1931},
    147: {x: 4806, y: 1735},
    148: {x: 5207, y: 1649},
    149: {x: 5548, y: 1577},
    150: {x: 6192, y: 1545},
    151: {x: 6546, y: 1636},
    152: {x: 6888, y: 1748},
    153: {x: 7242, y: 1807},
    154: {x: 7741, y: 1879},
    155: {x: 8142, y: 1918},
    156: {x: 8799, y: 1774},
    157: {x: 9206, y: 1676},
    158: {x: 9573, y: 1643},
    159: {x: 9764, y: 1951},
    160: {x: 9777, y: 2298},
    161: {x: 9751, y: 2730},
    162: {x: 9744, y: 3058},
    163: {x: 9580, y: 3392},
    164: {x: 8851, y: 3359},
    165: {x: 8503, y: 3359},
    166: {x: 8122, y: 3359},
    167: {x: 7413, y: 3392},
    168: {x: 6947, y: 3490},
    169: {x: 6494, y: 3536},
    170: {x: 6139, y: 3654},
    171: {x: 5831, y: 3654},
    172: {x: 5397, y: 3516},
    173: {x: 5168, y: 3372},
    174: {x: 4885, y: 3248},
    175: {x: 4557, y: 2684},
    176: {x: 4485, y: 2422},
    177: {x: 4163, y: 1872},
    178: {x: 3953, y: 1656},
    179: {x: 3671, y: 1492},
    180: {x: 3336, y: 1328},
    181: {x: 3034, y: 1217},
    182: {x: 2686, y: 1348},
    183: {x: 2686, y: 1689},
    184: {x: 2640, y: 2042},
    185: {x: 2489, y: 2278},
};

const canvas = document.getElementById('overlay');
const ctx = canvas.getContext('2d');
const img = document.getElementById('map-img');

let simulationData = []; 
let currentRoundIndex = 0;

window.addEventListener('resize', adjustSize);

function adjustSize() {
    // sizing to img
    canvas.width = img.naturalWidth;
    canvas.height = img.naturalHeight;

    // refresh
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
        let state = roadState[number-1]; 

        ctx.beginPath();
        ctx.arc(point.x, point.y, 150, 0, 2 * Math.PI); // circle size
        //  Stroke thickness
        ctx.lineWidth = 15;

        if (state === 1) {
            ctx.fillStyle = "rgba(255, 255, 0, 0.6)"; // Yellow
            ctx.strokeStyle = "black";
        } else if (state === 2) {
            ctx.fillStyle = "rgba(255, 0, 0, 0.6)"; // Red
            ctx.strokeStyle = "black";
        } else if(state == 3){
            ctx.fillStyle = "rgba(162, 0, 255, 0.8)";  // Purple
            ctx.fill();
            ctx.strokeStyle = "white";
            ctx.stroke();
        }else if(state == 4){
            ctx.fillStyle = "rgba(0, 217, 255, 0.87)";  // Blue
            ctx.fill();
            ctx.strokeStyle = "white";
            ctx.stroke();
        }else {
            ctx.fillStyle = "rgba(0, 255, 0, 0.4)";  // Green
            ctx.fill();
            ctx.strokeStyle = "black";
            ctx.stroke();
        }

            ctx.fill();
            ctx.stroke();

            // Text label

            ctx.fillStyle = "black";
            ctx.font = "60px Arial";
            ctx.fillText(number, point.x - 20, point.y + 15);
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