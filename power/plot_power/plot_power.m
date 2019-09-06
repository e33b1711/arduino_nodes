clear all;
close all;
filename = './gp.txt';
[time,power] = importfile(filename);

plot(time, power);
grid on;
grid minor;
title('Power drawn from grid');