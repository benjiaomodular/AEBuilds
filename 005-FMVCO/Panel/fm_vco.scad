include <../../AEPanelMaker/AEPanelMaker/panel.scad>

title = "fm vco";
title_x = 47;
title_y = 70 ;
title_font_size = 5;
title_font = "Liberation Sans:style=bold";
title_rotate = 90;

units = 2;
inputs = 3;
outputs = 3;

pots_rv09 = [
    [4, 23, "Carrier"],
    [4, 13, "Mod Freq"],
    [4, 3, "Mod Level"]];

leds = [];

generate_panel(); 