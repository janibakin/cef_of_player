const canvas = document.getElementById('c');
const ctx = canvas.getContext('2d');
const wrapper = document.getElementById('canvas-wrapper');

let x = 0, y = 0, vx = 4, vy = 3;
const r = 20;

function resizeCanvas() {
  canvas.width = wrapper.clientWidth;
  canvas.height = wrapper.clientHeight;

  x = canvas.width / 2;
  y = canvas.height / 2;

  ctx.clearRect(0, 0, canvas.width, canvas.height);
}
window.addEventListener('resize', () => {
  resizeCanvas();
});
resizeCanvas();

function loop() {
  requestAnimationFrame(loop);

  const w = canvas.width;
  const h = canvas.height;

  ctx.clearRect(0, 0, w, h);

  x += vx;
  y += vy;

  if (x - r < 0 || x + r > w) vx *= -1;
  if (y - r < 0 || y + r > h) vy *= -1;

  ctx.fillStyle = '#000';
  ctx.beginPath();
  ctx.arc(x, y, r, 0, Math.PI * 2);
  ctx.fill();
}
loop();