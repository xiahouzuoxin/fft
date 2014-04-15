% PLOT RESULT:
% Test wather the C program FFT and XCORR is right
% 
% RESULT:
% Good

close all;
clc
clear all;

x = load('test_fft.txt');
y = complex(x(:,1),x(:,2));

% 使用输入信号为复数的FFT计算
figure,
subplot(2,2,1),plot(abs(y(1:128)));
title('FFT Result');
xlim([0,128]);
subplot(2,2,3),plot(real(y(129:256)));
title('IFFT Result');
axis([0,128,-1,1]);

% 使用输入信号为实数的FFT计算
subplot(2,2,2),plot(abs(y(257:384)));
title('FFT Result with REAL SIGNAL');
xlim([0,128]);
subplot(2,2,4),plot(y(385:512));
title('IFFT Result with REAL SIGNAL');
axis([0,128,-1,1]);


if exist('test_xcorr.txt', 'file')
    dy = 2048;
    x = load('test_xcorr.txt');
    y = complex(x(:,1),x(:,2));
    
    figure,
    subplot(3,2,1);plot(y(1:dy));title('SIGNAL1');xlim([0,2048]);
    subplot(3,2,3);plot(y(dy+1:2*dy));title('SIGNAL2');xlim([0,2048]);
    subplot(3,2,5);plot(y(2*dy+1:3*dy));title('SIGNAL3');xlim([0,2048]);
    subplot(3,2,2);plot(abs(y(3*dy+1:4*dy)));title('FFT1');xlim([0,2048]);
    subplot(3,2,4);plot(abs(y(4*dy+1:5*dy)));title('FFT2');xlim([0,2048]);
    subplot(3,2,6);plot(abs(y(5*dy+1:6*dy)));title('FFT3');xlim([0,2048]);
    
    figure,
    subplot(3,2,1);plot(abs(y(6*dy+1:7*dy)));title('CONJ1');xlim([0,2048]);
    subplot(3,2,3);plot(abs(y(7*dy+1:8*dy)));title('CONJ2');xlim([0,2048]);
    subplot(3,2,5);plot(abs(y(8*dy+1:9*dy)));title('CONJ3');xlim([0,2048]);
    subplot(3,2,2);plot(real(y(9*dy+1:10*dy)));title('IFFT1');xlim([0,2048]);
    subplot(3,2,4);plot(real(y(10*dy+1:11*dy)));title('IFFT2');xlim([0,2048]);
    subplot(3,2,6);plot(real(y(11*dy+1:12*dy)));title('IFFT3');xlim([0,2048]);  
    
    y01 = abs(y(9*dy+1:10*dy));
    y01(21:dy-20) = 0;
    y02 = abs(y(10*dy+1:11*dy));
    y02(21:dy-20) = 0;
    y12 = abs(y(11*dy+1:12*dy));
    y12(21:dy-20) = 0;
    [m t01] = max(y01);
    [m t20] = max(y02);
    [m t12] = max(y12);
    
    % Matlab索引值从0开始
    % If txy>0 means x is ahead y abs(txy) nodes
    t01 = t01 - 1; 
    t20 = t20 - 1;
    t12 = t12 - 1;
    if (t01>dy/2) 
         t01 = t01 - dy;
    end
    if (t20>dy/2) 
         t20 = t20 - dy;
    end
    if (t12>dy/2) 
         t12 = t12 - dy;
    end
    
    fprintf('\n==== Delay Time Calculate By Matlab ====\n');
    fprintf('t01=%.2f, t20=%.2f, t12=%.2f\n', t01, t20, t12);
    
    t01 = y(12*dy + 1);
    t20 = y(12*dy + 2);
    t12 = y(12*dy + 3);
    fprintf('\n==== Delay Time Calculate By C ====\n');
    fprintf('t01=%.2f, t20=%.2f, t12=%.2f\n', t01, t20, t12);    
end
