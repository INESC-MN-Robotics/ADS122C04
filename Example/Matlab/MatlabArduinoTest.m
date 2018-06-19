clear all;
s=serial('COM7');
s.BaudRate=115200;
set(s,'ByteOrder','littleEndian'); %sets the byte order to arduino's, which is littleEndian
s.InputBufferSize = 12; %read only one byte at time

fopen(s);

L1=0; L2=0; L3=0; L4=0;
R1=0; R2=0; R3=0; R4=0;

x=0;
x2=0;
lineL1 = animatedline('Color', 'r', 'Linewidth', 1);
lineL2 = animatedline('Color', 'g', 'Linewidth', 1);
lineL3 = animatedline('Color', 'b', 'linewidth', 1);
lineL4 = animatedline('Color', 'k', 'linewidth', 1);
lineR1 = animatedline('Color', 'r', 'LineStyle', '--', 'Linewidth', 1);
lineR2 = animatedline('Color', 'g', 'LineStyle', '--', 'Linewidth', 1);
lineR3 = animatedline('Color', 'b', 'LineStyle', '--', 'linewidth', 1);
lineR4 = animatedline('Color', 'k', 'LineStyle', '--', 'linewidth', 1);
legend('L00.CH0', 'L00.CH1', 'L00.CH2', 'L00.CH3', 'R11.CH0', 'R11.CH1', 'R11.CH2', 'R11.CH3');

drawnow;
set(gca, 'YScale', 'log');
while x < 60
    x = x+1;
L1 = fscanf(s, "%s");
L2 = fscanf(s, "%s");
L3 = fscanf(s, "%s");
L4 = fscanf(s, "%s");
R1 = fscanf(s, "%s");
R2 = fscanf(s, "%s");
R3 = fscanf(s, "%s");
R4 = fscanf(s, "%s");

y1 = str2double(L1);
y2 = str2double(L2);
y3 = str2double(L3);
y4 = str2double(L4);
y5 = str2double(R1);
y6 = str2double(R2);
y7 = str2double(R3);
y8 = str2double(R4);


addpoints(lineL1, x, y1);
addpoints(lineL2, x, y2);
addpoints(lineL3, x, y3);
addpoints(lineL4, x, y4);
addpoints(lineR1, x, y5);
addpoints(lineR2, x, y6);
addpoints(lineR3, x, y7);
addpoints(lineR4, x, y8);
grid on;
hold on;
drawnow;
title('Tension Values');
xlabel('Time in Seconds');
ylabel('Voltage');
pause(0.1);
end;    

fclose(s);
delete(s);
