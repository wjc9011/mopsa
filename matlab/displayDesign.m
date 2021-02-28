
chip_name = 'science';
load(strcat('../converted_geometry/', chip_name, '.mat')) % load geometry information as cell array

% plot geometry
total = numel(polylines);
figure(1)
for i = 1: 200
    lines = polylines{i, 1};
    lines
    lines(:,2), lines(:,3)
    plot(lines(:,2), lines(:,3))
    hold on
    clear('lines')
  %  break;
end