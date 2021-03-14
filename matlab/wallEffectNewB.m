function res = wallEffectNew(particleRadius, particlePosition, polylines)
% close all
% particleRadius = 10
% particlePosition = current_position
total_polylines = numel(polylines);
alpha = 0: pi / 100 : 2*pi;
xp = particleRadius * cos(alpha) + particlePosition(1, 1);
yp = particleRadius * sin(alpha) + particlePosition(1, 2);

for j = 1: total_polylines
    this_polyline = polylines{j,1};
    if this_polyline(1, :) == this_polyline(end, :)
        covered_points = [];
        current_polyline = polylines{j,1};
        xv = current_polyline(: , 2);
        yv = current_polyline(: , 3);
        in = inpolygon(xp, yp, xv, yv);
        total = numel(in);
        i = 1;
        count = 0;
        while i <= total
            if in(i) == 1
                count = count + 1;
                covered_points = [covered_points; xp(i) yp(i)]; 
        %        break;
            end
            i = i + 1;
        end
        %count
        if count > 200 || count == 0
            tmp_res = [particlePosition 0]; % not covered
%         elseif count > 100
%             middle_point = [(covered_points(1, 1) + covered_points(end, 1)) / 2 (covered_points(1, 2) + covered_points(end, 2)) / 2];
% 
%             direction = middle_point - particlePosition;
%             while 1
%                 fixed_position = particlePosition - direction * abs(count - 201) / 201 * particleRadius / 1;
%                 break
%             end
%             tmp_res = [fixed_position 1]; % covered
        else
%             figure
%             plot(xq', yq', 'g')
%             hold on
%             plot(xv, yv)
%             hold on
%             plot(covered_points(:, 1), covered_points(:, 2))
%             hold on 
%             plot(covered_points(1, 1), covered_points(1, 2), 'r*')
%             hold on
%             plot(covered_points(end, 1), covered_points(end, 2), 'r*')
%             hold on 
            middle_point = [(covered_points(1, 1) + covered_points(end, 1)) / 2 (covered_points(1, 2) + covered_points(end, 2)) / 2];

            direction = particlePosition - middle_point;
            while 1
                fixed_position = particlePosition + direction * count / 201 * particleRadius / 1;
                % check fixed_position
                break

            end
      fprintf("\n");
      fprintf("len = %d\n", length(xp));
      fprintf("xp = %f %f %f\n", xp(1), xp(2), xp(3));
      fprintf("yp = %f %f %f\n", yp(1), yp(2), yp(3));
      fprintf("r = %f\n", particleRadius);
      fprintf("Alpha %f %f %f\n", alpha(1), alpha(2), alpha(3));
      fprintf("Count = %d\n", count);
      fprintf("covered_points begin = (%.6f, %.6f)\n", covered_points(1, 1), covered_points(1, 2));
      fprintf("covered_points end = (%.6f, %.6f)\n", covered_points(end, 1), covered_points(end, 2));
      fprintf("middle_point = (%.6f, %.6f)\n", middle_point(end, 1), middle_point(end, 2));
      fprintf("direction = (%.6f, %.6f)\n", direction(end, 1), direction(end, 2));
      fprintf("particlePosition = (%.6f, %.6f)\n", particlePosition(end, 1), particlePosition(end, 2));
      fprintf("fixed_position = (%.6f, %.6f)\n", fixed_position(end, 1), fixed_position(end, 2));
      pause
            tmp_res = [fixed_position 1]; % covered
%             plotCircle(fixed_position, particleRadius, '.')
            break
        end
    end
end

res = tmp_res;

