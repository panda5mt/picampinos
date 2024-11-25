% 画像の読み込み
img = imread('cameraman.tif'); % サンプル画像 (256x256)
img = double(img);

% パラメータ設定
window_size = 128; % ウィンドウサイズ (128x128)
step = 64;         % スライド間隔
[rows, cols] = size(img);

% FFT結果と再構成用変数の初期化
fft_storage = {}; % FFT結果を保存
reconstructed_img = zeros(rows, cols);
weight_matrix = zeros(rows, cols);

% スライディングウィンドウのFFTループ
fft_index = 1; % FFT結果保存用インデックス
for r = 1:step:(rows - window_size + 1)
    for c = 1:step:(cols - window_size + 1)
        % 現在のウィンドウを抽出
        window = img(r:r+window_size-1, c:c+window_size-1);
        
        % ゼロパディング (2^nサイズに拡張)
        padded_window = zeros(256, 256); % 次の2のべき乗サイズ
        padded_window(1:window_size, 1:window_size) = window;
        
        % FFTを実行
        fft_window = fft2(padded_window);
        
        % フィルタを適用（例: 高周波を強調するフィルタ）
        [u, v] = meshgrid(1:256, 1:256);
        filter = exp(-0.002 * ((u - 128).^2 + (v - 128).^2)); % 高周波強調フィルタ
        fft_filtered = fft_window .* filter; % フィルタリング
        
        % FFT結果を保存
        fft_storage{fft_index} = fft_window;%fft_filtered;
        fft_index = fft_index + 1;
    end
end

% IFFTのループ
fft_index = 1;
for r = 1:step:(rows - window_size + 1)
    for c = 1:step:(cols - window_size + 1)
        % 保存したFFT結果を取得
        fft_filtered = fft_storage{fft_index};
        
        % IFFTを実行
        ifft_window = real(ifft2(fft_filtered));
        
        % 元サイズにトリム
        ifft_window_trimmed = ifft_window(1:window_size, 1:window_size);
        
        % 再構成用の画像に加算（重ね合わせ）
        reconstructed_img(r:r+window_size-1, c:c+window_size-1) = ...
            reconstructed_img(r:r+window_size-1, c:c+window_size-1) + ifft_window_trimmed;
        
        % 加算回数を記録（重みづけに使用）
        weight_matrix(r:r+window_size-1, c:c+window_size-1) = ...
            weight_matrix(r:r+window_size-1, c:c+window_size-1) + 1;
        
        fft_index = fft_index + 1;
    end
end

% 再構成された画像を正規化（重みを割る）
reconstructed_img = reconstructed_img ./ weight_matrix;

% 結果の表示
figure;
subplot(1, 2, 1);
imshow(img, []);
title('Original Image');

subplot(1, 2, 2);
imshow(reconstructed_img, []);
title('Reconstructed Image after Filtering');
