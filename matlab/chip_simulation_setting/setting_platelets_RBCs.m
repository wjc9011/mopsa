
chip_name = 'platelets_RBCs';

init_x_shift = 20;
init_y_shift = -10; % shift from middle of y; that is init_y = (min_y+max_y)/2 + init_y_shift

resolution = 1;

dPs = [1, 10, 5];

boundary_x_ratio = 0.95;
boundary_max_timestep = 100000;

alpha = 1; % calibration for vx
beta = 1.45; % calibration for vy

start_vx = 0.1;
start_vy = -0.1;

output_folder = "../sim_output/platelets_RBCs/";

dump_debug_file = 0;
