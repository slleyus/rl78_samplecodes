r01an3796eu0100-rl78 readme_e.txt

Unzip the file to a location of your choosing.

Directory structure will look like this:
\---an-r01an3796eu0100-rl78
    +---readme_e.txt                 (this file)
    +---r01an3796eu0100-rl78.pdf    	(app note)
    \workspace
 	   +---RL78_PWM_Dither_project.zip  (archive to import from)
        	\RL78_PWM_Dither_project
			\---.settings
			\---HardwareDebug
			\---src
	     			+---*.asm
	     			+---*.c
	     			+---*.txt
	     			\---inc
					+---*.h
			+---.cproject
			+---.Debuglinker
			+---.HarwareDebuglinker
			+---.info
			+---.project
			+---custom.bat
			+---makefile.init
			+---r_option_cc.txt
			+---RL78_PWM_Dither_project Debug.launch
			+---RL78_PWM_Dither_project HardwareDebug.launch
	
	
Start up e2studio with empty workspace and import the 
RL78_PWM_Dither_project.zip archive