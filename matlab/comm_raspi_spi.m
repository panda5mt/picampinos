clear all;
clc
mypi = raspi('raspberrypi.local','pi', 'rasbberry'); % MATLAB Desktop
%mypi= raspi(); % MATLAB Online

enableSPI(mypi);
myspidev = spidev(mypi,'CE1',0,2000000); % nCS = CE1, Mode 0, 32MHz 

len = 2*1024;
out = zeros(1,len);
indata = ones(1,len);
%for j=1:5
    tic
    %for i = 1:1:len
        out = writeRead(myspidev,indata,'uint8');
        disp(out);
    %end
    toc
%end



%disableSPI(mypi);
%mypi = 0;