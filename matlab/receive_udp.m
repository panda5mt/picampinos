clc;
clear all;
udpr = dsp.UDPReceiver( ...
    'RemoteIPAddress','169.254.6.130', ...
    'LocalIPPort',1024, ...
    'MessageDataType', 'uint32', ...
    'MaximumMessageLength',640);
setup(udpr); 

bytesSent = 0;
bytesReceived = 0;
while (true)
    % seek header
    while true
        dataReceived = udpr();
        if (false == isempty(dataReceived)) 
            %convertCharsToStrings( dataReceived )
            a =0;
            if dataReceived(1) == 0xdeadbeef
                disp("OK");
                break;
            end
        end
    end

    // store image
    bytesReceived = 0;
    rcv = [];
    for i=1:480
        while true
            dataReceived = udpr();
            if (false == isempty(dataReceived)) 
                rcv = [rcv dataReceived];
                bytesReceived = bytesReceived + length(dataReceived);
                break;
            end
        end
    end
    
    bytesReceived
    % display
    RGB_img = zeros(480,640,3,'uint8');
    img = zeros(480,640,'uint32');
    lower5 = hex2dec('1f') .* ones(480,640,'uint32'); % 0x1f 0x1f ....
    lower6 = hex2dec('3f') .* ones(480,640,'uint32'); % 0x3f 0x3f ....
    lower8 = hex2dec('ff') .* ones(480,640,'uint32'); % 0xff 0xff ....
    lower16 = 65535 .* ones(480,640,'uint32'); % 0xffff 0xffff ....
    
    
    len = 2*640/4; % one horizontal line
    out = zeros(480,len,'uint32');
    indata = ones(1,len);
    askbuf = 0xffffffff;
    respbuf = 0xDEADBEEF;
    
    h = 1;
    for HGT = 1:480
        out(h,:) = [rcv(:,HGT)];
        h = h + 1;
    end
    

    %% decode image
    for HGT = 1:480 
        for WID = 1:1:320
            try
                data = (out(HGT,WID)); % convert endian
                %data = bin2dec(fliplr(dec2bin(data,32))); % bit reverse
                img(HGT, WID*2) = data;
                img(HGT, WID*2-1) = bitshift(data,-16);
            catch 
                fprintf('Got error : %s, but replaced dummy data.\n',data);      % if got error,
                data = '0xAAAAAAAA';                    % insert dummy data
                img(HGT, WID) = (data);
                img(HGT, WID+1) = bitshift((data),-16); 
            end
            
        end
    end

    %img = bitshift((bitand(lower16, img)),-16);    
    img = bitshift(swapbytes(bitand(lower16, img)),-16);
    imgR = (255/63) .* bitand(lower5, bitshift(img,-11));    % Red component
    imgG = (255/127).* bitand(lower6, bitshift(img,-5));    % Green component
    imgB = (255/63) .* bitand(lower5, img);	% Blue component

    RGB_img(:,:,1) = imgR;
    RGB_img(:,:,2) = imgG;
    RGB_img(:,:,3) = imgB;

    imshow(uint8(RGB_img));
    drawnow
end
    imwrite(RGB_img,"untitle.jpg");
