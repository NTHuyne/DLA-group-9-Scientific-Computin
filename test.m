function [ ] = test( )
%UNTITLED3 Summary of this function goes here
%   Detailed explanation goes here
x = 0;
while x <= 0
    R1 = 200*x;
    R2 = 200 * (x + 1) - 1;
    A = dlmread('result.txt', ' ', [R1 0 R2 199]);
    imagesc(A);colorbar; pause(0.000000000001);
    x = x + 1;
end

end

