% fast blur function(Osafune-Blur)
function img = f_blur(img, n, mode)

    % MODE: 1 = normal(gaussian + median), extended(gaussian + gaussian)
arguments
    img
    n 
    mode = 1 % normal mode
end
    kernel = pascal_1d(n);
    k_sum = sum(kernel,"all");
    kernel_1d = kernel ./k_sum;
    
    x_size = size(img,2);
    y_size = size(img,1);
    z_size = size(img,3);
    
    for c=1:z_size
        i = img(:,:,c);
        for y=1:y_size
            i(y,:) = conv(i(y,:),kernel_1d,'same');
        end
        for x=1:x_size
        switch mode
            case 1
            i(:,x) = medfilt1(double(i(:,x)),floor(n/2));
            otherwise
            i(:,x) = conv(i(:,x),kernel_1d,'same');
        end
        end
        img(:,:,c) = i;
    end

end
