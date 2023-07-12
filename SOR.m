function [] = SOR()
x = dlmread('output.txt');
hAxes = gca;
imagesc(hAxes, x);
colormap( hAxes , jet );
end

