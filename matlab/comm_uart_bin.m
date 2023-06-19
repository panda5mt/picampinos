clc;
global a img1;
a = 1;
img1 = zeros(480,640*2/4,'uint32');
spList = serialportlist("available"); 
disp(spList);

% Windows
serialObj = serialport(spList(1),921600); % I have no idea choosing correct COMx

disp('start.');
flush(serialObj);

%% read frames from Raspi module
% trigger "readSpData" if 640*2 byte can be read from UART
configureCallback(serialObj,"byte",128,@readSpData);

