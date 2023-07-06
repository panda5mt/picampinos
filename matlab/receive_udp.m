% video receiver via UDP
% RGB565 format
% our original video format
% frame start: '0xdeadbeef', row_size_in_words(type:uint32,unit is in
% words(not bytes)) , columb_sizein_words(type:uint32), total blocks per
% frame(type:uint32) 
% next udp packet: '0xbeefbeef', matrix_row_number(uint32), matrix_columb_number(uint32),data_length(uint32),data....
% next udp packet ...
% next udp packet ...
% frame end: '0xdeaddead' 

clc;
clear;
close all;
% open port '1024'. This is SFP CAMERA's port.
frame_initialized = false;
udpr = dsp.UDPReceiver( ...
    'LocalIPPort',1024, ...
    'MessageDataType', 'uint32', ...
    'MaximumMessageLength',640);

% udp setup
setup(udpr); 
wordsReceived = 0;
row_size = 0;
col_size = 0;
blk_size = 0;
frame_start_packet = 0xdeadbeef;
header_pixel_data = 0xbeefbeef;
frame_end_packet = 0xdeaddead;
frame_counter = 0;
% image processing setup
RGB_img = zeros(480,640,3,'uint8');
img = zeros(480,640,'uint32');
lower5 = hex2dec('1f') .* ones(480,640,'uint32'); % 0x1f 0x1f ....
lower6 = hex2dec('3f') .* ones(480,640,'uint32'); % 0x3f 0x3f ....
lower8 = hex2dec('ff') .* ones(480,640,'uint32'); % 0xff 0xff ....
lower16 = 65535 .* ones(480,640,'uint32'); % 0xffff 0xffff ....
tStart = tic;
while (true)
    % seek header
    
    while true
        dataReceived = udpr();
        if  false == isempty(dataReceived) && frame_start_packet == dataReceived(1) 
            %convertCharsToStrings( dataReceived )
            frame_counter = frame_counter + 1;
            row_size = dataReceived(2);
            col_size = dataReceived(3);
            blk_size = dataReceived(4);
            break;
            
        end
    end

    wordsReceived = 0;
    %rcv = [];
    if false == frame_initialized
        out = zeros(row_size,col_size,'uint32');
        frame_initialized = true;
    end
    for i=1:blk_size
        while true
            dataReceived = udpr();
            if false == isempty(dataReceived) && header_pixel_data == dataReceived(1)  
                r = dataReceived(2); % row number
                c = dataReceived(3); % columb number
                sz = dataReceived(4); % data size

                if (r > 0 && c > 0 && sz > 0)  
                    out(r, c:(c + sz - 1)) = dataReceived(5:5+sz-1);
                    wordsReceived = wordsReceived + length(dataReceived) - 4;
                    break;
                end
%             elseif frame_end_packet == dataReceived(1)
%                 break;
            end
        end
    end
    
    %wordsReceived
    

    %% decode image
    for HGT = 1:480 
        for WID = 1:1:320
            try
                data = (out(HGT,WID));
                %data = bin2dec(fliplr(dec2bin(data,32))); % bit reverse
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
    imgR = (255/63) .* bitand(lower5, bitshift(img,-11));   % Red component
    imgG = (255/127).* bitand(lower6, bitshift(img,-5));    % Green component
    imgB = (255/63) .* bitand(lower5, img);	                % Blue component

    RGB_img(:,:,1) = imgR;
    RGB_img(:,:,2) = imgG;
    RGB_img(:,:,3) = imgB;

    imshow(imresize(uint8(RGB_img),3));
    drawnow
    
    if(frame_counter > 40)
        tEnd = toc(tStart);
        fps = frame_counter / tEnd;
        disp(fps);
        frame_counter = 0;
        tStart = tic;
    end
end
imwrite(RGB_img,"untitle.jpg");
