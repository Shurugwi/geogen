function GetInfo(info_type){
	switch(info_type){
		case "name":
			return "Test";
		case "description":
			return "Your map description.";
		case "args":
			GGen_AddIntArg("width","Width","Width of the map.", 1024, 128, 20000, 1);
			GGen_AddIntArg("height","Height","Width of the map.", 1024, 128, 20000, 1);
			
			return 0;
	}
}

function Generate(){
	GGen_InitProgress(21);

	local width = GGen_GetParam("width");
	local height = GGen_GetParam("height");

	local value_multiplier = 10;

	local base = GGen_Data_2D(width, height);

	GGen_IncreaseProgress();

	base.Gradient(width - 11, height - 11, 10, 10, 245 * value_multiplier, 10 * value_multiplier, true);
	base.Gradient(width / 8, height / 8, 7 * width / 8, 7 * height / 8, 0, 255 * value_multiplier, false);

	GGen_IncreaseProgress();

	local radial = GGen_Data_2D(width, height);
	radial.RadialGradient(3 * width / 4, height / 4, width / 5, 50 * value_multiplier, 200 * value_multiplier, false);

	GGen_IncreaseProgress();

	base.Union(radial);
	base.UnionTo(0, height / 2, radial);

	GGen_IncreaseProgress();

	radial.ResizeCanvas(width, height, width / 2, 0);
	radial.Invert();
	radial.Add(255 * value_multiplier);

	GGen_IncreaseProgress();

	base.Intersection(radial);
	base.IntersectionTo(0, height / 2, radial);

	radial = null;

	base.ReturnAs("1-gradient, set ops., canvas");
	GGen_IncreaseProgress();

	base.ScaleValuesTo(100, 110);

	base.ScaleValuesTo(0, 255 * value_multiplier);

	GGen_IncreaseProgress();

	base.Smooth(20);

	base.ReturnAs("2-scaleValues, smooth")
	GGen_IncreaseProgress();

	base.Rotate(-45, true);

	GGen_IncreaseProgress();

	local profile = GGen_Data_1D(width);
	
	profile.Noise(4, width / 5);
	profile.ScaleValuesTo(0, height / 10);
	
	base.Shift(profile, GGEN_VERTICAL, GGEN_DISCARD_AND_FILL);

	profile = null;

	base.ReturnAs("3-rotate, 1D noise, 1D scale, shift");
	GGen_IncreaseProgress();
	
	local mask = GGen_Data_2D(base);
	
	mask.Monochrome(0);
	mask.Invert();
	mask.Add(1);
	
	GGen_IncreaseProgress();
		
	collectgarbage();	
	
	local noise = GGen_Data_2D(width, height);	
	
	GGen_IncreaseProgress();
	
	noise.Noise(4, width / 8);
	
	GGen_IncreaseProgress();
	
	noise.ScaleValuesTo(0, 255 * value_multiplier);

	base.AddMasked(noise, mask, true);

	mask = noise = null;
	
	base.ReturnAs("4-mask, monochrome, noise");
	GGen_IncreaseProgress();

	base.ScaleTo(width / 2, height / 2, false);

	GGen_IncreaseProgress();

	local new_base = GGen_Data_2D(width, height);
	
	new_base.Pattern(base);

	GGen_IncreaseProgress();

	base = new_base;

	base.ScaleTo(width, height / 2, false);
	
	local base2 = GGen_Data_2D(base);
	
	base.ResizeCanvas(width, height, 0, 0);
	
	GGen_IncreaseProgress();
	
	base2.Flip(GGEN_VERTICAL);
	
	base.AddTo(0, height / 2, base2);

	base2 = null;

	base.ReturnAs("5-scaleTo, pattern, flip, addto");
	GGen_IncreaseProgress();
	
	base.Flood(0.7);
	
	GGen_IncreaseProgress();
	
	base.Clamp(0, 3 * base.Max() / 4);
	
	base.ReturnAs("6-clamp, max, flood");
	
	collectgarbage();
	
	GGen_IncreaseProgress();

	base.Shear(0, 1, true);
	
	return base;
}