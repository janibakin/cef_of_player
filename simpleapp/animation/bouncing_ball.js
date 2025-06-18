const canvas = document.getElementById('c');
const ctx = canvas.getContext('2d');
let w = canvas.width, h = canvas.height;

let x = w / 2, y = h / 4, vx = 3, vy = 2, r = 40;

function loop() {
    requestAnimationFrame(loop);
    ctx.clearRect(0, 0, w, h);
    x += vx; y += vy;
    if (x < r || x > w - r) vx *= -1;
    if (y < r || y > h - r) vy *= -1;
    ctx.fillStyle = '#ff4081';
    ctx.beginPath();
    ctx.arc(x, y, r, 0, Math.PI * 2);
    ctx.fill();
}
loop();