-- Level 1 - Treasure Island

introState = 1;
maxIntroStates = 9;

function UpdateIntroState() 
	if introState > maxIntroStates then
		return;
	end

	if introState == 1 then

		music_play("bassy1.ogg");
		world_play_input_script("data/input/level1.1.input");

	elseif introState == 2 then

		world_textbox("Amaya: Jouten!| |(Press C to continue)~Amaya: ...~Amaya: Jouten!!!!~Amaya: Where the hell..|Where could he be?", "amaya-icon.png");
		
	elseif introState == 3 then

		world_textbox("Jouten:| |TAAAAALLLY|HOOOOOO!~", "jouten-icon.png");

	elseif introState == 4 then

		world_play_input_script("data/input/level1.2.input");
	
	elseif introState == 5 then

		world_textbox("Jouten: Hey Amaya!||What's new?", "jouten-icon.png");
	
	elseif introState == 6 then

		world_textbox("Amaya: You know, you really scare me sometimes.~Amaya: But, I need your help, which is weird.|Let's get to your|workshop, FAST.", "amaya-icon.png");
	
	elseif introState == 7 then
		
		world_textbox("Jouten: No prob. It should|be a quick climb|from here.", "jouten-icon.png");
	
	elseif introState == 8 then
		
		world_textbox("Amaya: Let's hurry up and get there!", "amaya-icon.png");
	
	elseif introState == 9 then

		world_create_cutbars("Pirates VS Ninjas|Level 1 - Treasure Island");

		-- Now they can't go offscreen
		world_allow_player_offscreen(0);

		-- start the game for real.
		music_play("level1.ogg");

	end
	
	introState = introState+1;
end

function Level_OnLoad() 
		
	window_fadein(6);

	-- until the cinematic is over, they can go offscreen
	world_allow_player_offscreen(1);

	introState = 1;
	UpdateIntroState();

end

function Input_DonePlaying() 
	UpdateIntroState();	
end

function Textbox_Done()
	UpdateIntroState();
end
