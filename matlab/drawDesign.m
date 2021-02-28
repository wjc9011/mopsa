clc;clear;close all
% JW Oct 2016 

% define something
%chip_name = 'critical-part';
%chip_name = 'DLD-triangle';
chip_name = 'cross';
load(strcat('./converted_geometry/', chip_name, '.mat')) % load geometry information as cell array

%% plot begin
display('ploting')

% plot geometry
total = numel(polylines);
figure(1)
for i = 1: total
    lines = polylines{i, 1};
   
    plot(lines(:,2), lines(:,3))
    hold on
    clear('lines')
end

%axis([0 1700 0 1700])
axis tight
hold off