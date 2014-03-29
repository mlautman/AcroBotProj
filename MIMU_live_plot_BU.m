%% ADC value liveplotting & logging.
%
% Reads ADC values from the M2 microcontroller.
% Plots the data realtime in a subplot.
% Logged into ADC_Log[time, ADC_Value] variable.
% Hit Ctrl-C to quit the program
%
% By Nick McGill
% Modified by Elizabeth Beattie in Fall 2012 for Acrobat IMU
% Reads m_imu values from the M2 microcontroller and plots the desired data
% in real time.
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%



%% Initialize program and USB port
% Close any existing open port connections
% For the first time running, comment this.
% if(exist('M2USB'))
%     fclose(M2USB);
% else
%     fclose(instrfindall);
% end
% 
% clear all
% close all
% 

%% If the above initialization does not work, please run the following commands manually and disconnect and reconnect USB.
% fclose(serial('COM5','Baudrate', 9600));
% fclose(instrfindall);
% clear all;
% close all;


%% VARIABLES
maxPoints = 20;         % Max number of data points displayed at a time.
t = 1:1:maxPoints;      % Create an evenly spaced matrix for X-axis.
IMU_Log = zeros(1,2);   % ADC data points logged here in format: [time ADC_Value]
IMU_live_plot_log = zeros(maxPoints,1); % Create an array to store ADC values.

IMU_RANGE = -2000:2000;   % signed integer 16-bit
LOGFREQUENCY = 1;  % Log the ADC value every certain number of times.  Lower = more data points.



%% SERIAL
%----> for ***WINDOZE***
% M2USB = serial('COM5','Baudrate', 9600);
% *** Use the device manager to check where the microcontroller is plugged
% into.

%----> for ***MAC***
M2USB = serial('/dev/tty.usbmodem411','Baudrate',9600);
% *** Check where your device is by opening terminal and entering the command:
% 'ls /dev/tty.usb' and tab-completing.

fopen(M2USB);       % Open up the port to the M2 microcontroller.
flushinput(M2USB);  % Remove anything extranneous that may be in the buffer.

% Send initial packet to get first set of data from microcontroller
%fwrite(M2USB,1);% Send a packet to the M2.
time = 0;       % Set the start time to 0.
i = 1;          % Set i to 1, the indexer.
tic;            % Start timer.



%% Run program forever
try
while 1
    
    %% Read in data and send confirmation packet
    m2_buffer = fgetl(M2USB);   % Load buffer
    %fwrite(M2USB,1);            % Confirmation packet
    
    %% Parse microcontroller data
	% Expecting data in the form: [uint ADC1]
    %m2_ADC = hex2dec(m2_buffer(1:4));
    % Expecting data in the form: [int ANGLE]
%    keyboard
%For use specifically with Acrobatv1.0 code
[RealinputACC, remainder] = strtok(m2_buffer);
RealinputACC = str2num(RealinputACC);
[RealinputGyr, Angle_Actual] =strtok(remainder);
RealinputGyr = str2num(RealinputGyr);
Angle_Actual = str2num(Angle_Actual);
%For general use
%      [m2_ax, remain] = str2num(strtok(m2_buffer));
%     [m2_ay, remain2] = strtok(remain);
%     [m2_az, remain3] = strtok(remain2);
%     [m2_gx, remain4] = strtok(remain3);
%     [m2_gy, remain5] = strtok(remain4);
%     [m2_gz, remain6] = strtok(remain5);
%     [m2_mx, remain7] = strtok(remain6);
%     [m2_my, remain8] = strtok(remain7);
%     [m2_mz] = strtok(remain8);
m2_buffer;
    time = toc; % Stamp the time the value was received
    
    % Remove the oldest entry.    
    IMU_live_plot_log = circshift(IMU_live_plot_log,-1);
    
    % Store most recent data at the end of the array
    IMU_live_plot_log(maxPoints,:) = Angle_Actual;
    
    %% Plotting
    figure(1);
    clf;
    hold on
    
    plot(t, IMU_live_plot_log(:,1),':or');
    title('Angle of Robot');
    xlabel('Time');
    ylabel('Angle of Robot');
    axis([0 maxPoints min(IMU_RANGE) max(IMU_RANGE)]);
    grid on
    pause(.04);
                                   
    hold off
    
    i=i+1;  % Incrememnt indexer
    %% Logging
    if(rem(i,LOGFREQUENCY) == 0)
        IMU_Log = [IMU_Log; time Angle_Actual];
    end
    
end

catch
    %Close serial object
    fclose(M2USB);
end
