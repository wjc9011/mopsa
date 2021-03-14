
chip_name = '80um';

init_x_shift = 50;
init_y_shift = -80; % shift from middle of y; that is init_y = (min_y+max_y)/2 + init_y_shift

resolution = 1;

dPs = [10, 1];

boundary_x_ratio = 0.95;
boundary_max_timestep = 100000;

alpha = 1; % calibration for vx
beta = 1.45; % calibration for vy

start_vx = 0.1;
start_vy = -0.1;
