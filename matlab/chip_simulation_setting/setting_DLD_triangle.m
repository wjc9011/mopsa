
chip_name = 'DLD_triangle';

init_x_shift = 15;
init_y_shift = 20; % shift from middle of y; that is init_y = (min_y+max_y)/2 + init_y_shift

resolution = 10;

dPs = [1, 5, 10];

boundary_x_ratio = 0.95;
boundary_max_timestep = 100000;

alpha = 1; % calibration for vx
beta = 1.45; % calibration for vy

start_vx = 0.1;
start_vy = -0.1;

output_folder = "../sim_output/DLD_triangle/";

dump_debug_file = 0;
