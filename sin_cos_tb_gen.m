% 通过查表表法计算sin（x) cos(x)
% x取值为PI/2 PI/4 PI/8 PI/16 ... PI/(2^k)
% 此程序用于生成表及计算使用查表发造成的误差

clc;
clear

k = 0:20;
x = pi./(2.^k);
y1 = sin(x);
y2 = cos(x);
plot(x,y1,x,y2),grid on

% 注；不在表中的值可采用线性插值的方法求取，以提高精度
fid = fopen('sin_tb.h', 'w');
fprintf(fid, 'const float sin_tb[] = {  // 精度(PI PI/2 PI/4 PI/8 PI/16 ... PI/(2^k))\n');

fprintf(fid, '%.6f, %.6f, %.6f, %.6f, %.6f, %.6f , %.6f, %.6f, %.6f, %.6f,\n', y1);
fprintf(fid, '\n};\n');
fclose(fid);

% 注；不在表中的值可采用线性插值的方法求取，以提高精度
fid = fopen('cos_tb.h', 'w');
fprintf(fid, 'const float cos_tb[] = {  // 精度(PI PI/2 PI/4 PI/8 PI/16 ... PI/(2^k))\n');

fprintf(fid, '%.6f, %.6f, %.6f, %.6f, %.6f, %.6f , %.6f, %.6f, %.6f, %.6f,\n', y2);
fprintf(fid, '\n};\n');
fclose(fid);
