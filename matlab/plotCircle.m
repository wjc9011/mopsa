function plotCircle(centerPoint, radius, color)
sita = 0: pi / 20: 2 * pi;
plot((centerPoint(1, 1) + radius * cos(sita)), (centerPoint(1, 2) + radius * sin(sita)), color);
hold on 