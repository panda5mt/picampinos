function thresh = my_graythresh(img)

    img = im2double(img);
    img = img .* 255;

    img = reshape(img,1,[]);
    total = numel(img); % total number of pixels in the image 
    %% OTSU automatic thresholding
    top = 256;
    sumB = 0;
    wB = 0;
    maximum = 0.0;

    hist_counts=hist(img, 0:top-1);

    sum1 = dot(0:top-1, hist_counts);
    for ii = 1:top
        wF = total - wB;
        if wB > 0 && wF > 0
            mF = (sum1 - sumB) / wF;
            val = wB * wF * ((sumB / wB) - mF) * ((sumB / wB) - mF);
            if ( val >= maximum )
                thresh = ii;
                maximum = val;
            end
        end
        wB = wB + hist_counts(ii);
        sumB = sumB + (ii-1) * hist_counts(ii);
    end
    thresh = thresh / top;
end

