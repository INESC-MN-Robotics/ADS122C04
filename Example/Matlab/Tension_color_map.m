
s=serial('COM7');
s.BaudRate=115200;
set(s,'ByteOrder','littleEndian'); %sets the byte order to arduino's, which is littleEndian
s.InputBufferSize = 12; %read only one byte at time


L1=0; L2=0; L3=0; L4=0;
R1=0; R2=0; R3=0; R4=0;



im = imread('Layout.jpg');
imshow(im);

R0R = rectangle('Position', [586,1322,200,200],'FaceColor',[0.2,0,0.2]);
R1R = rectangle('Position', [682,998,200,200], 'FaceColor', [0.2, 0, 0.2]);
R2R = rectangle('Position', [1372, 1316,200,200], 'FaceColor',[0.2,0,0.2]);
R3R = rectangle('Position', [1281,998,200,200], 'FaceColor', [0.2,0,0.2]);

L0R = rectangle('Position', [574,1814,200,200],'FaceColor',[0.2,0,0.2]);
L1R = rectangle('Position', [676,2144,200,200],'FaceColor',[0.2,0,0.2]);
L2R = rectangle('Position', [1372,1826,200,200],'FaceColor',[0.2,0,0.2]);
L3R = rectangle('Position', [1269,2149,200,200],'FaceColor',[0.2,0,0.2]);

x = 0
while x < 120
    x = x+1;
    fopen(s);
    L1 = fscanf(s, "%s");
    L2 = fscanf(s, "%s");
    L3 = fscanf(s, "%s");
    L4 = fscanf(s, "%s");
    R1 = fscanf(s, "%s");
    R2 = fscanf(s, "%s");
    R3 = fscanf(s, "%s");
    R4 = fscanf(s, "%s");

    L0 = str2double(L1);
    L1 = str2double(L2);
    L2 = str2double(L3);
    L3 = str2double(L4);
    R0 = str2double(R1);
    R1 = str2double(R2);
    R2 = str2double(R3);
    R3 = str2double(R4);

    L0 = (L0*0.001);
    L1 = (L1*0.001);
    L2 = (L2*0.001);
    L3 = (L3*0.001);
    R0 = (R0*0.001);
    R1 = (R1*0.001);
    R2 = (R2*0.001);
    R3 = (R3*0.001);

    %R11-CH0
    if R0 >= 1.0
        R0 = 1.0;
    end
    R0R.FaceColor = [0.2,R0,0.2];
    drawnow;
    %R11-CH1
    if R1 >= 1.0
        R1 = 1.0;
    end
    R1R.FaceColor = [0.2, R1, 0.2];
    drawnow;
    %R11-CH2
    if R2 >= 1.0
        R2 = 1.0;
    end
    R2R.FaceColor = [0.2,R2,0.2];
    drawnow;
    %R11-CH3
    if R3 >= 1.0
        R3 = 1.0;
    end
    R3R.FaceColor = [0.2,R3,0.2];
    drawnow;
    %L00-CH0
    if L0 >= 1.0
        L0 = 1.0;
    end
    L0R.FaceColor = [0.2,L0,0.2];
    drawnow;
    %L00-CH1
    if L1 >= 1.0
        L1 = 1.0;
    end
    L1R.FaceColor = [0.2,L1,0.2];
    drawnow;
    %L00-CH2
    if L2 >= 1.0
        L2 = 1.0;
    end
    L2R.FaceColor = [0.2,L2,0.2];
    drawnow;
    %L00-CH3
    if L3 >= 1.0
        L3 = 1.0;
    end
    L3R.FaceColor = [0.2,L3,0.2];
    drawnow;
    fclose(s);
end
