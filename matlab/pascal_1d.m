function array = pascal_1d(n)
    tmp = zeros(1,n+1);
    for k=0:n
        % nCrを求めめE        tmp(k+1) = nchoosek(n,k); 
    end
    array = tmp;
end

