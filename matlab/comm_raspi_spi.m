clc;
clear;
format long;

RGB_img = zeros(480,640,3,'uint8');
img = zeros(480,640,'uint32');
lower5 = hex2dec('1f') .* ones(480,640,'uint32'); % 0x1f 0x1f ....
lower6 = hex2dec('3f') .* ones(480,640,'uint32'); % 0x3f 0x3f ....
lower8 = hex2dec('ff') .* ones(480,640,'uint32'); % 0xff 0xff ....
lower16 = 65535 .* ones(480,640,'uint32'); % 0xffff 0xffff ....

%% setup raspberry pi
mypi= raspi();
enableSPI(mypi);
myspidev = spidev(mypi,'CE1',0,8000000); % nCS = CE1, Mode 0, 2MHz 

len = 2*640/4; % one horizontal line
out = zeros(480,len,'uint32');
indata = ones(1,len);
askbuf = 0xffffffff;
respbuf = 0xDEADBEEF;


while (true)
    %% start capture
    tic
    % seek start buffer
    while(true)
        aaa = writeRead(myspidev,swapbytes(askbuf),'uint32');
        if (swapbytes(aaa) == respbuf)
        break;
        end
        
    end
    disp("GO!");
    for i = 1:1:480
        out(i,:) = writeRead(myspidev,indata,'uint32');
    end
    toc

    %% decode image
    for HGT = 1:480 
        for WID = 1:1:320
            try
                data = swapbytes(out(HGT,WID)); % convert endian
                img(HGT, WID*2-1) = data;
                img(HGT, WID*2) = bitshift(data,-16);
            catch 
                fprintf('Got error : %s, but replaced dummy data.\n',data);      % if got error,
                data = '0xAAAAAAAA';                    % insert dummy data
                img(HGT, WID) = (data);
                img(HGT, WID+1) = bitshift((data),-16); 
            end
            
        end
    end

    img = bitshift(swapbytes(bitand(lower16, img)),-16);
    imgR = (255/63) .* bitand(lower5, bitshift(img,-11));    % Red component
    imgG = (255/127).* bitand(lower6, bitshift(img,-5));    % Green component
    imgB = (255/63) .* bitand(lower5, img);	% Blue component

    RGB_img(:,:,1) = imgR;
    RGB_img(:,:,2) = imgG;
    RGB_img(:,:,3) = imgB;

    imshow(uint8(RGB_img));
    imwrite(RGB_img,"untitle.jpg");
end

