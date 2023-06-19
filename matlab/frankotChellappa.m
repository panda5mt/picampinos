function depths = frankotChellappa(dfdx, dfdy)
    %FRANKOTCHELLAPPA Generates surface given gradients.
    %   DEPTHS = FRANKOTCHELLAPPA(DFDX, DFDY) reconstructs a depth map from the
    %   gradients DFDX and DFDX using an implementation of Frankot and
    %   Chellappa's algorithm for constructing an integrable surface from
    %   gradient information.
    %
    %   DEPTHS is the inferred depths.

    % Reference:
    %
    % Robert T. Frankot and Rama Chellappa A Method for Enforcing Integrability
    % in Shape from Shading IEEE PAMI Vol 10, No 4 July 1988. pp 439-451
    %
    % Note this code just implements the surface integration component of the
    % paper (Equation 21 in the paper).  It does not implement their shape from
    % shading algorithm.

    % Copyright (c) 2004 Peter Kovesi School of Computer Science & Software
    % Engineering The University of Western Australia
    % http://www.csse.uwa.edu.au/
    %
    % Permission is hereby granted, free of charge, to any person obtaining a
    % copy of this software and associated documentation files (the
    % "Software"), to deal in the Software without restriction, subject to the
    % following conditions:
    %
    % The above copyright notice and this permission notice shall be included
    % in all copies or substantial portions of the Software.
    %
    % The Software is provided "as is", without warranty of any kind.

    % October 2004

    if ~all(size(dfdx) == size(dfdy))
        error('Gradient matrices must match');
    end

    [nrows, ncols] = size(dfdx);

    % The following sets up matrices specifying frequencies in the x and y
    % directions corresponding to the Fourier transforms of the gradient
    % data.  They range from -0.5 cycles/pixel to + 0.5 cycles/pixel.

    [wx, wy] = meshgrid(linspace(-pi/2, pi/2, ncols), linspace(pi/2, -pi/2, nrows));

    % Quadrant shift to put zero frequency at the appropriate edge

    wx = ifftshift(wx);
    wy = ifftshift(wy);

    % Fourier transforms of gradients

    dfdxfft = fft2(dfdx);
    dfdyfft = fft2(dfdy);

    % Integrate in the frequency domain by phase shifting by pi/2 and weighting
    % the Fourier coefficients by their frequencies in x and y and then
    % dividing by the squared frequency.  To avoid division by 0 eps is added
    % to the denominator.

    % Equation 21:

    depthsfft = (1i*wx.*dfdxfft + 1i*wy.*dfdyfft)./(wx.^2 + wy.^2 + eps);

    % Reconstruction

    depths = real(ifft2(depthsfft))/2;
end