%clc;clear;close all
% JW Oct 2016 

%setting

% load model velocityProfile
load(strcat('../converted_geometry/', chip_name, '.mat')) % load geometry information as cell array
node = csvread(strcat('../data/', chip_name,'-x.csv'), 9, 0);
node(:, 3) = [];
x = csvread(strcat('../data/', chip_name,'-x.csv'), 9, 2);
y = csvread(strcat('../data/', chip_name,'-y.csv'), 9, 2);

% calculate some limits
total_polylines = numel(polylines);
total_nodes = numel(node) / 2;
sorted_node = sortrows([node (1:total_nodes)'], 1);
geo_x_max = max(node(:, 1))
geo_x_min = min(node(:, 1))
geo_y_max = max(node(:, 2))
geo_y_min = min(node(:, 2))

fprintf("%lf %lf %lf %lf\n\n", geo_x_min, geo_x_max, geo_y_min, geo_y_max);
%initial_position = [geo_x_min + init_x_shift, geo_y_min + init_y_shift];
%resolution = max([geo_x_max geo_y_max]) / 2; % unit 10^(-6) s
initial_position = [geo_x_min + init_x_shift, ((geo_y_min + geo_y_max)/2) + init_y_shift];

display(resolution);
display(initial_position);

trajectories = [];
% simulation begin
for i=1:length(dPs)
    dP = dPs(i);
    fprintf("Simulate particle with diameter = %f\n", dP)

    trajectory = [initial_position 0]; % initiate particle trajectory
 
    current_position = initial_position;
    pre_vx = start_vx;
    pre_vy = start_vy;
    cnt = 0;
    if dump_debug_file == 1
      debug_filename = strcat("mdebug_", chip_name, "_", num2str(dP), '.log') 
      fp_debug = fopen(debug_filename, "w");
      fprintf("Dump debug file to %s\n", debug_filename);
    end

    while cnt < boundary_max_timestep
        cnt = cnt + 1;
        covered_nodes = findCoveredNodes(dP, current_position, sorted_node, node);
        vx = getMean(covered_nodes, x) * alpha;
        vy = getMean(covered_nodes, y) * beta;
    
        if (vx == 0) && (vy == 0) % in case of math failure
            vx = pre_vx;
            vy = pre_vy;
        end

        if dump_debug_file == 1
          fprintf(fp_debug, "TimeStep: %d\n", cnt);
          fprintf(fp_debug, "Current position: %f %f\n", ...
            current_position(1), current_position(2));
          fprintf(fp_debug, "Current velocity: %f %f\n", vx, vy);
          fprintf(fp_debug, "covered nodes:\n");
          for j = 1:length(covered_nodes);
            fprintf(fp_debug, "%f %f\n", ...
              node(covered_nodes(j), 1), node(covered_nodes(j), 2));
              %covered_nodes(j), covered_nodes(j));
          end
        end

        % determine if touching walls
        next_position = current_position + resolution * [vx vy];
        % determine if touching walls & fix it if it does
        try
            fixed_position = wallEffectNewB(dP / 2, next_position, polylines);
        catch
            fixed_position = [next_position 0];
        end
        % move to next step
        trajectory = [trajectory; fixed_position];
        current_position = fixed_position(1 : 2);

        fprintf("Step %d: current_position = (%f, %f)\r", cnt, current_position(1), current_position(2))
        %display( ["step = ", cnt , ": " , current_position]);

        pre_vx = vx;
        pre_vy = vy;

        if current_position(1, 1) >= geo_x_max || current_position(1, 2) >= geo_y_max ...
            || current_position(1, 2) <= geo_y_min || current_position(1, 1) <= geo_x_min
            display('break: automatically terminated!')
            break;
        end
        if current_position(1, 1) >= (geo_x_max * boundary_x_ratio) 
            display('break: mannually terminated!')
            break;
        end

        if cnt > boundary_max_timestep
            display('break: > max_timestep = ' + boundary_max_timestep)
            break;
        end
    end % while

    if dump_debug_file == 1
      fclose(fp_debug)
    end
    
    trajectories{i} = trajectory;

    % write output to file
    filename = strcat(output_folder, "matlab_", chip_name, "_", string(dP), ".txt");
    fprintf("Output to %s\n", filename);
    fprintf("# of position = %d\n", length(trajectory));
  
    fp = fopen(filename, "w");
    fprintf(fp, "%d %f\n", length(trajectory), dP);
    for j = 1:length(trajectory);
      fprintf(fp, "%f %f\n", trajectory(j, 1), trajectory(j, 2));
    end
    fclose(fp);
end

fprintf("\n");
exit

%%% Write output
%for i =1:length(dPs);
%  current_data = trajectories{i};
%
%  filename = strcat(output_folder, "matlab_", chip_name, "_", string(dPs(i)), ".txt");
%  fprintf("Output to %s\n", filename);
%  fprintf("# of position = %d\n", length(current_data));
%
%  fp = fopen(filename, "w");
%  fprintf(fp, "%d %f\n", length(current_data), dPs(i));
%  for i = 1:length(current_data);
%    fprintf(fp, "%f %f\n", current_data(i, 1), current_data(i, 2));
%  end
%  fclose(fp);
%end

%exit
%% plot begin
display('ploting')

% plot geometry
total = numel(polylines);
figure(1)
for i = 1: total
    lines = polylines{i, 1};
    % to = min(numel(lines(:, 2)), 49);
    plot(lines(:,2), lines(:,3))
    hold on
    clear('lines')
end

% plot particle trajectory

colors = ['k', 'g'];

for i=1:length(dPs)
    trajectory = trajectories{i};
    dP = dPs(i);
    color = colors(i);
    total = numel(trajectory) / 3;
    j = 1;
    while j <= total
       plotCircle([trajectory(j, 1:2)], dP / 2, color)
       %if trajectory(j, 3) == 0
       %     plotCircle([trajectory(j, 1:2)], dP / 2, 'k')
       % else
       %     plotCircle([trajectory(j, 1:2)], dP / 2, 'g')
       % end
        j = j + 10;
    end
end

%axis([0 1500 0 3000])
axis equal
hold off



