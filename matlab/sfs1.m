
clear;
close all;
% 画像読み込み
RGB_I = (imread('./img/dusseldorf-nord.jpg'));

% グレースケールに変換
if size(RGB_I,3) == 3
    I = rgb2gray(RGB_I);
else
    I = RGB_I;
end
I = im2double(I);


% 光源の方向を設定
%
%   y
%   ↑
%   |
%   |   z (奥行き、画面内へ)
%   |  ↙
%   |
%   ───────────→ x
%


pos1 =[0.7; .21; 1]; % z方向からの光源を仮定

% FFTを使う方法
tic

%pos = estimate_lightsource(I);
[pos,k] = estimateLightSource(I);
pos = pos * k;

[p,q] = estimate_normal(I, pos);
Z2 = fcmethod(q,p,false);
disp(pos)
toc(tic)

% 結果を表示する図を生成
figure;
imshow(RGB_I); % 画像を表示
hold on; % この図に更にプロットを追加する
% 深度データをカラーマップとともに重ねて表示
h = imagesc(-Z2,'AlphaData', .95); % 透明度を95%に設定
colormap('pink'); % カラーマップを設定
colorbar; % カラーバーを表示

% 軸の設定
axis equal;
axis off;
drawnow; % 図の更新

function [p, q] = estimate_normal(image, light_pos)
lx = light_pos(1) ;
ly = light_pos(2) ;
lz = light_pos(3) ;

% 画像勾配を計算（輝度から法線を推定）
p = image * lx / lz;
q = image * ly / lz;

end


