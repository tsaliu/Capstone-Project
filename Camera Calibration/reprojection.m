clear all;close all;clc

%function r = reprojection(p1,p2)
k1=-5;
k2=-5;

%Intrinsic Matrix with unit mm
K_temp = [2.454484381983271e+03, 0, 0;-0.381085258365849,2.454270891896557e+03,0;...
    1.351743018547775e+03, 9.732412832576541e+02, 1];
K =K_temp';
B = (K')^(-1)*K^(-1);
lambda = B(3,3)-((B(1,3))^2-K(2,3)*(B(1,2)*B(1,3)-B(1,1)*B(2,3)))/B(1,1);

r = lambda*(K)^(-1)*[k1;k2;1]
%end

