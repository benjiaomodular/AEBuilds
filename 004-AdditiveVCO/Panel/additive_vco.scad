include <../../AEPanelMaker/AEPanelMaker/panel.scad>

title = "additive";
title_x = 47;
title_y = 70 ;
title_font_size = 5;
title_font = "Liberation Sans:style=bold";
title_rotate = 90;

units = 2;
inputs = 3;
outputs = 3;

pots_rv09 = [
    [4, 23, "Fund"],
    [4, 13, "Gain"],
    [4, 3, "Harm"]];

leds = [];

generate_panel(); 