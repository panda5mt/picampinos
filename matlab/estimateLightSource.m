function [L, k] = estimateLightSource(img)
    % 入力画像の読み込み
    %img = imread(imagePath);
    gray_img = img ; %rgb2gray(img);

    % Sobelフィルタを使用して画像の勾配を計算
    [Gx, Gy] = imgradientxy(gray_img);
    [~, Gdir] = imgradient(Gx, Gy);

    % 法線ベクトルを計算
    normals = cat(3, -Gx, -Gy, ones(size(gray_img)));
    normals = normals ./ sqrt(sum(normals.^2, 3));

    % Lambertian反射モデルに基づく光源の方向推定
    % I = k * (L・N)
    % I: 画素の明るさ, k: 反射率, L: 光源の方向, N: 法線ベクトル

    % 照明の強度 (仮定: 画素の明るさを正規化)
    intensity = (gray_img);% / 255;

    % 法線ベクトルと強度を用いて光源方向を推定
    L = sum(sum(normals .* repmat(intensity, [1, 1, 3]), 1), 2);
    L = reshape(L, [3, 1]);
    L = L / sqrt(sum(L.^2));

    % 光源強度の推定
    k = mean(intensity(:)) / dot(L, mean(reshape(normals, [], 3), 1));

    % 結果の表示
    disp('推定された光源の方向:');
    disp(L');
    disp('推定された光源の強度:');
    disp(k);
end
