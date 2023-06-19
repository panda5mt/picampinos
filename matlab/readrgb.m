% This program is part of 'PICAMPINOS' (https://github.com/panda5mt/picampinos/)
% RP2040 generates RGP565 datas via its USB-UART stdio.
% Store these datas pipe(on unix/macOS) or Teraterm(on Windows).
% This program encodes that stored data in an image (640 x 480, 16bit
% color).

clc;
clear;
format long;
%s = serialport("/dev/cu.usbserial-DM02NSUI",921600);
RGB_img = zeros(480,640,3,'uint8');
img = zeros(480,640,'uint32');
lower5 = hex2dec('1f') .* ones(480,640,'uint32'); % 0x1f 0x1f ....
lower6 = hex2dec('3f') .* ones(480,640,'uint32'); % 0x3f 0x3f ....
lower8 = hex2dec('ff') .* ones(480,640,'uint32'); % 0xff 0xff ....
lower16 = 65535 .* ones(480,640,'uint32'); % 0xffff 0xffff ....

fileID = fopen('teraterm.log');

while (true)
    tline = fgetl(fileID);
    %disp(tline);
    if contains(string(tline),'!srt') % got string '!srt'?
        disp("start signal received...");
        break;
    end
end
    

 for HGT = 1:480 
     for WID = 1:2:640
        
        data = fgetl(fileID);
        if -1 ~= data
            %disp(data);
            try
                img(HGT, WID) = hex2dec(data);
                img(HGT, WID+1) = bitshift(hex2dec(data),-16);
            catch 
                fprintf('Got error : %s, but replaced dummy data.\n',(data));       % if got error,
                data = '0xAAAAAAAA';                                                % insert dummy data
                img(HGT, WID) = hex2dec(data);
                img(HGT, WID+1) = bitshift(hex2dec(data),-16); 
            end
        end
     end
 end
 
fclose(fileID);
img = bitshift(swapbytes(bitand(lower16, img)),-16);
%img = bitand(lower16, img);
imgR = (255/63) .* bitand(lower5, bitshift(img,-11));    % Red component
imgG = (255/127).* bitand(lower6, bitshift(img,-5));    % Green component
imgB = (255/63) .* bitand(lower5, img);	% Blue component

RGB_img(:,:,1) = imgR;
RGB_img(:,:,2) = imgG;
RGB_img(:,:,3) = imgB;

imshow(uint8(RGB_img));
imwrite(RGB_img,"untitle.jpg");
%disp(RGB_img);

