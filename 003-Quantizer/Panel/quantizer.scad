include <../../AEPanelMaker/AEPanelMaker/panel.scad>

title = "quantizer";
title_x = 8;
title_y = 70 ;
title_font_size = 5;
title_font = "Liberation Sans:style=bold";
title_rotate = 90;

units = 1;
inputs = 4 ;
outputs = 4;

pots_rv09 = [];

leds = [];

generate_panel();